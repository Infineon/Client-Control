/*
 * (c) 2021-2026, Infineon Technologies AG, or an affiliate of Infineon
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


#include "app_host_gatt.h"
#include "app_host_le_audio.h"
#include "wiced_hci_le_audio.h"
#include "app_host.h"
#include "string.h"

bool app_host_le_audio_get_media_players(uint16_t conn_id)
{
    wiced_bt_le_audio_cmd_data_t data;
    app_host_log("Sending LE Audio get media players");
    data.conn_id = conn_id;
    return wiced_hci_le_audio_get_media_players(&data);
}

bool app_host_le_audio_set_media_player(uint16_t conn_id, uint8_t len, uint8_t* p_player_name)
{
    wiced_hci_bt_le_audio_set_media_player_t data;
    app_host_log("Sending LE Audio set media player");
    data.len = len;
    data.conn_id = conn_id;
    memcpy(data.player_name, p_player_name, len);
    return wiced_hci_le_audio_set_media_player(&data);
}

bool app_host_le_audio_play(uint16_t conn_id, uint8_t ch_cnt, wiced_bt_le_audio_config_t *p_audio_config)
{

    return wiced_hci_le_audio_play(conn_id, ch_cnt, p_audio_config);
}

bool app_host_le_audio_pause(uint16_t conn_id)
{
    return wiced_hci_le_audio_pause(conn_id);
}

bool app_host_le_audio_command(uint16_t conn_id, uint8_t cmd)
{
    wiced_bt_le_audio_cmd_data_t data;
    data.conn_id = conn_id;

    switch(cmd)
    {
    case WICED_LE_AUDIO_VOL_UP_CMD:
        return wiced_hci_le_audio_volume_up(&data);
    case WICED_LE_AUDIO_VOL_DOWN_CMD:
        return wiced_hci_le_audio_volume_down(&data);
    case WICED_LE_AUDIO_MUTE_CMD:
        return wiced_hci_le_audio_mute(&data);
    case WICED_LE_AUDIO_UNMUTE_CMD:
        return wiced_hci_le_audio_unmute(&data);
    case WICED_LE_AUDIO_UNMUTE_RELATIVE_VOL_UP_CMD:
        return wiced_hci_le_audio_unmute_relative_vol_up(&data);
    case WICED_LE_AUDIO_UNMUTE_RELATIVE_VOL_DOWN_CMD:
        return wiced_hci_le_audio_unmute_relative_vol_down(&data);
    }

    return false;
}

void app_host_le_audio_set_abs_volume(uint16_t conn_id, uint8_t vol)
{
    wiced_hci_bt_le_audio_vol_t data;
    data.conn_id = conn_id;
    data.vol = vol;
    wiced_hci_le_audio_set_volume(&data);
}

bool app_host_le_audio_generate_call(uint16_t conn_id, uint8_t uri_len, uint8_t* p_call_URI, uint8_t f_len, uint8_t* p_friendly_name, uint8_t in_ch_cnt, uint8_t out_ch_cnt, wiced_bt_le_audio_config_t *p_in_audio_config, wiced_bt_le_audio_config_t *p_out_audio_config)
{
    wiced_hci_bt_call_data_t data;
    app_host_log("Generating call from the server...");
    data.conn_id = conn_id;

    if(uri_len >= MAX_CALL_URI)
    {
       uri_len = (MAX_CALL_URI-1);
    }
    data.uri_len = uri_len;
    memcpy(data.call_URI, p_call_URI, uri_len);

    if(f_len >= MAX_CALL_FRIENDLY_NAME)
    {
       f_len = (MAX_CALL_FRIENDLY_NAME-1);
    }
    else if(f_len == 0)
    {
       f_len = strlen("Unknown");
       memcpy(p_friendly_name, "Unknown", f_len);
       app_host_log("Call friendly name NOT provided...");
    }

    data.fri_name_len = f_len;
    memcpy(data.friendly_name, p_friendly_name, f_len);

    return wiced_hci_call_control_generate_call(&data, in_ch_cnt, out_ch_cnt, p_in_audio_config, p_out_audio_config);
}

bool app_host_le_audio_handle_call_action(uint16_t conn_id, uint8_t call_id,wiced_bt_ga_tbs_call_action_t action)
{
    wiced_bt_ga_tbs_call_control_point_t data;
    uint16_t opcode = WICED_BT_GA_CCP_UNKNOWN_OPCODE;

    switch (action)
    {
    case WICED_BT_GA_CCP_ACTION_ACCEPT_CALL:
        app_host_log("Accepting the call... conn id %d call_id %d ", conn_id, call_id);
        opcode = HCI_CONTROL_LE_AUDIO_COMMAND_ACCEPT_CALL;
        break;
    case WICED_BT_GA_CCP_ACTION_HOLD_CALL:
        app_host_log("call on hold... conn id %d call_id %d ", conn_id, call_id);
        opcode = HCI_CONTROL_LE_AUDIO_COMMAND_HOLD_CALL;
        break;
    case WICED_BT_GA_CCP_ACTION_RETRIEVE_CALL:
        app_host_log("call retrieved... conn id %d call_id %d ", conn_id, call_id);
        opcode = HCI_CONTROL_LE_AUDIO_COMMAND_RETRIEVE_CALL;
        break;
    case WICED_BT_GA_CCP_ACTION_JOIN_CALL:
        app_host_log("call join... conn id %d call_id %d ", conn_id, call_id);
        opcode = HCI_CONTROL_LE_AUDIO_COMMAND_JOIN_CALL;
        break;
    default:
        app_host_log("Unknown opcode %d",opcode);
        break;
    }
    data.conn_id = conn_id;
    data.opcode = opcode;
    data.call_id = call_id;

    return wiced_hci_call_control_handle_call_action(&data);
}

bool app_host_le_audio_originate_call(uint16_t conn_id, uint8_t *p_call_uri, uint8_t uri_len)
{
    return wiced_hci_call_control_originate_call(conn_id, p_call_uri, uri_len);
}

bool app_host_le_audio_terminate_call(uint16_t conn_id, uint8_t call_id, bool is_reject)
{
    wiced_bt_ga_tbs_call_control_point_t data;
    app_host_log("terminating the call...");
    data.conn_id = conn_id;
    data.termination_data.call_id = call_id;
    if (is_reject)
        data.termination_data.termination_reason = WICED_BT_GA_TBS_REMOTE_CALL_END;
    else
        data.termination_data.termination_reason = WICED_BT_GA_TBS_SERVER_CALL_END;

    return wiced_hci_call_control_terminate_call(&data);
}

bool app_host_le_audio_broadcast_source_start_streaming(uint8_t start,
                                                        uint32_t codec_config,
                                                        uint8_t bis_count,
                                                        uint32_t num_channels,
                                                        uint8_t encryption,
                                                        uint32_t broadcast_id,
                                                        uint8_t *broadcast_code)
{
    wiced_bt_ga_broadcast_start_stop_streaming_t data = {0};

    data.start = start;
    data.codec_config = codec_config;
    data.channel_counts = num_channels;
    data.bis_count = bis_count;
    data.enable_encryption = encryption;
	data.broadcast_id =broadcast_id;

    if(broadcast_code)
    {
        memcpy(data.broadcast_code, broadcast_code, 16);
    }

    return wiced_hci_broadcast_source_start_streaming(&data);
}

bool app_host_le_audio_broadcast_sink_find_sources(uint8_t start)
{
    return wiced_hci_broadcast_sink_find_source(start);
}

bool app_host_le_audio_broadcast_assistant_scan_source(uint8_t start)
{
    return wiced_hci_broadcast_assistant_scan_source(start);
}

bool app_host_le_audio_broadcast_sink_get_streams(uint32_t broadcast_id)
{
    return wiced_hci_broadcast_sink_get_streams(broadcast_id);
}

bool app_host_le_audio_broadcast_sink_cancel_pa_sync(void)
{
    return wiced_hci_broadcast_sink_cancel_pa_sync();
}

bool app_host_le_audio_broadcast_sink_terminate_pa_sync(uint32_t broadcast_id)
{
    return wiced_hci_broadcast_sink_terminate_pa_sync(broadcast_id);
}

bool app_host_le_audio_broadcast_sink_sync_to_stream(uint8_t listen, uint8_t *broadcast_code, uint32_t broadcast_id, uint32_t bis_index_bits)
{
    return wiced_hci_broadcast_sink_sync_to_stream(listen, broadcast_code, broadcast_id, bis_index_bits);
}

bool app_host_le_audio_broadcast_assistant_select_source(uint8_t listen, uint16_t conn_id, uint8_t *broadcast_code, uint32_t broadcast_id, uint8_t use_past)
{
    app_host_log("ADD/Remove source to delegator");
    return wiced_hci_broadcast_assistant_select_source(listen, conn_id, broadcast_code, broadcast_id, use_past);
}

bool app_host_le_audio_start_voice_capture(uint8_t start, uint16_t conn_id, uint8_t ch_cnt, wiced_bt_le_audio_config_t *p_audio_config)
{
    app_host_log("Start/Stop Voice Capture");
    return wiced_hci_le_audio_start_voice_capture(start, conn_id, ch_cnt, p_audio_config);
}

bool app_host_le_audio_start_stop_bidir_mic(uint8_t start, uint16_t conn_id, uint8_t in_ch_cnt, uint8_t out_ch_cnt, wiced_bt_le_audio_config_t *p_in_audio_config, wiced_bt_le_audio_config_t *p_out_audio_config)
{
    app_host_log("Start/Stop Bidir MIC");
    return wiced_hci_le_audio_start_stop_bidir_mic(start, conn_id, in_ch_cnt, out_ch_cnt, p_in_audio_config, p_out_audio_config);
}

bool app_host_le_audio_mics_mute(uint16_t conn_id, uint8_t mute)
{
    return wiced_hci_le_audio_mics_mute(conn_id, mute);
}

bool app_host_le_audio_mics_aics_mute(uint16_t conn_id, uint32_t instance, uint8_t mute)
{
     return wiced_hci_le_audio_mics_aics_mute(conn_id, instance,  mute);
}

bool app_host_le_audio_increase_mics_aics_gain(uint16_t conn_id, uint32_t instance)
{
    return wiced_hci_le_audio_set_mics_aics_gain(conn_id, instance, APP_HOST_LE_AUDIO_INCREASE_GAIN, 0);
}

bool app_host_le_audio_decrease_mics_aics_gain(uint16_t conn_id, uint32_t instance)
{
    return wiced_hci_le_audio_set_mics_aics_gain(conn_id, instance, APP_HOST_LE_AUDIO_DECREASE_GAIN, 0);
}

bool app_host_le_audio_set_mics_aics_gain(uint16_t conn_id, uint32_t instance, uint8_t gain)
{
    return wiced_hci_le_audio_set_mics_aics_gain(conn_id, instance, APP_HOST_LE_AUDIO_SET_GAIN, gain);
}

bool app_host_le_audio_hap_read_preset(uint16_t conn_id)
{
    return wiced_hci_le_audio_hap_read_preset_record(conn_id);
}

bool app_host_le_audio_hap_set_active_preset(uint16_t conn_id, uint8_t preset_index)
{
    return wiced_hci_le_audio_hap_set_active_preset(conn_id, APP_HOST_LE_AUDIO_SET_ACTIVE_PRESET, preset_index);
}

bool app_host_le_audio_hap_set_preset_name(uint16_t conn_id, uint8_t preset_index, const char *p_name)
{
    return wiced_hci_le_audio_hap_set_preset_name(conn_id, preset_index, p_name);
}

bool app_host_le_audio_hap_set_next_preset(uint16_t conn_id)
{
    return wiced_hci_le_audio_hap_set_active_preset(conn_id, APP_HOST_LE_AUDIO_SET_NEXT_PRESET, 0);
}

bool app_host_le_audio_hap_set_previous_preset(uint16_t conn_id)
{
    return wiced_hci_le_audio_hap_set_active_preset(conn_id, APP_HOST_LE_AUDIO_SET_PREVIOUS_PRESET, 0);
}

bool app_host_le_audio_set_lock_state(uint16_t conn_id, uint8_t lock_val)
{
    return wiced_hci_le_audio_set_lock_state(conn_id, lock_val);
}
