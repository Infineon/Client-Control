/*
 * (c) 2016-2026, Infineon Technologies AG, or an affiliate of Infineon
 * Technologies AG. All rights reserved.
 * This software, associated documentation and materials ("Software") is
 * owned by Infineon Technologies AG or one of its affiliates ("Infineon")
 * and is protected by and subject to worldwide patent protection, worldwide
 * copyright laws, and international treaty provisions. Therefore, you may use
 * this Software only as provided in the license agreement accompanying the
 * software package from which you obtained this Software. If no license
 * agreement applies, then any use, reproduction, modification, translation, or
 * compilation of this Software is prohibited without the express written
 * permission of Infineon.
 *
 * Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
 * IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
 * THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
 * SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
 * Infineon reserves the right to make changes to the Software without notice.
 * You are responsible for properly designing, programming, and testing the
 * functionality and safety of your intended application of the Software, as
 * well as complying with any legal requirements related to its use. Infineon
 * does not guarantee that the Software will be free from intrusion, data theft
 * or loss, or other breaches ("Security Breaches"), and Infineon shall have
 * no liability arising out of any Security Breaches. Unless otherwise
 * explicitly approved by Infineon, the Software may not be used in any
 * application where a failure of the Product or any consequences of the use
 * thereof can reasonably be expected to result in personal injury.
 */
#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include "wiced_types.h"

#ifdef __linux
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/stat.h>

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#endif

#define BNEP_MTU_MIN 1691
#define BNEP_MAX_PENDING_TX_DATA  3

#ifdef __linux
static int  tap_fd = -1;
static uint8_t network_buffer[BNEP_MTU_MIN];
static size_t  network_buffer_len = 0;
static uint8_t pan_ind_buf[ETH_FRAME_LEN];
static uint16_t  pan_ind_len = 0;
static char tap_dev_name[16];

// Linux uses single control device to bring up tunX or tapX interface
static const char * tap_dev = "/dev/net/tun";
static const char * tap_dev_name_template =  "bnep%d";
static pthread_t tap_netif_thread = 0;
static sem_t     tap_queue_sem;
#endif

#include "app_host.h"
#include "app_host_panu.h"
#include "wiced_hci_panu.h"

static bool is_panu_connected = false;
static uint16_t panu_handle = 0;

static void (*btstack_network_send_packet_callback)(bd_addr_t dst, bd_addr_t src, uint16_t protocol, const uint8_t * packet, uint16_t size);
extern void app_host_log(const char * fmt, ...);
extern void script_handle_event(uint16_t opcode, uint32_t is_connected, uint32_t port_handle, uint8_t * p_data, uint32_t len);

uint8_t *scru_dump_hex(uint8_t *p, char *p_title, uint32_t len, uint32_t trace_layer, uint32_t trace_type)
{
    uint32_t xx, yy;
    char buff1[100], buff2[20];

    UNUSED(trace_layer);
    UNUSED(trace_type);

    if (p_title)
    {
        app_host_log("%s\n", p_title);
    }

    memset(buff2, ' ', 16);
    buff2[16] = 0;

    yy = sprintf(buff1, "%04x: ", 0);
    for (xx = 0; xx < len; xx++)
    {
        if ((xx) && ((xx & 15) == 0))
        {
            app_host_log("    %s  %s\n", buff1, buff2);
            yy = sprintf(buff1, "%04x: ", xx);
            memset(buff2, ' ', 16);
        }
        yy += sprintf(&buff1[yy], "%02x ", *p);

        if ((*p >= ' ') && (*p <= 'z'))
            buff2[xx & 15] = *p;
        else
            buff2[xx & 15] = '.';

        p++;
    }

    /* Pad out the remainder */
    for (;; xx++)
    {
        if ((xx & 15) == 0)
        {
            app_host_log("    %s  %s\n", buff1, buff2);
            break;
        }
        yy += sprintf(&buff1[yy], "   ");
    }

    return (p);
}

#ifdef __linux
/******************************************************************************
 **
 ** Function         app_pan_netif_read_thread
 **
 ** Description      receive data from network interface
 **
 ** Parameters
 **
 ** Returns          void
 **
 ******************************************************************************/
static void* btstack_network_netif_read_thread(void *p)
{
#ifdef __linux
    struct ether_header ehdr;
    int ret;
    UNUSED(p);

    app_host_log("app_pan_netif_read_thread");

    while (tap_fd >= 0)
    {
        bd_addr_t src, dst;
        uint16_t protocol, data_len;
        uint8_t *p_data = network_buffer + sizeof(ehdr);

        ret = read(tap_fd, network_buffer, ETH_FRAME_LEN);
        if (ret < 0)
        {
            app_host_log("read data from network interface. Status:%d", ret);
            break;
        }
        else if (ret <= (signed int)sizeof(ehdr))
        {
            app_host_log("Too short data from TAP. Length:%d", ret);

            continue;
        }
        network_buffer_len = ret;
        app_host_log("receive from TAP0: %d\n", ret);


        memcpy(&ehdr, network_buffer, sizeof(ehdr));

        memcpy(dst, &ehdr.ether_dhost, ETH_ALEN);
        memcpy(src, &ehdr.ether_shost, ETH_ALEN);
        protocol = ntohs(ehdr.ether_type);
        data_len = network_buffer_len - sizeof(ehdr);

        sem_wait(&tap_queue_sem);

        //scru_dump_hex((uint8_t *)(network_buffer), "From TAP:", ret, 0, 0);

        //if ((ehdr.ether_dhost[0] & 1) != 0)
        {

            if(btstack_network_send_packet_callback)
            {
                btstack_network_send_packet_callback(dst, src, protocol, p_data, data_len);
            }

            /* if multicast packet, only one packet is forwarded */
            if (ehdr.ether_dhost[0] & 1)
            {
            }
        }
     }
#else
     UNUSED(p);
#endif
    return NULL;
}
#endif

/******************************************************************************
 **
 ** Function         btstack_network_pan_start
 **
 ** Description      Start PAN application
 **
 ** Returns          status
 **
 ******************************************************************************/
int btstack_network_pan_start(void)
{
    app_host_log("btstack_network_pan_start");
#ifdef __linux
    int tap_sem_value = 0;
    pthread_attr_t thread_attr;

    if( 0 == sem_getvalue(&tap_queue_sem, &tap_sem_value))
    {
        while(tap_sem_value < BNEP_MAX_PENDING_TX_DATA)
        {
            sem_post(&tap_queue_sem);
            tap_sem_value++;
        }
    }

    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

    if (pthread_create(&tap_netif_thread, &thread_attr, &btstack_network_netif_read_thread, NULL) < 0 )
    {
        return -1;
    }
#endif
    return 0;
}

/******************************************************************************
 **
 ** Function         btstack_network_pan_stop
 **
 ** Description      Stop PAN application
 **
 ** Returns          status
 **
 ******************************************************************************/
int btstack_network_pan_stop(void)
{
    app_host_log("app_pan_stop +++");
#ifdef __linux
    if (tap_netif_thread != 0)
    {
        if(pthread_cancel(tap_netif_thread) < 0)
        {
            return -1;
        }
        pthread_join(tap_netif_thread, NULL);
        tap_netif_thread = 0;
    }
#endif
    app_host_log("app_pan_stop ---");
    return 0;
}


/**
 * @brief Initialize network interface
 * @param send_packet_callback
 */
void btstack_network_init(void (*send_packet_callback)(bd_addr_t dst, bd_addr_t src, uint16_t protocol, const uint8_t * packet, uint16_t size))
{
#ifdef __linux
    sem_init(&tap_queue_sem, 0, BNEP_MAX_PENDING_TX_DATA);
    btstack_network_send_packet_callback = send_packet_callback;
#else
    UNUSED(send_packet_callback);
    btstack_network_send_packet_callback = NULL;
#endif
}

/**
 * @text This code requries a TUN/TAP interface to connect the Bluetooth network interface
 * with the native system. It has been tested on Linux and OS X, but should work on any
 * system that provides TUN/TAP with minor modifications.
 *
 * On Linux, TUN/TAP is available by default. On OS X, tuntaposx from
 * http://tuntaposx.sourceforge.net needs to be installed.
 *
 * The *tap_alloc* function sets up a virtual network interface with the given Bluetooth Address.
 * It is rather low-level as it sets up and configures a network interface.
 *
 * @brief Bring up network interfacd
 * @param network_address
 * @return 0 if ok
 */
int btstack_network_up(bd_addr_t network_address)
{
#ifdef __linux
    struct ifreq ifr;
    int fd_dev;
    int fd_socket;

    if( (fd_dev = open(tap_dev, O_RDWR)) < 0 ) {
        app_host_log("TAP: Error opening %s: %s\n", tap_dev, strerror(errno));
        return -1;
    }


    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    strncpy(ifr.ifr_name, tap_dev_name_template, IFNAMSIZ);  // device name pattern

    int err;
    if( (err = ioctl(fd_dev, TUNSETIFF, (void *) &ifr)) < 0 ) {
        app_host_log("TAP: Error setting device name: %s\n", strerror(errno));
        close(fd_dev);
        return -1;
    }
    if ((err = ioctl(fd_dev, TUNGETIFF, (void *)&ifr)) < 0)
    {
        app_host_log("ioctl error:%d, errno:%s", err, strerror(errno));

        close(fd_dev);

        return -1;
    }
    strcpy(tap_dev_name, ifr.ifr_name);

    //fd_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    fd_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd_socket < 0) {
        close(fd_dev);
        app_host_log("TAP: Error opening netlink socket: %s\n", strerror(errno));
        return -1;
    }

    // Configure the MAC address of the newly created bnep(x)
    // device to the local bd_address
    memset (&ifr, 0, sizeof(struct ifreq));
    strcpy(ifr.ifr_name, tap_dev_name);
    err = ioctl(fd_dev, SIOCGIFHWADDR, &ifr);
    if(err < 0)
    {
        app_host_log("Could not get network hardware for interface:%s, errno:%s",
                tap_dev_name, strerror(errno));

        close(fd_socket);
        close(fd_dev);

        return -1;
    }

    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
    memcpy(ifr.ifr_hwaddr.sa_data, network_address, sizeof(bd_addr_t));
    if (ioctl(fd_socket, SIOCSIFHWADDR, &ifr) == -1) {
        close(fd_dev);
        close(fd_socket);
        app_host_log("TAP: Error setting hw addr: %s\n", strerror(errno));
        exit(1);
        return -1;
    }

    // Bring the interface up
    if (ioctl(fd_socket, SIOCGIFFLAGS, &ifr) == -1) {
        close(fd_dev);
        close(fd_socket);
        app_host_log("TAP: Error reading interface flags: %s\n", strerror(errno));
        return -1;
    }

    if ((ifr.ifr_flags & IFF_UP) == 0) {
        ifr.ifr_flags |= IFF_UP;

        if (ioctl(fd_socket, SIOCSIFFLAGS, &ifr) == -1) {
            close(fd_dev);
            close(fd_socket);
            app_host_log("TAP: Error set IFF_UP: %s\n", strerror(errno));
            return -1;
        }
    }

    close(fd_socket);

    tap_fd = fd_dev;
    app_host_log("BNEP device \"%s\" allocated", tap_dev_name);

    if(btstack_network_pan_start())
    {
        return -1;
    }
#else
    UNUSED(network_address);
#endif

    return 0;
}

/**
 * @brief Get network name after network was activated
 * @note e.g. tapX on Linux, might not be useful on all platforms
 * @returns network name
 */
const char * btstack_network_get_name(void)
{
#ifdef __linux
    return tap_dev_name;
#else
    return NULL;
#endif
}

/**
 * @brief Bring up network interface
 * @param network_address
 * @return 0 if ok
 */
int btstack_network_down(void)
{
    app_host_log("BNEP channel closed");
#ifdef __linux
    if (tap_fd >= 0)
    {
        close(tap_fd);
        tap_fd = -1;
    }

    btstack_network_pan_stop();
#endif
    return 0;
}

/**
 * @brief Receive packet on network interface, e.g., forward packet to TCP/IP stack
 * @param packet
 * @param size
 */
void btstack_network_process_packet(bd_addr_t dst, bd_addr_t src, uint16_t protocol, const uint8_t * packet, uint16_t size)
{
#ifdef __linux
    struct ether_header ehdr;

    memcpy(&ehdr.ether_dhost, dst, ETH_ALEN);
    memcpy(&ehdr.ether_shost, src, ETH_ALEN);
    ehdr.ether_type = htons(protocol);

    memcpy(pan_ind_buf, &ehdr, sizeof(ehdr));
    pan_ind_len = sizeof(ehdr);
    memcpy(pan_ind_buf + pan_ind_len, packet, size);
    pan_ind_len += size;

    app_host_log("write to TAP device %d \n", pan_ind_len);
    //scru_dump_hex((uint8_t *)(pan_ind_buf), "To TAP:", pan_ind_len, 0, 0);

    if (tap_fd < 0) return;
    // Write out the ethernet frame to the tap device

    int rc = write(tap_fd, pan_ind_buf, pan_ind_len);
    if (rc < 0) {
        app_host_log("TAP: Could not write to TAP device: %s", strerror(errno));
    } else
    if (rc != size) {
        app_host_log("TAP: Package written only partially %d of %d bytes", rc, size);
    }
#else
    UNUSED(dst);
    UNUSED(src);
    UNUSED(protocol);
    UNUSED(packet);
    UNUSED(size);
#endif
}

/**
 * @brief Notify network interface that packet from send_packet_callback was sent and the next packet can be delivered.
 */
void btstack_network_packet_sent(void)
{
#ifdef __linux
    sem_post(&tap_queue_sem);

    // Re-enable the tap device data source
    //btstack_run_loop_enable_data_source_callbacks(&tap_dev_ds, DATA_SOURCE_CALLBACK_READ);
#endif
}

/*
 * @section Network packet handler
 *
 * @text A pointer to the network packet is stored and a BNEP_EVENT_CAN_SEND_NOW requested
 */
/* LISTING_START(networkPacketHandler): Network Packet Handler */
static void app_host_panu_send_packet(bd_addr_t dst, bd_addr_t src, uint16_t protocol, const uint8_t * packet, uint16_t size)
{
    wiced_hci_bt_panu_send_data_t *p_data = (wiced_hci_bt_panu_send_data_t *)malloc(sizeof(wiced_hci_bt_panu_send_data_t) + size);

    if(p_data)
    {
        p_data->handle = panu_handle;
        memcpy(p_data->dst, dst, BDA_LEN);
        memcpy(p_data->src, src, BDA_LEN);
        p_data->protocol = protocol;
        p_data->data_len = size;
        memcpy(p_data->data, packet, size);

        wiced_hci_panu_send_data(p_data);

        free(p_data);
    }
}

void app_host_panu_init(void)
{
    btstack_network_init(&app_host_panu_send_packet);
}

bool app_host_panu_connect(uint8_t bda[6])
{
    wiced_hci_bt_panu_connect_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_panu_handle != WICED_NULL_HANDLE))
    {
        app_host_log("PANU already connected p_dev->m_panu_handle = 0x%x", p_dev->m_panu_handle);
        is_panu_connected = true;
        return false;
    }

    app_host_log("Sending PANU Connect");

    memcpy(data.bda, bda, BDA_LEN);

    return wiced_hci_panu_connect(&data);
}

bool app_host_panu_disconnect(uint8_t bda[6])
{
    wiced_hci_bt_panu_disconnect_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);

    if (NULL == p_dev)
        return false;

    if (WICED_NULL_HANDLE == (data.handle = p_dev->m_panu_handle))
        return false;


    app_host_log("Sending PANU Disconnect Command, Handle: 0x%04x", data.handle);
    return wiced_hci_panu_disconnect(&data);
}


void app_host_panu_event(uint16_t opcode, uint8_t * p_data, uint32_t len)
{
    uint8_t    bda[6] = {0};
    wiced_hci_bt_device_t *device = 0;
    uint16_t  handle = 0;
    int       i = 0;
    unsigned char * p_rx_data = NULL;
    uint32_t rx_data_len = 0;

    app_host_log("app_host_panu_event opcode = 0x%04x",opcode);

    switch (opcode)
    {
        case HCI_CONTROL_PANU_EVENT_CONNECTED :
        {
            bd_addr_t local_addr = {0};//{0xBE, 0x50, 0x0A, 0x11, 0x00, 0x02};
            for (i = 0; i < 6; i++)
            {
                bda[5 - i] = p_data[i];
                local_addr[i] = p_data[i+6];
            }

            handle = (uint16_t)(p_data[12] + (p_data[13] << 8));

            app_host_log("PANU connected %02x:%02x:%02x:%02x:%02x:%02x handle %04x",
                bda[0], bda[1], bda[2], bda[3], bda[4], bda[5], handle);

            app_host_log("PANU local BDA %02x:%02x:%02x:%02x:%02x:%02x handle %04x",
                local_addr[0], local_addr[1], local_addr[2], local_addr[3], local_addr[4], local_addr[5], handle);

            // find device in the list with received address and save the connection handle
            if ((device = app_host_find_device(bda)) == 0)
                device = app_host_add_device(bda);

            device->m_panu_handle = handle;
            device->m_conn_type |= WICED_CONNECTION_TYPE_PANU;
            is_panu_connected = true;
            panu_handle = handle;
            btstack_network_up(local_addr);
        }
        break;

        case HCI_CONTROL_PANU_EVENT_SERVICE_NOT_FOUND:
        {
            handle = (uint16_t)(p_data[0] | (p_data[1] << 8));
            app_host_log("PANU SERVICE NOT FOUND, Handle: 0x%04x", handle);
            device = app_host_find_device_by_connection(WICED_CONNECTION_TYPE_PANU, handle);
            if (device)
            {
                device->m_panu_handle = WICED_NULL_HANDLE;
                device->m_conn_type &= ~WICED_CONNECTION_TYPE_PANU;
            }
            is_panu_connected = false;
        }
        break;

        case HCI_CONTROL_PANU_EVENT_CONNECTION_FAILED:
        {
            handle = (uint16_t)(p_data[0] | (p_data[1] << 8));
            app_host_log("PANU connect failed, Handle: 0x%04x", handle);
            device = app_host_find_device_by_connection(WICED_CONNECTION_TYPE_PANU, handle);
            if (device)
            {
                device->m_panu_handle = WICED_NULL_HANDLE;
                device->m_conn_type &= ~WICED_CONNECTION_TYPE_PANU;
            }
            is_panu_connected = false;
        }
        break;

        case HCI_CONTROL_PANU_EVENT_DISCONNECTED:
        {
            handle = (uint16_t)(p_data[0] | (p_data[1] << 8));
            app_host_log("PANU disconnected, Handle: 0x%04x", handle);
            device = app_host_find_device_by_connection(WICED_CONNECTION_TYPE_PANU, handle);
            if (device)
            {
                device->m_panu_handle = WICED_NULL_HANDLE;
                device->m_conn_type &= ~WICED_CONNECTION_TYPE_PANU;
            }
            is_panu_connected = false;
            panu_handle = 0;
            btstack_network_down();
        }
        break;

        case HCI_CONTROL_PANU_EVENT_RECV_DATA:
        {
            uint8_t *p = p_data, ext = 0, forward = 0;
            bd_addr_t src, dst;
            uint16_t protocol, data_len;

            //scru_dump_hex((uint8_t *)(p_data), "From CHIP:", len, 0, 0);

            STREAM_TO_UINT16(handle, p);
            STREAM_TO_BDADDR(src, p);
            STREAM_TO_BDADDR(dst, p);
            STREAM_TO_UINT16(protocol, p);
            STREAM_TO_UINT16(data_len, p);
            //STREAM_TO_UINT8(ext, p);
            //STREAM_TO_UINT8(forward, p);

            UNUSED(ext);
            UNUSED(forward);

            app_host_log("PANU recevied data protocol: %d, length: %d", protocol, data_len);

            btstack_network_process_packet(dst, src, protocol, p, data_len);
        }
        break;

        case HCI_CONTROL_PANU_EVENT_TX_FLOW_CB:
        {
            handle = (uint16_t)(p_data[0] | (p_data[1] << 8));
            app_host_log("PANU Tx Flow: %d", p_data[2]);
        }
        break;

        case HCI_CONTROL_PANU_EVENT_SEND_DATA_COMP:
        {
            handle = (uint16_t)(p_data[0] | (p_data[1] << 8));
            app_host_log("PANU Tx completed: %d", handle);

            btstack_network_packet_sent();
        }
        break;
    }

    script_handle_event(opcode, is_panu_connected, handle, p_rx_data, rx_data_len);
    app_host_handle_event(opcode, p_data, len);
}
