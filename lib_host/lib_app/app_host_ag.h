
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


#ifndef APP_HOST_AG_H
#define APP_HOST_AG_H

// Audio Gateway commands
bool app_host_ag_connect(uint8_t bda[BDA_LEN]);
bool app_host_ag_disconnect(uint8_t bda[BDA_LEN]);
bool app_host_ag_audio_open(uint8_t bda[BDA_LEN]);
bool app_host_ag_audio_close(uint8_t bda[BDA_LEN]);
bool app_host_ag_send_clcc_response(uint8_t bda[6], uint8_t *call_id_list, uint8_t num_item);
bool app_host_ag_update_cind(char *cind, uint8_t length);
bool app_host_ag_send_callsetup_status(uint8_t bda[6], uint8_t status);
bool app_host_ag_send_ciev(uint8_t bda[6], char *ciev, uint8_t length);
bool app_host_ag_send_ring_cmd(uint8_t bda[6]);
bool app_host_ag_send_clip_cmd(uint8_t bda[6]);
bool app_host_ag_send_ccwa_cmd(uint8_t bda[6]);
bool app_host_ag_send_ok_cmd(uint8_t bda[6]);
bool app_host_ag_send_error_cmd(uint8_t bda[6]);
bool app_host_ag_send_spk_vol_cmd(uint8_t bda[6], int vol);
bool app_host_ag_send_mic_vol_cmd(uint8_t bda[6], int vol);
// Audio Gateway events
void app_host_ag_event(uint16_t opcode, uint8_t * p_data, uint32_t len);


#endif
