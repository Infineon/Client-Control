
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

#ifndef APP_HOST_PANU_H
#define APP_HOST_PANU_H

#include "app_host.h"
#include "wiced_hci_panu.h"
#include "hci_control_api.h"

typedef uint8_t bd_addr_t[6];


/* API_START */

/**
 * @brief Initialize network interface
 * @param send_packet_callback
 */
void btstack_network_init(void (*send_packet_callback)(bd_addr_t dst, bd_addr_t src, uint16_t protocol, const uint8_t * packet, uint16_t size));

/**
 * @brief Bring up network interfacd
 * @param network_address
 * @return 0 if ok
 */
int  btstack_network_up(bd_addr_t network_address);

/**
 * @brief Shut down network interfacd
 * @param network_address
 * @return 0 if ok
 */
int  btstack_network_down(void);

/**
 * @brief Receive packet on network interface, e.g., forward packet to TCP/IP stack
 * @param packet
 * @param size
 */
void btstack_network_process_packet(bd_addr_t dst, bd_addr_t src, uint16_t protocol, const uint8_t * packet, uint16_t size);

/**
 * @brief Notify network interface that packet from send_packet_callback was sent and the next packet can be delivered.
 */
void btstack_network_packet_sent(void);

/**
 * @brief Get network name after network was activated
 * @note e.g. tapX on Linux, might not be useful on all platforms
 * @return network name
 */
const char * btstack_network_get_name(void);

/* API_END */

void app_host_panu_event(uint16_t opcode, uint8_t * p_data, uint32_t len);
void app_host_panu_init(void);

bool app_host_panu_connect(uint8_t bda[6]);
bool app_host_panu_disconnect(uint8_t bda[6]);

#endif
