
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

#ifndef WICED_HCI_GATT_H
#define WICED_HCI_GATT_H

#include "wiced_types.h"


// WICED HCI command
bool wiced_hci_send_command(uint16_t command, uint8_t * payload, uint32_t len);

typedef struct
{
    uint8_t address_type;
    uint8_t address[BDA_LEN];
} wiced_hci_bt_gatt_connect_data_t;

extern bool wiced_hci_gatt_connect(wiced_hci_bt_gatt_connect_data_t * data);

typedef struct
{
    uint8_t address_type;
    uint8_t address[BDA_LEN];
} wiced_hci_bt_gatt_cancel_connect_data_t;

extern bool wiced_hci_gatt_cancel_connect(wiced_hci_bt_gatt_cancel_connect_data_t * data);

typedef struct
{
    uint16_t conn_handle;
} wiced_hci_bt_gatt_le_disconnect_data_t;

extern bool wiced_hci_gatt_le_disconnect(wiced_hci_bt_gatt_le_disconnect_data_t * data);

typedef struct
{
    uint16_t con_handle;
} wiced_hci_gatt_disc_data_t;

bool wiced_hci_gatt_disc_services(wiced_hci_gatt_disc_data_t * data);


typedef struct
{
    uint32_t s_handle;
    uint32_t e_handle;
    uint16_t conn_handle;
} wiced_hci_bt_gatt_disc_chars_data_t;
bool wiced_hci_gatt_disc_chars(wiced_hci_bt_gatt_disc_chars_data_t * data);

typedef struct
{
    uint32_t s_handle;
    uint32_t e_handle;
    uint16_t conn_handle;
} wiced_hci_bt_gatt_disc_desc_data_t;
bool wiced_hci_gatt_disc_desc(wiced_hci_bt_gatt_disc_desc_data_t * data);

typedef struct
{
    uint16_t conn_handle;
    uint32_t hdlc;
    uint8_t * str;
    uint32_t num_bytes;
} wiced_hci_bt_gatt_send_not_data_t;

bool wiced_hci_gatt_send_notif(wiced_hci_bt_gatt_send_not_data_t * data);

typedef struct
{
    uint16_t conn_handle;
    uint32_t hdlc;
    uint8_t * value;
    uint32_t num_bytes;
} wiced_hci_bt_gatt_send_indicate_data_t;

bool wiced_hci_gatt_send_indicate(wiced_hci_bt_gatt_send_indicate_data_t * data);

typedef struct
{
    uint16_t conn_handle;
    uint32_t hdlc;
    uint32_t num_bytes;
    uint8_t * value;
} wiced_hci_bt_gatt_write_char_data_t;

bool wiced_hci_gatt_write_char(wiced_hci_bt_gatt_write_char_data_t * data);
bool wiced_hci_gatt_write_char_norspn(wiced_hci_bt_gatt_write_char_data_t * data);

typedef struct
{
    uint16_t conn_handle;
    uint32_t hdlc;
} wiced_hci_bt_gatt_read_char_data_t;

bool wiced_hci_gatt_read_char(wiced_hci_bt_gatt_read_char_data_t * data);

bool wiced_hci_gatt_write_response(uint8_t * p_data);
bool wiced_hci_gatt_read_response(uint8_t * p_data);
bool wiced_hci_gatt_start_stop_advert(bool start_stop, uint8_t pairing_method, uint8_t tx_power);

#endif // WICED_HCI_H
