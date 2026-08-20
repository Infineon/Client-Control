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


#include "app_host.h"
#include <string.h>

bool is_spp_connected = false;

bool app_host_spp_connect(uint8_t bda[6])
{
    wiced_hci_bt_spp_connect_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_spp_handle != WICED_NULL_HANDLE))
    {
        app_host_log("SPP already connected");
        is_spp_connected = true;
        return false;
    }

    app_host_log("Sending SPP Connect");

    memcpy(data.bda, bda, BDA_LEN);

    return wiced_hci_spp_connect(&data);
}

bool app_host_spp_disconnect(uint8_t bda[6])
{
    wiced_hci_bt_spp_disconnect_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);

    if (NULL == p_dev)
        return false;

    if (WICED_NULL_HANDLE == (data.handle = p_dev->m_spp_handle))
        return false;


    app_host_log("Sending SPP Disconnect Command, Handle: 0x%04x", data.handle);
    p_dev->m_spp_handle = WICED_NULL_HANDLE;
    p_dev->m_conn_type &= ~WICED_CONNECTION_TYPE_SPP;
    return wiced_hci_spp_disconnect(&data);
}


bool app_host_spp_send(uint8_t bda[6], uint8_t * buffer, uint32_t len)
{
    wiced_hci_bt_spp_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);

    if (NULL == p_dev)
        return false;

    if (WICED_NULL_HANDLE == (data.handle = p_dev->m_spp_handle))
        return false;
    data.data = buffer;
    data.length = len;
    app_host_log("Sending SPP send data Command, Handle: 0x%04x", data.handle);
    return wiced_hci_spp_send_data(&data);
}

extern void script_handle_event(uint16_t opcode, uint32_t is_connected, uint32_t port_handle, uint8_t * p_data, uint32_t len);

void app_host_spp_event(uint16_t opcode, uint8_t * p_data, uint32_t len)
{
    uint8_t    bda[6];
    wiced_hci_bt_device_t *device = 0;
    uint16_t  handle = 0;
    int       i = 0;
    unsigned char * p_rx_data = NULL;
    uint32_t rx_data_len = 0;

    switch (opcode)
    {
        // SPP connected with peer
        case HCI_CONTROL_SPP_EVENT_CONNECTED :
        {
            for (i = 0; i < 6; i++)
                bda[5 - i] = p_data[i];

            handle = (uint16_t)(p_data[6] + (p_data[7] << 8));

            app_host_log("SPP connected %02x:%02x:%02x:%02x:%02x:%02x handle %04x",
                bda[0], bda[1], bda[2], bda[3], bda[4], bda[5], handle);

            // find device in the list with received address and save the connection handle
            if ((device = app_host_find_device(bda)) == 0)
                device = app_host_add_device(bda);

            device->m_spp_handle = handle;
            device->m_conn_type |= WICED_CONNECTION_TYPE_SPP;
            is_spp_connected = true;
        }
        break;

        case HCI_CONTROL_SPP_EVENT_DISCONNECTED:
        {
            handle = (uint16_t)(p_data[0] | (p_data[1] << 8));
            app_host_log("SPP disconnected, Handle: 0x%04x", handle);
            if ((device = app_host_find_device_by_handle(handle)))
            {
                device->m_spp_handle = WICED_NULL_HANDLE;
                device->m_conn_type &= ~WICED_CONNECTION_TYPE_SPP;
            }
        }
        break;

        case HCI_CONTROL_SPP_EVENT_RX_DATA:
            handle = p_data[0] + (p_data[1] << 8);
            p_rx_data = &(p_data[2]);
            rx_data_len = len - 2;
            break;
    }

    script_handle_event(opcode, is_spp_connected, handle, p_rx_data, rx_data_len);
    app_host_handle_event(opcode, p_data, len);
}
