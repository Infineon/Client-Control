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


#include "app_host.h"


void app_host_ans_event(uint16_t opcode, uint8_t * p_data, uint32_t len)
{
    switch (opcode)
    {
    case HCI_CONTROL_ANS_EVENT_ANS_ENABLED:
        {
            if(len == 2)
            {
                g_app.m_ans_supported_new_alerts = (uint16_t)(p_data[0] | (p_data[1] << 8));
                g_app.m_ans_supported_unread_alerts = (uint16_t)(p_data[0] | (p_data[1] << 8));
                g_app.m_ans_supported_alerts = (uint16_t)(p_data[0] | (p_data[1] << 8));

                app_host_log("HCI_CONTROL_ANS_EVENT_ANS_ENABLED, ID %d", g_app.m_ans_supported_alerts);
            }
        }
        break;
    case HCI_CONTROL_ANS_EVENT_CONNECTION_UP:
        g_app.m_ans_connected = true;
        app_host_log("HCI_CONTROL_ANS_EVENT_CONNECTION_UP");
        break;
    case HCI_CONTROL_ANS_EVENT_CONNECTION_DOWN:
        g_app.m_ans_connected = false;
        app_host_log("HCI_CONTROL_ANS_EVENT_CONNECTION_DOWN");
        break;

    case HCI_CONTROL_ANS_EVENT_COMMAND_STATUS:

        break;
    }

}


void app_host_anc_event(uint16_t opcode, uint8_t * p_data, uint32_t len)
{
    uint8_t status = 0;
    uint8_t cmd_id = 0;
    uint8_t category_id = 0;
    UNUSED(len);

    switch (opcode)
    {
    case HCI_CONTROL_ANC_EVENT_ANC_ENABLED:
        app_host_log("HCI_CONTROL_ANC_EVENT_ANC_ENABLED");
        g_app.m_anc_connected = true;
        break;

    case HCI_CONTROL_ANC_EVENT_SERVER_SUPPORTED_NEW_ALERTS:
        status = p_data[0];
        g_app.m_anc_server_supported_new_alerts = (uint16_t)(p_data[1] | (p_data[2] << 8));
        app_host_log("HCI_CONTROL_ANC_EVENT_SERVER_SUPPORTED_NEW_ALERTS, %d", g_app.m_anc_server_supported_new_alerts);
        break;

    case HCI_CONTROL_ANC_EVENT_SERVER_SUPPORTED_UNREAD_ALERTS:
        status = p_data[0];
        g_app.m_anc_server_supported_unread_alerts = (uint16_t)(p_data[1] | (p_data[2] << 8));
        app_host_log("HCI_CONTROL_ANC_EVENT_SERVER_SUPPORTED_UNREAD_ALERTS, %d", g_app.m_anc_server_supported_unread_alerts);
        break;

    case HCI_CONTROL_ANC_EVENT_CONTROL_ALERTS:
        status = p_data[0];
        cmd_id = p_data[1];
        category_id = p_data[2];
        app_host_log("HCI_CONTROL_ANC_EVENT_CONTROL_ALERTS, cmd_id %d, category_id %d", cmd_id, category_id);
        break;

    case HCI_CONTROL_ANC_EVENT_ENABLE_NEW_ALERTS:
        status = p_data[0];
        g_app.m_b_new_alerts_enabled = true;
        app_host_log("HCI_CONTROL_ANC_EVENT_ENABLE_NEW_ALERTS");
        break;

    case HCI_CONTROL_ANC_EVENT_DISABLE_NEW_ALERTS:
        status = p_data[0];
        g_app.m_b_new_alerts_enabled = false;
        app_host_log("HCI_CONTROL_ANC_EVENT_DISABLE_NEW_ALERTS");
        break;

    case HCI_CONTROL_ANC_EVENT_ENABLE_UNREAD_ALERTS:
        status = p_data[0];
        g_app.m_b_unread_alerts_enabled = true;
        app_host_log("HCI_CONTROL_ANC_EVENT_ENABLE_UNREAD_ALERTS");
        break;

    case HCI_CONTROL_ANC_EVENT_DISABLE_UNREAD_ALERTS:
        status = p_data[0];
        g_app.m_b_unread_alerts_enabled = false;
        app_host_log("HCI_CONTROL_ANC_EVENT_DISABLE_UNREAD_ALERTS");
        break;

    case HCI_CONTROL_ANC_EVENT_ANC_DISABLED:
        g_app.m_anc_connected = false;
        app_host_log("HCI_CONTROL_ANC_EVENT_ANC_DISABLED");
        break;

    case HCI_CONTROL_ANC_EVENT_COMMAND_STATUS:

        break;
    }

    UNUSED(status);

}
