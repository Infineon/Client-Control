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

#include "wiced_hci_hidd.h"
#include "hci_control_api.h"
#include "app_host.h"
#include <string.h>
#include <stdio.h>

bool wiced_hci_hidd_disconnect()
{
    return wiced_hci_send_command(HCI_CONTROL_HIDD_COMMAND_DISCONNECT, 0, 0);
}

bool wiced_hci_hidd_connect()
{
    return wiced_hci_send_command(HCI_CONTROL_HIDD_COMMAND_CONNECT, 0, 0);
}

bool wiced_hci_hidd_send_report(wiced_hci_bt_hidd_report_t * data)
{
	uint8_t cmd[60];

	cmd[0] = data->channel;
	cmd[1] = data->report_type;
	memcpy(&(cmd[2]), data->report, data->report_len);

	return wiced_hci_send_command(HCI_CONTROL_HIDD_COMMAND_SEND_REPORT, cmd, 2 + data->report_len);
}

bool wiced_hci_hidd_pairing_mode(wiced_hci_bt_hidd_paring_mode_data_t * data)
{
    uint8_t    cmd[1];
	cmd[0] = data->pairing_mode;
    return wiced_hci_send_command(HCI_CONTROL_HIDD_COMMAND_ACCEPT_PAIRING, cmd, 1);
}

bool wiced_hci_hidd_key(uint8_t key, uint8_t keyDown)
{
    uint8_t    cmd[2];
	cmd[0] = key;
    cmd[1] = keyDown;
    return wiced_hci_send_command(HCI_CONTROL_HIDD_COMMAND_KEY, cmd, 2);
}

bool wiced_hci_bt_hidd_send_key(wiced_hci_bt_hidd_send_key_data_t * data)
{
    char buff[100];
    uint8_t  cmd[60] = { 0 };
    int i = 0;
    cmd[0] = HCI_CONTROL_HID_REPORT_CHANNEL_INTERRUPT;
    cmd[1] = HCI_CONTROL_HID_REPORT_TYPE_INPUT;
    cmd[2] = HCI_CONTROL_HID_REPORT_ID;
    cmd[3] = data->cap_lock ? 0x20 : 0x00 |
        data->ctrl_key ? 0x80 : 0x00 |
        data->alt_key ? 0x40 : 0x00;

    for (i = 0; (i < 6) && (data->buffer[i] != 0); i++)
    {
        if ((data->buffer[i] >= '1') && (data->buffer[i] <= '9'))
            cmd[5 + i] = 0x1e + data->buffer[i] - '1';
        if (data->buffer[i] == '0')
            cmd[5 + i] = 0x27;
        if ((data->buffer[i] >= 'a') && (data->buffer[i] <= 'z'))
            cmd[5 + i] = 0x04 + data->buffer[i] - 'a';
    }

    sprintf(buff, "Send key report: %02x %02x %02x %02x %02x %02x %02x %02x %02x",
       cmd[2], cmd[3], cmd[4], cmd[5], cmd[6], cmd[7], cmd[8], cmd[9], cmd[9]);
    app_host_log(buff);

	bool rval = wiced_hci_send_command(HCI_CONTROL_HIDD_COMMAND_SEND_REPORT, cmd, 11);

    if (data->btn_up)
    {
        for (i = 0; i < 7; i++)
            cmd[4 + i] = 0;
        rval &= wiced_hci_send_command(HCI_CONTROL_HIDD_COMMAND_SEND_REPORT, cmd, 11);
    }

	return rval;
}

bool wiced_hci_bt_hidd_cap_lock(wiced_hci_bt_hidd_cap_lock_data_t *data)
{
    uint8_t cmd[60] = { 0 };
    int i = 0;
    cmd[0] = HCI_CONTROL_HID_REPORT_CHANNEL_INTERRUPT;
    cmd[1] = HCI_CONTROL_HID_REPORT_TYPE_INPUT;
    cmd[2] = HCI_CONTROL_HID_REPORT_ID;
    cmd[3] = data->cap_lock ? 0x20 : 0x00 |
        data->ctrl_key ? 0x80 : 0x00 |
        data->alt_key ? 0x40 : 0x00;
    for (i = 0; i < 7; i++)
        cmd[4 + i] = 0;

    return wiced_hci_send_command(HCI_CONTROL_HIDD_COMMAND_SEND_REPORT, cmd, 11);
}

bool wiced_hci_bt_hidd_virtual_unplug()
{
    return wiced_hci_send_command(HCI_CONTROL_HIDD_COMMAND_VIRTUAL_UNPLUG, 0, 0);
}

bool wiced_hci_hidd_get_host_info()
{
    return wiced_hci_send_command(HCI_CONTROL_HIDD_COMMAND_HID_HOST_ADDR, 0, 0);
}
