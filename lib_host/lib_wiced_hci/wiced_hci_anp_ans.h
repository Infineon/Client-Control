
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
 * Definitions for WICED HCI AG
 */

#ifndef WICED_HCI_ANP_ANS_H
#define WICED_HCI_ANP_ANS_H

// ANP
#define ANP_ALERT_CATEGORY_ID_SIMPLE_ALERT          0
#define ANP_ALERT_CATEGORY_ID_EMAIL                 1
#define ANP_ALERT_CATEGORY_ID_NEWS                  2
#define ANP_ALERT_CATEGORY_ID_CALL                  3
#define ANP_ALERT_CATEGORY_ID_MISSED_CALL           4
#define ANP_ALERT_CATEGORY_ID_SMS_OR_MMS            5
#define ANP_ALERT_CATEGORY_ID_VOICE_MAIL            6
#define ANP_ALERT_CATEGORY_ID_SCHEDULE_ALERT        7
#define ANP_ALERT_CATEGORY_ID_HIGH_PRI_ALERT        8
#define ANP_ALERT_CATEGORY_ID_INSTANT_MESSAGE       9

#define ANP_NOTIFY_CATEGORY_COUNT                   (ANP_ALERT_CATEGORY_ID_INSTANT_MESSAGE + 1)

/* Special value used by only Alert notification
 * client in notify immedietely control
 */
#define ANP_ALERT_CATEGORY_ID_ALL_CONFIGURED        0xff

// ANS
typedef struct
{
    uint16_t alert;
} wiced_hci_bt_anp_alert_data_t;

bool wiced_hci_ans_command_set_supported_new_alert_category(wiced_hci_bt_anp_alert_data_t *p_data);
bool wiced_hci_ans_command_set_supported_unread_alert_category(wiced_hci_bt_anp_alert_data_t *p_data);


typedef struct
{
    uint8_t alert_category;
} wiced_hci_bt_anp_alert_category_data_t;

bool wiced_hci_ans_command_generate_alert(wiced_hci_bt_anp_alert_category_data_t *p_data);
bool wiced_hci_ans_command_clear_alert(wiced_hci_bt_anp_alert_category_data_t *p_data);

// ANC

bool wiced_hci_anc_command_read_server_supported_new_alerts();
bool wiced_hci_anc_command_read_server_supported_unread_alerts();

#define ANP_ALERT_CONTROL_CMD_ENABLE_NEW_ALERTS                    0
#define ANP_ALERT_CONTROL_CMD_ENABLE_UNREAD_STATUS                 1
#define ANP_ALERT_CONTROL_CMD_DISABLE_NEW_ALERTS                   2
#define ANP_ALERT_CONTROL_CMD_DISABLE_UNREAD_ALERTS                3
#define ANP_ALERT_CONTROL_CMD_NOTIFY_NEW_ALERTS_IMMEDIATE          4
#define ANP_ALERT_CONTROL_CMD_NOTIFY_UNREAD_ALERTS_IMMEDIATE       5

#define ANP_ALERT_CONTROL_CMD_COUNT   (ANP_ALERT_CONTROL_CMD_NOTIFY_UNREAD_ALERTS_IMMEDIATE + 1)


typedef struct
{
    uint8_t cmd_id;
    uint8_t alert_category;
} wiced_hci_bt_anc_control_alert_data_t;

bool wiced_hci_anc_command_control_alerts(wiced_hci_bt_anc_control_alert_data_t *p_data);
bool wiced_hci_anc_enable_new_alerts();
bool wiced_hci_anc_enable_unread_alerts();
bool wiced_hci_anc_disable_new_alerts();
bool wiced_hci_anc_disable_unread_alerts();

#endif
