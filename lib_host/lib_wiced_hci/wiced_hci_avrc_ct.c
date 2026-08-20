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

#include "wiced_hci.h"
#include "hci_control_api.h"


bool wiced_hci_avrc_ct_connect(wiced_bt_avrc_ct_connect_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    BDADDR_TO_STREAM(p_cmd, p_data->bda);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_CONNECT, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_disconnect(wiced_bt_avrc_ct_disconnect_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_DISCONNECT, cmd, (uint32_t)(p_cmd - cmd));

}

bool wiced_hci_avrc_ct_play(wiced_bt_avrc_ct_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_PLAY, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_stop(wiced_bt_avrc_ct_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_STOP, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_pause(wiced_bt_avrc_ct_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_PAUSE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_next_track(wiced_bt_avrc_ct_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_NEXT_TRACK, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_previous_track(wiced_bt_avrc_ct_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_PREVIOUS_TRACK, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_volume_up(wiced_bt_avrc_ct_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_VOLUME_UP, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_volume_down(wiced_bt_avrc_ct_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_VOLUME_DOWN, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_mute(wiced_bt_avrc_ct_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_MUTE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_repeat(wiced_bt_avrc_ct_settings_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);
    UINT8_TO_STREAM(p_cmd, p_data->setting);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_SET_REPEAT_MODE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_shuffle(wiced_bt_avrc_ct_settings_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);
    UINT8_TO_STREAM(p_cmd, p_data->setting);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_SET_SHUFFLE_MODE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_volume_level(wiced_bt_avrc_ct_volume_level_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);
    UINT8_TO_STREAM(p_cmd, p_data->volume_level);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_VOLUME_LEVEL, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_skip_forward_pressed(wiced_bt_avrc_ct_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_BEGIN_FAST_FORWARD, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_skip_forward_released(wiced_bt_avrc_ct_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_END_FAST_FORWARD, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_skip_backward_pressed(wiced_bt_avrc_ct_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_BEGIN_REWIND, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_ct_skip_backward_released(wiced_bt_avrc_ct_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_END_REWIND, cmd, (uint32_t)(p_cmd - cmd));
}

// Simulate button press on stero headphone
// (implementation is embedded application dependent)
bool wiced_hci_avrc_ct_button_press()
{
    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_BUTTON_PRESS, 0, 0);
}

// Simulate long button press on stero headphone (press and hold)
// (implementation is embedded application dependent)
bool wiced_hci_avrc_ct_long_button_press()
{
    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_LONG_BUTTON_PRESS, 0, 0);
}

bool wiced_hci_avrc_unit_info(wiced_bt_avrc_ct_cmd_unit_info_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_UNIT_INFO, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_avrc_sub_unit_info(wiced_bt_avrc_ct_cmd_sub_unit_info_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);

    return wiced_hci_send_command(HCI_CONTROL_AVRC_CONTROLLER_COMMAND_SUB_UNIT_INFO, cmd, (uint32_t)(p_cmd - cmd));
}
