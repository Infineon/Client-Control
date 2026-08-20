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
#include "wiced_hci_dm.h"
#include "hci_control_api.h"

// set local device name
bool wiced_hci_dm_setdevname(wiced_hci_dm_setdevname_data_t * dev_name)
{
    uint8_t   cmd[60];
    int    commandBytes = 0;

    commandBytes = strlen(dev_name->device_name);
    memcpy(cmd, dev_name->device_name, commandBytes);

    return wiced_hci_send_command(HCI_CONTROL_COMMAND_SET_LOCAL_NAME, cmd, commandBytes);
}

bool wiced_hci_dm_setdevaddr(wiced_hci_dm_setdevaddr_data_t *device_addr)
{
    uint8_t   cmd[16];
    uint8_t *p = cmd;
    int     commandBytes = BDA_LEN;

    BDADDR_TO_STREAM(p, device_addr->device_addr);

    return wiced_hci_send_command(HCI_CONTROL_COMMAND_SET_LOCAL_BDA, cmd, commandBytes);
}

bool wiced_hic_dm_set_pairing_mode(wiced_hci_dm_set_pairing_mode_data_t * data)
{
    uint8_t pairing_mode = data->mode;
    return wiced_hci_send_command(HCI_CONTROL_COMMAND_SET_PAIRING_MODE, &pairing_mode, 1);
}

bool wiced_hci_dm_set_vis(wiced_hci_dm_set_vis_data_t* data)
{
    uint8_t   cmd[60];
    int    commandBytes = 0;

    cmd[commandBytes++] = data->discoverable ? 1:0; //discoverable
    cmd[commandBytes++] = data->connectable ? 1:0; ; //CONNECTABLE

    return wiced_hci_send_command(HCI_CONTROL_COMMAND_SET_VISIBILITY, cmd, commandBytes);
}

bool wiced_hci_dm_reset()
{
    return wiced_hci_send_command(HCI_CONTROL_COMMAND_RESET, 0, 0);
}

bool wiced_hci_dm_delete_nvram_data(wiced_hci_dm_delete_nvram_data_t *data)
{
    uint8_t cmd[10];
    cmd[0] = (uint8_t)data->nvram_id;
    cmd[1] = (uint8_t)(data->nvram_id >> 8);
    return wiced_hci_send_command(HCI_CONTROL_COMMAND_DELETE_NVRAM_DATA, cmd, 2);
}

bool wiced_hci_dm_inquiry(wiced_hci_dm_inquiry_data_t * data)
{
    uint8_t command[] = { data->start_stop ? 1 : 0 };
    return wiced_hci_send_command(HCI_CONTROL_COMMAND_INQUIRY, command, 1);
}

bool wiced_hci_dm_le_scan(wiced_hci_dm_le_set_scan_data_t * data)
{
    uint8_t  command[2];
    command[0] = data->enable ? 1 : 0;    // scan command, len 1, enable = 1, disable = 0
    command[1] = data->filter_adv ? 1 : 0; // Enable ascs filtering
    return wiced_hci_send_command(HCI_CONTROL_LE_COMMAND_SCAN, command, 2);
}

bool wiced_hci_dm_push_pairing_host_info(wiced_hic_dm_push_host_info_data_t *data)
{
    return wiced_hci_send_command(HCI_CONTROL_HIDD_COMMAND_PUSH_PAIRING_HOST_INFO, data->nvram_data, data->len);
}

bool wiced_hci_dm_push_nvram_data(wiced_hci_dm_push_nvram_data_t *data)
{
    uint8_t cmd[1000];
    uint16_t offset = 0;
    cmd[0] = (uint8_t)data->nvram_id;
    cmd[1] = (uint8_t)(data->nvram_id >> 8);
    if(data->len < 255)
    {
        memcpy(&cmd[2], data->nvram_data, data->len);
        return wiced_hci_send_command(HCI_CONTROL_COMMAND_PUSH_NVRAM_DATA, cmd, 2 + data->len);
    }

    bool res = true;
    while(data->len)
    {
        int len_to_send = (data->len > 255) ? 255 : data->len;
        cmd[2] = (uint8_t)(offset);
        cmd[3] = (uint8_t)(offset >> 8);
        memcpy(&cmd[4], data->nvram_data + offset, len_to_send);
        res = wiced_hci_send_command(HCI_CONTROL_COMMAND_PUSH_NVRAM_DATA_LONG, cmd, 4 + len_to_send);
        offset += len_to_send;
        data->len -= len_to_send;
    }
    return res;
}

bool wiced_hci_dm_hidh_add(wiced_hci_dm_bda_data_t * data)
{
    uint8_t cmd_param[6];
    cmd_param[0] = data->bda[5];
    cmd_param[1] = data->bda[4];
    cmd_param[2] = data->bda[3];
    cmd_param[3] = data->bda[2];
    cmd_param[4] = data->bda[1];
    cmd_param[5] = data->bda[0];

    // Add this device (to allow it to reconnect)
    return wiced_hci_send_command(HCI_CONTROL_HIDH_COMMAND_ADD, cmd_param, BDA_LEN);
}

bool wiced_hci_dm_add_battery_client(wiced_hci_dm_bda_data_t * data)
{
    uint8_t cmd_param[6];
    cmd_param[0] = data->bda[5];
    cmd_param[1] = data->bda[4];
    cmd_param[2] = data->bda[3];
    cmd_param[3] = data->bda[2];
    cmd_param[4] = data->bda[1];
    cmd_param[5] = data->bda[0];

    return wiced_hci_send_command(HCI_CONTROL_BATT_CLIENT_COMMAND_ADD, cmd_param, BDA_LEN);
}

bool wiced_hci_dm_add_findme_locator(wiced_hci_dm_bda_data_t * data)
{
    uint8_t cmd_param[6];
    cmd_param[0] = data->bda[5];
    cmd_param[1] = data->bda[4];
    cmd_param[2] = data->bda[3];
    cmd_param[3] = data->bda[2];
    cmd_param[4] = data->bda[1];
    cmd_param[5] = data->bda[0];

    return wiced_hci_send_command(HCI_CONTROL_FINDME_LOCATOR_COMMAND_ADD, cmd_param, BDA_LEN);
}

bool wiced_hci_dm_get_version_info()
{
    return wiced_hci_send_command(HCI_CONTROL_MISC_COMMAND_GET_VERSION, 0, 0);
}

bool wiced_hci_dm_set_app_traces(wiced_hci_dm_set_app_traces_data_t * data)
{
    uint8_t cmd[2];
    cmd[0] = data->enable ? 1 : 0; // Enable/Disable Bluetooth HCI trace
    cmd[1] = data->route;         //  Routing option

    // send command to configure traces
    return wiced_hci_send_command(HCI_CONTROL_COMMAND_TRACE_ENABLE, cmd, 2);
}

bool wiced_hci_dm_user_confirm(wiced_hci_dm_user_confirm_data_t * data)
{
    uint8_t command[20];
    int i = 0;

    for (i = 0; i < 6; i++)
        command[5 - i] = data->bda[i];

    command[6] = data->confirm ? 1 : 0; // 1 - accept, 0 - do not accept

    return wiced_hci_send_command(HCI_CONTROL_COMMAND_USER_CONFIRMATION, command, 7);
}

bool wiced_hci_dm_unbond_device(wiced_hci_dm_bda_data_t * data)
{
    uint8_t cmd_param[6];

    cmd_param[0] = data->bda[5];
    cmd_param[1] = data->bda[4];
    cmd_param[2] = data->bda[3];
    cmd_param[3] = data->bda[2];
    cmd_param[4] = data->bda[1];
    cmd_param[5] = data->bda[0];

    return wiced_hci_send_command(HCI_CONTROL_COMMAND_UNBOND_DEVICE, cmd_param, BDA_LEN);
}
