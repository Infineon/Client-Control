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
#include <string.h>

bool app_host_hidh_connect(uint8_t bda[6])
{
    wiced_hci_bt_hidh_connect_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);

    if(p_dev && (p_dev->m_hidh_handle != WICED_NULL_HANDLE))
    {
        app_host_log("HID HOST already connected");
        return false;
    }
    app_host_log("Sending HID HOST Connect BdAddr:%02x:%02x:%02x:%02x:%02x:%02x",
        bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);

    memcpy(data.bda, bda, BDA_LEN);
    return wiced_hci_hidh_connect(&data);
}

bool app_host_hidh_set_report(uint16_t handle, uint8_t channel, uint8_t report_type, uint8_t report_id, char * string, uint32_t length)
{
    wiced_hci_bt_hidh_set_report_data_t data;

    data.handle = handle;
    data.channle = channel;
    data.report_type = report_type;
    data.report_id = report_id;
    data.string = string;
    data.length = length;

    app_host_log("Sending HID SetReport Handle:%d Channel:%d Type:%d Id:%d len:%d",
        handle, channel, report_type, report_id, length);

    return wiced_hci_hidh_set_report(&data);
}

bool app_host_hidh_get_report(uint16_t nHandle, uint8_t report_type, uint8_t report_id)
{
    wiced_hci_bt_hidh_get_report_data_t data;

    data.handle = nHandle;
    data.report_type = report_type;
    data.report_id = report_id;

    app_host_log("Sending HID GetReport Handle:%d Type:%d Id:%d",
        nHandle, report_type, report_id);

    return wiced_hci_hidh_get_report(&data);
}

bool app_host_hidh_disconnect(uint8_t bda[6])
{
    wiced_hci_bt_hidh_disconnect_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if (!p_dev || (p_dev->m_hidh_handle == WICED_NULL_HANDLE))
    {
        app_host_log("app_host_hidh_disconnect() device not connected");
        return false;
    }

    data.handle = p_dev->m_hidh_handle;
    return wiced_hci_hidh_disconnect(&data);
}

bool app_host_hidh_virtual_unplug(uint8_t bda[6])
{
    wiced_hci_bt_hidh_unplag_data_t data;
    memcpy(data.bda, bda, BDA_LEN);
    return wiced_hci_hidh_virtual_unplug(&data);
}

bool app_host_hidh_get_desc(uint16_t nHandle)
{
    wiced_hci_bt_hidh_get_desc_data_t data;
    data.handle = nHandle;
    return wiced_hci_hidh_get_desc(&data);
}

bool app_host_hidh_set_proto(uint16_t nHandle, uint8_t protocol)
{
    wiced_hci_bt_hidh_set_proto_data_t data;
    data.nHandle = nHandle;
    data.protocol = protocol;
    return wiced_hci_hidh_set_proto(&data);
}

bool app_host_hidh_set_wakeup_pattern(uint8_t bda[6], uint8_t report_id, uint8_t *report_pattern, uint8_t report_len)
{
    wiced_hci_bt_hidh_pattern_data_t data;
    data.report_id = report_id;
    data.report_len = report_len;
    memset(&(data.report_pattern[0]), 0, sizeof(data.report_pattern));
    memcpy(&(data.report_pattern[0]), report_pattern, report_len);
    memcpy(data.bda, bda, BDA_LEN);
    return wiced_hci_hidh_set_wakeup_pattern(&data);
}
bool app_host_hidh_set_wakeup_control(uint8_t wakeup_gpio, uint8_t wakeup_polarity, uint8_t wakeup_state)
{
    wiced_hci_bt_hidh_wakeup_data_t data;
    data.wakeup_gpio = wakeup_gpio;
    data.wakeup_polarity = wakeup_polarity;
    data.wakeup_state = wakeup_state;

    return wiced_hci_hidh_set_wakeup_control(&data);
}

void app_host_hidh_event(uint16_t opcode, uint8_t *p_data, uint32_t len)
{
    uint8_t    bda[6];
    wiced_hci_bt_device_t *device = 0;
    uint16_t  handle = 0;
    int       i = 0;

    switch (opcode)
    {
        // HIDH connected with peer
        case HCI_CONTROL_HIDH_EVENT_CONNECTED :
        {
            for (i = 0; i < 6; i++)
                bda[5 - i] = p_data[i + 1];

            handle = (uint16_t)(p_data[7] + (p_data[8] << 8));
            device = app_host_find_device(bda);

            // if connected succussfully
            if (p_data[0]==0)
            {
                // find device in the list with received address and save the connection handle
                if (device == NULL)
                    device = app_host_add_device(bda);

                device->m_hidh_handle = handle;
                device->m_conn_type |= WICED_CONNECTION_TYPE_HIDH;

                wiced_hci_bt_hidh_connected_data_t data;
                memcpy(&data.bda, bda, BDA_LEN);
                wiced_hci_hidh_connected(&data);
            }
            else if (device)
            {
                device->m_hidh_handle = WICED_NULL_HANDLE;
            }
        }
        break;

    case HCI_CONTROL_HIDH_EVENT_DISCONNECTED:
        handle = (uint16_t)(p_data[0] | (p_data[1] << 8));

        if ((device = app_host_find_device_by_connection(WICED_CONNECTION_TYPE_HIDH, handle)))
        {
            device->m_hidh_handle = WICED_NULL_HANDLE;
            device->m_conn_type &= ~WICED_CONNECTION_TYPE_HIDH;
        }
        break;
    }

    app_host_handle_event(opcode, p_data, len);
}
