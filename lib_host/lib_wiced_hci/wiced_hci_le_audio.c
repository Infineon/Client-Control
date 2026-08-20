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

#include "wiced_types.h"
#include "hci_control_api.h"
#include "wiced_hci_le_audio.h"
#include "wiced_bt_defs.h"
#include "wiced_hci.h"
#include <string.h>


bool wiced_hci_le_audio_get_media_players(wiced_bt_le_audio_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->conn_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_GET_MEDIA_PLAYERS, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_set_media_player(wiced_hci_bt_le_audio_set_media_player_t *p_data)
{
    uint8_t    cmd[50];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->conn_id);
    UINT8_TO_STREAM(p_cmd, p_data->len);
    ARRAY_TO_STREAM(p_cmd, p_data->player_name, p_data->len);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_SET_MEDIA_PLAYER, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_play(uint16_t conn_id, uint8_t ch_cnt, wiced_bt_le_audio_config_t *p_audio_config)
{
    uint8_t    cmd[20];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, conn_id);
    UINT8_TO_STREAM(p_cmd, ch_cnt);
    if(p_audio_config)
    {
        UINT32_TO_STREAM(p_cmd, p_audio_config->sampling_frequency);
        UINT32_TO_STREAM(p_cmd, p_audio_config->frame_duration);
        UINT32_TO_STREAM(p_cmd, p_audio_config->sdu_interval);
        UINT16_TO_STREAM(p_cmd, p_audio_config->ocpf);
    }

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_PLAY, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_pause(uint16_t conn_id)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, conn_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_PAUSE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_volume_up(wiced_bt_le_audio_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->conn_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_VOL_UP, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_volume_down(wiced_bt_le_audio_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->conn_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_VOL_DOWN, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_mute(wiced_bt_le_audio_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->conn_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_MUTE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_unmute(wiced_bt_le_audio_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->conn_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_UNMUTE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_set_volume(wiced_hci_bt_le_audio_vol_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->conn_id);
    UINT8_TO_STREAM(p_cmd, p_data->vol);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_ABS_VOL, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_unmute_relative_vol_down(wiced_bt_le_audio_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->conn_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_UNMUTE_VOL_DOWN, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_broadcast_sink_play_pause(uint16_t listen, uint8_t *broadcast_code)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, listen);

    if(broadcast_code)
    {
        ARRAY_TO_STREAM(p_cmd, broadcast_code, 16);
    }
    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_BROADCAST_SINK_LISTEN_TO_BROADCAST, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_unmute_relative_vol_up(wiced_bt_le_audio_cmd_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->conn_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_UNMUTE_VOL_UP, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_call_control_generate_call(wiced_hci_bt_call_data_t *p_data, uint8_t in_ch_cnt, uint8_t out_ch_cnt, wiced_bt_le_audio_config_t *p_in_audio_config, wiced_bt_le_audio_config_t *p_out_audio_config)
{
    uint8_t    cmd[80];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->conn_id);
    UINT8_TO_STREAM(p_cmd, p_data->uri_len);
    ARRAY_TO_STREAM(p_cmd, p_data->call_URI, p_data->uri_len);
    UINT8_TO_STREAM(p_cmd, p_data->fri_name_len);
    ARRAY_TO_STREAM(p_cmd, p_data->friendly_name, p_data->fri_name_len);

    UINT8_TO_STREAM(p_cmd, in_ch_cnt);
    if(p_in_audio_config)
    {
        UINT32_TO_STREAM(p_cmd, p_in_audio_config->sampling_frequency);
        UINT32_TO_STREAM(p_cmd, p_in_audio_config->frame_duration);
        UINT32_TO_STREAM(p_cmd, p_in_audio_config->sdu_interval);
        UINT16_TO_STREAM(p_cmd, p_in_audio_config->ocpf);
    }

     UINT8_TO_STREAM(p_cmd, out_ch_cnt);
    if(p_out_audio_config)
    {
        UINT32_TO_STREAM(p_cmd, p_out_audio_config->sampling_frequency);
        UINT32_TO_STREAM(p_cmd, p_out_audio_config->frame_duration);
        UINT32_TO_STREAM(p_cmd, p_out_audio_config->sdu_interval);
        UINT16_TO_STREAM(p_cmd, p_out_audio_config->ocpf);
    }

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_PLACE_CALL, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_set_rmt_call_hold(uint8_t call_id)
{
    uint8_t    cmd[30];
    uint8_t     *p_cmd = cmd;

    UINT8_TO_STREAM(p_cmd, call_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_REM_HOLD_CALL, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_set_rmt_hold_retrieve(uint8_t call_id)
{
    uint8_t    cmd[30];
    uint8_t     *p_cmd = cmd;

    UINT8_TO_STREAM(p_cmd, call_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_REM_HOLD_RETRIEVE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_call_control_handle_call_action(wiced_bt_ga_tbs_call_control_point_t *p_data)
{
    uint8_t   cmd[50];
    uint8_t   *p_cmd = cmd;
    uint16_t opcode = p_data->opcode;

    UINT16_TO_STREAM(p_cmd, p_data->conn_id);
    UINT8_TO_STREAM(p_cmd, p_data->call_id);

    return wiced_hci_send_command(opcode, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_call_control_originate_call(uint16_t conn_id, uint8_t *p_call_uri, uint8_t uri_len)
{
    uint8_t    cmd[50];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, conn_id);
    UINT8_TO_STREAM(p_cmd, uri_len);
    ARRAY_TO_STREAM(p_cmd, p_call_uri, uri_len);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_ORIGINATE_CALL, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_call_control_terminate_call(wiced_bt_ga_tbs_call_control_point_t *p_data)
{
    uint8_t    cmd[50];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->conn_id);
    UINT8_TO_STREAM(p_cmd, p_data->termination_data.call_id);
    UINT8_TO_STREAM(p_cmd, p_data->termination_data.termination_reason);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_TERMINATE_CALL, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_broadcast_source_start_streaming(wiced_bt_ga_broadcast_start_stop_streaming_t *p_data)
{
    uint8_t    cmd[50];
    uint8_t     *p_cmd = cmd;

    UINT8_TO_STREAM(p_cmd, p_data->start);
    UINT32_TO_STREAM(p_cmd, p_data->codec_config);
    UINT8_TO_STREAM(p_cmd, p_data->enable_encryption);
    UINT32_TO_STREAM(p_cmd, p_data->channel_counts);
    UINT32_TO_STREAM(p_cmd, p_data->broadcast_id);
    ARRAY_TO_STREAM(p_cmd, p_data->broadcast_code, 16);
    UINT8_TO_STREAM(p_cmd, p_data->bis_count);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_BROADCAST_SOURCE_START_STREAMIMG, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_broadcast_sink_find_source(uint8_t start)
{
    uint8_t    cmd[5];
    uint8_t     *p_cmd = cmd;

    UINT8_TO_STREAM(p_cmd, start);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_BROADCAST_SINK_FIND_SOURCES, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_broadcast_assistant_scan_source(uint8_t start)
{
    uint8_t    cmd[5];
    uint8_t     *p_cmd = cmd;

    UINT8_TO_STREAM(p_cmd, start);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_BROADCAST_ASSISTANT_SCAN_SOURCE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_broadcast_sink_get_streams(uint32_t broadcast_id)
{
    uint8_t    cmd[5];
    uint8_t     *p_cmd = cmd;

    UINT32_TO_STREAM(p_cmd, broadcast_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_GET_BIS_INFO, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_broadcast_sink_cancel_pa_sync(void)
{
    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_CANCEL_PA_SYNC, NULL, 0);
}

bool wiced_hci_broadcast_sink_terminate_pa_sync(uint32_t broadcast_id)
{
    uint8_t    cmd[5];
    uint8_t     *p_cmd = cmd;

    UINT32_TO_STREAM(p_cmd, broadcast_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_TERMINATE_PA_SYNC, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_broadcast_sink_sync_to_stream(uint8_t listen, uint8_t *broadcast_code, uint32_t broadcast_id, uint32_t bis_index_bits)
{
    uint8_t    cmd[64] = {0};
    uint8_t     *p_cmd = cmd;

    UINT8_TO_STREAM(p_cmd, listen);
    UINT32_TO_STREAM(p_cmd, broadcast_id);
    UINT32_TO_STREAM(p_cmd, bis_index_bits);
    UINT8_TO_STREAM(p_cmd, (broadcast_code)?1:0);
    if(broadcast_code)
    {
        ARRAY_TO_STREAM(p_cmd, broadcast_code, 16);
    }

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_BROADCAST_SINK_SYNC_TO_SOURCES, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_broadcast_assistant_select_source(uint8_t listen, uint16_t conn_id, uint8_t *broadcast_code, uint32_t broadcast_id, uint8_t use_past)
{
    uint8_t    cmd[50];
    uint8_t     *p_cmd = cmd;

    UINT8_TO_STREAM(p_cmd, listen);
    UINT16_TO_STREAM(p_cmd, conn_id);
    UINT8_TO_STREAM(p_cmd, use_past);
    UINT32_TO_STREAM(p_cmd, broadcast_id);
    UINT8_TO_STREAM(p_cmd, (broadcast_code)?1:0);
    if(broadcast_code)
    {
        ARRAY_TO_STREAM(p_cmd, broadcast_code, 16);
    }

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_BROADCAST_ASSISTANT_SELECT_SOURCE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_start_voice_capture(uint8_t start, uint16_t conn_id, uint8_t ch_cnt, wiced_bt_le_audio_config_t *p_audio_config)
{
    uint8_t    cmd[20];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, conn_id);
    UINT8_TO_STREAM(p_cmd, start);
    UINT8_TO_STREAM(p_cmd, ch_cnt);
    if(p_audio_config)
    {
        UINT32_TO_STREAM(p_cmd, p_audio_config->sampling_frequency);
        UINT32_TO_STREAM(p_cmd, p_audio_config->frame_duration);
        UINT32_TO_STREAM(p_cmd, p_audio_config->sdu_interval);
        UINT16_TO_STREAM(p_cmd, p_audio_config->ocpf);
    }

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_START_STOP_MIC, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_start_stop_bidir_mic(uint8_t start, uint16_t conn_id, uint8_t in_ch_cnt, uint8_t out_ch_cnt, wiced_bt_le_audio_config_t *p_in_audio_config, wiced_bt_le_audio_config_t *p_out_audio_config)
{
    uint8_t    cmd[40];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, conn_id);
    UINT8_TO_STREAM(p_cmd, start);
    UINT8_TO_STREAM(p_cmd, in_ch_cnt);
    if(p_in_audio_config)
    {
        UINT32_TO_STREAM(p_cmd, p_in_audio_config->sampling_frequency);
        UINT32_TO_STREAM(p_cmd, p_in_audio_config->frame_duration);
        UINT32_TO_STREAM(p_cmd, p_in_audio_config->sdu_interval);
        UINT16_TO_STREAM(p_cmd, p_in_audio_config->ocpf);
    }
    UINT8_TO_STREAM(p_cmd, out_ch_cnt);
    if(p_out_audio_config)
    {
        UINT32_TO_STREAM(p_cmd, p_out_audio_config->sampling_frequency);
        UINT32_TO_STREAM(p_cmd, p_out_audio_config->frame_duration);
        UINT32_TO_STREAM(p_cmd, p_out_audio_config->sdu_interval);
        UINT16_TO_STREAM(p_cmd, p_out_audio_config->ocpf);
    }

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_CONVO_STREAM_START_STOP, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_mics_mute(uint16_t conn_id, uint8_t mute)
{
    uint8_t    cmd[5];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, conn_id);
    UINT8_TO_STREAM(p_cmd, mute);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_MICP_MUTE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_mics_aics_mute(uint16_t conn_id, uint32_t instance, uint8_t mute)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, conn_id);
    UINT8_TO_STREAM(p_cmd, mute);
    UINT32_TO_STREAM(p_cmd, instance);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_MICP_AICS_MUTE, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_set_mics_aics_gain(uint16_t conn_id, uint32_t instance, uint8_t opcode, uint8_t gain)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, conn_id);
    UINT8_TO_STREAM(p_cmd, opcode);
    UINT32_TO_STREAM(p_cmd, instance);
    UINT8_TO_STREAM(p_cmd, gain);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_MICP_AICS_SET_GAIN, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_hap_read_preset_record(uint16_t conn_id)
{
    uint8_t cmd[5];
    uint8_t     *p_cmd = cmd;
    UINT16_TO_STREAM(p_cmd, conn_id);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_HAS_READ_PRESET, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_hap_set_active_preset(uint16_t conn_id, uint8_t opcode, uint8_t preset_index)
{
    uint8_t cmd[5];
    uint8_t     *p_cmd = cmd;
    UINT16_TO_STREAM(p_cmd, conn_id);
    UINT8_TO_STREAM(p_cmd, opcode);
    UINT8_TO_STREAM(p_cmd, preset_index);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_HAS_SET_PRESET, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_hap_set_preset_name(uint16_t conn_id, uint8_t preset_index, const char *p_name)
{
    uint8_t cmd[45];
    uint8_t     *p_cmd = cmd;
    UINT16_TO_STREAM(p_cmd, conn_id);
    UINT8_TO_STREAM(p_cmd, preset_index);
    ARRAY_TO_STREAM(p_cmd, p_name, (uint8_t)strlen(p_name));

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_HAS_WRITE_PRESET_NAME, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_audio_set_lock_state(uint16_t conn_id, uint8_t lock_val)
{
    uint8_t cmd[5];
    uint8_t     *p_cmd = cmd;
    UINT16_TO_STREAM(p_cmd, conn_id);
    UINT8_TO_STREAM(p_cmd, lock_val);

    return wiced_hci_send_command(HCI_CONTROL_LE_AUDIO_COMMAND_SET_LOCK_STATE, cmd, (uint32_t)(p_cmd - cmd));
}
