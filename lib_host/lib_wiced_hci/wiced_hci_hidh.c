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

#include "wiced_hci_hidh.h"
#include "hci_control_api.h"
#include "stdio.h"
#include "wiced_hci.h"

bool wiced_hci_hidh_connect(wiced_hci_bt_hidh_connect_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    BDADDR_TO_STREAM(p_cmd, p_data->bda);

    return wiced_hci_send_command(HCI_CONTROL_HIDH_COMMAND_CONNECT, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_hidh_set_report(wiced_hci_bt_hidh_set_report_data_t *p_data)
{
    uint8_t cmd[1000];
    uint8_t * p_cmd = &(cmd[0]);
    uint8_t * p;
    int value;
    int rv;

    UINT16_TO_STREAM(p_cmd, p_data->handle);
    UINT8_TO_STREAM(p_cmd,  p_data->channle);
    UINT8_TO_STREAM(p_cmd,  p_data->report_type);
    UINT8_TO_STREAM(p_cmd,  p_data->report_id);

    p = (uint8_t *) p_data->string;

    while (*p != 0)
    {
        rv = sscanf((char *)p, "%02x", &value);
        if (rv == 1)
        {
            UINT8_TO_STREAM(p_cmd, value);
            p += 2;
            if (*p == ' ')
                p++;
        }
        else
            break;
    }

    return wiced_hci_send_command(HCI_CONTROL_HIDH_COMMAND_SET_REPORT, cmd, (uint32_t)(p_cmd - cmd));
}


bool wiced_hci_hidh_get_report(wiced_hci_bt_hidh_get_report_data_t *p_data)
{
    uint8_t cmd[1000];
    uint8_t * p_cmd = &(cmd[0]);

    UINT16_TO_STREAM(p_cmd, p_data->handle);
    UINT8_TO_STREAM(p_cmd, p_data->report_type);
    UINT8_TO_STREAM(p_cmd, p_data->report_id);
    UINT16_TO_STREAM(p_cmd, 0x0000);    // Length (unused in this demo app)

    return wiced_hci_send_command(HCI_CONTROL_HIDH_COMMAND_GET_REPORT, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_hidh_disconnect(wiced_hci_bt_hidh_disconnect_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_HIDH_COMMAND_DISCONNECT, cmd, (uint32_t)(p_cmd - cmd));
}


bool wiced_hci_hidh_connected(wiced_hci_bt_hidh_connected_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    BDADDR_TO_STREAM(p_cmd, p_data->bda);

    return wiced_hci_send_command(HCI_CONTROL_HIDH_COMMAND_ADD, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_hidh_virtual_unplug(wiced_hci_bt_hidh_unplag_data_t * data)
{
    uint8_t  cmd[6];
    uint8_t *p_cmd = cmd;

    BDADDR_TO_STREAM(p_cmd, data->bda);

    wiced_hci_send_command(HCI_CONTROL_HIDH_COMMAND_REMOVE, cmd, (uint32_t)(p_cmd - cmd));
    return wiced_hci_send_command(HCI_CONTROL_BATT_CLIENT_COMMAND_REMOVE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_hidh_get_desc(wiced_hci_bt_hidh_get_desc_data_t *data)
{
    uint8_t cmd[2];
    uint8_t *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, data->handle);
    return wiced_hci_send_command(HCI_CONTROL_HIDH_COMMAND_GET_DESCRIPTOR, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_hidh_set_proto(wiced_hci_bt_hidh_set_proto_data_t * data)
{
    uint8_t   cmd[2 + 1];
    uint8_t *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, data->nHandle);
    UINT8_TO_STREAM(p_cmd, data->protocol);

    return wiced_hci_send_command(HCI_CONTROL_HIDH_COMMAND_SET_PROTOCOL, cmd, (uint32_t)(p_cmd - cmd));
}


bool wiced_hci_hidh_set_wakeup_pattern(wiced_hci_bt_hidh_pattern_data_t * data)
{
    uint8_t command[200] = { 0 };
    uint32_t commandBytes = 0;
    int i = 0;

    for (i = 0; i < 6; i++)
        command[commandBytes++] = data->bda[5 - i];
    command[commandBytes++] = 1; // Add
    command[commandBytes++] = data->report_id;
    for (i = 0; i < data->report_len; i++)
        command[commandBytes++] = data->report_pattern[i];

    return wiced_hci_send_command(HCI_CONTROL_HIDH_COMMAND_WAKEUP_PATTERN_SET, command, commandBytes);
}


bool wiced_hci_hidh_set_wakeup_control(wiced_hci_bt_hidh_wakeup_data_t * data)
{
    uint8_t command[200] = { 0 };
    uint32_t commandBytes = 0;

    command[commandBytes++] = data->wakeup_state;
    command[commandBytes++] = data->wakeup_gpio;
    command[commandBytes++] = data->wakeup_polarity;

    return wiced_hci_send_command(HCI_CONTROL_HIDH_COMMAND_WAKEUP_CONTROL, command, commandBytes);
}
