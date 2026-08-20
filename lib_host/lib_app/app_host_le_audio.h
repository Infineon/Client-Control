
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
#include "wiced_hci_le_audio.h"
#ifndef APP_HOST_LE_AUDIO_H
#define APP_HOST_LE_AUDIO_H

#define WICED_LE_AUDIO_PLAY_CMD        1
#define WICED_LE_AUDIO_STOP_CMD        2
#define WICED_LE_AUDIO_PAUSE_CMD       3
#define WICED_LE_AUDIO_VOL_UP_CMD      4
#define WICED_LE_AUDIO_VOL_DOWN_CMD    5
#define WICED_LE_AUDIO_MUTE_CMD        6
#define WICED_LE_AUDIO_UNMUTE_CMD      7
#define WICED_LE_AUDIO_UNMUTE_RELATIVE_VOL_UP_CMD      8
#define WICED_LE_AUDIO_UNMUTE_RELATIVE_VOL_DOWN_CMD    9
#define WICED_LE_AUDIO_BROADCAST_SINK_PLAY_PAUSE_CMD         10

#define APP_HOST_LE_AUDIO_INCREASE_GAIN 1
#define APP_HOST_LE_AUDIO_DECREASE_GAIN 2
#define APP_HOST_LE_AUDIO_SET_GAIN 3

#define APP_HOST_LE_AUDIO_SET_ACTIVE_PRESET 1
#define APP_HOST_LE_AUDIO_SET_NEXT_PRESET 2
#define APP_HOST_LE_AUDIO_SET_PREVIOUS_PRESET 3

#define BDA_LEN     6       /**< Device Bluetooth Address Length */

bool app_host_le_audio_command(uint16_t conn_id, uint8_t cmd);
void app_host_le_audio_set_abs_volume(uint16_t conn_id, uint8_t vol);
void app_host_le_audio_event(uint16_t opcode, uint8_t * p_data, uint16_t len);
bool app_host_le_audio_get_media_players(uint16_t conn_id);
bool app_host_le_audio_set_media_player(uint16_t conn_id, uint8_t len, uint8_t* p_player_name);
bool app_host_le_audio_play(uint16_t conn_id, uint8_t ch_cnt, wiced_bt_le_audio_config_t *p_audio_config);
bool app_host_le_audio_pause(uint16_t conn_id);

bool app_host_le_audio_originate_call(uint16_t conn_id, uint8_t *p_call_uri, uint8_t uri_len);
bool app_host_le_audio_generate_call(uint16_t conn_id, uint8_t uri_len, uint8_t* p_call_URI,uint8_t f_len, uint8_t* p_friendly_name, uint8_t in_ch_cnt, uint8_t out_ch_cnt, wiced_bt_le_audio_config_t *p_in_audio_config, wiced_bt_le_audio_config_t *p_out_audio_config);
bool app_host_le_audio_handle_call_action(uint16_t conn_id, uint8_t call_id, wiced_bt_ga_tbs_call_action_t action);
bool app_host_le_audio_terminate_call(uint16_t conn_id, uint8_t call_id, bool is_reject);
bool app_host_le_audio_broadcast_source_start_streaming(uint8_t start,uint32_t codec_config, uint8_t bis_count, uint32_t num_channels,
                                                        uint8_t encryption,
                                                        uint32_t broadcast_id,
                                                        uint8_t *broadcast_code);
bool app_host_le_audio_broadcast_sink_find_sources(uint8_t start);
bool app_host_le_audio_broadcast_assistant_scan_source(uint8_t start);
bool app_host_le_audio_broadcast_sink_get_streams(uint32_t broadcast_id);
bool app_host_le_audio_broadcast_sink_cancel_pa_sync(void);
bool app_host_le_audio_broadcast_sink_terminate_pa_sync(uint32_t broadcast_id);
bool app_host_le_audio_broadcast_sink_sync_to_stream(uint8_t listen, uint8_t *broadcast_code, uint32_t broadcast_id, uint32_t bis_index_bits);
bool app_host_le_audio_broadcast_assistant_select_source(uint8_t listen, uint16_t conn_id, uint8_t *broadcast_code, uint32_t broadcast_id, uint8_t use_past);
bool app_host_le_audio_start_voice_capture(uint8_t start, uint16_t conn_id, uint8_t ch_cnt, wiced_bt_le_audio_config_t *p_audio_config);
bool app_host_le_audio_start_stop_bidir_mic(uint8_t start, uint16_t conn_id, uint8_t in_ch_cnt, uint8_t out_ch_cnt, wiced_bt_le_audio_config_t *p_in_audio_config, wiced_bt_le_audio_config_t *p_out_audio_config);

bool app_host_le_audio_mics_mute(uint16_t conn_id, uint8_t mute);
bool app_host_le_audio_mics_aics_mute(uint16_t conn_id, uint32_t instance, uint8_t mute);
bool app_host_le_audio_increase_mics_aics_gain(uint16_t conn_id, uint32_t instance);
bool app_host_le_audio_decrease_mics_aics_gain(uint16_t conn_id, uint32_t instance);
bool app_host_le_audio_set_mics_aics_gain(uint16_t conn_id, uint32_t instance, uint8_t gain);

bool app_host_le_audio_hap_read_preset(uint16_t conn_id);
bool app_host_le_audio_hap_set_active_preset(uint16_t conn_id, uint8_t preset_index);
bool app_host_le_audio_hap_set_preset_name(uint16_t conn_id, uint8_t preset_index, const char *name);
bool app_host_le_audio_hap_set_next_preset(uint16_t conn_id);
bool app_host_le_audio_hap_set_previous_preset(uint16_t conn_id);
bool app_host_le_audio_set_lock_state(uint16_t conn_id, uint8_t lock_val);
#endif //APP_HOST_LE_AUDIO_H
