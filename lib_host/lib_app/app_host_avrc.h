
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


#ifndef APP_HOST_AVRC_H
#define APP_HOST_AVRC_H

// AVRCP
bool app_host_avrc_ct_connect(uint8_t bda[BDA_LEN]);
bool app_host_avrc_ct_disconnect(uint8_t bda[BDA_LEN]);
bool app_host_avrc_unit_info(uint8_t bda[BDA_LEN], uint16_t handle);
bool app_host_avrc_sub_unit_info(uint8_t bda[BDA_LEN], uint16_t handle);

#define WICED_AVRCP_CT_PLAY_CMD        1
#define WICED_AVRCP_CT_STOP_CMD        2
#define WICED_AVRCP_CT_PAUSE_CMD       3
#define WICED_AVRCP_CT_NEXT_CMD        5
#define WICED_AVRCP_CT_PREVIOUS_CMD    6
#define WICED_AVRCP_CT_VOL_UP_CMD      7
#define WICED_AVRCP_CT_VOL_DOWN_CMD    8
#define WICED_AVRCP_CT_MUTE_CMD        9
#define WICED_AVRCP_CT_FF_CMD_PRESS    10
#define WICED_AVRCP_CT_REV_CMD_PRESS   11
#define WICED_AVRCP_CT_FF_CMD_RELEASE  12
#define WICED_AVRCP_CT_REV_CMD_RELEASE 13


/* Define the Player Application Settings IDs */
#define WICED_MAX_RC_APP_SETTING_VALUE                4
#define WICED_AVRC_PLAYER_SETTING_REPEAT              0x02
#define WICED_AVRC_PLAYER_SETTING_SHUFFLE             0x03

/* Define the possible values of the Player Application Settings */
#define WICED_AVRC_PLAYER_VAL_OFF                     0x01
#define WICED_AVRC_PLAYER_VAL_ON                      0x02
#define WICED_AVRC_PLAYER_VAL_SINGLE_REPEAT           0x02
#define WICED_AVRC_PLAYER_VAL_ALL_REPEAT              0x03
#define WICED_AVRC_PLAYER_VAL_GROUP_REPEAT            0x04
#define WICED_AVRC_PLAYER_VAL_ALL_SHUFFLE             0x02
#define WICED_AVRC_PLAYER_VAL_GROUP_SHUFFLE           0x03
#define WICED_AVRC_PLAYER_VAL_ALL_SCAN                0x02
#define WICED_AVRC_PLAYER_VAL_GROUP_SCAN              0x03

bool app_host_avrc_ct_command(uint8_t bda[BDA_LEN], uint16_t handle, uint8_t cmd);
bool app_host_avrc_ct_repeat(uint8_t bda[BDA_LEN], uint16_t handle, uint8_t setting);
bool app_host_avrc_ct_shuffle(uint8_t bda[BDA_LEN], uint16_t handle, uint8_t setting);
bool app_host_avrc_ct_volume_level(uint8_t bda[BDA_LEN], uint16_t handle, uint8_t vol_level);

void app_host_avrc_ct_event(uint16_t opcode, uint8_t * p_data, uint16_t len);


#endif
