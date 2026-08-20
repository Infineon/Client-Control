
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

#ifndef WICED_HCI_HIDD_H
#define WICED_HCI_HIDD_H

#include "wiced_types.h"


// WICED HCI command
bool wiced_hci_send_command(uint16_t command, uint8_t * payload, uint32_t len);

// HID device
typedef struct
{
	uint8_t channel;
	uint8_t report_type;
	uint8_t * report;
	uint8_t report_len;
} wiced_hci_bt_hidd_report_t;

typedef struct
{
    uint8_t pairing_mode;
} wiced_hci_bt_hidd_paring_mode_data_t;


typedef struct
{
	uint8_t cap_lock;
	uint8_t ctrl_key;
	uint8_t alt_key;
	uint8_t buffer[6];
	uint8_t btn_up;
} wiced_hci_bt_hidd_send_key_data_t;

typedef struct
{
	uint8_t cap_lock;
	uint8_t ctrl_key;
	uint8_t alt_key;
} wiced_hci_bt_hidd_cap_lock_data_t;

bool wiced_hci_hidd_connect();
bool wiced_hci_hidd_disconnect();
bool wiced_hci_hidd_send_report(wiced_hci_bt_hidd_report_t * data);
bool wiced_hci_hidd_pairing_mode(wiced_hci_bt_hidd_paring_mode_data_t * data);
bool wiced_hci_hidd_get_host_info();
bool wiced_hci_bt_hidd_send_key(wiced_hci_bt_hidd_send_key_data_t *data);
bool wiced_hci_bt_hidd_cap_lock(wiced_hci_bt_hidd_cap_lock_data_t *data);
bool wiced_hci_bt_hidd_virtual_unplug();
bool wiced_hci_hidd_key(uint8_t key, uint8_t keyDown);


#endif // WICED_HCI_H
