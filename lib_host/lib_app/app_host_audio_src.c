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

bool app_host_audio_src_connect(wiced_hci_bt_audio_source_connect_data_t *p_data)
{
    wiced_hci_bt_device_t* p_dev = app_host_find_device(p_data->bda);
    if(p_dev && (p_dev->m_audio_handle != WICED_NULL_HANDLE))
    {
        app_host_log("AV SRC already connected");
        return false;
    }

    app_host_log("Sending AV Connect");

    return wiced_hci_audio_src_connect(p_data);
}

bool app_host_audio_src_disconnect(uint8_t bda[6])
{
    wiced_hci_bt_audio_source_disconnect_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(p_dev && (p_dev->m_audio_handle != WICED_NULL_HANDLE))
    {
        app_host_log("Sending AV disconnect");
        data.handle = p_dev->m_audio_handle;
        p_dev->m_audio_handle = WICED_NULL_HANDLE;
        p_dev->m_conn_type &= ~WICED_CONNECTION_TYPE_AUDIO;
        return wiced_hci_audio_src_disconnect(&data);
    }

    app_host_log("Device not connected as AV");
    return false;
}

bool app_host_audio_src_start(uint8_t bda[6], uint8_t sample_freq, uint8_t audio_mode)
{
    wiced_hci_bt_audio_source_start_data_t start;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(!p_dev || (p_dev && (p_dev->m_audio_handle == WICED_NULL_HANDLE)))
    {
        app_host_log("AV not connected");
        return false;
    }

    app_host_log("Sending Audio Start Handle: 0x%04x", p_dev->m_audio_handle);

    start.audio_mode = audio_mode;
    start.sample_freq = sample_freq;
    start.handle = p_dev->m_audio_handle;

    return wiced_hci_audio_src_audio_start(&start);
}


bool app_host_audio_src_stop(uint8_t bda[6])
{
    wiced_hci_bt_audio_source_stop_data_t data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device(bda);
    if(!p_dev || (p_dev && (p_dev->m_audio_handle == WICED_NULL_HANDLE)))
    {
        app_host_log("AV not connected");
        return false;
    }

    app_host_log("Sending Audio stop: 0x%04x", p_dev->m_audio_handle);

    data.handle = p_dev->m_audio_handle;

    return wiced_hci_audio_src_audio_stop(&data);
}

bool app_host_audio_src_audio_data(uint16_t handle, uint8_t *p_data, uint16_t len)
{
    wiced_hci_bt_audio_source_audio_data_t audio_data;
    wiced_hci_bt_device_t* p_dev = app_host_find_device_by_connection(WICED_CONNECTION_TYPE_AUDIO, handle);

    if(!p_dev || (p_dev && (p_dev->m_audio_handle == WICED_NULL_HANDLE)))
    {
        app_host_log("AV not connected");
        return false;
    }

    audio_data.p_data = p_data;
    audio_data.len = len;

    return wiced_hci_audio_src_audio_data(&audio_data);
}

bool app_host_audio_src_audio_data_format(uint16_t handle, uint8_t format, uint8_t audio_route)
{
    wiced_hci_bt_audio_source_audio_data_format_t payload;
    wiced_hci_bt_device_t* p_dev = app_host_find_device_by_connection(WICED_CONNECTION_TYPE_AUDIO, handle);

    if(!p_dev || (p_dev && (p_dev->m_audio_handle == WICED_NULL_HANDLE)))
    {
        app_host_log("AV not connected");
        return false;
    }

    payload.format = format;
    payload.audio_route = audio_route;
    return wiced_hci_audio_src_audio_data_format(&payload);
}

// Handle WICED HCI events for AV
void app_host_audio_src_event(uint16_t opcode, uint8_t * p_data, uint32_t len)
{
    uint8_t    bda[6];
    wiced_hci_bt_device_t *device = 0;
    uint16_t  handle = 0;
    int i;
    uint8_t data1, data2;

    UNUSED(len);

    switch (opcode)
    {
        // AV Src connected
        case HCI_CONTROL_AUDIO_EVENT_CONNECTED:
        {
            for (i = 0; i < 6; i++)
                bda[5 - i] = p_data[i];

            data1 = p_data[i++];
            data2 = p_data[i++];
            handle = (uint16_t)(data1 + (data2 << 8));

            // find device in the list with received address and save the connection handle
            if ((device = app_host_find_device(bda)) == 0)
                device = app_host_add_device(bda);

            device->m_audio_handle = handle;
            device->m_conn_type |= WICED_CONNECTION_TYPE_AUDIO;

            app_host_log("Audio Connected, Handle: 0x%04x", handle);

        }
        break;

        // AV Src disconnected
        case HCI_CONTROL_AUDIO_EVENT_DISCONNECTED:
        {
            handle = (uint16_t)(p_data[0] | (p_data[1] << 8));
            device = app_host_find_device_by_connection(WICED_CONNECTION_TYPE_AUDIO, handle);
            if(device)
            {
                device->m_audio_handle = WICED_NULL_HANDLE;
                device->m_conn_type &= ~WICED_CONNECTION_TYPE_AUDIO;
            }

            app_host_log("Audio disconnected, Handle: 0x%04x", handle);
        }
        break;

        // Streaming started
        case HCI_CONTROL_AUDIO_EVENT_STARTED:
        {
            handle = (uint16_t)(p_data[0] | (p_data[1] << 8));
            device = app_host_find_device_by_connection(WICED_CONNECTION_TYPE_AUDIO, handle);
            if(device)
                device->m_device_state.audio_src_state.audio_started = true;
            app_host_log("Audio started");

        }
        break;

        // Streaming stopped
        case HCI_CONTROL_AUDIO_EVENT_STOPPED:
        {
            handle = (uint16_t)(p_data[0] | (p_data[1] << 8));
            app_host_log("Audio stopped");
            device = app_host_find_device_by_connection(WICED_CONNECTION_TYPE_AUDIO, handle);
            if(device)
                device->m_device_state.audio_src_state.audio_started = false;
        }
        break;

        // Embedded app requested audio data
        case HCI_CONTROL_AUDIO_EVENT_REQUEST_DATA:
            break;

        case HCI_CONTROL_AUDIO_EVENT_COMMAND_COMPLETE:
            break;

        case HCI_CONTROL_AUDIO_EVENT_COMMAND_STATUS:
            break;

        case HCI_CONTROL_AUDIO_EVENT_CONNECTION_FAILED:
            break;

        default:
            break;
     }

}
