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
#include <memory.h>
#include "wiced_hci_gatt.h"
#include "hci_control_api.h"

bool wiced_hci_gatt_connect(wiced_hci_bt_gatt_connect_data_t * data)
{
    uint8_t command[20];
    uint32_t    commandBytes = 0;
    int i = 0;

    command[commandBytes++] = data->address_type;

    for (i = 0; i < 6; i++)
        command[commandBytes++] = data->address[5 - i];

    return wiced_hci_send_command(HCI_CONTROL_LE_COMMAND_CONNECT, command, commandBytes);
}


bool wiced_hci_gatt_cancel_connect(wiced_hci_bt_gatt_cancel_connect_data_t * data)
{
    // send command to connect
    uint8_t command[20] = {0};
    uint32_t    commandBytes = 0;
    int i = 0;

    // type and BDADDR
    command[commandBytes++] = data->address_type;

    for (i = 0; i < 6; i++)
        command[commandBytes++] = data->address[5 - i];

   return wiced_hci_send_command(HCI_CONTROL_LE_COMMAND_CANCEL_CONNECT, command, 7);
}

bool wiced_hci_gatt_le_disconnect(wiced_hci_bt_gatt_le_disconnect_data_t * data)
{
    uint8_t   command[3] = {0};
    uint32_t    commandBytes = 0;

    command[commandBytes++] = data->conn_handle & 0xff;
    command[commandBytes++] = (data->conn_handle >> 8) & 0xff;
    return wiced_hci_send_command(HCI_CONTROL_LE_COMMAND_DISCONNECT, command, commandBytes);
}

bool wiced_hci_gatt_disc_services(wiced_hci_gatt_disc_data_t * data)
{
    uint8_t command[8] = { 0 };
    uint32_t    commandBytes = 0;
    command[commandBytes++] = data->con_handle & 0xff;
    command[commandBytes++] = (data->con_handle >> 8) & 0xff;
    command[commandBytes++] = 1;         // start handle
    command[commandBytes++] = 0;
    command[commandBytes++] = 0xff;      // end handle
    command[commandBytes++] = 0xff;
    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_DISCOVER_SERVICES, command, commandBytes);
}

bool wiced_hci_gatt_disc_chars(wiced_hci_bt_gatt_disc_chars_data_t * data)
{
    uint8_t   command[20]  = { 0 };
    uint32_t    commandBytes = 0;

    command[commandBytes++] = data->conn_handle & 0xff;
    command[commandBytes++] = (data->conn_handle >> 8) & 0xff;
    command[commandBytes++] = data->s_handle & 0xff;       // start handle
    command[commandBytes++] = (data->s_handle >> 8) & 0xff;
    command[commandBytes++] = data->e_handle & 0xff;       // end handle
    command[commandBytes++] = (data->e_handle >> 8) & 0xff;

    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_DISCOVER_CHARACTERISTICS, command, commandBytes);
}

bool wiced_hci_gatt_disc_desc(wiced_hci_bt_gatt_disc_desc_data_t * data)
{
    uint8_t   command[20]  = { 0 };
    uint32_t    commandBytes = 0;

    command[commandBytes++] = data->conn_handle & 0xff;
    command[commandBytes++] = (data->conn_handle >> 8) & 0xff;
    command[commandBytes++] = data->s_handle & 0xff;       // start handle
    command[commandBytes++] = (data->s_handle >> 8) & 0xff;
    command[commandBytes++] = data->e_handle & 0xff;       // end handle
    command[commandBytes++] = (data->e_handle >> 8) & 0xff;
    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_DISCOVER_DESCRIPTORS, command, commandBytes);
}

bool wiced_hci_gatt_send_notif(wiced_hci_bt_gatt_send_not_data_t * data)
{
    static uint8_t  command[32]  = { 0 };
    uint32_t    commandBytes = 0;
    bool rval = true;
    static unsigned char prev_send = 0;
    command[commandBytes++] = data->conn_handle & 0xff;
    command[commandBytes++] = (data->conn_handle >> 8) & 0xff;
    command[commandBytes++] = (uint8_t)(data->hdlc & 0xff);
    command[commandBytes++] = (uint8_t)((data->hdlc >> 8) & 0xff);
#ifdef REPEAT_NOTIFICATIONS_FOREVER
    if (sending_notifications)
    {
        command[commandBytes] = prev_send++;
    }
    else
#endif
    {
        memcpy(&(command[commandBytes]), data->str, data->num_bytes);
      //  QString str = ui->edtBLEHandleValue->text();
        //num_bytes = GetHexValue(&command[commandBytes], sizeof(command) - commandBytes, data->str);
        prev_send = command[commandBytes] + 1;
    }
    rval = wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_NOTIFY, command, commandBytes + data->num_bytes);
#ifdef REPEAT_NOTIFICATIONS_FOREVER
    sending_notifications = TRUE;
#else
    UNUSED(prev_send);
#endif
    return rval;
}

bool wiced_hci_gatt_send_indicate(wiced_hci_bt_gatt_send_indicate_data_t * data)
{
    uint8_t   command[32]  = { 0 };
    uint32_t    commandBytes = 0;

    command[commandBytes++] = data->conn_handle & 0xff;
    command[commandBytes++] = (data->conn_handle >> 8) & 0xff;
    command[commandBytes++] = (uint8_t)(data->hdlc & 0xff);
    command[commandBytes++] = (uint8_t)((data->hdlc >> 8) & 0xff);
    memcpy(&(command[commandBytes]), data->value, data->num_bytes);

    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_INDICATE, command, commandBytes + data->num_bytes);
}

static bool do_gatt_write(wiced_hci_bt_gatt_write_char_data_t * data, uint16_t opcode)
{
    uint8_t   command[32] = { 0 };
    uint32_t    commandBytes = 0;

    command[commandBytes++] = data->conn_handle & 0xff;
    command[commandBytes++] = (data->conn_handle >> 8) & 0xff;
    command[commandBytes++] = (uint8_t)(data->hdlc & 0xff);
    command[commandBytes++] = (uint8_t)((data->hdlc >> 8) & 0xff);
    memcpy(&command[commandBytes], data->value, data->num_bytes);
    return wiced_hci_send_command(opcode, command, commandBytes + data->num_bytes);
}

bool wiced_hci_gatt_write_char(wiced_hci_bt_gatt_write_char_data_t * data)
{
    return do_gatt_write(data, HCI_CONTROL_GATT_COMMAND_WRITE_REQUEST);
}

bool wiced_hci_gatt_write_char_norspn(wiced_hci_bt_gatt_write_char_data_t * data)
{
    return do_gatt_write(data, HCI_CONTROL_GATT_COMMAND_WRITE_COMMAND);
}

bool wiced_hci_gatt_read_char(wiced_hci_bt_gatt_read_char_data_t * data)
{
    uint32_t    commandBytes = 0;
    uint8_t   command[32] = { 0 };

    command[commandBytes++] = data->conn_handle & 0xff;
    command[commandBytes++] = (data->conn_handle >> 8) & 0xff;
    command[commandBytes++] = (uint8_t)(data->hdlc & 0xff);
    command[commandBytes++] = (uint8_t)((data->hdlc >> 8) & 0xff);

    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_READ_REQUEST, command, commandBytes);
}

bool wiced_hci_gatt_write_response(uint8_t * p_data)
{
    // send 1 byte result code 0
    p_data[4] = 0;
    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_WRITE_RESPONSE, p_data, 5);
}

bool wiced_hci_gatt_read_response(uint8_t * p_data)
{
    // send 1 byte response with value 100;
    p_data[4] = 100;
    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_READ_RESPONSE, p_data, 5);
}
