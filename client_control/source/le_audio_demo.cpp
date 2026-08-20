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

/*
 * Sample MCU application for LE Audio sink using WICED HCI protocol.
 */
#include "app_include.h"
extern "C"
{
#include "app_host.h"
#include "app_host_le_audio.h"
#include "wiced_hci_le_audio.h"
}
#include "QDebug"
#include "mainwindow.h"
#include <QByteArray>
#include <QMessageBox>
#include <QFileInfo>

Q_DECLARE_METATYPE(CBtDevice *)

#define TAG "LE Audio"

#define START 1
#define STOP 0
#define SYNC 1
#define TERMINATE 0
#define ADD 1
#define REMOVE 0

uint32_t le_audio_dev_role = HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE;

#define LEPL_APPEARANCE_GENERIC_AUDIO_SOURCE 0X0880    /**< Device appearance as Generic Audio Source */
#define LEHS_APPEARANCE_WEARABLE_DEVICE_HEADPHONE 0x0943 /**< Device appearance as headphone */
#define LEHS_APPEARANCE_WEARABLE_DEVICE_LEFT_EARBUD 0x0945 /**< Device appearance as left earbud */
#define LEHS_APPEARANCE_WEARABLE_DEVICE_RIGHT_EARBUD 0x0946 /**< Device appearance as right earbud */

extern AudioFileWriter * pAudioFileWriter;

#define STRING_START_BROADCAST "Start Broadcast"
#define STRING_SYNC_TO_STREAM "Sync to Stream"
#define STRING_DISCOVER_SOURCES "Discover Sources"
#define STRING_STOP_DISCOVER "Stop Discovery"
#define STRING_TERMINATE_STREAM "Terminate Stream"
#define STRING_CALL_ON_HOLD "Call on hold....."
#define STRING_ON_GOING_CALL "Ongoing call....."
#define STRING_STOP_ADVERTISEMENT "Stop Adv"
#define STRING_START_ADVERTISEMENT "Start Adv"
#define STRING_PLAY "Play"
#define STRING_PAUSE "Pause"
#define STRING_START_MIC "Start"
#define STRING_STOP_MIC "Stop"
#define STRING_MUTE "Mute"
#define STRING_UNMUTE "Unmute"
#define STRING_CONNECT "Connect"
#define STRING_DISCONNECT "Disconnect"
#define STRING_REMOTE_RETRIEVE "Remote Retrieve"
#define STRING_SIMULATE_REMOTE_HOLD "Simulate Remote Hold"
#define STRING_MUTE_MICP "Mute MICP"
#define STRING_UNMUTE_MICP "Unmute MICP"
#define STRING_MUTED "Muted"
#define STRING_UNMUTED "Unmuted"
#define STRING_GET_STREAMS "Get Streams"
#define STRING_CANCEL_PA_SYNC "Cancel PA Sync"
#define STRING_TERMINATE_PA_SYNC "Terminate PA Sync"
#define STRING_ACCEPT_CALL "Accept Call"
#define STRING_REJECT_CALL "Reject Call"
#define STRING_PUT_ON_HOLD "Put on Hold"
#define STRING_TERMINATE_CALL "Terminate Call"
#define STRING_JOIN_CALL "Join Call"
#define STRING_RETRIEVE_CALL "Retrieve Call"
#define STRING_INITIATE_CALL "Initiate Call"
#define STRING_SIMULATE_CALL "Simulate Call"
#define STRING_SET_LOCK "Set lock"
#define STRING_RELEASE_LOCK "Release lock"
#define STRING_GENERIC_AUDIO_SOURCE "Generic Audio Source"
#define STRING_HEADPHONE "Headphone"
#define STRING_EARBUD_LEFT "Earbud Left"
#define STRING_EARBUD_RIGHT "Earbud Right"


enum
{
    BAP_CODEC_CONFIG_8_1_1,
    BAP_CODEC_CONFIG_8_1_2,
    BAP_CODEC_CONFIG_8_2_1,
    BAP_CODEC_CONFIG_8_2_2,
    BAP_CODEC_CONFIG_16_1_1,
    BAP_CODEC_CONFIG_16_1_2,
    BAP_CODEC_CONFIG_16_2_1,
    BAP_CODEC_CONFIG_16_2_2,
    BAP_CODEC_CONFIG_24_1_1,
    BAP_CODEC_CONFIG_24_1_2,
    BAP_CODEC_CONFIG_24_2_1,
    BAP_CODEC_CONFIG_24_2_2,
    BAP_CODEC_CONFIG_32_1_1,
    BAP_CODEC_CONFIG_32_1_2,
    BAP_CODEC_CONFIG_32_2_1,
    BAP_CODEC_CONFIG_32_2_2,
    BAP_CODEC_CONFIG_441_1_1,
    BAP_CODEC_CONFIG_441_1_2,
    BAP_CODEC_CONFIG_441_2_1,
    BAP_CODEC_CONFIG_441_2_2,
    BAP_CODEC_CONFIG_48_1_1,
    BAP_CODEC_CONFIG_48_1_2,
    BAP_CODEC_CONFIG_48_2_1,
    BAP_CODEC_CONFIG_48_2_2,
    BAP_CODEC_CONFIG_48_3_1,
    BAP_CODEC_CONFIG_48_3_2,
    BAP_CODEC_CONFIG_48_4_1,
    BAP_CODEC_CONFIG_48_4_2,
    BAP_CODEC_CONFIG_48_5_1,
    BAP_CODEC_CONFIG_48_5_2,
    BAP_CODEC_CONFIG_48_6_1,
    BAP_CODEC_CONFIG_48_6_2,
};

enum
{
    BAP_AUDIO_CONFIG_8_1,
    BAP_AUDIO_CONFIG_8_2,
    BAP_AUDIO_CONFIG_16_1,
    BAP_AUDIO_CONFIG_16_2,
    BAP_AUDIO_CONFIG_24_1,
    BAP_AUDIO_CONFIG_24_2,
    BAP_AUDIO_CONFIG_32_1,
    BAP_AUDIO_CONFIG_32_2,
    BAP_AUDIO_CONFIG_441_1,
    BAP_AUDIO_CONFIG_441_2,
    BAP_AUDIO_CONFIG_48_1,
    BAP_AUDIO_CONFIG_48_2,
    BAP_AUDIO_CONFIG_48_3,
    BAP_AUDIO_CONFIG_48_4,
    BAP_AUDIO_CONFIG_48_5,
    BAP_AUDIO_CONFIG_48_6,
};

QString config_str[] =
{
    "8_1 (7.5ms - 27.73 kbps)",
    "8_2 (10ms - 24 kbps)",
    "16_1 (7.5ms - 32 kbps)",
    "16_2 (10ms - 32 kbps)",
    "24_1 (7.5ms - 48 kbps)",
    "24_2 (10ms - 48 kbps)",
    "32_1 (7.5ms - 64 kbps)",
    "32_2 (10ms - 64 kbps)",
    "441_1 (7.5ms - 95.06 kbps)",
    "441_2 (10ms - 95.55 kbps)",
    "48_1 (7.5ms - 80 kbps)",
    "48_2 (10ms - 80 kbp)",
    "48_3 (7.5ms - 96 kbps)",
    "48_4 (10ms - 96 kbps)",
    "48_5 (7.5ms - 124.8 kbps)",
    "48_6 (10ms - 124 kbps)"
};

enum
{
    TBS_CALL_STATE_INCOMING,
    TBS_CALL_STATE_DIALING,
    TBS_CALL_STATE_ALERTING,
    TBS_CALL_STATE_ACTIVE,
    TBS_CALL_STATE_LOCALLY_HELD,
    TBS_CALL_STATE_REMOTELY_HELD,
    TBS_CALL_STATE_LOCALLY_AND_REMOTELY_HELD,
};

uint32_t sampling_freq[] ={
    8000,
    11025,
    16000,
    22025,
    24000,
    32000,
    44100,
    48000,
    88200,
    96000,
    176400,
    192000
};

uint32_t frame_duration[] ={
    7500,
    10000
};

wiced_bt_le_audio_config_t  audio_config[] =
{
     {.audio_config_id = BAP_AUDIO_CONFIG_8_1, .sampling_frequency = 8000, .frame_duration = 7500, .sdu_interval = 7500, .ocpf = 26},
     {.audio_config_id = BAP_AUDIO_CONFIG_8_2, .sampling_frequency = 8000, .frame_duration = 10000, .sdu_interval = 10000, .ocpf = 30},
     {.audio_config_id = BAP_AUDIO_CONFIG_16_1, .sampling_frequency = 16000, .frame_duration = 7500, .sdu_interval = 7500, .ocpf = 30},
     {.audio_config_id = BAP_AUDIO_CONFIG_16_2, .sampling_frequency = 16000, .frame_duration = 10000, .sdu_interval = 10000, .ocpf = 40},
     {.audio_config_id = BAP_AUDIO_CONFIG_24_1, .sampling_frequency = 24000, .frame_duration = 7500, .sdu_interval = 7500, .ocpf = 45},
     {.audio_config_id = BAP_AUDIO_CONFIG_24_2, .sampling_frequency = 24000, .frame_duration = 10000, .sdu_interval = 10000, .ocpf = 60},
     {.audio_config_id = BAP_AUDIO_CONFIG_32_1, .sampling_frequency = 32000, .frame_duration = 7500, .sdu_interval = 7500, .ocpf = 60},
     {.audio_config_id = BAP_AUDIO_CONFIG_32_2, .sampling_frequency = 32000, .frame_duration = 10000, .sdu_interval = 10000, .ocpf = 80},
     {.audio_config_id = BAP_AUDIO_CONFIG_441_1, .sampling_frequency = 44100, .frame_duration = 7500, .sdu_interval = 8163, .ocpf = 97},
     {.audio_config_id = BAP_AUDIO_CONFIG_441_2, .sampling_frequency = 44100, .frame_duration = 10000, .sdu_interval = 10884, .ocpf = 130},
     {.audio_config_id = BAP_AUDIO_CONFIG_48_1, .sampling_frequency = 48000, .frame_duration = 7500, .sdu_interval = 7500, .ocpf = 75},
     {.audio_config_id = BAP_AUDIO_CONFIG_48_2, .sampling_frequency = 48000, .frame_duration = 10000, .sdu_interval = 10000, .ocpf = 100},
     {.audio_config_id = BAP_AUDIO_CONFIG_48_3, .sampling_frequency = 48000, .frame_duration = 7500, .sdu_interval = 7500, .ocpf = 90},
     {.audio_config_id = BAP_AUDIO_CONFIG_48_4, .sampling_frequency = 48000, .frame_duration = 10000, .sdu_interval = 10000, .ocpf = 120},
     {.audio_config_id = BAP_AUDIO_CONFIG_48_5, .sampling_frequency = 48000, .frame_duration = 7500, .sdu_interval = 7500, .ocpf = 117},
     {.audio_config_id = BAP_AUDIO_CONFIG_48_6, .sampling_frequency = 48000, .frame_duration = 10000, .sdu_interval = 10000, .ocpf = 155}
};
bool fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}


// Helper function
uint16_t le_audio_update_conn_id(CBtDevice *p_device)
{
    uint16_t conn_id = 0;

    if((le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE)|
        (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SINK) |
        (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_SERVER)|
        (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_CLIENT) |
        (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_ASSISTANT) ||
            (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_REMOTE_CONTROL))
    {
        conn_id = p_device->get_connection_handle();
    }

    return conn_id;
}

void MainWindow::le_audio_handle_device_change(CBtDevice * pDev)
{
    if(pDev->get_connection_handle())
    {
        ui->btn_le_audio_connectToPeer->setText(STRING_DISCONNECT);
        if((le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE) || (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_SERVER))
        {
            ui->le_audio_mics_aics_desc->clear();
            ui->le_audio_hap_preset_name->clear();
            QMap<uint32_t, QString>::Iterator itr_32;
            for(itr_32 =pDev->aics_des.begin(); itr_32 != pDev->aics_des.end(); itr_32++)
                ui->le_audio_mics_aics_desc->addItem(itr_32.value(), itr_32.key());
            QMap<uint8_t, QString>::Iterator itr_8;
            for(itr_8 = pDev->preset_rec.begin(); itr_8 != pDev->preset_rec.end(); itr_8++)
                ui->le_audio_hap_preset_name->addItem(itr_8.value(), itr_8.key());
            if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE)
            {
                if(pDev->le_audio_state.media_state == 1)
                    ui->btn_le_audio_pl_playPause->setText(STRING_PAUSE);
                else
                   ui->btn_le_audio_pl_playPause->setText(STRING_PLAY);
                if(pDev->le_audio_state.mic_state)
                    ui->btn_le_audio_pl_startVoiceCapture->setText(STRING_STOP_MIC);
                else
                    ui->btn_le_audio_pl_startVoiceCapture->setText(STRING_START_MIC);

            }
            else if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_SERVER)
            {
                if(pDev->le_audio_state.bidir_mic_state)
                    ui->btn_le_audio_pl_bidirmic_start_stop->setText(STRING_STOP_MIC);
                else
                    ui->btn_le_audio_pl_bidirmic_start_stop->setText(STRING_START_MIC);
            }

            le_audio_pl_update_listed_audio_config(pDev);
        }
    }
    else
    {
        ui->btn_le_audio_connectToPeer->setText(STRING_CONNECT);
        if((le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE) || (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_SERVER))
        {
            ui->le_audio_mics_aics_desc->clear();
            ui->btn_le_audio_pl_playPause->setText(STRING_PLAY);
            ui->btn_le_audio_pl_startVoiceCapture->setText(STRING_START_MIC);
            ui->btn_le_audio_pl_bidirmic_start_stop->setText(STRING_START_MIC);
            ui->le_audio_pl_list_cc_in_audioConfig->clear();
            ui->le_audio_pl_list_cc_out_audioConfig->clear();
            ui->le_audio_pl_list_media_audioConfig->clear();
            ui->le_audio_pl_list_mic_audioConfig->clear();
        }
    }
}

// Initialize app
void MainWindow::InitUnicastSink()
{
    ui->tabLEAudio->setEnabled(TRUE);
    le_audio_reset_ui();
}


void MainWindow::le_audio_handle_update_call_state(uint16_t conn_id, uint8_t *p_data)
{
    uint8_t call_id,call_state, uri_len;
    QString call_uri;

    UNUSED(conn_id);

    STREAM_TO_UINT8(call_id, p_data);
    STREAM_TO_UINT8(call_state, p_data);

    CBtDevice *p_device = FindInList(WICED_CONNECTION_TYPE_LE, conn_id, ui->cbBLEDeviceList);
    if(!is_le_audio_player() && !p_device) return;
    switch(call_state)
    {
    case TBS_CALL_STATE_INCOMING:{
        STREAM_TO_UINT8(uri_len, p_data);
        int len = uri_len;
        while(p_data && uri_len)
        {
            call_uri += *p_data;
            p_data++;
            uri_len--;
        }

        Log("[%s] call uri %s Incoming call %x ", TAG, call_uri.toStdString().c_str(), call_id);
        if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_CLIENT)
        {
            InitLeAudioHeadsetCallControl(TRUE);

            ui->le_audio_hs_ccp_call_id->setText( QString::number(call_id, 10));
            if(len) ui->le_audio_hs_ccp_call_URI->setText(QUrl::fromPercentEncoding(call_uri.toUtf8()));
            ui->btn_le_audio_hs_ccp_accept_retrieve_call->setText(STRING_ACCEPT_CALL);
            ui->btn_le_audio_hs_ccp_reject_terminate_call->setText(STRING_REJECT_CALL);
        }
        else if(is_le_remote_control())
        {
            ui->le_audio_rc_callId->setText( QString::number(call_id, 10));
            ui->btn_le_audio_rc_InitiateCall->setText(STRING_ACCEPT_CALL);
        }
    }
        break;
    case TBS_CALL_STATE_DIALING:
        if(is_le_audio_player())
        {
           ui->le_audio_pl_Call_Id->setText( QString::number(call_id, 10));
           ui->btn_le_audio_pl_simulateCall->setText(STRING_TERMINATE_CALL);
        }
        else
        {
            Log("[%s] call id %d call state %s", TAG, call_id, "Dialing");
            InitLeAudioHeadsetCallControl(TRUE);
            ui->le_audio_hs_ccp_call_id->setText( QString::number(call_id, 10));
            ui->btn_le_audio_hs_ccp_accept_retrieve_call->setText("");
            ui->btn_le_audio_hs_ccp_reject_terminate_call->setText(STRING_TERMINATE_CALL);
        }
        break;
    case TBS_CALL_STATE_ALERTING:
        Log("[%s] call id %d call state %s", TAG, call_id, "Alerting");
        break;
    case TBS_CALL_STATE_ACTIVE:
        Log("[%s] call id %d call state %s", TAG, call_id, "Active Call");
        if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_CLIENT)
        {
            ui->le_audio_hs_ccp_call_id->setText( QString::number(call_id, 10));
            ui->btn_le_audio_hs_ccp_accept_retrieve_call->setText(STRING_PUT_ON_HOLD);
            ui->btn_le_audio_hs_ccp_reject_terminate_call->setText(STRING_TERMINATE_CALL);
        }
        else if(is_le_remote_control())
        {
            ui->le_audio_rc_callId->setText( QString::number(call_id, 10));
            ui->btn_le_audio_rc_InitiateCall->setText(STRING_PUT_ON_HOLD);
        }
        else if(is_le_audio_player())
        {
             ui->btn_le_audio_pl_simulateHold->setText(STRING_SIMULATE_REMOTE_HOLD);
        }
        break;
    case TBS_CALL_STATE_LOCALLY_HELD:
        Log("[%s] call id %d call state %s", TAG, call_id, "Locally Held");
        if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_CLIENT)
        {
            ui->le_audio_hs_ccp_call_id->setText( QString::number(call_id, 10));
            ui->btn_le_audio_hs_ccp_accept_retrieve_call->setText(STRING_RETRIEVE_CALL);
            ui->btn_le_audio_hs_ccp_reject_terminate_call->setText(STRING_TERMINATE_CALL);
        }
        else if(is_le_remote_control())
        {
            ui->le_audio_rc_callId->setText( QString::number(call_id, 10));
            ui->btn_le_audio_rc_InitiateCall->setText(STRING_RETRIEVE_CALL);
        }
        break;
    case TBS_CALL_STATE_REMOTELY_HELD:
        Log("[%s] call id %d call state %s", TAG, call_id, "Remotely Held");
        if(is_le_audio_player())
        {
             ui->btn_le_audio_pl_simulateHold->setText(STRING_REMOTE_RETRIEVE);
        }
        else
        {
            ui->btn_le_audio_hs_ccp_accept_retrieve_call->setText(STRING_PUT_ON_HOLD);
        }
        break;
    case TBS_CALL_STATE_LOCALLY_AND_REMOTELY_HELD:
        Log("[%s] call id %d call state %s", TAG, call_id, "Locally and Remotely Held");
        if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_CLIENT)
        {
            ui->le_audio_hs_ccp_call_id->setText( QString::number(call_id, 10));
            ui->btn_le_audio_hs_ccp_accept_retrieve_call->setText(STRING_RETRIEVE_CALL);
            ui->btn_le_audio_hs_ccp_reject_terminate_call->setText(STRING_TERMINATE_CALL);
        }
        else if(is_le_remote_control())
        {
            ui->le_audio_rc_callId->setText( QString::number(call_id, 10));
            ui->btn_le_audio_rc_InitiateCall->setText(STRING_RETRIEVE_CALL);
        }
        else if(is_le_audio_player())
        {
             ui->le_audio_rc_callId->setText( QString::number(call_id, 10));
             ui->btn_le_audio_pl_simulateHold->setText(STRING_REMOTE_RETRIEVE);
        }
        break;
    }
}

void MainWindow::le_audio_handle_call_terminated(uint8_t *p_data)
{
    uint16_t conn_id;
    uint8_t call_id, termination_reason;
    STREAM_TO_UINT16(conn_id, p_data);
    STREAM_TO_UINT8(call_id, p_data);
    STREAM_TO_UINT8(termination_reason, p_data)

    Log("[%s] call terminated conn id %d call id %d termination reason %d", TAG, conn_id, call_id, termination_reason);
    if(is_le_audio_player())
        ui->btn_le_audio_pl_simulateCall->setText(STRING_SIMULATE_CALL);
    else
        InitLeAudioHeadsetCallControl(FALSE);
}

void MainWindow::le_audio_handle_update_call_friendly_name(uint8_t *p_data)
{
    uint8_t f_name_len;
    QString g_friendly_name;

    STREAM_TO_UINT8(f_name_len, p_data);
    while(p_data && f_name_len )
    {
        g_friendly_name+=*p_data;
        p_data++;
        f_name_len--;
    }
    Log("[%s] friendly name %s", TAG, g_friendly_name.toStdString().c_str());
    ui->le_audio_hs_ccp_call_friendly_name->setText(g_friendly_name);
}

void MainWindow::le_audio_handle_mics_aics_description(uint8_t *p_data)
{
    uint16_t conn_id;
    uint32_t instance;
    uint32_t desc_len;
    char desc[25];

    STREAM_TO_UINT16(conn_id, p_data);
    STREAM_TO_UINT32(instance, p_data);
    STREAM_TO_UINT32(desc_len, p_data);
    if(desc_len) memcpy(desc, p_data, desc_len);
    desc[desc_len] = '\0';

    CBtDevice *p_device = FindInList(WICED_CONNECTION_TYPE_LE, conn_id, ui->cbBLEDeviceList);
    if(p_device && ((le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE) ||
                    (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_SERVER)))
    {
        p_device->aics_des[instance] = desc;
        if(p_device == GetSelectedLEDevice())
        {
            if((ui->le_audio_mics_aics_desc->currentIndex() == -1) || (ui->le_audio_mics_aics_desc->currentData().toUInt() != instance))
            {
                ui->le_audio_mics_aics_desc->addItem(desc, instance);
            }
            else
            {
                ui->le_audio_mics_aics_desc->setItemText(ui->le_audio_mics_aics_desc->currentIndex(), desc);
            }
        }
    }
    else if(((le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SINK) ||
             (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_CLIENT)))
    {
        if((ui->le_audio_mics_aics_desc->currentIndex() == -1)  || (ui->le_audio_mics_aics_desc->currentData().toUInt() != instance))
        {
            ui->le_audio_mics_aics_desc->addItem(desc, instance);
        }
        else
        {
            ui->le_audio_mics_aics_desc->setItemText(ui->le_audio_mics_aics_desc->currentIndex(), desc);
        }
    }

}
void MainWindow::le_audio_handle_mics_mute_status(uint8_t *p_data)
{
    uint16_t conn_id;
    uint8_t mute_state;
    STREAM_TO_UINT16(conn_id, p_data);
    STREAM_TO_UINT8(mute_state, p_data);

    Log("[%s] conn_id 0x%x %s" ,TAG,conn_id, (mute_state)?STRING_MUTED:STRING_UNMUTED);
    CBtDevice *p_device =  GetSelectedLEDevice();
    if((le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE) ||
                    (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_SERVER))
    {
        if(!p_device || (p_device->get_connection_handle()!= conn_id))
            return;
    }
    if(mute_state)
        ui->btn_le_audio_mics_mute->setText(STRING_UNMUTE_MICP);
    else
        ui->btn_le_audio_mics_mute->setText(STRING_MUTE_MICP);
}

void MainWindow::le_audio_handle_mics_aics_input_status(uint8_t *p_data)
{
    uint16_t conn_id;
    uint32_t instance;
    uint8_t mute_state;
    int8_t gain;

    STREAM_TO_UINT16(conn_id, p_data);
    STREAM_TO_UINT32(instance, p_data);
    STREAM_TO_UINT8(gain, p_data);
    STREAM_TO_UINT8(mute_state, p_data);

    Log("[%s] conn_id 0x%x instance %d mute state %s gain %d" ,TAG, conn_id, instance, (mute_state)?STRING_MUTED:STRING_UNMUTED, gain);

    CBtDevice *p_device =  GetSelectedLEDevice();
    if((le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE) ||
                    (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_SERVER))
    {
        if(!p_device || (p_device->get_connection_handle()!= conn_id))
            return;
    }
    if((ui->le_audio_mics_aics_desc->currentData().toUInt() == instance))
    {
        if(mute_state)
            ui->btn_le_audio_mics_aics_mute->setText(STRING_UNMUTE);
        else
            ui->btn_le_audio_mics_aics_mute->setText(STRING_MUTE);
        ui->le_audio_mics_aics_gain->setText(QString::number(gain));
    }
}
void MainWindow::le_audio_handle_preset_record(uint8_t *p_data, uint8_t len)
{
    uint16_t conn_id;
    uint8_t preset_index;
    char name[43];

    STREAM_TO_UINT16(conn_id, p_data);
    STREAM_TO_UINT8(preset_index, p_data);
    len-=3;
    memcpy(name, p_data, len);
    name[len] = '\0';
    CBtDevice *p_device = FindInList(WICED_CONNECTION_TYPE_LE, conn_id, ui->cbBLEDeviceList);
    if(p_device && ((le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE) ||
                    (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_SERVER)))
    {
        p_device->preset_rec[preset_index] = name;
        if(p_device == GetSelectedLEDevice())
        {
            int index = ui->le_audio_hap_preset_name->findData(preset_index);
            if(index == -1)
            {
                ui->le_audio_hap_preset_name->addItem(name, preset_index);
            }
            else
            {
                ui->le_audio_hap_preset_name->setItemText(index, name);
            }
        }
    }
}

void MainWindow::le_audio_handle_active_preset(uint8_t *p_data)
{
    uint16_t conn_id;
    uint8_t preset_index;
    STREAM_TO_UINT16(conn_id, p_data);
    STREAM_TO_UINT8(preset_index, p_data);

    CBtDevice *p_device = FindInList(WICED_CONNECTION_TYPE_LE, conn_id, ui->cbBLEDeviceList);
    if(p_device && ((le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE) ||
                    (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_SERVER)))
    {
        if(p_device == GetSelectedLEDevice())
        {
            int index = ui->le_audio_hap_preset_name->findData(preset_index);
            if(index!=-1)
            {
                ui->le_audio_hap_preset_name->setCurrentIndex(index);
            }
        }
    }
}

void MainWindow::le_audio_handle_mic_status(uint8_t *p_data)
{
    uint16_t conn_id;
    uint8_t status;
    STREAM_TO_UINT16(conn_id, p_data);
    STREAM_TO_UINT8(status, p_data);

    CBtDevice *p_device = FindInList(WICED_CONNECTION_TYPE_LE, conn_id, ui->cbBLEDeviceList);
    if(p_device)
    {
        p_device->le_audio_state.mic_state = status;
    }

    if(p_device == GetSelectedLEDevice())
    {
        if(status)
            ui->btn_le_audio_pl_startVoiceCapture->setText(STRING_STOP_MIC);
        else
            ui->btn_le_audio_pl_startVoiceCapture->setText(STRING_START_MIC);
    }
}

void MainWindow::le_audio_handle_convo_stream_status(uint8_t *p_data)
{
    uint16_t conn_id;
    uint8_t status;
    STREAM_TO_UINT16(conn_id, p_data);
    STREAM_TO_UINT8(status, p_data);

    CBtDevice *p_device = FindInList(WICED_CONNECTION_TYPE_LE, conn_id, ui->cbBLEDeviceList);
    if(p_device)
    {
        p_device->le_audio_state.bidir_mic_state = status;
    }

    if(p_device == GetSelectedLEDevice())
    {
        if(status)
            ui->btn_le_audio_pl_bidirmic_start_stop->setText(STRING_STOP_MIC);
        else
            ui->btn_le_audio_pl_bidirmic_start_stop->setText(STRING_START_MIC);
    }
}

typedef enum
{
    BAP_CODEC_CAPABILITIES_INVALID_TYPE,
    BAP_CODEC_CAPABILITIES_SUPPORTED_SAMPLING_FREQUENCIES_TYPE,
    BAP_CODEC_CAPABILITIES_SUPPORTED_FRAME_DURATIONS_TYPE,
    BAP_CODEC_CAPABILITIES_SUPPORTED_AUDIO_CHANNEL_COUNTS_TYPE,
    BAP_CODEC_CAPABILITIES_SUPPORTED_OCTETS_PER_CODEC_FRAME_TYPE,
    BAP_CODEC_CAPABILITIES_SUPPORTED_MAX_CODEC_FRAMES_PER_SDU_TYPE,
    BAP_CODEC_CAPABILITIES_MAX_TYPE = BAP_CODEC_CAPABILITIES_SUPPORTED_MAX_CODEC_FRAMES_PER_SDU_TYPE
} bap_codec_capabilities_type_t;

uint16_t get_sf_index(uint8_t audio_config_idx)
{
    for(uint32_t i = 0; i< sizeof(sampling_freq)/sizeof(sampling_freq[0]); i++)
    {
        if(audio_config[audio_config_idx].sampling_frequency == sampling_freq[i])
            return i;
    }
    return 0xffff;
}

uint8_t get_fd_index(uint8_t audio_config_idx)
{
   if(audio_config[audio_config_idx].frame_duration == 7500)
       return 0;
   else if(audio_config[audio_config_idx].frame_duration == 10000)
       return 1;
    return 0xff;
}

bool if_audio_config_supported(uint8_t audio_config, uint16_t sf_bits, uint8_t fd_bits)
{
    if(sf_bits & (1 <<(get_sf_index(audio_config))) && (fd_bits & (1 << get_fd_index(audio_config))))
        return TRUE;
    return FALSE;
}

void MainWindow::le_audio_pl_update_listed_audio_config(CBtDevice *p_device)
{
    if(!p_device || p_device != GetSelectedLEDevice())
        return;

    ui->le_audio_pl_list_cc_in_audioConfig->clear();
    ui->le_audio_pl_list_cc_out_audioConfig->clear();
    ui->le_audio_pl_list_media_audioConfig->clear();
    ui->le_audio_pl_list_mic_audioConfig->clear();

    for(int i =BAP_AUDIO_CONFIG_8_1; i<= BAP_AUDIO_CONFIG_48_6; i++)
    {
        uint8_t sink_supported = p_device->sink_supported_config[i].is_supported;
        uint8_t source_supported = p_device->source_supported_config[i].is_supported;

        if(sink_supported)
        {
            ui->le_audio_pl_list_media_audioConfig->addItem(config_str[i], i);
            ui->le_audio_pl_list_cc_out_audioConfig->addItem(config_str[i], i);
        }
        if(source_supported)
        {
            ui->le_audio_pl_list_mic_audioConfig->addItem(config_str[i], i);
            ui->le_audio_pl_list_cc_in_audioConfig->addItem(config_str[i], i);
        }
    }
}

void MainWindow::le_audio_handle_pacs_record(uint8_t *p_data)
{
    uint16_t conn_id;
    STREAM_TO_UINT16(conn_id, p_data);
    CBtDevice *p_device = FindInList(WICED_CONNECTION_TYPE_LE, conn_id, ui->cbBLEDeviceList);
    if(!p_device)
    {
        return;
    }

    uint8_t is_sink;
    STREAM_TO_UINT8(is_sink, p_data);
    uint8_t num_of_records;
    STREAM_TO_UINT8(num_of_records, p_data);
    for(int i = 0; i<num_of_records; i++)
    {
        uint8_t coding_format;
        uint16_t company_id;
        uint16_t vendor_specific_id;
        STREAM_TO_UINT8(coding_format, p_data);
        STREAM_TO_UINT16(company_id, p_data);
        STREAM_TO_UINT16(vendor_specific_id, p_data);
        uint8_t csc_len;
        STREAM_TO_UINT8(csc_len, p_data);
        uint16_t min_data_per_frame = 0;  /**< Min data per codec frame */
        uint16_t sf = 0;                 /**< Sampling frequency */
        uint8_t frame_duration = 0;      /**< Frame duration */
        uint8_t audio_ch_count = 0;      /**< Audio channel count */
        uint8_t frame_per_sdu = 0;       /**< Frame per sdu */
        while(csc_len > 2)
        {
            uint8_t len;
            uint8_t type;
            STREAM_TO_UINT8(len, p_data);
            STREAM_TO_UINT8(type, p_data);
            switch(type)
            {
            case BAP_CODEC_CAPABILITIES_SUPPORTED_SAMPLING_FREQUENCIES_TYPE:
                STREAM_TO_UINT16(sf, p_data);
                break;
            case BAP_CODEC_CAPABILITIES_SUPPORTED_FRAME_DURATIONS_TYPE:
                STREAM_TO_UINT8(frame_duration, p_data);
                break;
            case BAP_CODEC_CAPABILITIES_SUPPORTED_AUDIO_CHANNEL_COUNTS_TYPE:
                STREAM_TO_UINT8(audio_ch_count, p_data);
                break;
            case BAP_CODEC_CAPABILITIES_SUPPORTED_OCTETS_PER_CODEC_FRAME_TYPE:
            {
                uint16_t max_data_per_frame = 0;
                STREAM_TO_UINT16(min_data_per_frame, p_data);
                STREAM_TO_UINT16(max_data_per_frame, p_data);
                UNUSED(max_data_per_frame);
            }break;
            case BAP_CODEC_CAPABILITIES_SUPPORTED_MAX_CODEC_FRAMES_PER_SDU_TYPE:
                STREAM_TO_UINT8(frame_per_sdu, p_data);
                break;
            default :
                p_data += (len - 1);
                break;
            }
            csc_len-=(len+1);
        }
        for(int config =BAP_AUDIO_CONFIG_8_1; config <= BAP_AUDIO_CONFIG_48_6; config++)
        {
            if(if_audio_config_supported(config, sf, frame_duration) && (min_data_per_frame <= audio_config[config].ocpf))
            {
                le_audio_codec_config_t *p_audio_config = (is_sink == 1)? & p_device->sink_supported_config[config] :& p_device->source_supported_config[config];
                p_audio_config->is_supported = 1;
                p_audio_config->coding_format = coding_format;
                p_audio_config->company_id = company_id;
                p_audio_config->vendor_specific_codec_id = vendor_specific_id;
                p_audio_config->channel_cnt = audio_ch_count;
                p_audio_config->codec_frames_per_sdu = frame_per_sdu;
                p_audio_config->sampling_frequency= audio_config[config].sampling_frequency;
                p_audio_config->frame_duration = audio_config[config].frame_duration;
                p_audio_config->min_ocpf = min_data_per_frame;
                p_audio_config->max_ocpf = audio_config[config].ocpf;
            }
        }
        uint8_t metadata_len;
        STREAM_TO_UINT8(metadata_len, p_data);
        p_data+=metadata_len;

    }
    le_audio_pl_update_listed_audio_config(p_device);
}

void MainWindow::le_audio_handle_csis_lock_state(uint8_t *p_data)
{
    uint16_t conn_id;
    uint8_t lock_state;
    STREAM_TO_UINT16(conn_id, p_data);
    CBtDevice *p_device = FindInList(WICED_CONNECTION_TYPE_LE, conn_id, ui->cbBLEDeviceList);
    if(!p_device)
    {
        return;
    }
    STREAM_TO_UINT8(lock_state, p_data);
    ui->btn_le_audio_csis_lock->setText((lock_state == 2)?STRING_RELEASE_LOCK:STRING_SET_LOCK);
}

void MainWindow::le_audio_handle_device_appearance(uint8_t *p_data)
{
    uint16_t appearance = 0;
    QString text = "";
    STREAM_TO_UINT16(appearance, p_data);
    if(appearance == LEHS_APPEARANCE_WEARABLE_DEVICE_LEFT_EARBUD)
        text = STRING_EARBUD_LEFT;
    else if(appearance == LEHS_APPEARANCE_WEARABLE_DEVICE_RIGHT_EARBUD)
        text = STRING_EARBUD_RIGHT;
    else if(appearance == LEHS_APPEARANCE_WEARABLE_DEVICE_HEADPHONE)
        text = STRING_HEADPHONE;
    else if(appearance == LEPL_APPEARANCE_GENERIC_AUDIO_SOURCE)
        text = STRING_GENERIC_AUDIO_SOURCE;
    else
        text = "Generic";
    ui->le_audio_device_appearance->setText(text);

}

void MainWindow::on_btn_le_audio_startLeAdv_clicked()
{
    QString text = ui->btn_le_audio_startLeAdv->text();
    bool is_swift_pair = ui->checkBoxSwiftPair->isChecked();
    uint8_t tx_power = ui->TxPowerVal->text().toInt();

    if (text == STRING_START_ADVERTISEMENT)
    {
        app_host_gatt_start_stop_advert(TRUE, is_swift_pair, tx_power);
        ui->btn_le_audio_startLeAdv->setText(STRING_STOP_ADVERTISEMENT);
    }
    else if (text == STRING_STOP_ADVERTISEMENT)
    {
        app_host_gatt_start_stop_advert(FALSE, is_swift_pair, tx_power);
        ui->btn_le_audio_startLeAdv->setText(STRING_START_ADVERTISEMENT);
    }
}

void MainWindow::on_btn_le_audio_hs_setMediaPlayer_clicked()
{
    CBtDevice * p_device = GetSelectedLEDevice();
    uint8_t player_name[50];
    memset(player_name, 0, 50);

    if (p_device == NULL)
        return;

    QString player = ui->le_audio_hs_playerlist->currentText();

    if (player.isNull())
        return;

    memcpy(&player_name, player.toStdString().c_str(), player.size());
    int len = player.size();
    Log("[%s] player %s", TAG, player_name);
    app_host_le_audio_set_media_player(p_device->get_connection_handle(), len, player_name);
}

typedef enum
{
    LEPL_APP_STATE_IDLE,
    LEPL_APP_STATE_BROADCAST,
    LEPL_APP_STATE_MEDIA,
    LEPL_APP_STATE_CALL,
    LEPL_APP_STATE_MIC,
    LEPL_APP_STATE_BIDIRMIC
} le_audio_lepl_app_state_t;

void MainWindow::HandleLEAudioRequestEvent(uint8_t *pu8Data, int len)
{
    if(len < 3)
    {
        Log("HandleLEAudioRequestEvent bad length");
        return;
    }

    if(!m_audio_started ){
        Log("[%s] audio stopped",__FUNCTION__);
        return;
    }


    int bytes_per_packet = pu8Data[0] | (pu8Data[1] << 8);
    int num_packets = pu8Data[2];

    //Log("pkt len %d bytes %d packets",bytes_per_packet, num_packets);

    m_uAudio.m_BytesPerPacket = bytes_per_packet;

    m_audio_packets.lock();
    m_uAudio.m_PacketsToSend += num_packets;

    m_audio_packets.unlock();
    // m_audio_play_status_send_limit_counter will be reset on play status timeout.
    m_audio_play_status_send_limit_counter += num_packets;
#ifdef A2DP_STATS
    m_audio_total_sent_pkt_count += num_packets;
#endif
    if (m_audio_play_status_send_limit_counter > m_audio_play_status_send_limit_count)
    {
        m_audio_play_status_send_limit_counter -= m_audio_play_status_send_limit_count;
        // TODO : Update player status from other thread module
        //PlayerStatus();
    }


    if (pAudioFileWriter == NULL)
    {
        Log("thread not running\n");
        return;
    }

    if (!m_uAudio.m_pAudioData)
    {
        Log("Setup the audio file\n");
        return;
    }

    // signal the thread to send down data to embedded app
    audio_tx_wait.wakeAll();
}

void MainWindow::HandleLEAudioStartEvent(uint8_t *p_data, int len)
{
    uint32_t app_state;
    QString audio_file_name = ui->le_audio_pl_broadcast_LeAudioFile->text();
    if(p_data && (len >=4))
    {
        STREAM_TO_UINT32(app_state, p_data);
        Log("[%s] %d", TAG, app_state);
        switch(app_state)
        {
        case LEPL_APP_STATE_MEDIA:
            audio_file_name = ui->le_audio_pl_media_LeAudioFile->text();
            break;
        case LEPL_APP_STATE_CALL:
            audio_file_name = ui->le_audio_pl_ringtone_LeAudioFile->text();
            break;
        default:
            break;
        }
    }

    if (!m_audio_started)
    {
        Log("Audio started");
        m_audio_format  = 0; // wav

        Log("Initializing audio file");
        if (!InitializeAudioFile(audio_file_name))
        {
            return;
        }
        m_uAudio.m_PacketsSent = 0;
        m_uAudio.m_PacketsToSend = 0;
        set_audio_started_status(true);

        Log("Initialized audio file: %s", audio_file_name.toStdString().c_str());
        //DisableAppTraces();
    }
}
void MainWindow::HandleLEAudioStopEvent(uint8_t *p_data, int len)
{
    UNUSED(p_data);
    UNUSED(len);

    m_audio_started = false;
    //EnableAppTraces();
    Log("Audio stopped");
}

void MainWindow::on_btn_le_audio_pl_media_findFile_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Open Audio File"),
        "", tr("Audio Files (*.wav)"));
    ui->le_audio_pl_media_LeAudioFile->setText(fileName);
    m_settings.setValue("LEAudioFile",fileName);
}

void MainWindow::on_btn_le_audio_pl_ringtone_findFile_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Open Audio File"),
        "", tr("Audio Files (*.wav)"));
    ui->le_audio_pl_ringtone_LeAudioFile->setText(fileName);
    m_settings.setValue("LEAudioFile",fileName);
}

void MainWindow::on_btn_le_audio_pl_broadcast_findFile_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Open Audio File"),
        "", tr("Audio Files (*.wav)"));
    ui->le_audio_pl_broadcast_LeAudioFile->setText(fileName);
    m_settings.setValue("LEAudioFile",fileName);
}


void MainWindow::on_btn_le_audio_hs_playPause_clicked()
{
    QString text = ui->btn_le_audio_hs_playPause->text();

    if (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SINK)
    {
        CBtDevice *p_device = GetSelectedLEDevice();
        if (p_device == NULL){
            Log("[%s] Headset play/pause pressed, no connection...", TAG);
            return;
        }

        Log("[%s] Headset %s pressed ", TAG, text.toStdString().c_str());
        if (text == STRING_PLAY)
        {
            if(p_device->is_app_status_ready()){
                app_host_le_audio_play(p_device->get_connection_handle(), 0, NULL);
            }else{
                Log("[%s] not ready to play", TAG);
            }
        }
        else if (text == STRING_PAUSE)
        {
            app_host_le_audio_pause(p_device->get_connection_handle());
        }
    }
}


void MainWindow::on_btn_le_audio_pl_playPause_clicked()
{
    QString text = ui->btn_le_audio_pl_playPause->text();

    Log("[%s] Player PlayPause pressed",TAG);
    if (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE)
    {
        CBtDevice *p_device = GetSelectedLEDevice();
        if (p_device == NULL){
            Log("[%s] Player PlayPause pressed, no device found",TAG);
            return;
        }


        if (text == STRING_PLAY)
        {
            if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE){

                if(ui->cbCommport->currentText() != "host-mode"){
                    /* invoke only if !host-mode */
                    if(!fileExists(ui->le_audio_pl_media_LeAudioFile->text())){
                        on_btn_le_audio_pl_media_findFile_clicked();
                    }
                }
            }

            if(p_device->is_app_status_ready())
            {
                uint8_t index = ui->le_audio_pl_list_media_audioConfig->currentData().toInt();
                wiced_bt_le_audio_config_t config;
                memcpy(&config, &audio_config[index], sizeof(wiced_bt_le_audio_config_t));
                if(config.ocpf < p_device->sink_supported_config[index].min_ocpf)
                    config.ocpf = p_device->sink_supported_config[index].min_ocpf;
                if(config.ocpf > p_device->sink_supported_config[index].max_ocpf)
                    config.ocpf = p_device->sink_supported_config[index].max_ocpf;
                m_settings.setValue("LEAudioCodecConfig",ui->le_audio_pl_list_media_audioConfig->currentText());

                app_host_le_audio_play(p_device->get_connection_handle(),p_device->sink_supported_config[index].channel_cnt, &config);
            }
        }
        else if (text == STRING_PAUSE)
        {
            app_host_le_audio_pause(p_device->get_connection_handle());
        }
    }
}

void MainWindow::on_btn_le_audio_pl_startVoiceCapture_clicked()
{
    QString text = ui->btn_le_audio_pl_startVoiceCapture->text();
    CBtDevice *p_device = GetSelectedLEDevice();
    if (p_device == NULL){
        Log("[%s] no device found",TAG);
        return;
    }

    if (text == STRING_START_MIC)
    {
        if(p_device->is_app_status_ready())
        {
            uint8_t index = ui->le_audio_pl_list_mic_audioConfig->currentData().toInt();
            wiced_bt_le_audio_config_t config;
            memcpy(&config, &audio_config[index], sizeof(wiced_bt_le_audio_config_t));
            if(config.ocpf < p_device->source_supported_config[index].min_ocpf)
                config.ocpf = p_device->source_supported_config[index].min_ocpf;
            if(config.ocpf > p_device->source_supported_config[index].max_ocpf)
                config.ocpf = p_device->source_supported_config[index].max_ocpf;

            m_settings.setValue("LEAudioCodecConfig",ui->le_audio_pl_list_mic_audioConfig->currentText());

            app_host_le_audio_start_voice_capture(1, p_device->get_connection_handle(), p_device->source_supported_config[index].channel_cnt, &config);
        }else{
            Log("StartMic: Device not ready to play");
        }
    }
    else if (text == STRING_STOP_MIC)
    {
        app_host_le_audio_start_voice_capture(0, p_device->get_connection_handle(), 0, NULL);
    }
}

void MainWindow::on_btn_le_audio_pl_bidirmic_start_stop_clicked()
{
    QString text = ui->btn_le_audio_pl_bidirmic_start_stop->text();
    CBtDevice *p_device = GetSelectedLEDevice();
    if (p_device == NULL){
        Log("[%s] no device found",TAG);
        return;
    }

    if (text == STRING_START_MIC)
    {
        if(p_device->is_app_status_ready())
        {
            uint8_t index = ui->le_audio_pl_list_cc_in_audioConfig->currentData().toInt();
            wiced_bt_le_audio_config_t in_config;
            wiced_bt_le_audio_config_t out_config;
            memcpy(&in_config, &audio_config[index], sizeof(wiced_bt_le_audio_config_t));
            if(in_config.ocpf < p_device->source_supported_config[index].min_ocpf)
                in_config.ocpf = p_device->source_supported_config[index].min_ocpf;
            if(in_config.ocpf > p_device->source_supported_config[index].max_ocpf)
                in_config.ocpf = p_device->source_supported_config[index].max_ocpf;

            index = ui->le_audio_pl_list_cc_out_audioConfig->currentData().toInt();
            memcpy(&out_config, &audio_config[index], sizeof(wiced_bt_le_audio_config_t));
            if(out_config.ocpf < p_device->sink_supported_config[index].min_ocpf)
                out_config.ocpf = p_device->sink_supported_config[index].min_ocpf;
            if(out_config.ocpf > p_device->sink_supported_config[index].max_ocpf)
                out_config.ocpf = p_device->sink_supported_config[index].max_ocpf;

            m_settings.setValue("LEAudioCodecConfig",ui->le_audio_pl_list_cc_in_audioConfig->currentText());

            app_host_le_audio_start_stop_bidir_mic(1, p_device->get_connection_handle(),p_device->source_supported_config[index].channel_cnt, p_device->sink_supported_config[index].channel_cnt, &in_config, &out_config);
        }else{
            Log("StartMic: Device not ready to play");
        }
    }
    else if (text == STRING_STOP_MIC)
    {
        app_host_le_audio_start_stop_bidir_mic(0, p_device->get_connection_handle(), 0, 0, NULL, NULL);
    }
}

void MainWindow::handle_bt_le_audio_muteUnmute_btn(QPushButton* p_btn)
{
    QString text = p_btn->text();
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;

    if(p_device){
        conn_id =p_device->get_connection_handle();
    }

    if (text == STRING_MUTE)
    {
        p_btn->setText(STRING_UNMUTE);
        app_host_le_audio_command(conn_id, WICED_LE_AUDIO_MUTE_CMD);
    }
    else if (text == STRING_UNMUTE)
    {
        p_btn->setText(STRING_MUTE);
        app_host_le_audio_command(conn_id, WICED_LE_AUDIO_UNMUTE_CMD);
    }
}

void MainWindow::on_btn_le_audio_muteUnmute_clicked()
{
    handle_bt_le_audio_muteUnmute_btn(ui->btn_le_audio_muteUnmute);
}

void MainWindow::handle_le_audio_volUpDown_clicked_btn(int cmd)
{
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;

    if(p_device)
        conn_id = le_audio_update_conn_id(p_device);

    app_host_le_audio_command(conn_id, cmd);
}

void MainWindow::on_btn_le_audio_volDown_clicked()
{
    if (ui->btn_le_audio_muteUnmute->text() == STRING_UNMUTE){
        handle_le_audio_volUpDown_clicked_btn(WICED_LE_AUDIO_UNMUTE_RELATIVE_VOL_DOWN_CMD);
    }else{
        handle_le_audio_volUpDown_clicked_btn(WICED_LE_AUDIO_VOL_DOWN_CMD);
    }

}

void MainWindow::on_btn_le_audio_volUp_clicked()
{
    if (ui->btn_le_audio_muteUnmute->text() == STRING_UNMUTE){
        handle_le_audio_volUpDown_clicked_btn(WICED_LE_AUDIO_UNMUTE_RELATIVE_VOL_UP_CMD);
    }else{
        handle_le_audio_volUpDown_clicked_btn(WICED_LE_AUDIO_VOL_UP_CMD);
    }
}

void MainWindow::handle_le_audio_absVol_btn(QString absvol_str)
{
    int absvol = absvol_str.toInt();

    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;

    if(p_device)
        conn_id = le_audio_update_conn_id(p_device);

    app_host_le_audio_set_abs_volume(conn_id, absvol > 255 ? 255 : absvol);
}

void MainWindow::on_btn_le_audio_absVol_clicked()
{
    handle_le_audio_absVol_btn(ui->le_audio_absVal->text());
}

void MainWindow::on_btn_le_audio_rc_playPause_clicked()
{
    QString text = ui->btn_le_audio_rc_playPause->text();

    if (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_REMOTE_CONTROL)
    {
        CBtDevice *p_device = GetSelectedLEDevice();
        if (p_device == NULL){
            Log("[%s] RC play/pause pressed, no connection...", TAG);
            return;
        }

        Log("[%s] RC %s pressed ", TAG, text.toStdString().c_str());
        if (text == STRING_PLAY)
        {
            if(p_device->is_app_status_ready()){
                app_host_le_audio_play(p_device->get_connection_handle(), 0, NULL);
            }else{
                Log("[%s] not ready to play", TAG);
            }
        }
        else if (text == STRING_PAUSE)
        {
            app_host_le_audio_pause(p_device->get_connection_handle());
        }
    }
}

void MainWindow::on_btn_le_audio_rc_setMediaPlayer_clicked()
{
    CBtDevice * p_device = GetSelectedLEDevice();
    uint8_t player_name[50];
    memset(player_name, 0, 50);

    if (p_device == NULL)
        return;

    QString player = ui->le_audio_rc_playerlist->currentText();

    if (player.isNull())
        return;

    memcpy(&player_name, player.toStdString().c_str(), player.size());
    int len = player.size();
    Log("[%s] player %s", TAG, player_name);
    app_host_le_audio_set_media_player(p_device->get_connection_handle(), len, player_name);
}

void MainWindow::on_btn_le_audio_rc_InitiateCall_clicked()
{
    CBtDevice *p_device =  GetSelectedLEDevice();
    if(!p_device || !p_device->get_connection_handle())
        return;
    uint16_t conn_id = p_device->get_connection_handle();

    ui->btn_le_audio_rc_TerminateCall->setEnabled(TRUE);
    QString text = ui->btn_le_audio_rc_InitiateCall->text();
    if(text == STRING_INITIATE_CALL)
    {
        app_host_le_audio_originate_call(conn_id, (uint8_t *)ui->le_audio_rc_callURI->text().toStdString().c_str(), ui->le_audio_rc_callURI->text().toStdString().length());
        m_settings.setValue("LEAudioCallURI", ui->le_audio_rc_callURI->text());
    }
    else if(text == STRING_ACCEPT_CALL)
    {
        app_host_le_audio_handle_call_action(conn_id, ui->le_audio_rc_callId->text().toUInt(), WICED_BT_GA_CCP_ACTION_ACCEPT_CALL);
    }
    else if(text == STRING_PUT_ON_HOLD)
    {
        app_host_le_audio_handle_call_action(conn_id, ui->le_audio_rc_callId->text().toUInt(), WICED_BT_GA_CCP_ACTION_HOLD_CALL);
    }
    else if(text == STRING_RETRIEVE_CALL)
    {
        app_host_le_audio_handle_call_action(conn_id, ui->le_audio_rc_callId->text().toUInt(), WICED_BT_GA_CCP_ACTION_RETRIEVE_CALL);
    }
}

void MainWindow::on_btn_le_audio_rc_TerminateCall_clicked()
{
    CBtDevice *p_device =  GetSelectedLEDevice();
    if(!p_device || !p_device->get_connection_handle())
        return;

    uint16_t conn_id = p_device->get_connection_handle();
    app_host_le_audio_terminate_call(conn_id, ui->le_audio_rc_callId->text().toUInt(), TRUE);
}

void MainWindow::le_audio_handle_connection()
{
    if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SINK)
    {
        QWidget * p_list[] = {
            ui->btn_le_audio_connectToPeer
        };
        initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), 1, 0);
    }
}

void  MainWindow::le_audio_handle_disconnection(uint8_t *p_data)
{
    uint16_t conn_id;
    STREAM_TO_UINT16(conn_id, p_data);

    if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SINK)
    {
        QWidget * p_list[] = {
            ui->btn_le_audio_connectToPeer
        };
        initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), 0, 0);
    }

    CBtDevice *p_device = FindInList(WICED_CONNECTION_TYPE_LE, conn_id, ui->cbBLEDeviceList);

    if(p_device)
    {
        p_device->le_audio_state.media_state = 0;
        p_device->le_audio_state.mic_state = 0;
        p_device->le_audio_state.bidir_mic_state = 0;
    }

    if(p_device != GetSelectedLEDevice())
        return;

    if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE)
    {
        ui->btn_le_audio_pl_playPause->setText(STRING_PLAY);
        ui->btn_le_audio_pl_startVoiceCapture->setText(STRING_START_MIC);
        ui->le_audio_mics_aics_desc->clear();
        ui->le_audio_pl_list_media_audioConfig->clear();
        ui->le_audio_pl_list_mic_audioConfig->clear();
    }
    if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_SERVER)
    {
        ui->le_audio_mics_aics_desc->clear();
        ui->btn_le_audio_pl_simulateCall->setText(STRING_SIMULATE_CALL);
        ui->btn_le_audio_pl_simulateHold->setText(STRING_SIMULATE_REMOTE_HOLD);
        ui->le_audio_pl_list_cc_in_audioConfig->clear();
        ui->le_audio_pl_list_cc_out_audioConfig->clear();
    }
    if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SINK)
    {
        ui->btn_le_audio_hs_playPause->setText(STRING_PLAY);
    }
    if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_REMOTE_CONTROL)
    {
        ui->btn_le_audio_rc_playPause->setText(STRING_PLAY);
        ui->btn_le_audio_rc_InitiateCall->setText(STRING_INITIATE_CALL);
    }
    if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_CLIENT)
    {
        InitLeAudioHeadsetCallControl(FALSE);
    }
}

void MainWindow::le_audio_handle_update_media_player_list(uint8_t len, uint8_t *p_data)
{
    int currlen = 0;
    int players_num;
    STREAM_TO_UINT8(players_num, p_data);
    for (int i = 0; i < players_num; i++)
    {
        char player[50];
        memset(player, 0, 50);
        STREAM_TO_UINT8(currlen, p_data);
        memcpy(player, p_data, currlen);
        Log("[%s] player %s", TAG, player);
        p_data = p_data + currlen;
        if(is_le_audio_headset())
        {
            int index = ui->le_audio_hs_playerlist->findText(player, Qt::MatchStartsWith);
            if (len != 0 && index == -1)
            {
                ui->le_audio_hs_playerlist->addItem(player);
            }
        }
        if(is_le_remote_control())
        {
            int index = ui->le_audio_rc_playerlist->findText(player, Qt::MatchStartsWith);
            if (len != 0 && index == -1)
            {
                ui->le_audio_rc_playerlist->addItem(player);
            }
        }
    }
}

void MainWindow::le_audio_handle_update_media_player_status(uint8_t *p_data)
{
    uint16_t conn_id;
    uint8_t status;
    QString new_btn_text = STRING_PLAY;

    STREAM_TO_UINT16(conn_id, p_data);
    STREAM_TO_UINT8(status, p_data);
    LogMediaState(status);

    CBtDevice *p_device = FindInList(WICED_CONNECTION_TYPE_LE, conn_id, ui->cbBLEDeviceList);

    if(p_device)
        p_device->le_audio_state.media_state = status;

    if(p_device != GetSelectedLEDevice())
        return;

    if (status == 1)
    {
        // playing
        new_btn_text = STRING_PAUSE;
    }

    if(is_le_audio_headset()){
        ui->btn_le_audio_hs_playPause->setText(new_btn_text);
    }
    else if(is_le_remote_control())
    {
        ui->btn_le_audio_rc_playPause->setText(new_btn_text);
    }
    else
    {
        ui->btn_le_audio_pl_playPause->setText(new_btn_text);
    }
}

static name_val_t le_audio_options[] =
{
    {"8_1_1 (7.5ms - 27.73 kbps-LL)",BAP_CODEC_CONFIG_8_1_1},
    {"8_1_2 (7.5ms - 27.73 kbps-HR)",BAP_CODEC_CONFIG_8_1_2},
    {"8_2_1 (10ms - 24 kbps-LL)",BAP_CODEC_CONFIG_8_2_1},
    {"8_2_2 (10ms - 24 kbps-HR)",BAP_CODEC_CONFIG_8_2_2},
    {"16_1_1 (7.5ms - 32 kbps-LL)",BAP_CODEC_CONFIG_16_1_1},
    {"16_1_2 (7.5ms - 32 kbps-HR)",BAP_CODEC_CONFIG_16_1_2},
    {"16_2_1 (10ms - 32 kbps-LL)",BAP_CODEC_CONFIG_16_2_1},
    {"16_2_2 (10ms - 32 kbps-HR)",BAP_CODEC_CONFIG_16_2_2},
    {"24_1_1 (7.5ms - 48 kbps-LL)",BAP_CODEC_CONFIG_24_1_1},
    {"24_1_2 (7.5ms - 48 kbps)-HR",BAP_CODEC_CONFIG_24_1_2},
    {"24_2_1 (10ms - 48 kbps-LL)",BAP_CODEC_CONFIG_24_2_1},
    {"24_2_2 (10ms - 48 kbps-HR)",BAP_CODEC_CONFIG_24_2_2},
    {"32_1_1 (7.5ms - 64 kbps-LL)",BAP_CODEC_CONFIG_32_1_1},
    {"32_1_2 (7.5ms - 64 kbps-HR)",BAP_CODEC_CONFIG_32_1_2},
    {"32_2_1 (10ms - 64 kbps-LL)",BAP_CODEC_CONFIG_32_2_1},
    {"32_2_2 (10ms - 64 kbps-HR)",BAP_CODEC_CONFIG_32_2_2},
    {"441_1_1 (7.5ms - 95.06 kbps-LL)",BAP_CODEC_CONFIG_441_1_1},
    {"441_1_2 (7.5ms - 95.06 kbps-HR)",BAP_CODEC_CONFIG_441_1_2},
    {"441_2_1 (10ms - 95.55 kbps-LL)",BAP_CODEC_CONFIG_441_2_1},
    {"441_2_2 (10ms - 95.55 kbps-HR)",BAP_CODEC_CONFIG_441_2_2},
    {"48_1_1 (7.5ms - 80 kbps-LL)",BAP_CODEC_CONFIG_48_1_1},
    {"48_1_2 (7.5ms - 80 kbps-HR)",BAP_CODEC_CONFIG_48_1_2},
    {"48_2_1 (10ms - 80 kbps-LL)",BAP_CODEC_CONFIG_48_2_1},
    {"48_2_2 (10ms - 80 kbps-HR)",BAP_CODEC_CONFIG_48_2_2},
    {"48_3_1 (7.5ms - 96 kbps-LL)",BAP_CODEC_CONFIG_48_3_1},
    {"48_3_2 (7.5ms - 96 kbps-HR)",BAP_CODEC_CONFIG_48_3_2},
    {"48_4_1 (10ms - 96 kbps-LL)",BAP_CODEC_CONFIG_48_4_1},
    {"48_4_2 (10ms - 96 kbps-HR)",BAP_CODEC_CONFIG_48_4_2},
    {"48_5_1 (7.5ms - 124.8 kbps-LL)",BAP_CODEC_CONFIG_48_5_1},
    {"48_5_2 (7.5ms - 124.8 kbps-HR)",BAP_CODEC_CONFIG_48_5_2},
    {"48_6_1 (10ms - 124 kbps-LL)",BAP_CODEC_CONFIG_48_6_1},
    {"48_6_2 (10ms - 124 kbps-HR)",BAP_CODEC_CONFIG_48_6_2}
};

void MainWindow::initCodecConfigOptions(QComboBox *p_box, const name_val_t *p_options, int num, int enable)
{
    p_box->clear();
    if(!enable){
        return;
    }

    QString previous = "";
    if(m_settings.value("LEAudioCodecConfig").toString().length()){
        previous = m_settings.value("LeAudioCodecConfig").toString();
    }
    int prev_index = 0;

    for(int index = 0; num--;p_options++, index++){
        p_box->addItem(p_options->name, QVariant::fromValue(p_options->data));
        if(previous == p_options->name){
            prev_index = index;
        }
    }

    p_box->setCurrentIndex(prev_index);
}

void MainWindow::init_le_audio_player_codec_config_options(int enable)
{
    initCodecConfigOptions(ui->le_audio_pl_list_broadcast_audioConfig, le_audio_options, sizeof(le_audio_options)/sizeof(le_audio_options[0]), enable);
}

void MainWindow::init_le_audio_player_bis_codec_config_options(int enable)
{
    init_le_audio_player_codec_config_options(enable);
}

void MainWindow::initWidgets(QWidget ** p_list, int num, int enable, int hide = 0)
{
    for(;num--;p_list++){
        if(*p_list){
            if(hide == 1){
                (*p_list)->hide();
            }else{
                (*p_list)->show();
                (*p_list)->setEnabled(enable);
            }
        }
    }
}

void MainWindow:: InitLeAudioHeadsetAdvControl(int enable)
{
    QWidget * p_list[] = {
        ui->btn_le_audio_startLeAdv,
        ui->checkBoxSwiftPair,
        ui->TX_power,
        ui->TxPowerVal
    };
    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable, (enable) ? 0:1);
}

void MainWindow::InitLeAudioHeadsetMediaControl(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_hs_mediaControl,
        ui->btn_le_audio_hs_setMediaPlayer,
        ui->btn_le_audio_hs_playPause,
        ui->le_audio_hs_playerlist,
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);

    ui->btn_le_audio_hs_playPause->setText(STRING_PLAY);
}

void MainWindow::InitLeAudioVolumeControl(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_volControl,
        ui->btn_le_audio_muteUnmute,
        ui->btn_le_audio_volDown,
        ui->btn_le_audio_volUp,
        ui->btn_le_audio_absVol
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);
    ui->btn_le_audio_muteUnmute->setText(STRING_MUTE);

    ui->le_audio_absVal->setValidator(new QIntValidator(0, 255, this));
}

void MainWindow::InitLeAudioMicrophoneControl(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_micpcontrol,
        ui->btn_le_audio_mics_mute,
        ui->btn_le_audio_mics_aics_mute,
        ui->btn_le_audio_inc_mics_aics_gain,
        ui->btn_le_audio_dec_mics_aics_gain,
        ui->btn_le_audio_set_mics_aics_gain,
        ui->le_audio_mics_aics_desc,
        ui->le_audio_mics_aics_gain
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);
    ui->btn_le_audio_muteUnmute->setText(STRING_MUTE);

    ui->le_audio_absVal->setValidator(new QIntValidator(0, 255, this));
}

void MainWindow::InitLeAudioPlayerMediaControl(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_pl_mediaControl,
        ui->le_audio_pl_list_media_audioConfig,
        ui->label_le_audio_pl_media_audioConfig,
        ui->label_le_audio_pl_media_LeAudioFile,
        ui->le_audio_pl_media_LeAudioFile,
        ui->btn_le_audio_pl_media_findFile,
        ui->btn_le_audio_pl_playPause
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);
    ui->le_audio_hs_playerlist->clear();
    ui->btn_le_audio_hs_playPause->setText(STRING_PLAY);
}

void MainWindow::InitLeAudioMICControl(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_pl_mic_control,
        ui->le_audio_pl_list_mic_audioConfig,
        ui->label_le_audio_pl_mic_audioConfig,
        ui->btn_le_audio_pl_startVoiceCapture,
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);
}

void MainWindow::InitLeAudioPlayerBiDirMICControl(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_pl_BiDirMIC_control,
        ui->le_audio_pl_list_cc_in_audioConfig,
        ui->le_audio_pl_list_cc_out_audioConfig,
        ui->label_le_audio_pl_cc_in_audioConfig,
        ui->label_le_audio_pl_cc_out_audioConfig,
        ui->btn_le_audio_pl_bidirmic_start_stop,
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);
}

void MainWindow::InitLeAudioPlayerCallControlSetup(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_pl_callControlSetup,
        ui->le_audio_pl_call_friendlyName,
        ui->le_audio_pl_callURI
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);

    if(m_settings.value("LEAudioCallFriendlyName").toString().length()){
        ui->le_audio_pl_call_friendlyName->setText(m_settings.value("LEAudioCallFriendlyName").toString());
    }
    if(m_settings.value("LEAudioCallURI").toString().length()){
        ui->le_audio_pl_callURI->setText(m_settings.value("LEAudioCallURI").toString());
    }

}

void MainWindow::InitLeAudioPlayerCallControls(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_pl_callControls,
        ui->btn_le_audio_pl_simulateCall,
        ui->btn_le_audio_pl_simulateHold,
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);
}

void MainWindow::InitLeAudioPlayerCallControlAudioConfig(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_pl_cc_audioConfig,
        ui->label_le_audio_pl_ringtone_LeAudioFile,
        ui->label_le_audio_pl_cc_in_audioConfig,
        ui->label_le_audio_pl_cc_out_audioConfig,
        ui->le_audio_pl_ringtone_LeAudioFile,
        ui->le_audio_pl_list_cc_in_audioConfig,
         ui->le_audio_pl_list_cc_in_audioConfig,
        ui->btn_le_audio_pl_ringtone_findFile
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);
}


void MainWindow::InitLeAudioPlayerBroadcastSource(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_pl_bcastSource,
        ui->label_le_audio_pl_broadcast_LeAudioFile,
        ui->label_le_audio_pl_cc_in_audioConfig,
        ui->label_le_audio_pl_cc_out_audioConfig,
        ui->le_audio_pl_bis_channelCount,
        ui->le_audio_pl_bisCount,
        ui->le_audio_pl_bisEncrypt,
        ui->le_audio_pl_bcastCode,
        ui->le_audio_pl_bcastID,
        ui->le_audio_pl_broadcast_LeAudioFile,
        ui->le_audio_pl_list_broadcast_audioConfig,
        ui->btn_le_audio_pl_broadcast_findFile,
        ui->btn_le_audio_pl_startBcast
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);
    init_le_audio_player_bis_codec_config_options(enable);

    ui->le_audio_pl_bcastCode->setText("12345678");
    ui->le_audio_pl_bcastID->setText("1234");
    init_le_audio_player_bis_codec_config_options(enable);
    ui->btn_le_audio_pl_startBcast->setText(STRING_START_BROADCAST);
}

void MainWindow::InitLeAudioBroadcastAssistant(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_bcastAssistant,
        ui->btn_le_audio_ba_scan_bcastStreams,
        ui->le_audio_ba_found_bcastStreams,
        ui->le_audio_ba_bcastCode,
        ui->btn_le_audio_ba_addSource,
        ui->btn_le_audio_ba_addSourceWithPast,
        ui->btn_le_audio_ba_removeSource
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);

    ui->btn_le_audio_ba_scan_bcastStreams->setText(STRING_DISCOVER_SOURCES);
    ui->le_audio_ba_found_bcastStreams->clear();
}

void MainWindow::InitLeAudioHeadsetBroadcastSink(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_hs_bcastSink,
        ui->btn_le_audio_hs_scan_bcastStreams,
        ui->btn_le_audio_hs_bcast_SyncToStream,
        ui->btn_le_audio_hs_bcast_get_Streams,
        ui->le_audio_hs_found_bcastSource,
    };
    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);

    if(!enable){
        ui->le_audio_hs_found_bcastSource->clear();
    }

    ui->btn_le_audio_hs_scan_bcastStreams->setText(STRING_DISCOVER_SOURCES);
    ui->btn_le_audio_hs_bcast_get_Streams->setText(STRING_GET_STREAMS);
    ui->btn_le_audio_hs_bcast_SyncToStream->setText(STRING_SYNC_TO_STREAM);
    ui->le_audio_hs_found_bcastSource->clear();
}

void MainWindow::InitLeAudioPlayeMICControls(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_pl_mic_control,
        ui->btn_le_audio_pl_startVoiceCapture
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);
    init_le_audio_player_bis_codec_config_options(enable);
    ui->btn_le_audio_pl_startVoiceCapture->setText(STRING_START_MIC);
}

void MainWindow :: InitLEAudioPlayerHAPControl(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_pl_hap_control,
        ui->le_audio_hap_preset_name,
        ui->btn_le_audio_pl_read_preset,
        ui->btn_le_audio_pl_hap_set_active_preset,
        ui->btn_le_audio_pl_read_preset,
        ui->btn_le_audio_pl_hap_set_next_preset,
        ui->btn_le_audio_pl_hap_set_previous_preset,
    };
    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);
}

void MainWindow::InitLeAudioHeadsetCallControl(int enable)
{
    QWidget * p_list[] ={ui->groupBox_le_audio_hs_call_control,
                        ui->le_audio_hs_ccp_call_friendly_name,
                        ui->le_audio_hs_ccp_call_URI,
                        ui->le_audio_hs_ccp_call_id,
                        ui->btn_le_audio_hs_ccp_accept_retrieve_call,
                        ui->btn_le_audio_hs_ccp_reject_terminate_call
                        };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);

    ui->btn_le_audio_hs_ccp_accept_retrieve_call->setText(STRING_ACCEPT_CALL);
    ui->btn_le_audio_hs_ccp_reject_terminate_call->setText(STRING_REJECT_CALL);
}

void MainWindow::InitLeAudioRemoteControlMediaControl(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_rc_mediaControl,
        ui->btn_le_audio_rc_setMediaPlayer,
        ui->btn_le_audio_rc_playPause,
        ui->le_audio_rc_playerlist,
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);

    ui->btn_le_audio_rc_playPause->setText(STRING_PLAY);
}

void MainWindow::InitLeAudioRemoteControlCallControlSetup(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_rc_callControlSetup,
        ui->le_audio_rc_call_friendlyName,
        ui->le_audio_rc_callURI,
        ui->le_audio_rc_callId
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);

    if(m_settings.value("LEAudioCallFriendlyName").toString().length()){
        ui->le_audio_rc_call_friendlyName->setText(m_settings.value("LEAudioCallFriendlyName").toString());
    }
    if(m_settings.value("LEAudioCallURI").toString().length()){
        ui->le_audio_rc_callURI->setText(m_settings.value("LEAudioCallURI").toString());
    }

}

void MainWindow::InitLeAudioRemoteControlCallControls(int enable)
{
    QWidget * p_list[] = {
        ui->groupBox_le_audio_rc_callControls,
        ui->btn_le_audio_rc_InitiateCall,
        ui->btn_le_audio_rc_TerminateCall,
    };

    initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), enable);
}

QString MainWindow::add_text_with_delimiter(QString text, QString add, QString delimiter = ", ")
{
    if(text.length()){
        return delimiter + add;
    }

    return add;
}

void MainWindow::UpdateLEAudioRole()
{
    uint32_t audio_role = le_audio_dev_role;
    uint32_t mask_bit = 0;
    QString text = "";

    if(audio_role){
        ui->groupBox_leaudio_connect->setEnabled(TRUE);
    }

   do
    {
        if(!(audio_role >> mask_bit)){
            break;
        }

        switch (audio_role & (1 << mask_bit))
        {
        case HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE:
        {
            text += add_text_with_delimiter(text, "Unicast Source");

            QWidget * p_list[] = {
                ui->btn_le_audio_connectToPeer
            };
            initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), 1, 0);

            InitLeAudioHeadsetAdvControl(FALSE);
            InitLeAudioPlayerMediaControl(TRUE);
            InitLeAudioVolumeControl(TRUE);
            InitLeAudioMicrophoneControl(TRUE);
            InitLEAudioPlayerHAPControl(TRUE);
            InitLeAudioPlayeMICControls(TRUE);
            InitLeAudioPlayerBiDirMICControl(TRUE);
        }break;
        case HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SINK:
        {
            text += add_text_with_delimiter(text, "Unicast Sink");

            QWidget * p_list[] = {
                ui->btn_le_audio_connectToPeer
            };
            initWidgets(p_list, sizeof(p_list)/sizeof(p_list[0]), 0, 1);

            InitLeAudioHeadsetAdvControl(TRUE);
            InitLeAudioHeadsetMediaControl(TRUE);
            InitLeAudioVolumeControl(TRUE);
            InitLeAudioMicrophoneControl(TRUE);
        }break;
        case HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_SERVER:
            text += add_text_with_delimiter(text, "Call Control Server (Phone)");
            InitLeAudioPlayerCallControlSetup(TRUE);
            InitLeAudioPlayerCallControlAudioConfig(TRUE);
            InitLeAudioPlayerCallControls(TRUE);
            InitLeAudioMicrophoneControl(TRUE);
            InitLEAudioPlayerHAPControl(TRUE);
            break;
        case HCI_CONTROL_LE_AUDIO_DEV_ROLE_CALL_CONTROL_CLIENT:
            text += add_text_with_delimiter(text, "Call Control Client (EB)");
            InitLeAudioMicrophoneControl(FALSE);
            break;
        case HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_SOURCE:
            text += add_text_with_delimiter(text, "Broadcast Source");
            InitLeAudioPlayerBroadcastSource(TRUE);
            break;
        case HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_SINK:
            text += add_text_with_delimiter(text, "Broadcast Sink");

            InitLeAudioHeadsetBroadcastSink(TRUE);
            InitLeAudioVolumeControl(TRUE);
            break;
        case HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_ASSISTANT:
            text += add_text_with_delimiter(text, "Broadcast Assistant");
            InitLeAudioBroadcastAssistant(TRUE);
            break;
        case HCI_CONTROL_LE_AUDIO_DEV_ROLE_REMOTE_CONTROL:
            text += add_text_with_delimiter(text, "LE Remote Control");
            InitLeAudioRemoteControlMediaControl(TRUE);
            InitLeAudioRemoteControlCallControlSetup(TRUE);
            InitLeAudioRemoteControlCallControls(TRUE);
            InitLeAudioVolumeControl(TRUE);
            InitLeAudioMicrophoneControl(TRUE);
            break;
        }
    }while(++mask_bit < 32);

    if(m_settings.value("LEAudioFile").toString().length()){
        ui->le_audio_pl_media_LeAudioFile->setText(m_settings.value("LEAudioFile").toString());
    }

    if(m_settings.value("LEAudioFile").toString().length()){
        ui->le_audio_pl_ringtone_LeAudioFile->setText(m_settings.value("LEAudioFile").toString());
    }

    if(m_settings.value("LEAudioFile").toString().length()){
        ui->le_audio_pl_broadcast_LeAudioFile->setText(m_settings.value("LEAudioFile").toString());
    }

    ui->le_audio_deviceRoleTxt->setText("Device Role: " + text);
    ui->btn_le_audio_startLeAdv->setText(STRING_START_ADVERTISEMENT);

    if(le_audio_dev_role & 0xf){
        ui->tabLeAudioRole->setCurrentWidget(ui->tabLEPlayer);
    }else if(le_audio_dev_role & 0x80){
        ui->tabLeAudioRole->setCurrentWidget(ui->tabLEBroadcastAssistant);
    }else if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_REMOTE_CONTROL){
        ui->tabLeAudioRole->setCurrentWidget(ui->tabLERemoteControl);
    }else {
        ui->tabLeAudioRole->setCurrentWidget(ui->tabLEHeadset);
    }
}

void MainWindow::LogMediaState(int state)
{
    switch (state)
    {
    case 0:
        Log("[%s] Media status inactive", TAG);
        break;
    case 1:
        Log("[%s] Media status playing", TAG);
        break;
    case 2:
        Log("[%s] Media status paused", TAG);
        break;
    case 3:
        Log("[%s] Media status seeking", TAG);
        break;
    }
}

bool MainWindow::is_le_audio_headset()
{
    if((le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SINK)||
            (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_SINK))
    {
        return TRUE;
    }

    return FALSE;
}

bool MainWindow::is_le_audio_player()
{
    if((le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SOURCE)||
            (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_SOURCE))
    {
        return TRUE;
    }

    return FALSE;
}

bool MainWindow::is_le_remote_control()
{
    if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_REMOTE_CONTROL)
    {
        return TRUE;
    }

    return FALSE;
}

void MainWindow::le_audio_handle_update_mute_state(int state)
{
    QString state_text = STRING_MUTE;

    if (state == 1)
    {
        state_text = STRING_UNMUTE;
    }
        ui->btn_le_audio_muteUnmute->setText(state_text);
}

void MainWindow::le_audio_handle_le_events(DWORD opcode, BYTE *rx_buf, DWORD len)
{
    switch(opcode)
    {
    case HCI_CONTROL_LE_EVENT_CONNECTED:
        le_audio_handle_connection();
        break;
    case HCI_CONTROL_LE_EVENT_DISCONNECTED:
        le_audio_handle_disconnection(rx_buf);
        break;
    }
    UNUSED(rx_buf);
    UNUSED(len);
}

// Handle WICED HCI events for AV sink
void MainWindow::le_audio_handle_audio_events(DWORD opcode, BYTE *rx_buf, DWORD len)
{
    uint8_t status;
    uint16_t conn_id;
    uint8_t mute;
    uint8_t *p_data = rx_buf;


    if (opcode == HCI_CONTROL_LE_AUDIO_EVENT_REQUEST_DATA)
    {
        HandleLEAudioRequestEvent(p_data, len);
        return;
    }else if(opcode == HCI_CONTROL_LE_AUDIO_EVENT_STARTED)
    {
        Log("[%s] audio started event", TAG);
        HandleLEAudioStartEvent(p_data, len);
        return;
    }else if(opcode == HCI_CONTROL_LE_AUDIO_EVENT_STOPPED)
    {
        Log("[%s] audio stopped event", TAG);
        HandleLEAudioStopEvent(p_data, len);
        return;
    }

    if (len <= 0) // HCI_CONTROL_LE_AUDIO_EVENT_STARTED & HCI_CONTROL_LE_AUDIO_EVENT_STOPPED events are supposed to come with len 0
        return;


    if (opcode == HCI_CONTROL_LE_AUDIO_EVENT_DEVICE_ROLE)
    {
        EnableTabs(HCI_CONTROL_GROUP_LE_AUDIO, true);
        STREAM_TO_UINT32(le_audio_dev_role, p_data);
        Log("[%s] device role %x", TAG, le_audio_dev_role);
        UpdateLEAudioRole();
        le_audio_start_app();
        return;
    }

    switch (opcode)
    {
    case HCI_CONTROL_LE_AUDIO_EVENT_MEDIA_PLAYER:
        STREAM_TO_UINT16(conn_id, p_data);
        le_audio_handle_update_media_player_list(len, p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_PLAY_STATUS:
        le_audio_handle_update_media_player_status(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_VOLUME_STATUS:
        STREAM_TO_UINT16(conn_id, p_data);
        STREAM_TO_UINT8(status, p_data);
        Log("[%s] Volume status %d", TAG, status);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_MUTE_STATUS:
        STREAM_TO_UINT16(conn_id, p_data);
        STREAM_TO_UINT8(mute, p_data);
        Log("[%s] Mute status %x", TAG, mute);
        le_audio_handle_update_mute_state(mute);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_MUTE_AND_VOLUME_STATUS:
        STREAM_TO_UINT16(conn_id, p_data);
        STREAM_TO_UINT8(status, p_data);
        STREAM_TO_UINT8(mute, p_data);
        Log("[%s] Volume status %d mute status %d", TAG, status, mute);
        le_audio_handle_update_mute_state(mute);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_CALL_STATE:
        STREAM_TO_UINT16(conn_id, p_data);
        le_audio_handle_update_call_state(conn_id, p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_CALL_TERMINATED:
        le_audio_handle_call_terminated(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_BROADCAST_STREAM_RSP:
        le_audio_handle_broadcast_stream_response_data(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_BIS_INFO:
        le_audio_handle_broadcast_stream_info(p_data, len);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_BROADCAST_STATUS_UPDATE:
        le_audio_handle_broadcast_status_update(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_CALL_FRIENDLY_NAME:
        STREAM_TO_UINT16(conn_id, p_data);
        le_audio_handle_update_call_friendly_name(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_MICS_AICS_DESC:
        le_audio_handle_mics_aics_description(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_MICS_MUTE_STATUS:
        le_audio_handle_mics_mute_status(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_MICS_AICS_INPUT_STATUS:
        le_audio_handle_mics_aics_input_status(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_PRESET_RECORD:
        le_audio_handle_preset_record(p_data, len);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_ACTIVE_PRESET:
        le_audio_handle_active_preset(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_MIC_STATUS:
        le_audio_handle_mic_status(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_CONVO_STREAM_STATE:
        le_audio_handle_convo_stream_status(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_PACS_RECORD:
        le_audio_handle_pacs_record(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_CSIS_LOCK_STATE:
        le_audio_handle_csis_lock_state(p_data);
        break;
    case HCI_CONTROL_LE_AUDIO_EVENT_DEVICE_APPEARANCE:
        le_audio_handle_device_appearance(p_data);
        break;
    }
}

// Handle WICED HCI events
void MainWindow::onHandleWicedEventLeAudio(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    if(!ui->tabLEAudio->isEnabled())
    {
        return;
    }

    switch (HCI_CONTROL_GROUP(opcode))
    {
    case HCI_CONTROL_GROUP_LE_AUDIO:
        le_audio_handle_audio_events(opcode, p_data, len);
        break;
    case HCI_CONTROL_GROUP_LE:
        le_audio_handle_le_events(opcode, p_data, len);
        break;
    }

}

// Add new device to combo box
CBtDevice *MainWindow::AddDeviceToListLeAudio(BYTE *addr, QComboBox *pCb, char *bd_name, uint16_t conn_id)
{
    CBtDevice *device = nullptr;
    char abuffer[100] = {0};
    int used =0;

    used += get_bd_string(addr, abuffer, sizeof(abuffer));
    if (bd_name)
    {
        snprintf(abuffer + used, sizeof(abuffer) - used, ", [%s]", bd_name);
    }

    // Check if device is already present
    int i = pCb->findText(abuffer, Qt::MatchStartsWith);
    if (i == -1)
    {
        QVariant qv;
        device = new CBtDevice(conn_id);
        if (bd_name && strlen(bd_name))
            strncpy(device->m_name, bd_name, sizeof(device->m_name) - 1);

        qv.setValue<CBtDevice *>(device);

        pCb->addItem(abuffer, qv);
        i = pCb->findText(abuffer, Qt::MatchStartsWith);

        Log("Added %s to index %d", device->get_bdaddr_string(), i);

        memcpy(device->m_address, addr, 6);
    }
    return device;
}

void MainWindow::on_btn_le_audio_connectToPeer_clicked()
{
    on_LeConnectPressed(ui->btn_le_audio_connectToPeer->text());
}

void MainWindow::on_btn_le_audio_pl_simulateCall_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();
    if (p_device == NULL){
        Log("[%s] no device found",TAG);
        return;
    }
    ui->btn_le_audio_pl_simulateHold->setEnabled(TRUE);

    QString text = ui->btn_le_audio_pl_simulateCall->text();
    uint16_t conn_id = p_device->get_connection_handle();
    if(text == STRING_SIMULATE_CALL)
    {
        if(!fileExists(ui->le_audio_pl_ringtone_LeAudioFile->text())){
            on_btn_le_audio_pl_ringtone_findFile_clicked();
        }
        uint8_t index = ui->le_audio_pl_list_cc_in_audioConfig->currentData().toInt();
        wiced_bt_le_audio_config_t in_config = {0, 0, 0, 0, 0};
        wiced_bt_le_audio_config_t out_config = {0, 0, 0, 0, 0};
        memcpy(&in_config, &audio_config[index], sizeof(wiced_bt_le_audio_config_t));
        if(in_config.ocpf < p_device->source_supported_config[index].min_ocpf)
            in_config.ocpf = p_device->source_supported_config[index].min_ocpf;
        if(in_config.ocpf > p_device->source_supported_config[index].max_ocpf)
            in_config.ocpf = p_device->source_supported_config[index].max_ocpf;

        index = ui->le_audio_pl_list_cc_out_audioConfig->currentData().toInt();
        memcpy(&out_config, &audio_config[index], sizeof(wiced_bt_le_audio_config_t));
        if(out_config.ocpf < p_device->sink_supported_config[index].min_ocpf)
            out_config.ocpf = p_device->sink_supported_config[index].min_ocpf;
        if(out_config.ocpf > p_device->sink_supported_config[index].max_ocpf)
            out_config.ocpf = p_device->sink_supported_config[index].max_ocpf;

        app_host_le_audio_generate_call(conn_id, ui->le_audio_pl_callURI->text().toStdString().length(),
                                        (uint8_t *)ui->le_audio_pl_callURI->text().toStdString().c_str(),
                                        ui->le_audio_pl_call_friendlyName->text().toStdString().length(),
                                        (uint8_t *)ui->le_audio_pl_call_friendlyName->text().toStdString().c_str(),
                                        p_device->source_supported_config[index].channel_cnt,
                                        p_device->sink_supported_config[index].channel_cnt,
                                        &in_config, &out_config);
        m_settings.setValue("LEAudioCallFriendlyName", ui->le_audio_pl_call_friendlyName->text());
        m_settings.setValue("LEAudioCallURI", ui->le_audio_pl_callURI->text());
        m_settings.setValue("LEAudioCodecConfig",ui->le_audio_pl_list_cc_in_audioConfig->currentText());
    }
    else if(text == STRING_TERMINATE_CALL)
    {
        uint8_t call_id = ui->le_audio_pl_Call_Id->text().toUInt();
        app_host_le_audio_terminate_call(conn_id, call_id, FALSE);
    }
}

void MainWindow::on_btn_le_audio_pl_simulateHold_clicked()
{
    QString state = ui->btn_le_audio_pl_simulateHold->text();
    uint8_t call_id = ui->le_audio_pl_Call_Id->text().toUInt();

    if (state == STRING_SIMULATE_REMOTE_HOLD)
    {
        wiced_hci_set_rmt_call_hold(call_id);
        ui->btn_le_audio_pl_simulateHold->setText(STRING_REMOTE_RETRIEVE);
    }
    else if(state == STRING_REMOTE_RETRIEVE)
    {
       wiced_hci_set_rmt_hold_retrieve(call_id);
       ui->btn_le_audio_pl_simulateHold->setText(STRING_SIMULATE_REMOTE_HOLD);
    }
}

void MainWindow::on_btn_le_audio_pl_startBcast_clicked()
{
    uint32_t broadcast_id = 0;
    uint32_t encryption = 0;
    uint32_t num_channels = 0;
    uint8_t bis_count =0;
    uint32_t codec_config = 0;
    bool status = FALSE;
    QByteArray broadcast_code = le_audio_get_broadcast_code(ui->le_audio_pl_bcastCode->text());

    if (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_SOURCE)
    {
        if (ui->btn_le_audio_pl_startBcast->text() == STRING_START_BROADCAST)
        {
            if(ui->cbCommport->currentText() != "host-mode"){
                /* invoke only if !host-mode */
                if(!fileExists(ui->le_audio_pl_broadcast_LeAudioFile->text())){
                    on_btn_le_audio_pl_broadcast_findFile_clicked();
                }
            }

            // get sampling frequencey, num of channels, encryption and start broadcast stream
            encryption = ui->le_audio_pl_bisEncrypt->isChecked();
            QString audio_type = ui->le_audio_pl_bis_channelCount->currentText();
            if (audio_type == "mono")
            {
                num_channels = 1;
                Log("mono stream");
            }
            else
            {
                num_channels = 2;
                Log("stereo stream");
            }
            bis_count = ui->le_audio_pl_bisCount->currentText().toInt();

            codec_config = ui->le_audio_pl_list_broadcast_audioConfig->itemData(ui->le_audio_pl_list_broadcast_audioConfig->currentIndex()).toInt();
            m_settings.setValue("LEAudioCodecConfig",ui->le_audio_pl_list_broadcast_audioConfig->currentText());

            broadcast_id = ui->le_audio_pl_bcastID->text().rightJustified(6, '0').toInt(&status, 16);
            if(!status)
                return;

            app_host_le_audio_broadcast_source_start_streaming(1, codec_config, bis_count,
                                                               num_channels, encryption, broadcast_id, (uint8_t *)broadcast_code.data());
            Log("Starting %sencrypted streaming, num channels %d count %d cfg %d id 0x%06x code %s", encryption ? "":"un",
                num_channels, bis_count, codec_config, broadcast_id, ui->le_audio_pl_bcastCode->text().toStdString().c_str());
            ui->btn_le_audio_pl_startBcast->setText(QStringLiteral("Stop Broadcast"));
        }
        else
        {
            app_host_le_audio_broadcast_source_start_streaming(0, codec_config, bis_count, num_channels, encryption, broadcast_id, NULL);
            Log("Stopping Streaming...");
            ui->btn_le_audio_pl_startBcast->setText(QStringLiteral(STRING_START_BROADCAST));
        }
    }
}

void MainWindow::le_audio_handle_scan_bcastStreams_click(QPushButton *p_btn, QComboBox *p_streams)
{
    QString text = p_btn->text();

    if (text == (STRING_DISCOVER_SOURCES))
    {
        p_streams->clear();
        Log("Looking for broadcast source streams...");

        if (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_SINK)
        {
            app_host_le_audio_broadcast_sink_find_sources(START);
        }
        else if (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_ASSISTANT)
        {
            app_host_le_audio_broadcast_assistant_scan_source(START);
        }
        else
            return;

        p_btn->setText(STRING_STOP_DISCOVER);
    }
    else
    {
        Log("Stop Looking for broadcast source streams...");

        if (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_SINK)
        {
            app_host_le_audio_broadcast_sink_find_sources(STOP);
        }

        else if (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_ASSISTANT)
        {
            app_host_le_audio_broadcast_assistant_scan_source(STOP);
        }
        p_btn->setText(STRING_DISCOVER_SOURCES);
    }
}

void MainWindow::on_btn_le_audio_hs_scan_bcastStreams_clicked()
{
    le_audio_handle_scan_bcastStreams_click(ui->btn_le_audio_hs_scan_bcastStreams, ui->le_audio_hs_found_bcastSource);
}

void MainWindow::on_btn_le_audio_hs_bcast_get_Streams_clicked()
{
    uint32_t broadcast_id = ui->le_audio_hs_found_bcastSource->currentData().toUInt();
    QString text = ui->btn_le_audio_hs_bcast_get_Streams->text();
    if(text == STRING_GET_STREAMS)
    {
         app_host_le_audio_broadcast_sink_get_streams(broadcast_id);
         ui->btn_le_audio_hs_bcast_get_Streams->setText(STRING_CANCEL_PA_SYNC);
    }
    else if(text == STRING_CANCEL_PA_SYNC)
    {
        app_host_le_audio_broadcast_sink_cancel_pa_sync();
        ui->btn_le_audio_hs_bcast_get_Streams->setText(STRING_GET_STREAMS);
    }
    else if(text == STRING_TERMINATE_PA_SYNC)
    {
        app_host_le_audio_broadcast_sink_terminate_pa_sync(broadcast_id);
        ui->btn_le_audio_hs_bcast_get_Streams->setText(STRING_GET_STREAMS);
    }
}

QByteArray MainWindow::le_audio_get_broadcast_code(QString code)
{
    QByteArray broadcast_code;

    Log("get broadcast code for %s", code.toStdString().c_str());

    if(code[0] == '0' && code[1].toUpper() == 'X'){
        code = code.mid(2, code.length() - 2);
        Log("get broadcast reduced code for %s", code.toStdString().c_str());
        broadcast_code = QByteArray::fromHex(code.rightJustified(32, '0').toLatin1());
    }else{
        broadcast_code = code.toUtf8().leftJustified(16, 0);
        std::reverse(broadcast_code.begin(), broadcast_code.end());
    }
    uint8_t * p = (uint8_t *)broadcast_code.data();
    Log("Broadcast code %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x",
        p[0], p[1], p[2], p[3],
            p[4], p[5], p[6], p[7],
            p[8], p[9], p[10], p[11],
            p[12], p[13], p[14], p[15]);

    return broadcast_code;
}

void MainWindow::on_btn_le_audio_hs_bcast_SyncToStream_clicked()
{
    uint32_t broadcast_id = ui->le_audio_hs_found_bcastSource->currentData().toUInt();
    QString src_text = ui->btn_le_audio_hs_bcast_SyncToStream->text();
    uint32_t bis_index_bits = ui->le_audio_hs_bcast_BISIndexes->text().rightJustified(8, '0').toUInt(NULL, 16);
    QByteArray broadcast_code = le_audio_get_broadcast_code(ui->le_audio_hs_bcastCode->text());

    if (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_SINK)
    {
        if (src_text == STRING_SYNC_TO_STREAM)
        {
            app_host_le_audio_broadcast_sink_sync_to_stream(SYNC, (uint8_t *)broadcast_code.data(), broadcast_id, bis_index_bits);
            ui->btn_le_audio_hs_bcast_SyncToStream->setText(STRING_TERMINATE_STREAM);
        }
        else
        {
            app_host_le_audio_broadcast_sink_sync_to_stream(TERMINATE, (uint8_t *)broadcast_code.data(), broadcast_id, bis_index_bits);
            ui->btn_le_audio_hs_bcast_SyncToStream->setText(STRING_SYNC_TO_STREAM);
            ui->btn_le_audio_hs_scan_bcastStreams->setText(STRING_DISCOVER_SOURCES);
        }
    }
}

void MainWindow:: on_btn_le_audio_hs_ccp_accept_retrieve_call_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;
    if(p_device)
    {
        conn_id = p_device->get_connection_handle();
    }

    if(conn_id == 0)
    {
        return;
    }

    QString text = ui->btn_le_audio_hs_ccp_accept_retrieve_call->text();
    uint8_t call_id = (uint8_t)ui->le_audio_hs_ccp_call_id->text().toUInt();
    if (text == STRING_ACCEPT_CALL)
    {
        app_host_le_audio_handle_call_action(conn_id, call_id, WICED_BT_GA_CCP_ACTION_ACCEPT_CALL);
    }
    else if(text == STRING_JOIN_CALL)
    {
        app_host_le_audio_handle_call_action(conn_id, call_id, WICED_BT_GA_CCP_ACTION_JOIN_CALL);
    }
    else if(text == STRING_PUT_ON_HOLD)
    {
        app_host_le_audio_handle_call_action(conn_id, call_id, WICED_BT_GA_CCP_ACTION_HOLD_CALL);
    }
    else if(text == STRING_RETRIEVE_CALL)
    {
        app_host_le_audio_handle_call_action(conn_id, call_id, WICED_BT_GA_CCP_ACTION_RETRIEVE_CALL);
    }

}
void MainWindow:: on_btn_le_audio_hs_ccp_reject_terminate_call_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;
    if(p_device)
    {
        conn_id = p_device->get_connection_handle();
    }

    if(conn_id == 0)
    {
        return;
    }

    QString text = ui->btn_le_audio_hs_ccp_reject_terminate_call->text();
    uint8_t call_id = (uint8_t)ui->le_audio_hs_ccp_call_id->text().toUInt();
    if (text == STRING_REJECT_CALL)
    {
        app_host_le_audio_terminate_call(conn_id, call_id, TRUE);
    }
    else if(text == STRING_TERMINATE_CALL)
    {
        app_host_le_audio_terminate_call(conn_id, call_id, FALSE);
    }
}

void MainWindow::le_audio_ba_handle_add_bcast_source(int add_stream, int use_past)
{
    CBtDevice *p_device = GetSelectedLEDevice();

    if (p_device == NULL)
        return;

    uint16_t conn_id = le_audio_update_conn_id(p_device);
    uint32_t broadcast_id = ui->le_audio_ba_found_bcastStreams->currentData().toUInt();
    QByteArray broadcast_code = le_audio_get_broadcast_code(ui->le_audio_ba_bcastCode->text());

    if(add_stream)
    {
        app_host_le_audio_broadcast_assistant_select_source(ADD, conn_id, (uint8_t *)broadcast_code.data(), broadcast_id, use_past);
    }
    else
    {
        app_host_le_audio_broadcast_assistant_select_source(REMOVE, conn_id, (uint8_t *)broadcast_code.data(), broadcast_id, 0);
    }

}

void MainWindow::on_btn_le_audio_ba_addSource_clicked()
{
    le_audio_ba_handle_add_bcast_source(1, 0);
}

void MainWindow::on_btn_le_audio_ba_addSourceWithPast_clicked()
{
    le_audio_ba_handle_add_bcast_source(1, 1);
}

void MainWindow::on_btn_le_audio_ba_removeSource_clicked()
{
    le_audio_ba_handle_add_bcast_source(0, 0);
}

void MainWindow::on_btn_le_audio_ba_scan_bcastStreams_clicked()
{
    le_audio_handle_scan_bcastStreams_click(ui->btn_le_audio_ba_scan_bcastStreams, ui->le_audio_ba_found_bcastStreams);
}

void MainWindow::on_btn_le_audio_mics_mute_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;
    if(p_device)
    {
        conn_id = p_device->get_connection_handle();
    }
    if(ui->btn_le_audio_mics_mute->text() == STRING_MUTE_MICP)
    {
        app_host_le_audio_mics_mute(conn_id, 1);
        ui->btn_le_audio_mics_mute->setText(STRING_UNMUTE_MICP);
    }
    else
    {
        app_host_le_audio_mics_mute(conn_id, 0);
        ui->btn_le_audio_mics_mute->setText(STRING_MUTE_MICP);
    }
}

void MainWindow::on_btn_le_audio_mics_aics_mute_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();

    uint16_t conn_id = 0;
    if(p_device)
    {
        conn_id = p_device->get_connection_handle();
    }
    uint32_t instance = ui->le_audio_mics_aics_desc->currentData().toUInt();
    if(ui->btn_le_audio_mics_aics_mute->text() == STRING_MUTE)
    {
        app_host_le_audio_mics_aics_mute(conn_id, instance, 1);
        ui->btn_le_audio_mics_aics_mute->setText(STRING_UNMUTE);
    }
    else
    {
        app_host_le_audio_mics_aics_mute(conn_id, instance, 0);
        ui->btn_le_audio_mics_aics_mute->setText(STRING_MUTE);
    }

}

void MainWindow::on_btn_le_audio_inc_mics_aics_gain_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;
    if(p_device)
    {
        conn_id = p_device->get_connection_handle();
    }
    uint32_t instance = ui->le_audio_mics_aics_desc->currentData().toUInt();
    app_host_le_audio_increase_mics_aics_gain(conn_id, instance);
}

void MainWindow::on_btn_le_audio_dec_mics_aics_gain_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;
    if(p_device)
    {
        conn_id = p_device->get_connection_handle();
    }
    uint32_t instance = ui->le_audio_mics_aics_desc->currentData().toUInt();
    app_host_le_audio_decrease_mics_aics_gain(conn_id, instance);
}

void MainWindow::on_btn_le_audio_set_mics_aics_gain_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;
    if(p_device)
    {
        conn_id = p_device->get_connection_handle();
    }
    uint32_t instance = ui->le_audio_mics_aics_desc->currentData().toUInt();
    app_host_le_audio_set_mics_aics_gain(conn_id, instance, ui->le_audio_mics_aics_gain->text().toInt());
}

void MainWindow::on_btn_le_audio_pl_read_preset_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;
    if(p_device)
    {
        ui->le_audio_hap_preset_name->clear();
        conn_id = p_device->get_connection_handle();
        app_host_le_audio_hap_read_preset(conn_id);
    }
}

void MainWindow::on_btn_le_audio_pl_hap_set_next_preset_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;
    if(p_device)
    {
        conn_id = p_device->get_connection_handle();
        app_host_le_audio_hap_set_next_preset(conn_id);
    }
}

void MainWindow::on_btn_le_audio_pl_hap_set_previous_preset_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;
    if(p_device)
    {
        conn_id = p_device->get_connection_handle();
        app_host_le_audio_hap_set_previous_preset(conn_id);
    }
}

void MainWindow::on_btn_le_audio_pl_hap_set_active_preset_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;
    if(p_device)
    {
        conn_id = p_device->get_connection_handle();
        app_host_le_audio_hap_set_active_preset(conn_id, ui->le_audio_hap_preset_name->currentData().toUInt());

    }
}

void MainWindow::on_btn_le_audio_pl_hap_set_preset_name_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();
    uint16_t conn_id = 0;
    if(p_device)
    {
        conn_id = p_device->get_connection_handle();
        app_host_le_audio_hap_set_preset_name(conn_id, ui->le_audio_hap_preset_name->currentData().toUInt(), ui->le_audio_hap_preset_name->currentText().toStdString().c_str());
    }
}

 void  MainWindow::on_btn_le_audio_csis_lock_clicked()
 {
     CBtDevice *p_device = GetSelectedLEDevice();
     uint16_t conn_id = 0;
     if(p_device)
     {
         conn_id = p_device->get_connection_handle();
         uint8_t lock_val = (ui->btn_le_audio_csis_lock->text() == STRING_SET_LOCK) ? 1:0;
         app_host_le_audio_set_lock_state(conn_id, lock_val);
     }
 }

bool app_host_le_audio_set_lock_state(uint16_t conn_id, uint8_t lock_val);

void MainWindow::le_audio_add_broadcast_stream_to_list(QComboBox * cb, uint32_t broadcast_id, QString br_name)
{
    QString br_id_name = QString("0x") + QString::number(broadcast_id, 16) + ", " + br_name;

    if(cb->findData(broadcast_id) == -1)
    {
        cb->addItem(br_id_name, broadcast_id);
        Log("Adding bcast device : %s", br_id_name.toStdString().c_str());
    }
}

typedef enum
{
    BAP_CODEC_CONFIG_INVALID_TYPE,
    BAP_CODEC_CONFIG_SAMPLING_FREQUENCY_TYPE,
    BAP_CODEC_CONFIG_FRAME_DURATION_TYPE,
    BAP_CODEC_CONFIG_AUDIO_CHANNEL_ALLOCATION_TYPE,
    BAP_CODEC_CONFIG_OCTETS_PER_CODEC_FRAME_TYPE,
    BAP_CODEC_CONFIG_LC3_BLOCKS_PER_SDU_TYPE
} bap_codec_configuration_type_t;

static uint8_t * get_values_for_codec_config_type (uint8_t *stream, uint8_t stream_len, uint8_t *len, uint8_t config_type)
{
    uint8_t length = 0;
    uint8_t type = 0;

    while(stream && (stream_len > 2))
    {
        STREAM_TO_UINT8(length, stream);
        STREAM_TO_UINT8(type, stream);
        if(type == config_type)
        {
            *len = length -1;
            return stream;
        }
        stream += (length-1);
        stream_len -= (length + 1);
    }
    return NULL;
}

void MainWindow::le_audio_handle_broadcast_stream_info(uint8_t *p_data, uint8_t stream_len)
{
    if(stream_len < 14) return;
    uint8_t bis_count;

    // Service uuid             : 2 Bytes
    // presentation delay       : 3 Bytes
    // Sub Group Count          : 1 Byte
    p_data  += 6;
    stream_len -= 6; //(2 + 3 + 1) Bytes

    // BIS Count                : 1 Byte
    STREAM_TO_UINT8(bis_count, p_data);
    // Coding Format            : 1 Byte
    // Company Id               : 2 Bytes
    // Vendor Specific Codec Id : 2 Bytes
    p_data += 5;
    stream_len -= 6; // (1 + 1 + 2 + 2) Bytes

    // CSC len                  : 1 Byte
    // CSC                      : Variable Size (value in csc_len)
    uint8_t csc_len;
    STREAM_TO_UINT8(csc_len, p_data);
    p_data += csc_len;
    stream_len -= (csc_len + 1); // (1 + csc_len) Bytes
    if(stream_len < 1) return;

    // Metadata len             : 1 Byte
    // Metadata                 : Variable Size (value in metadata_len)
    uint8_t metadata_len;
    STREAM_TO_UINT8(metadata_len, p_data);
    p_data += metadata_len;
    stream_len -= (metadata_len + 1);

    for(int i =0; i< bis_count; i++)
    {
        if(stream_len < 2) return;
        uint8_t bis_index;
        uint32_t aca = 0;
        uint8_t val_len;
        uint8_t *p_val = NULL;

        // BIS Index             : 1 Byte
        // CSC len               : 1 Byte
        STREAM_TO_UINT8(bis_index, p_data);
        STREAM_TO_UINT8(csc_len, p_data);
        p_val = get_values_for_codec_config_type(p_data, csc_len, &val_len, BAP_CODEC_CONFIG_AUDIO_CHANNEL_ALLOCATION_TYPE);
        if(p_val && (val_len == 4))
        {
           memcpy(&aca, p_val, val_len);
           Log("[%s] BIS Index %d aca: %d ", TAG, bis_index, aca);
        }
        else
        {
            Log("[%s] BIS Index %d: No location \n", TAG, bis_index);
        }
        p_data += csc_len;
        stream_len -= (csc_len + 2); //(csc_len + 1 + 1)
    }
}

void MainWindow::le_audio_handle_broadcast_stream_response_data(uint8_t *p_data)
{
    uint32_t broadcast_id;
    uint8_t len;
    char br_name[20];

    STREAM_TO_UINT32(broadcast_id, p_data);


    memset(br_name, 0, 20);
    STREAM_TO_UINT8(len, p_data);
    if(len >= 20)len = (20-1);
    memcpy(br_name, p_data, len);


    if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_SINK){
        le_audio_add_broadcast_stream_to_list(ui->le_audio_hs_found_bcastSource, broadcast_id, br_name);
    }else if(le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_BROADCAST_ASSISTANT){
        le_audio_add_broadcast_stream_to_list(ui->le_audio_ba_found_bcastStreams, broadcast_id, br_name);
    }
}

static const name_val_t broadcast_status_val[]  = {
    { "PA Sync established", HCI_CONTROL_LEA_BROADCAST_PA_SYNC_ESTABLISHED },
    { "PA Sync lost", HCI_CONTROL_LEA_BROADCAST_PA_SYNC_LOST },
    { "BIG Sync established", HCI_CONTROL_LEA_BROADCAST_BIG_SYNC_ESTABLISHED },
    { "BIG Sync lost", HCI_CONTROL_LEA_BROADCAST_BIG_SYNC_LOST },
    { "BIG Sync terminated", HCI_CONTROL_LEA_BROADCAST_BIG_SYNC_TERMINATED}
};

const char * MainWindow::get_name_for_val(const name_val_t * p_vals, int num_vals, int value, const char * def)
{
    for(int i = 0; i < num_vals; i++, p_vals++){
        if(QString(p_vals->data) == value){
            return p_vals->name;
        }
    }

    return def;
}

void MainWindow::le_audio_handle_broadcast_status_update(uint8_t *p_data)
{
    uint32_t broadcast_status;

    STREAM_TO_UINT8(broadcast_status, p_data);
    ui->btn_le_audio_hs_bcast_get_Streams->setText(STRING_GET_STREAMS);

    switch(broadcast_status)
    {
    case HCI_CONTROL_LEA_BROADCAST_BIG_SYNC_ESTABLISHED:
    {
        //BIG sync established
        ui->btn_le_audio_hs_bcast_SyncToStream->setText(STRING_TERMINATE_STREAM);
    }break;
    case HCI_CONTROL_LEA_BROADCAST_BIG_SYNC_LOST:
    case HCI_CONTROL_LEA_BROADCAST_BIG_SYNC_TERMINATED:
    {
        //BIG sync established
        ui->btn_le_audio_hs_bcast_SyncToStream->setText(STRING_SYNC_TO_STREAM);
    }break;
    case HCI_CONTROL_LEA_BROADCAST_PA_SYNC_ESTABLISHED:
        ui->btn_le_audio_hs_bcast_get_Streams->setText(STRING_TERMINATE_PA_SYNC);
        break;
    }

    if((broadcast_status >= HCI_CONTROL_LEA_BROADCAST_PA_SYNC_ESTABLISHED) &&
       (broadcast_status <= HCI_CONTROL_LEA_BROADCAST_BIG_SYNC_LOST))
    {
        Log("status : %s",
            get_name_for_val(broadcast_status_val, sizeof(broadcast_status_val)/sizeof(broadcast_status_val[0]), broadcast_status));
    }
}

void MainWindow::le_audio_reset_ui()
{
    ui->btn_le_audio_connectToPeer->setText(STRING_CONNECT);

    /* headset */
    InitLeAudioHeadsetMediaControl(FALSE);
    InitLeAudioVolumeControl(FALSE);
    InitLeAudioMicrophoneControl(FALSE);
    InitLeAudioHeadsetBroadcastSink(FALSE);

    /* player */
    InitLeAudioPlayerMediaControl(FALSE);
    InitLeAudioPlayerCallControlSetup(FALSE);
    InitLeAudioPlayerCallControls(FALSE);
    InitLeAudioPlayerCallControlAudioConfig(FALSE);
    InitLeAudioPlayerBroadcastSource(FALSE);

    /* assistant */
    InitLeAudioBroadcastAssistant(FALSE);

    ui->btn_le_audio_startLeAdv->setText(STRING_START_ADVERTISEMENT);


    //reset_controls();
    ui->le_audio_deviceRoleTxt->setText("Device Role");
}

void MainWindow::le_audio_start_app(void)
{
    if(0 != (le_audio_dev_role & HCI_CONTROL_LE_AUDIO_DEV_ROLE_UNICAST_SINK))
    {
        on_btn_le_audio_startLeAdv_clicked();
    }
}
