
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

/*
 * Definitions for WICED HCI
 */

#ifndef WICED_HCI_HIDH_H
#define WICED_HCI_HIDH_H

#include "wiced_types.h"
#include "wiced_hci.h"

// WICED HCI command
bool wiced_hci_send_command(uint16_t command, uint8_t * payload, uint32_t len);

// HID HOST
typedef struct
{
    uint16_t handle;
    uint8_t channle;
    uint8_t report_type;
    uint8_t report_id;
    char * string;
    uint32_t length;
} wiced_hci_bt_hidh_set_report_data_t;

typedef struct
{
    uint16_t nHandle;
    uint8_t protocol;
}wiced_hci_bt_hidh_set_proto_data_t;

typedef struct
{
    uint8_t bda[BDA_LEN];
    uint8_t report_id;
    uint8_t report_len;
    uint8_t report_pattern[255];
} wiced_hci_bt_hidh_pattern_data_t;

typedef struct
{
    uint8_t wakeup_state;
    uint8_t wakeup_gpio;
    uint8_t wakeup_polarity;
} wiced_hci_bt_hidh_wakeup_data_t;


typedef struct
{
    uint16_t handle;
    uint8_t report_type;
    uint8_t report_id;
} wiced_hci_bt_hidh_get_report_data_t;

typedef wiced_hci_bt_bda_t wiced_hci_bt_hidh_connect_data_t ;
typedef wiced_hci_bt_handle_t wiced_hci_bt_hidh_disconnect_data_t;
typedef wiced_hci_bt_handle_t wiced_hci_bt_hidh_get_desc_data_t;
typedef wiced_hci_bt_bda_t  wiced_hci_bt_hidh_unplag_data_t;
typedef wiced_hci_bt_bda_t  wiced_hci_bt_hidh_connected_data_t;

bool wiced_hci_hidh_set_report(wiced_hci_bt_hidh_set_report_data_t *p_data);
bool wiced_hci_hidh_get_report(wiced_hci_bt_hidh_get_report_data_t *p_data);
bool wiced_hci_hidh_disconnect(wiced_hci_bt_hidh_disconnect_data_t *p_data);
bool wiced_hci_hidh_virtual_unplug(wiced_hci_bt_hidh_unplag_data_t * p_data);
bool wiced_hci_hidh_get_desc(wiced_hci_bt_hidh_get_desc_data_t *data);
bool wiced_hci_hidh_set_proto(wiced_hci_bt_hidh_set_proto_data_t * data);
bool wiced_hci_hidh_set_wakeup_pattern(wiced_hci_bt_hidh_pattern_data_t * data);
bool wiced_hci_hidh_set_wakeup_control(wiced_hci_bt_hidh_wakeup_data_t * data);
bool wiced_hci_hidh_connect(wiced_hci_bt_hidh_connect_data_t * data);
bool wiced_hci_hidh_connected(wiced_hci_bt_hidh_connected_data_t *data);


#endif // WICED_HCI_H
