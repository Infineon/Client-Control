
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

#ifndef WICED_HCI_AVRC_CT_H
#define WICED_HCI_AVRC_CT_H

// AVRC CT
typedef wiced_hci_bt_bda_t wiced_bt_avrc_ct_connect_data_t;
typedef wiced_hci_bt_handle_t wiced_bt_avrc_ct_disconnect_data_t;
typedef wiced_hci_bt_handle_t wiced_bt_avrc_ct_cmd_data_t;
typedef wiced_hci_bt_handle_t wiced_bt_avrc_ct_cmd_unit_info_data_t;
typedef wiced_hci_bt_handle_t wiced_bt_avrc_ct_cmd_sub_unit_info_data_t;

bool wiced_hci_avrc_ct_connect(wiced_bt_avrc_ct_connect_data_t *p_data);
bool wiced_hci_avrc_ct_disconnect(wiced_bt_avrc_ct_disconnect_data_t *p_data);
bool wiced_hci_avrc_ct_play(wiced_bt_avrc_ct_cmd_data_t *p_data);
bool wiced_hci_avrc_ct_stop(wiced_bt_avrc_ct_cmd_data_t *p_data);
bool wiced_hci_avrc_ct_pause(wiced_bt_avrc_ct_cmd_data_t *p_data);
bool wiced_hci_avrc_ct_next_track(wiced_bt_avrc_ct_cmd_data_t *p_data);
bool wiced_hci_avrc_ct_previous_track(wiced_bt_avrc_ct_cmd_data_t *p_data);
bool wiced_hci_avrc_ct_volume_up(wiced_bt_avrc_ct_cmd_data_t *p_data);
bool wiced_hci_avrc_ct_volume_down(wiced_bt_avrc_ct_cmd_data_t *p_data);
bool wiced_hci_avrc_ct_mute(wiced_bt_avrc_ct_cmd_data_t *p_data);
bool wiced_hci_avrc_ct_skip_forward_pressed(wiced_bt_avrc_ct_cmd_data_t *p_data);
bool wiced_hci_avrc_ct_skip_forward_released(wiced_bt_avrc_ct_cmd_data_t *p_data);
bool wiced_hci_avrc_ct_skip_backward_pressed(wiced_bt_avrc_ct_cmd_data_t *p_data);
bool wiced_hci_avrc_ct_skip_backward_released(wiced_bt_avrc_ct_cmd_data_t *p_data);
bool wiced_hci_avrc_ct_button_press();
bool wiced_hci_avrc_ct_long_button_press();
bool wiced_hci_avrc_unit_info(wiced_bt_avrc_ct_cmd_unit_info_data_t *p_data);
bool wiced_hci_avrc_sub_unit_info(wiced_bt_avrc_ct_cmd_sub_unit_info_data_t *p_data);

// Setting type
#define WICED_AVRC_PLAYER_SETTING_REPEAT              0x02
#define WICED_AVRC_PLAYER_SETTING_SHUFFLE             0x03

// Repeat settings
#define WICED_AVRC_PLAYER_VAL_OFF_REPEAT              0x01
#define WICED_AVRC_PLAYER_VAL_SINGLE_REPEAT           0x02
#define WICED_AVRC_PLAYER_VAL_ALL_REPEAT              0x03
#define WICED_AVRC_PLAYER_VAL_GROUP_REPEAT            0x04

// Shuffle settings
#define WICED_AVRC_PLAYER_VAL_OFF_SHUFFLE             0x01
#define WICED_AVRC_PLAYER_VAL_ALL_SHUFFLE             0x02
#define WICED_AVRC_PLAYER_VAL_GROUP_SHUFFLE           0x03

typedef struct
{
    uint16_t handle;
    uint8_t  setting;
} wiced_bt_avrc_ct_settings_data_t;

bool wiced_hci_avrc_ct_repeat(wiced_bt_avrc_ct_settings_data_t *p_data);
bool wiced_hci_avrc_ct_shuffle(wiced_bt_avrc_ct_settings_data_t *p_data);

typedef struct
{
    uint16_t handle;
    uint8_t  volume_level;
} wiced_bt_avrc_ct_volume_level_data_t;

bool wiced_hci_avrc_ct_volume_level(wiced_bt_avrc_ct_volume_level_data_t *p_data);

#endif // WICED_HCI_H
