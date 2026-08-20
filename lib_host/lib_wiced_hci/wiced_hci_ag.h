
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
 * Definitions for WICED HCI AG
 */

#ifndef WICED_HCI_AG_H
#define WICED_HCI_AG_H

#include "wiced_types.h"

// Audio Gateway

typedef wiced_hci_bt_bda_t wiced_hci_bt_ag_connect_data_t;
typedef wiced_hci_bt_handle_t wiced_hci_bt_ag_disconnect_data_t;
typedef wiced_hci_bt_handle_t wiced_hci_bt_ag_audio_open_data_t;
typedef wiced_hci_bt_handle_t wiced_hci_bt_ag_audio_close_data_t;

typedef struct
{
    uint16_t handle;
    uint8_t num_calls;
    uint8_t call_list_status[5];
} wiced_hci_bt_ag_clcc_res_t;

typedef struct
{
    char cind_str[20];
} wiced_hci_bt_ag_cind_t;

typedef struct
{
    uint16_t handle;
    char ciev_str[20];
} wiced_hci_bt_ag_ciev_t;

bool wiced_hci_ag_connect(wiced_hci_bt_ag_connect_data_t *p_data);
bool wiced_hci_ag_disconnect(wiced_hci_bt_ag_disconnect_data_t *p_data);
bool wiced_hci_ag_audio_open(wiced_hci_bt_ag_audio_open_data_t *p_data);
bool wiced_hci_ag_audio_close(wiced_hci_bt_ag_audio_close_data_t *p_data);
bool wiced_hci_ag_send_clcc_response(wiced_hci_bt_ag_clcc_res_t *p_data);
bool wiced_hci_ag_send_cind(wiced_hci_bt_ag_cind_t *p_data);
bool wiced_hci_ag_send_ciev(wiced_hci_bt_ag_ciev_t *p_data);
bool wiced_hci_ag_send_ring_cmd(uint16_t handle);
bool wiced_hci_ag_send_ccwa_cmd(uint16_t handle);
bool wiced_hci_ag_send_clip_cmd(uint16_t handle);
bool wiced_hci_ag_send_ok_cmd(uint16_t handle);
bool wiced_hci_ag_send_error_cmd(uint16_t handle);
bool wiced_hci_ag_send_spk_vol_cmd(uint16_t handle, int volume);
bool wiced_hci_ag_send_mic_vol_cmd(uint16_t handle, int volume);
#endif
