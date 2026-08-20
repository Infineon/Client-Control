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
#include "wiced_hci_audio_src.h"
#include "wiced_hci_ag.h"
#include "app_host.h"
#include <string.h>

bool app_host_ag_connect(uint8_t bda[6])
{

    wiced_hci_bt_ag_connect_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);

    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        app_host_log("AG already connected");
        return false;
    }

    app_host_log("Sending AG Connect");

    memcpy(data.bda, bda, BDA_LEN);
    return wiced_hci_ag_connect(&data);
}


bool app_host_ag_disconnect(uint8_t bda[6])
{
    wiced_hci_bt_ag_disconnect_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        app_host_log("Sending AG disconnect");
        data.handle = p_dev->m_ag_handle;

        p_dev->m_ag_handle = WICED_NULL_HANDLE;
        p_dev->m_conn_type &= ~WICED_CONNECTION_TYPE_AG;
        p_dev->m_device_state.ag_state.ag_audio_opened = false;
        return wiced_hci_ag_disconnect(&data);
    }

    app_host_log("Device not connected as AG");
    return false;

}


bool app_host_ag_audio_open(uint8_t bda[6])
{
    wiced_hci_bt_ag_audio_open_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        app_host_log("Sending AG open");
        data.handle = p_dev->m_ag_handle;
        return wiced_hci_ag_audio_open(&data);
    }

    app_host_log("Device not connected as AG");
    return false;
}


bool app_host_ag_audio_close(uint8_t bda[6])
{
    wiced_hci_bt_ag_audio_close_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        app_host_log("Sending AG close");
        data.handle = p_dev->m_ag_handle;
        p_dev->m_device_state.ag_state.ag_audio_opened = false;
        return wiced_hci_ag_audio_close(&data);
    }

    app_host_log("Device not connected as AG");
    return false;
}

bool app_host_ag_send_clcc_response(uint8_t bda[6], uint8_t *call_id_list, uint8_t num_item)
{
    wiced_hci_bt_ag_clcc_res_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        app_host_log("Sending AG CLCL RESPONSE");
        data.num_calls = num_item;
        data.handle = p_dev->m_ag_handle;
        memcpy(data.call_list_status, call_id_list, num_item *sizeof(uint8_t));
        return wiced_hci_ag_send_clcc_response(&data);
    }

    app_host_log("Device not connected as AG");
    return false;
}

bool app_host_ag_update_cind(char *cind, uint8_t length)
{
    wiced_hci_bt_ag_cind_t data;

    app_host_log("Sending AG CIND");
    memcpy(data.cind_str, cind, length);
    data.cind_str[length]='\0';
    return wiced_hci_ag_send_cind(&data);
}

void app_host_ag_event(uint16_t opcode, uint8_t * p_data, uint32_t len)
{
    uint8_t    bda[6];
    wiced_hci_bt_device_t *device = 0;
    uint16_t  handle = 0;
    int i;

    switch (opcode)
    {
        // AG connected with peer
        case HCI_CONTROL_AG_EVENT_OPEN:
        {
            handle = (uint16_t)(p_data[0] | (p_data[1] << 8));
            if (p_data[8] == HCI_CONTROL_HF_STATUS_SUCCESS)
            {
                for (i = 0; i < 6; i++)
                    bda[5 - i] = p_data[2 + i];

                // find device in the list with received address and save the connection handle
                if ((device = app_host_find_device(bda)) == 0)
                    device = app_host_add_device(bda);

                device->m_ag_handle = handle;
                device->m_conn_type |= WICED_CONNECTION_TYPE_AG;

                device->m_device_state.ag_state.ag_audio_opened = false;

                app_host_log("HCI_CONTROL_AG_EVENT_OPEN");
            }
        }
        break;

        // AG diconnected from peer
        case HCI_CONTROL_AG_EVENT_CLOSE:
        {
            handle = (uint16_t)(p_data[0] | (p_data[1] << 8));
            device = app_host_find_device_by_connection(WICED_CONNECTION_TYPE_AG, handle);

            if(device)
            {
                device->m_ag_handle = WICED_NULL_HANDLE;
                device->m_conn_type &= ~WICED_CONNECTION_TYPE_AG;

                device->m_device_state.ag_state.ag_audio_opened = false;

                app_host_log("HCI_CONTROL_AG_EVENT_CLOSE");
            }
        }
        break;

        case HCI_CONTROL_AG_EVENT_CONNECTED:
        {
        }
        break;

        case HCI_CONTROL_AG_EVENT_AUDIO_OPEN:
        {
            handle   = (uint16_t) (p_data[0] | (p_data[1] << 8));
            device = app_host_find_device_by_connection(WICED_CONNECTION_TYPE_AG, handle);

            if(device)
            {
                device->m_device_state.ag_state.ag_audio_opened = true;
            }
            app_host_log("HCI_CONTROL_AG_EVENT_AUDIO_OPEN");
        }
        break;

        case HCI_CONTROL_AG_EVENT_AUDIO_CLOSE:
        {
            handle   = (uint16_t) (p_data[0] | (p_data[1] << 8));
            device = app_host_find_device_by_connection(WICED_CONNECTION_TYPE_AG, handle);

            if(device)
            {
                device->m_device_state.ag_state.ag_audio_opened = false;
            }
            app_host_log("HCI_CONTROL_AG_EVENT_AUDIO_CLOSE");
        }
        break;
    }

    app_host_handle_event(opcode, p_data, len);

}

bool app_host_ag_send_ciev(uint8_t bda[6], char *ciev, uint8_t length)
{
    wiced_hci_bt_ag_ciev_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        app_host_log("Sending AG CIEV %s",ciev);
        data.handle = p_dev->m_ag_handle;
        memcpy(data.ciev_str, ciev, length);
        data.ciev_str[length]='\0';
        return wiced_hci_ag_send_ciev(&data);
    }

    app_host_log("Device not connected as AG");
    return false;
}

bool app_host_ag_send_ring_cmd(uint8_t bda[6])
{
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        return wiced_hci_ag_send_ring_cmd(p_dev->m_ag_handle);
    }
    app_host_log("Device not connected as AG");
    return false;
}

bool app_host_ag_send_clip_cmd(uint8_t bda[6])
{
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        return wiced_hci_ag_send_clip_cmd(p_dev->m_ag_handle);
    }
    app_host_log("Device not connected as AG");
    return false;
}

bool app_host_ag_send_ccwa_cmd(uint8_t bda[6])
{
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        return wiced_hci_ag_send_ccwa_cmd(p_dev->m_ag_handle);
    }
    app_host_log("Device not connected as AG");
    return false;
}

bool app_host_ag_send_ok_cmd(uint8_t bda[6])
{
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        return wiced_hci_ag_send_ok_cmd(p_dev->m_ag_handle);
    }
    app_host_log("Device not connected as AG");
    return false;
}

bool app_host_ag_send_error_cmd(uint8_t bda[6])
{
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        return wiced_hci_ag_send_error_cmd(p_dev->m_ag_handle);
    }
    app_host_log("Device not connected as AG");
    return false;
}

bool app_host_ag_send_spk_vol_cmd(uint8_t bda[6], int vol)
{
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        return wiced_hci_ag_send_spk_vol_cmd(p_dev->m_ag_handle, vol);
    }
    app_host_log("Device not connected as AG");
    return false;
}

bool app_host_ag_send_mic_vol_cmd(uint8_t bda[6], int vol)
{
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_ag_handle != WICED_NULL_HANDLE))
    {
        return wiced_hci_ag_send_mic_vol_cmd(p_dev->m_ag_handle, vol);
    }
    app_host_log("Device not connected as AG");
    return false;
}
