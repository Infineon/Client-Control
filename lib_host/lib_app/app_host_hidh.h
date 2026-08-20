
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


#ifndef APP_HOST_HIDH_H
#define APP_HOST_HIDH_H

#include "app_host.h"
#include "wiced_hci_hidh.h"
#include "hci_control_api.h"

// HID HOST
bool app_host_hidh_connect(uint8_t bda[6]);
bool app_host_hidh_set_report(uint16_t handle, uint8_t channle, uint8_t report_type, uint8_t report_id, char * string, uint32_t length);
bool app_host_hidh_get_report(uint16_t handle, uint8_t report_type, uint8_t report_id);
bool app_host_hidh_disconnect(uint8_t bda[6]);
bool app_host_hidh_virtual_unplug(uint8_t bda[6]);
bool app_host_hidh_get_desc(uint16_t nHandle);
bool app_host_hidh_set_proto(uint16_t nHandle, uint8_t protocol);
bool app_host_hidh_set_wakeup_pattern(uint8_t bda[6], uint8_t report_id, uint8_t *report_pattern, uint8_t report_len);
bool app_host_hidh_set_wakeup_control(uint8_t wakeup_gpio, uint8_t wakeup_polarity, uint8_t m_hidh_wakeup_state);
void app_host_hidh_event(uint16_t opcode, uint8_t *p_data, uint32_t len);


#endif
