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


bool wiced_hci_ans_command_set_supported_new_alert_category(wiced_hci_bt_anp_alert_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->alert);

    return wiced_hci_send_command(HCI_CONTROL_ANS_COMMAND_SET_SUPPORTED_NEW_ALERT_CATEGORIES, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_ans_command_set_supported_unread_alert_category(wiced_hci_bt_anp_alert_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->alert);

    return wiced_hci_send_command(HCI_CONTROL_ANS_COMMAND_SET_SUPPORTED_UNREAD_ALERT_CATEGORIES, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_ans_command_generate_alert(wiced_hci_bt_anp_alert_category_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT8_TO_STREAM(p_cmd, p_data->alert_category);

    return wiced_hci_send_command(HCI_CONTROL_ANS_COMMAND_GENERATE_ALERT, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_ans_command_clear_alert(wiced_hci_bt_anp_alert_category_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT8_TO_STREAM(p_cmd, p_data->alert_category);

    return wiced_hci_send_command(HCI_CONTROL_ANS_COMMAND_CLEAR_ALERT, cmd, (uint32_t)(p_cmd - cmd));
}

/********************* ANC *******************************/

bool wiced_hci_anc_command_read_server_supported_new_alerts()
{
    return wiced_hci_send_command(HCI_CONTROL_ANC_COMMAND_READ_SERVER_SUPPORTED_NEW_ALERTS, 0, 0);
}

bool wiced_hci_anc_command_read_server_supported_unread_alerts()
{
    return wiced_hci_send_command(HCI_CONTROL_ANC_COMMAND_READ_SERVER_SUPPORTED_UNREAD_ALERTS, 0, 0);
}

bool wiced_hci_anc_command_control_alerts(wiced_hci_bt_anc_control_alert_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT8_TO_STREAM(p_cmd, p_data->cmd_id);
    UINT8_TO_STREAM(p_cmd, p_data->alert_category);

    return wiced_hci_send_command(HCI_CONTROL_ANC_COMMAND_CONTROL_ALERTS, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_anc_enable_new_alerts()
{
    return wiced_hci_send_command(HCI_CONTROL_ANC_COMMAND_ENABLE_NEW_ALERTS, 0, 0);
}

bool wiced_hci_anc_enable_unread_alerts()
{
    return wiced_hci_send_command(HCI_CONTROL_ANC_COMMAND_ENABLE_UNREAD_ALERTS, 0, 0);
}

bool wiced_hci_anc_disable_new_alerts()
{
    return wiced_hci_send_command(HCI_CONTROL_ANC_COMMAND_DISABLE_NEW_ALERTS, 0, 0);
}

bool wiced_hci_anc_disable_unread_alerts()
{
    return wiced_hci_send_command(HCI_CONTROL_ANC_COMMAND_DISABLE_UNREAD_ALERTS, 0, 0);
}
