
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
 * Definitions for WICED HCI Audio
 */

#ifndef WICED_HCI_AUDIO_H
#define WICED_HCI_AUDIO_H

#include "wiced_hci.h"

#define AUDIO_SRC_ROUTE_I2S         0
#define AUDIO_SRC_ROUTE_UART        1
#define AUDIO_SRC_ROUTE_SINE_WAVE   2

typedef struct
{
    unsigned char bda[BDA_LEN];
    unsigned char audio_route;
} wiced_hci_bt_audio_source_connect_data_t;

typedef wiced_hci_bt_handle_t wiced_hci_bt_audio_source_disconnect_data_t ;

typedef wiced_hci_bt_handle_t wiced_hci_bt_audio_source_stop_data_t ;

#define AUDIO_SRC_SAMPLING_FREQ_16      0
#define AUDIO_SRC_SAMPLING_FREQ_32      1
#define AUDIO_SRC_SAMPLING_FREQ_44_1    2
#define AUDIO_SRC_SAMPLING_FREQ_48      3

#define AUDIO_SRC_CHANNEL_MODE_MONO     0
#define AUDIO_SRC_CHANNEL_MODE_STEREO   1

typedef struct
{
    uint16_t handle;
    uint8_t sample_freq;
    uint8_t audio_mode;
} wiced_hci_bt_audio_source_start_data_t;

typedef struct
{
    uint8_t *p_data;
    uint16_t len;
} wiced_hci_bt_audio_source_audio_data_t;

#define AUDIO_SRC_AUDIO_DATA_FORMAT_PCM 0
#define AUDIO_SRC_AUDIO_DATA_FORMAT_MP3 1

typedef struct
{
    uint8_t format;
    uint8_t audio_route;
} wiced_hci_bt_audio_source_audio_data_format_t;

#define AUDIO_SRC_FEATURE_I2S_INPUT (1 << 0)
#define AUDIO_SRC_FEATURE_MP3_FORMAT (1 << 1)

bool wiced_hci_audio_src_connect(wiced_hci_bt_audio_source_connect_data_t *p_data);
bool wiced_hci_audio_src_disconnect(wiced_hci_bt_audio_source_disconnect_data_t *p_data);
bool wiced_hci_audio_src_audio_start(wiced_hci_bt_audio_source_start_data_t *p_data);
bool wiced_hci_audio_src_audio_stop(wiced_hci_bt_audio_source_stop_data_t *p_data);
bool wiced_hci_audio_src_audio_data(wiced_hci_bt_audio_source_audio_data_t *p_data);
bool wiced_hci_audio_src_audio_data_format(wiced_hci_bt_audio_source_audio_data_format_t *p_data);

#endif
