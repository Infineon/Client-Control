
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
 * Definitions for WICED HCI
 */

#ifndef WICED_HCI_LE_AUDIO_H
#define WICED_HCI_LE_AUDIO_H

#include "wiced_types.h"

// LE Audio
#define MAX_PLAYER_NAME 50
#define MAX_CALL_FRIENDLY_NAME 30
#define MAX_CALL_URI 30
#define MAX_URI_LEN 50
#define MAX_BROADCAST_CODE_LEN 16
#define BDA_LEN     6
typedef struct
{
    uint16_t conn_id;
} wiced_hci_bt_le_audio_conn_id_t;

typedef struct
{
    uint16_t conn_id;
    uint8_t vol;
} wiced_hci_bt_le_audio_vol_t;

typedef struct
{
    uint16_t conn_id;
    uint8_t len;
    uint8_t player_name[MAX_PLAYER_NAME];
} wiced_hci_bt_le_audio_set_media_player_t;

/** Call action values */
typedef enum
{
    WICED_BT_GA_CCP_ACTION_ACCEPT_CALL = 0x00,    /**< Answer the incoming call. */
    WICED_BT_GA_CCP_ACTION_TERMINATE_CALL = 0x01, /**< End the currently active/outgoing/held call. */
    WICED_BT_GA_CCP_ACTION_HOLD_CALL = 0x02,      /**< Place the currently active or alerting call on local hold. */
    WICED_BT_GA_CCP_ACTION_RETRIEVE_CALL = 0x03,  /**< Move a locally held call to an active call. Move a locally and
                                                     remotely held call to a remotely held call */
    WICED_BT_GA_CCP_ACTION_ORIGINATE = 0x04,      /**< Place a call */
    WICED_BT_GA_CCP_ACTION_JOIN_CALL = 0x05, /**< Put a call to the active state and join all calls that are in the active state. */
    WICED_BT_GA_CCP_UNKNOWN_OPCODE   = 0x06, /**< unknown opcode */
} wiced_bt_ga_tbs_call_action_t;

/** All posible termination reason */
typedef enum
{
    WICED_BT_GA_TBS_IMPROPER_REMOTE_CALLER_ID   = 0x00, /**< remote Caller ID value used to place a call was formed improperly. */
    WICED_BT_GA_TBS_CALL_FAIL               = 0x01, /**< unable to make the call */
    WICED_BT_GA_TBS_REMOTE_CALL_END         = 0x02, /**< remote party ended the call */
    WICED_BT_GA_TBS_SERVER_CALL_END         = 0x03, /**< call ended from the server */
    WICED_BT_GA_TBS_LINE_BUSY               = 0x04, /**< line busy */
    WICED_BT_GA_TBS_NETWORK_CONGESTION      = 0x05, /**< network congestion */
    WICED_BT_GA_TBS_CLIENT_TERMINATED       = 0x06, /**< client terminated the call */
    WICED_BT_GA_TBS_NO_SERVICE              = 0x07, /**< No service */
    WICED_BT_GA_TBS_NO_ANSWER               = 0x08, /**< No answer */
    WICED_BT_GA_TBS_UNSPECIFIED             = 0x09, /**< Reason is not specified */
} wiced_bt_ga_tbs_call_termination_reason_t;

/* termination reason */
typedef struct
{
    uint8_t                            call_id;                    /**< call id to be terminated */
    wiced_bt_ga_tbs_call_termination_reason_t termination_reason;         /**< call termination reason */
} wiced_bt_ga_tbs_call_termination_reason_data_t;

typedef struct
{
    uint16_t conn_id;
    uint8_t uri_len;
    uint8_t call_URI[MAX_URI_LEN];
    uint8_t fri_name_len;
    uint8_t friendly_name[MAX_URI_LEN];
} wiced_hci_bt_call_data_t;

typedef struct
{

    uint16_t conn_id; /**< TBS control point opcode*/
    uint16_t opcode;
    union {
        uint8_t  call_id; /**< Call ID in case of WICED_BT_GA_CCP_ACTION_ACCEPT_CALL, WICED_BT_GA_CCP_ACTION_TERMINATE_CALL,
                                                   WICED_BT_GA_CCP_ACTION_HOLD_CALL, WICED_BT_GA_CCP_ACTION_RETRIEVE_CALL*/
        wiced_hci_bt_call_data_t call_data_t;
        wiced_bt_ga_tbs_call_termination_reason_data_t termination_data; /**<Reason for termination of the call */
    };
} wiced_bt_ga_tbs_call_control_point_t;

typedef struct
{
    uint8_t start;
    uint32_t codec_config;
    uint8_t enable_encryption;
    uint32_t channel_counts;
	uint32_t broadcast_id;
    uint8_t broadcast_code[MAX_BROADCAST_CODE_LEN];
    uint8_t bis_count;
} wiced_bt_ga_broadcast_start_stop_streaming_t;

typedef struct
{
    uint8_t audio_config_id;
    uint32_t sampling_frequency;
    uint32_t frame_duration;
    uint32_t sdu_interval;
    uint32_t ocpf;
}wiced_bt_le_audio_config_t;


typedef wiced_hci_bt_le_audio_conn_id_t wiced_bt_le_audio_cmd_data_t;
typedef wiced_hci_bt_le_audio_vol_t wiced_bt_le_audio_vol_t;

bool wiced_hci_le_audio_play(uint16_t conn_id, uint8_t ch_cnt, wiced_bt_le_audio_config_t *p_audio_config);
bool wiced_hci_le_audio_stop(wiced_bt_le_audio_cmd_data_t *p_data);
bool wiced_hci_le_audio_pause(uint16_t conn_id);
bool wiced_hci_le_audio_volume_up(wiced_bt_le_audio_cmd_data_t *p_data);
bool wiced_hci_le_audio_volume_down(wiced_bt_le_audio_cmd_data_t *p_data);
bool wiced_hci_le_audio_mute(wiced_bt_le_audio_cmd_data_t *p_data);
bool wiced_hci_le_audio_unmute(wiced_bt_le_audio_cmd_data_t *p_data);
bool wiced_hci_le_audio_unmute(wiced_bt_le_audio_cmd_data_t *p_data);
bool wiced_hci_le_audio_get_media_players(wiced_bt_le_audio_cmd_data_t *p_data);
bool wiced_hci_le_audio_set_media_player(wiced_hci_bt_le_audio_set_media_player_t *p_data);
bool wiced_hci_le_audio_set_volume(wiced_hci_bt_le_audio_vol_t *p_data);
bool wiced_hci_le_audio_unmute_relative_vol_up(wiced_bt_le_audio_cmd_data_t *p_data);
bool wiced_hci_le_audio_unmute_relative_vol_down(wiced_bt_le_audio_cmd_data_t *p_data);
bool wiced_hci_le_audio_broadcast_sink_play_pause(uint16_t listen, uint8_t *broadcast_code);
bool wiced_hci_le_audio_broadcast_sink_play_pause_broadcast_code(uint16_t conn_id);

bool wiced_hci_call_control_generate_call(wiced_hci_bt_call_data_t *p_data, uint8_t in_ch_cnt, uint8_t out_ch_cnt, wiced_bt_le_audio_config_t *p_in_audio_config, wiced_bt_le_audio_config_t *p_out_audio_config);
bool wiced_hci_call_control_handle_call_action(wiced_bt_ga_tbs_call_control_point_t *p_data);
bool wiced_hci_call_control_originate_call(uint16_t conn_id, uint8_t *p_call_uri, uint8_t uri_len);
bool wiced_hci_call_control_terminate_call(wiced_bt_ga_tbs_call_control_point_t *p_data);
bool wiced_hci_set_rmt_call_hold(uint8_t call_id);
bool wiced_hci_set_rmt_hold_retrieve(uint8_t call_id);

bool wiced_hci_broadcast_source_start_streaming(wiced_bt_ga_broadcast_start_stop_streaming_t *p_data);
bool wiced_hci_broadcast_sink_find_source(uint8_t start);
bool wiced_hci_broadcast_assistant_scan_source(uint8_t start);
bool wiced_hci_broadcast_sink_get_streams(uint32_t broadcast_id);
bool wiced_hci_broadcast_sink_cancel_pa_sync(void);
bool wiced_hci_broadcast_sink_terminate_pa_sync(uint32_t broadcast_id);
bool wiced_hci_broadcast_sink_sync_to_stream(uint8_t listen, uint8_t *broadcast_code, uint32_t broadcast_id, uint32_t bis_index_bits);
bool wiced_hci_broadcast_assistant_select_source(uint8_t listen, uint16_t conn_id, uint8_t *broadcast_code, uint32_t broadcast_id, uint8_t use_past);
bool wiced_hci_le_audio_start_voice_capture(uint8_t start, uint16_t conn_id, uint8_t ch_cnt, wiced_bt_le_audio_config_t *p_audio_config);
bool wiced_hci_le_audio_start_stop_bidir_mic(uint8_t start, uint16_t conn_id, uint8_t in_ch_cnt, uint8_t out_ch_cnt, wiced_bt_le_audio_config_t *p_in_audio_config, wiced_bt_le_audio_config_t *p_out_audio_config);

bool wiced_hci_le_audio_mics_mute(uint16_t conn_id, uint8_t mute);
bool wiced_hci_le_audio_mics_aics_mute(uint16_t conn_id, uint32_t instance, uint8_t mute);
bool wiced_hci_le_audio_set_mics_aics_gain(uint16_t conn_id, uint32_t instance, uint8_t opcode, uint8_t gain);

bool wiced_hci_le_audio_hap_read_preset_record(uint16_t conn_id);
bool wiced_hci_le_audio_hap_set_active_preset(uint16_t conn_id, uint8_t opcode, uint8_t preset_index);
bool wiced_hci_le_audio_hap_set_preset_name(uint16_t conn_id, uint8_t preset_index, const char *p_name);
bool wiced_hci_le_audio_set_lock_state(uint16_t conn_id, uint8_t lock_val);

#endif // WICED_HCI_H
