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
 * Sample MCU application for BLE or BR/EDR HID Device using WICED HCI protocol.
 */

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioOutput>
#include "app_include.h"
#include "usb_kb_usage.h"
#include "hci_control_api.h"
#include "rpc.pb.h"

extern "C"
{
#include "app_host_hidd.h"
}

#define AUDIO_CODEC (m_device_capability_audio & (HCI_CONTROL_HIDD_AUDIO_ADPCM | HCI_CONTROL_HIDD_AUDIO_OPUS | HCI_CONTROL_HIDD_AUDIO_MSBC))

#ifndef HCI_CONTROL_HIDD_COMMAND_GET_ULL_FEATURES
 #define HCI_CONTROL_HIDD_COMMAND_GET_ULL_FEATURES           ( ( HCI_CONTROL_GROUP_HIDD << 8 ) | 0x0E )     /* Read ULL feature charactericstics from NVRAM */
 #define HCI_CONTROL_HIDD_COMMAND_SAVE_ULL_FEATURES          ( ( HCI_CONTROL_GROUP_HIDD << 8 ) | 0x0F )     /* Write ULL feature charactericstics to NVRAM */
 #define HCI_CONTROL_HIDD_EVENT_ULL_FEATURE                  ( ( HCI_CONTROL_GROUP_HIDD << 8 ) | 0x0B )    /* ULL feature */
#endif

//extern const CHAR *szAdvState[];

// Initialize app
void MainWindow::InitBLEHIDD()
{
    m_pairing_mode = 0;
    m_host_type = 0;
    m_host_valid = false;
    m_b_is_hidd = false;
    m_hiddAudioRaw_fp = NULL;
    m_hiddAudioRaw_write_fp = NULL;
    m_device_capability_audio = m_device_capability_motion = m_device_capability_ir = m_device_capability_ull = 0;
    ui->cbBLEHIDInterupt->clear();
    ui->cbBLEHIDReport->clear();

    ui->cbBLEHIDInterupt->addItem("Control Channel", HCI_CONTROL_HID_REPORT_CHANNEL_CONTROL);
    ui->cbBLEHIDInterupt->addItem("Interrupt Channel", HCI_CONTROL_HID_REPORT_CHANNEL_INTERRUPT);

    ui->cbBLEHIDReport->addItem("Other", HCI_CONTROL_HID_REPORT_TYPE_OTHER);
    ui->cbBLEHIDReport->addItem("Input", HCI_CONTROL_HID_REPORT_TYPE_INPUT);
    ui->cbBLEHIDReport->addItem("Output", HCI_CONTROL_HID_REPORT_TYPE_OUTPUT);
    ui->cbBLEHIDReport->addItem("Feature", HCI_CONTROL_HID_REPORT_TYPE_FEATURE);

    ui->cbBLEHIDInterupt->setCurrentIndex(1);
    ui->cbBLEHIDReport->setCurrentIndex(1);
    btnBLEHIDSendKeyInit();
    UpdateHIDD_ui_host();
    UpdateHIDD_ui_pairing();
}

void MainWindow::btnBLEHIDSendKeyInit()
{
    memset(keyRpt_buf, 0, KEYRPT_BUF_SIZE);
    keyRpt_buf[0] = HCI_CONTROL_HID_REPORT_CHANNEL_INTERRUPT;
    keyRpt_buf[1] = HCI_CONTROL_HID_REPORT_TYPE_INPUT;
    keyRpt_buf[2] = HCI_CONTROL_HID_REPORT_ID;
    ui->cbBLEHIDCapLock->setChecked(false);
    ui->cbBLEHIDCtrl->setChecked(false);
    ui->cbBLEHIDAlt->setChecked(false);
}

void MainWindow::btnBLEHIDSendKey()
{
    Log("Send key report: RptId:%02x Modifier:%02x %02x Keys:%02x %02x %02x %02x %02x %02x",
       keyRpt_buf[2], keyRpt_buf[3], keyRpt_buf[4], keyRpt_buf[5], keyRpt_buf[6], keyRpt_buf[7], keyRpt_buf[8], keyRpt_buf[9], keyRpt_buf[10]);
    wiced_hci_send_command(HCI_CONTROL_HIDD_COMMAND_SEND_REPORT, keyRpt_buf, KEYRPT_SIZE);
}

void MainWindow::btnBLEHIDSendKeyDown(BYTE c, QPushButton * button)
{
    bool release = ui->cbBLEHIDHold->isChecked() && btnBLEHIDSendKeyRelease(c, button);
    bool send = release;

    if (!release)
    {

        for (int i=KEYRPT_CODE;i<KEYRPT_SIZE;i++)
        {
            // if empty, then use it
            if (!keyRpt_buf[i])
            {
                keyRpt_buf[i] = c;
                setHIDD_buttonColor(button, Qt::blue);
                send = true;
                break;
            }
        }
    }

    if (send)
        btnBLEHIDSendKey();
}

const char * MainWindow::hidd_media_key_str(BYTE c)
{
#define MAX_HIDD_MEDIA_KEY 8
    static const char * hidd_media_str[MAX_HIDD_MEDIA_KEY+1] = {
        "IR",
        "AUDIO",
        "MOTION",
        "CONNECT",
        "HOME",
        "BACK",
        "MUTE",
        "POWER",
        "Unknown"
    };

    c -= HCI_CONTROL_HIDD_KEY_IR;
    if (c>MAX_HIDD_MEDIA_KEY)
    {
        c = MAX_HIDD_MEDIA_KEY;
    }
    return hidd_media_str[c];
}

void MainWindow::btnBLEHIDSendMedia(BYTE c, bool pressed, QPushButton * button)
{
    setHIDD_buttonColor(button, pressed ? Qt::blue : Qt::white);
    app_host_hidd_key(c, pressed);
    Log("Send media %s key %s to device", hidd_media_key_str(c), pressed?"pressed":"released");
}

bool MainWindow::btnBLEHIDSendKeyRelease(BYTE c, QPushButton * button)
{
    for (int i=KEYRPT_CODE;i<KEYRPT_SIZE;i++)
    {
        // if found the key, then shift it
        if (keyRpt_buf[i]==c)
        {
            setHIDD_buttonColor(button, Qt::white);
            do {
                keyRpt_buf[i] = keyRpt_buf[i+1];
            } while (keyRpt_buf[i] && ++i < KEYRPT_SIZE);
            return true;
        }
    }
    return false;
}

void MainWindow::btnBLEHIDSendKeyUp(BYTE c, QPushButton * button)
{
    if (!ui->cbBLEHIDHold->isChecked())
    {
        btnBLEHIDSendKeyRelease(c, button);
        btnBLEHIDSendKey();
    }
}

void MainWindow::on_cbBLEHIDHold_clicked()
{
    if (!ui->cbBLEHIDHold->isChecked())
    {
        bool sendRpt;

        sendRpt = btnBLEHIDSendKeyRelease(USB_USAGE_1, ui->btnBLEHIDSendKey_1);
        sendRpt |= btnBLEHIDSendKeyRelease(USB_USAGE_2, ui->btnBLEHIDSendKey_2);
        sendRpt |= btnBLEHIDSendKeyRelease(USB_USAGE_3, ui->btnBLEHIDSendKey_3);
        sendRpt |= btnBLEHIDSendKeyRelease(USB_USAGE_A, ui->btnBLEHIDSendKey_a);
        sendRpt |= btnBLEHIDSendKeyRelease(USB_USAGE_B, ui->btnBLEHIDSendKey_b);
        sendRpt |= btnBLEHIDSendKeyRelease(USB_USAGE_C, ui->btnBLEHIDSendKey_c);
        sendRpt |= btnBLEHIDSendKeyRelease(USB_USAGE_ENTER, ui->btnBLEHIDSendKey_enter);
        sendRpt |= btnBLEHIDSendKeyRelease(USB_USAGE_ESCAPE, ui->btnBLEHIDSendKey_esc);
        sendRpt |= btnBLEHIDSendKeyRelease(USB_USAGE_DOWN_ARROW, ui->btnBLEHIDSendKey_down);
        sendRpt |= btnBLEHIDSendKeyRelease(USB_USAGE_UP_ARROW, ui->btnBLEHIDSendKey_up);
        sendRpt |= btnBLEHIDSendKeyRelease(USB_USAGE_LEFT_ARROW, ui->btnBLEHIDSendKey_left);
        sendRpt |= btnBLEHIDSendKeyRelease(USB_USAGE_RIGHT_ARROW, ui->btnBLEHIDSendKey_right);
        if (sendRpt)
            btnBLEHIDSendKey();
    }
}

void MainWindow::on_btnBLEHIDSendKey_1_pressed()
{
    btnBLEHIDSendKeyDown(USB_USAGE_1, ui->btnBLEHIDSendKey_1);
}

void MainWindow::on_btnBLEHIDSendKey_1_released()
{
    btnBLEHIDSendKeyUp(USB_USAGE_1, ui->btnBLEHIDSendKey_1);
}

void MainWindow::on_btnBLEHIDSendKey_2_pressed()
{
    btnBLEHIDSendKeyDown(USB_USAGE_2, ui->btnBLEHIDSendKey_2);
}

void MainWindow::on_btnBLEHIDSendKey_2_released()
{
    btnBLEHIDSendKeyUp(USB_USAGE_2, ui->btnBLEHIDSendKey_2);
}

void MainWindow::on_btnBLEHIDSendKey_3_pressed()
{
    btnBLEHIDSendKeyDown(USB_USAGE_3, ui->btnBLEHIDSendKey_3);
}

void MainWindow::on_btnBLEHIDSendKey_3_released()
{
    btnBLEHIDSendKeyUp(USB_USAGE_3, ui->btnBLEHIDSendKey_3);
}

void MainWindow::on_btnBLEHIDSendKey_audio_pressed()
{
    btnBLEHIDSendMedia(HCI_CONTROL_HIDD_KEY_AUDIO, true, ui->btnBLEHIDSendKey_audio);
}

void MainWindow::on_btnBLEHIDSendKey_audio_released()
{
    btnBLEHIDSendMedia(HCI_CONTROL_HIDD_KEY_AUDIO, false, ui->btnBLEHIDSendKey_audio);
}

void MainWindow::on_btnBLEHIDSendKey_ir_pressed()
{
    btnBLEHIDSendMedia(HCI_CONTROL_HIDD_KEY_IR, true, ui->btnBLEHIDSendKey_ir);
}

void MainWindow::on_btnBLEHIDSendKey_ir_released()
{
    btnBLEHIDSendMedia(HCI_CONTROL_HIDD_KEY_IR, false, ui->btnBLEHIDSendKey_ir);
}

void MainWindow::on_btnBLEHIDSendKey_motion_pressed()
{
    btnBLEHIDSendMedia(HCI_CONTROL_HIDD_KEY_MOTION, true, ui->btnBLEHIDSendKey_motion);
}

void MainWindow::on_btnBLEHIDSendKey_motion_released()
{
    btnBLEHIDSendMedia(HCI_CONTROL_HIDD_KEY_MOTION, false, ui->btnBLEHIDSendKey_motion);
}

void MainWindow::on_btnBLEHIDSendKey_a_pressed()
{
    btnBLEHIDSendKeyDown(USB_USAGE_A, ui->btnBLEHIDSendKey_a);
}

void MainWindow::on_btnBLEHIDSendKey_a_released()
{
    btnBLEHIDSendKeyUp(USB_USAGE_A, ui->btnBLEHIDSendKey_a);
}

void MainWindow::on_btnBLEHIDSendKey_b_pressed()
{
    btnBLEHIDSendKeyDown(USB_USAGE_B, ui->btnBLEHIDSendKey_b);
}

void MainWindow::on_btnBLEHIDSendKey_b_released()
{
    btnBLEHIDSendKeyUp(USB_USAGE_B, ui->btnBLEHIDSendKey_b);
}

void MainWindow::on_btnBLEHIDSendKey_c_pressed()
{
    btnBLEHIDSendKeyDown(USB_USAGE_C, ui->btnBLEHIDSendKey_c);
}

void MainWindow::on_btnBLEHIDSendKey_c_released()
{
    btnBLEHIDSendKeyUp(USB_USAGE_C, ui->btnBLEHIDSendKey_c);
}

void MainWindow::on_btnBLEHIDSendKey_esc_pressed()
{
    btnBLEHIDSendKeyDown(USB_USAGE_ESCAPE, ui->btnBLEHIDSendKey_esc);
}

void MainWindow::on_btnBLEHIDSendKey_esc_released()
{
    btnBLEHIDSendKeyUp(USB_USAGE_ESCAPE, ui->btnBLEHIDSendKey_esc);
}

void MainWindow::on_btnBLEHIDSendKey_up_pressed()
{
    btnBLEHIDSendKeyDown(USB_USAGE_UP_ARROW, ui->btnBLEHIDSendKey_up);
}

void MainWindow::on_btnBLEHIDSendKey_up_released()
{
    btnBLEHIDSendKeyUp(USB_USAGE_UP_ARROW, ui->btnBLEHIDSendKey_up);
}

void MainWindow::on_btnBLEHIDSendKey_enter_pressed()
{
    btnBLEHIDSendKeyDown(USB_USAGE_ENTER, ui->btnBLEHIDSendKey_enter);
}

void MainWindow::on_btnBLEHIDSendKey_enter_released()
{
    btnBLEHIDSendKeyUp(USB_USAGE_ENTER, ui->btnBLEHIDSendKey_enter);
}

void MainWindow::on_btnBLEHIDSendKey_left_pressed()
{
    btnBLEHIDSendKeyDown(USB_USAGE_LEFT_ARROW, ui->btnBLEHIDSendKey_left);
}

void MainWindow::on_btnBLEHIDSendKey_left_released()
{
    btnBLEHIDSendKeyUp(USB_USAGE_LEFT_ARROW, ui->btnBLEHIDSendKey_left);
}

void MainWindow::on_btnBLEHIDSendKey_down_pressed()
{
    btnBLEHIDSendKeyDown(USB_USAGE_DOWN_ARROW, ui->btnBLEHIDSendKey_down);
}

void MainWindow::on_btnBLEHIDSendKey_down_released()
{
    btnBLEHIDSendKeyUp(USB_USAGE_DOWN_ARROW, ui->btnBLEHIDSendKey_down);
}

void MainWindow::on_btnBLEHIDSendKey_right_pressed()
{
    btnBLEHIDSendKeyDown(USB_USAGE_RIGHT_ARROW, ui->btnBLEHIDSendKey_right);
}

void MainWindow::on_btnBLEHIDSendKey_right_released()
{
    btnBLEHIDSendKeyUp(USB_USAGE_RIGHT_ARROW, ui->btnBLEHIDSendKey_right);
}

void MainWindow::on_btnBLEHIDSendKey_back_pressed()
{
    btnBLEHIDSendMedia(HCI_CONTROL_HIDD_KEY_BACK, true, ui->btnBLEHIDSendKey_back);
}

void MainWindow::on_btnBLEHIDSendKey_back_released()
{
    btnBLEHIDSendMedia(HCI_CONTROL_HIDD_KEY_BACK, false, ui->btnBLEHIDSendKey_back);
}

void MainWindow::on_btnBLEHIDSendKey_home_pressed()
{
    btnBLEHIDSendMedia(HCI_CONTROL_HIDD_KEY_HOME, true, ui->btnBLEHIDSendKey_home);
}

void MainWindow::on_btnBLEHIDSendKey_home_released()
{
    btnBLEHIDSendMedia(HCI_CONTROL_HIDD_KEY_HOME, false, ui->btnBLEHIDSendKey_home);
}

void MainWindow::on_btnBLEHIDSendKey_mute_pressed()
{
    btnBLEHIDSendMedia(HCI_CONTROL_HIDD_KEY_MUTE, true, ui->btnBLEHIDSendKey_mute);
}

void MainWindow::on_btnBLEHIDSendKey_mute_released()
{
    btnBLEHIDSendMedia(HCI_CONTROL_HIDD_KEY_MUTE, false, ui->btnBLEHIDSendKey_mute);
}

void MainWindow::setHIDD_buttonColor(QPushButton * button, const QColor &color)
{
    QPalette pal = button->palette();
    pal.setColor(QPalette::Button, color);
    button->setPalette(pal);
    button->setAutoFillBackground(true);
    button->update();
}

void MainWindow::setHIDD_HostAddr(unsigned char * ad)
{
    m_host_valid = (ad!=nullptr);
    if (m_host_valid)
    {
        for (int i=0;i<6;i++)
            m_host_ad[i]= *ad++;
    }
    UpdateHIDD_ui_host();
}

void MainWindow::setHIDD_linkChange(unsigned char * ad, bool linkUp)
{
    m_connected = linkUp;
    if (linkUp)
    {
        if (ad!=nullptr)
        {
            m_host_valid = true;

            for (int i=0;i<6;i++)
                m_host_ad[i]= *ad++;
        }
        ui->btnBLEHIDConnectDisconnect->setText("Disconnect");
    }
    else
    {
        ui->btnBLEHIDConnectDisconnect->setText("Connect");
    }
    UpdateHIDD_ui_host();
}

// Connect to peer device
void MainWindow::UpdateHIDD_ui_host()
{
    if (m_b_is_hidd)
    {
        ui->btnBLEHIDDVirtualUnplug->setEnabled(m_host_valid);
        ui->btnBLEHIDConnectDisconnect->setEnabled(m_host_valid);
//        ui->btnBLEHIDSendKey_1->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_2->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_3->setEnabled(m_connected);
        ui->btnBLEHIDSendKey_audio->setEnabled(AUDIO_CODEC);
        ui->btnBLEHIDSendKey_ir->setEnabled(m_device_capability_ir);
        ui->btnBLEHIDSendKey_motion->setEnabled(m_device_capability_motion);
//        ui->btnBLEHIDSendKey_a->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_b->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_c->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_enter->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_esc->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_up->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_down->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_left->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_right->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_back->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_mute->setEnabled(m_connected);
//        ui->btnBLEHIDSendKey_home->setEnabled(m_connected);
//        ui->cbBLEHIDHold->setEnabled(m_host_valid);
//        ui->cbBLEHIDCapLock->setEnabled(m_host_valid);
//        ui->cbBLEHIDCtrl->setEnabled(m_host_valid);
//        ui->cbBLEHIDAlt->setEnabled(m_host_valid);
//        ui->cbBLEHIDInterupt->setEnabled(m_host_valid);
//        ui->cbBLEHIDReport->setEnabled(m_host_valid);
        ui->btnAudioRawFileSend->setEnabled(m_connected && m_device_capability_audio);
//        ui->cbAudioSelect->setEnabled(m_connected && m_device_capability_audio);

        setHIDD_buttonColor(ui->btnBLEHIDHost, m_connected ? Qt::red : Qt::white);
        if (m_host_valid)
        {
            char strBda[100];
            sprintf(strBda, "%s Host: %02x:%02x:%02x:%02x:%02x:%02x",!m_host_type?"":m_host_type==RPC_BT_DEVICE_TYPE_BT_DEVICE_TYPE_BLE?"LE":"BT",m_host_ad[0],m_host_ad[1],m_host_ad[2],m_host_ad[3],m_host_ad[4],m_host_ad[5]);
            ui->btnBLEHIDHost->setText(strBda);

            if(m_host_type==RPC_BT_DEVICE_TYPE_BT_DEVICE_TYPE_BLE)
            {
                ui->btnBLEUnbond->setEnabled(true);
                ui->btnUnbond->setEnabled(false);
            }
            else
            {
                ui->btnBLEUnbond->setEnabled(false);
                ui->btnUnbond->setEnabled(true);
            }
        }
        else
        {
            ui->cbDeviceList->clear();
            ui->cbBLEDeviceList->clear();
            ui->btnUnbond->setEnabled(false);
            ui->btnBLEUnbond->setEnabled(false);
            ui->btnBLEHIDHost->setText("Host: unpaired");
        }
    }
}

void MainWindow::UpdateHIDD_ui_pairing()
{
    QColor color[5] = {Qt::white, Qt::cyan, Qt::darkCyan, Qt::green, Qt::darkGreen};
    setHIDD_buttonColor(ui->btnBLEHIDPairingMode, color[m_pairing_mode < 5 ? m_pairing_mode : 0]);
    if (m_pairing_mode)
    {
        ui->btnBLEHIDPairingMode->setText("Exit Pairing Mode");
    }
    else
    {
        ui->btnBLEHIDPairingMode->setText("Enter Pairing Mode");
    }
}

// Connect to peer device
void MainWindow::on_btnBLEHIDConnectDisconnect_clicked()
{
    if (m_connected)
    {
        Log("Sending HID Disconnect Command");
        app_host_hidd_disconnect();
    }
    else
    {
        Log("Sending HID Connect Command");
        app_host_hidd_connect();
    }
}

// Send HID report
void MainWindow::on_btnBLEHIDSendReport_clicked()
{
   char szLog[80] = { 0 };
   uint8_t report[50];
   uint8_t report_len = 0;

   QVariant v1 = ui->cbBLEHIDInterupt->currentData();
   uint8_t channel = static_cast<BYTE> (v1.toUInt());

   QVariant v2 = ui->cbBLEHIDReport->currentData();
   uint8_t report_type = static_cast<BYTE>(v2.toUInt());

   QString str = ui->lineEditBLEHIDSendText->text();

   report_len = static_cast<BYTE>(GetHexValue(&(report[0]), 50, str));

   for (int i = 0; i < report_len; i++)
       sprintf(&szLog[strlen(szLog)], "%02x ", report[i]);

   Log("Sending HID Report: channel %d, type %d, %s",  channel, report_type, szLog);
   app_host_hidd_send_report(channel, report_type, report, report_len);
}

// Enter pairing mode
void MainWindow::on_btnBLEHIDPairingMode_clicked()
{
    bool cmd_enter = !m_pairing_mode;
    Log("Issue %s pairing mode command", cmd_enter ? "enter" : "exit");
    app_host_hidd_pairing_mode(cmd_enter);
}

void MainWindow::on_cbBLEHIDCapLock_clicked()
{
    if (ui->cbBLEHIDCapLock->isChecked())
       keyRpt_buf[KEYRPT_MODIFIER] |= USB_MODKEY_MASK_LEFT_SHIFT;
    else
       keyRpt_buf[KEYRPT_MODIFIER] &= ~USB_MODKEY_MASK_LEFT_SHIFT;
    btnBLEHIDSendKey();
}

void MainWindow::on_btnBLEHIDDVirtualUnplug_clicked()
{
    Log("Sending HIDD Virtual Unplug Command");
    setHIDD_HostAddr(nullptr);
    app_host_hidd_virtual_unplug();
}


void MainWindow::on_cbBLEHIDCtrl_clicked()
{
    if (ui->cbBLEHIDCtrl->isChecked())
       keyRpt_buf[KEYRPT_MODIFIER] |= USB_MODKEY_MASK_LEFT_CTL;
    else
       keyRpt_buf[KEYRPT_MODIFIER] &= ~USB_MODKEY_MASK_LEFT_CTL;
    btnBLEHIDSendKey();
}

void MainWindow::on_cbBLEHIDAlt_clicked()
{
    if (ui->cbBLEHIDAlt->isChecked())
       keyRpt_buf[KEYRPT_MODIFIER] |= USB_MODKEY_MASK_LEFT_ALT;
    else
       keyRpt_buf[KEYRPT_MODIFIER] &= ~USB_MODKEY_MASK_LEFT_ALT;
    btnBLEHIDSendKey();
}

#define MAX_LINK_STRING 10
static char const * link_string[MAX_LINK_STRING] = {
                            "Initialized",
                            "Disconnected",
                            "Discoverable",
                            "Connectable",
                            "Connected",
                            "Disconnecting",
                            "Reconnecting",
                            "Directed_uBCS_adv",
                            "Undirected_uBCS_adv",
                            "Invalid"};


void ull_feature_init(QLineEdit * rpt_id, QComboBox * rpt_type, QCheckBox * powerSaving, QCheckBox * repetition)
{
    rpt_id->setText("");
    rpt_type->setCurrentIndex(0);
    rpt_type->setEnabled(false);
    powerSaving->setChecked(false);
    repetition->setChecked(false);
}

void ull_feature_assign(QLineEdit * rpt_id, QComboBox * rpt_type, QCheckBox * powerSaving, QCheckBox * repetition,uint8_t * p_data, unsigned int pos, unsigned int len, int input_index)
{
    if (pos <= len)
    {
        rpt_type->setEnabled(true);
        if (pos < len)
        {
            char str[10];
            sprintf(str, "%d", p_data[pos++]);
            rpt_id->setText(str);
            rpt_type->setCurrentIndex((p_data[pos]&1 ? 1 : 0)+input_index);
            powerSaving->setChecked(p_data[pos]&2);
            repetition->setChecked(p_data[pos]&4);
        }
    }
}

// Handle WICED HCI events for BLE/BR HID device
void MainWindow::onHandleWicedEventBLEHIDD(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    char   trace[1024];

    switch (opcode)
    {
        case HCI_CONTROL_EVENT_DEVICE_STARTED:
//            ui->btnBLEHIDPairingMode->setText("Enter Pairing Mode");
            break;

        case HCI_CONTROL_HIDD_EVENT_OPENED:
            if (len)
            {
                unsigned char data[7];
                memcpy(data,p_data,6);
                data[6]=RPC_BT_DEVICE_TYPE_BT_DEVICE_TYPE_BREDR;
                SetDevicePaired(data, 7);
                setHIDD_HostAddr(data);
                setHIDD_linkChange(data, TRUE);
                Log("HID link up: %02x:%02x:%02x:%02x:%02x:%02x",p_data[0],p_data[1],p_data[2],p_data[3],p_data[4],p_data[5]);
            }
            else
            {
                setHIDD_linkChange(nullptr, TRUE);
                Log("HID link up");
            }
            break;

        case HCI_CONTROL_HIDD_EVENT_STATE_CHANGE:
            Log("%s Device state changed to %s",p_data[1]==RPC_BT_DEVICE_TYPE_BT_DEVICE_TYPE_BLE?"LE":"BT", link_string[p_data[2]>=MAX_LINK_STRING?MAX_LINK_STRING-1:p_data[2]]);
            if (p_data[1]==RPC_BT_DEVICE_TYPE_BT_DEVICE_TYPE_BREDR)
            {
                unsigned char new_pairing_mode = p_data[2]==2;   // discoverable mode
                if (m_pairing_mode != new_pairing_mode)
                {
                    m_pairing_mode = new_pairing_mode;
                    UpdateHIDD_ui_pairing();
                }
            }
            break;

        case HCI_CONTROL_HIDD_EVENT_HOST_ADDR:
            Log("Paired Host count = %d, %s", p_data[0] & 0x7f, p_data[0] & 0x80 ? "connected":"disconnected");
            if (p_data[0])
            {
                p_data[8] = p_data[1] ? RPC_BT_DEVICE_TYPE_BT_DEVICE_TYPE_BLE : RPC_BT_DEVICE_TYPE_BT_DEVICE_TYPE_BREDR;
                Log("%s Host: %02x:%02x:%02x:%02x:%02x:%02x",p_data[8]==RPC_BT_DEVICE_TYPE_BT_DEVICE_TYPE_BLE?"LE":"BT",p_data[2],p_data[3],p_data[4],p_data[5],p_data[6],p_data[7]);
                SetDevicePaired(&p_data[2], 7);
                setHIDD_HostAddr(&p_data[2]);
                setHIDD_linkChange(&p_data[2], p_data[0] & 0x80 ? TRUE : FALSE);
            }
            else
            {
                setHIDD_HostAddr(nullptr);
                setHIDD_linkChange(nullptr, FALSE);
            }
            break;

        case HCI_CONTROL_LE_EVENT_ADVERTISEMENT_STATE:
//            Log("%s", szAdvState[p_data[0]<=4?p_data[0]:0]);
            m_pairing_mode = p_data[0];
            UpdateHIDD_ui_pairing();
            break;

        case HCI_CONTROL_HIDD_EVENT_VIRTUAL_CABLE_UNPLUGGED:
            Log("HID Virtual Cable Unplugged");
            break;

        case HCI_CONTROL_HIDD_EVENT_DATA:
            sprintf(trace, "Recv HID Report type:%d ", p_data[0]);
            for (uint i = 0; i < len - 1; i++)
                sprintf(&trace[strlen(trace)], "%02x ", p_data[i + 1]);
            Log(trace);
            break;

        case HCI_CONTROL_HIDD_EVENT_CLOSED:
            setHIDD_linkChange(nullptr, FALSE);
            Log("HID Link down reason: %d ", p_data[0]);
            break;

        case HCI_CONTROL_HIDD_EVENT_AUDIO_DATA_REQ:
            {
                unsigned int count = p_data[0] + (p_data[1] << 8); // little endian 16-bit data
                send_AudioRawData(count);
            }
            break;

        case HCI_CONTROL_HIDD_EVENT_AUDIO_DATA:
            if (m_hiddAudioRaw_write_fp)
            {
                fwrite(p_data, sizeof(uint16_t), len/2, m_hiddAudioRaw_write_fp);
                m_audioRawDataWrSize += len;
                char s[20];
                sprintf(s, "%d bytes", m_audioRawDataWrSize);
                ui->audioRawDataWrSize->setText(s);
            }
            break;

        case HCI_CONTROL_HIDD_EVENT_ULL_FEATURE:
            {
                char str[10];
                char buff[500] = "";
                for (uint8_t i=0;i < len; i++)
                {

                    sprintf(str, "%02X ",p_data[i]);
                    strcat(buff, str);
                }
                Log("Received ull features event: %s",buff);
                if (len >= 7  && len <= 21)
                {
                    ui->ull_op_mode_checkBox->setChecked(p_data[0] & 1);

                    ui->checkBox_1ms->setChecked   (p_data[1] & 0x01);
                    ui->checkBox_2ms->setChecked   (p_data[1] & 0x02);
                    ui->checkBox_3ms->setChecked   (p_data[1] & 0x04);
                    ui->checkBox_4ms->setChecked   (p_data[1] & 0x08);
                    ui->checkBox_5ms->setChecked   (true); // mondatory
                    ui->checkBox_1_25ms->setChecked(p_data[1] & 0x20);
                    ui->checkBox_2_5ms->setChecked (p_data[1] & 0x40);
                    ui->checkBox_3_75ms->setChecked(p_data[1] & 0x80);
                    ui->checkBox_7_5ms->setChecked (true); // mondatory

                    sprintf(str, "%d", p_data[3]);
                    ui->lineEdit_d2h_pdu_size->setText(str);
                    sprintf(str, "%d", p_data[4]);
                    ui->lineEdit_h2d_pdu_size->setText(str);

                    // initialize rpt features
                    ull_feature_init(ui->lineEdit_ull_rpt_id1, ui->comboBox_ull_rpt_type1, ui->checkBox_power_saving1, ui->checkBox_repetition1);
                    ull_feature_init(ui->lineEdit_ull_rpt_id2, ui->comboBox_ull_rpt_type2, ui->checkBox_power_saving2, ui->checkBox_repetition2);
                    ull_feature_init(ui->lineEdit_ull_rpt_id3, ui->comboBox_ull_rpt_type3, ui->checkBox_power_saving3, ui->checkBox_repetition3);
                    ull_feature_init(ui->lineEdit_ull_rpt_id4, ui->comboBox_ull_rpt_type4, ui->checkBox_power_saving4, ui->checkBox_repetition4);
                    ull_feature_init(ui->lineEdit_ull_rpt_id5, ui->comboBox_ull_rpt_type5, ui->checkBox_power_saving5, ui->checkBox_repetition5);
                    ull_feature_init(ui->lineEdit_ull_rpt_id6, ui->comboBox_ull_rpt_type6, ui->checkBox_power_saving6, ui->checkBox_repetition6);
                    ull_feature_init(ui->lineEdit_ull_rpt_id7, ui->comboBox_ull_rpt_type7, ui->checkBox_power_saving7, ui->checkBox_repetition7);

                    ull_feature_assign(ui->lineEdit_ull_rpt_id0, ui->comboBox_ull_rpt_type0, ui->checkBox_power_saving0, ui->checkBox_repetition0, p_data, 5, len, 0);
                    ull_feature_assign(ui->lineEdit_ull_rpt_id1, ui->comboBox_ull_rpt_type1, ui->checkBox_power_saving1, ui->checkBox_repetition1, p_data, 7, len, 1);
                    ull_feature_assign(ui->lineEdit_ull_rpt_id2, ui->comboBox_ull_rpt_type2, ui->checkBox_power_saving2, ui->checkBox_repetition2, p_data, 9, len, 1);
                    ull_feature_assign(ui->lineEdit_ull_rpt_id3, ui->comboBox_ull_rpt_type3, ui->checkBox_power_saving3, ui->checkBox_repetition3, p_data, 11, len, 1);
                    ull_feature_assign(ui->lineEdit_ull_rpt_id4, ui->comboBox_ull_rpt_type4, ui->checkBox_power_saving4, ui->checkBox_repetition4, p_data, 13, len, 1);
                    ull_feature_assign(ui->lineEdit_ull_rpt_id5, ui->comboBox_ull_rpt_type5, ui->checkBox_power_saving5, ui->checkBox_repetition5, p_data, 15, len, 1);
                    ull_feature_assign(ui->lineEdit_ull_rpt_id6, ui->comboBox_ull_rpt_type6, ui->checkBox_power_saving6, ui->checkBox_repetition6, p_data, 17, len, 1);
                    ull_feature_assign(ui->lineEdit_ull_rpt_id7, ui->comboBox_ull_rpt_type7, ui->checkBox_power_saving7, ui->checkBox_repetition7, p_data, 19, len, 1);
                }
                else
                {
                    Log("Invalid ull features data length: %d", len);
                }
            }

            break;

        case HCI_CONTROL_HIDD_EVENT_CAPABILITY:
            m_device_capability_audio = p_data[0];
            m_device_capability_motion = p_data[1];
            m_device_capability_ir = p_data[2];
            if (len > 3)
            {
                m_device_capability_ull = p_data[3];
            }
            UpdateHIDD_ui_host();
            if (m_device_capability_audio)
            {
                char s[100];
                sprintf(s, "Codec: %s, 16KHz%s, %s MIC", m_device_capability_audio & HCI_CONTROL_HIDD_AUDIO_ADPCM ? "ADPCM" :
                                       m_device_capability_audio & HCI_CONTROL_HIDD_AUDIO_OPUS ? "OPUS" :
                                       m_device_capability_audio & HCI_CONTROL_HIDD_AUDIO_MSBC ? "mSBC" : "None",
                                       m_device_capability_audio & HCI_CONTROL_HIDD_AUDIO_8K ? "/8kHz":"",
                                       m_device_capability_audio & HCI_CONTROL_HIDD_AUDIO_DIGITAL_MIC ? "Digital":"Analog");
                ui->hidd_audio_groupBox->setEnabled(true);
                ui->HID_AudioCodec->setText(s);
                if (m_device_capability_audio & HCI_CONTROL_HIDD_AUDIO_8K)
                {
                    ui->sampleRate16KHz->setEnabled(true);
                    ui->sampleRate16KHz_2->setEnabled(true);
                }
                else
                {
                    ui->sampleRate16KHz->setChecked(true);
                    ui->sampleRate16KHz_2->setChecked(true);
                    ui->sampleRate16KHz->setEnabled(false);
                    ui->sampleRate16KHz_2->setEnabled(false);
                }
                if (m_device_capability_audio & (HCI_CONTROL_HIDD_AUDIO_ADPCM | HCI_CONTROL_HIDD_AUDIO_OPUS | HCI_CONTROL_HIDD_AUDIO_MSBC))
                {
                    ui->send_encoded->setEnabled(true);
                    ui->record_encoded->setEnabled(true);
                }
                else
                {
                    ui->send_encoded->setChecked(false);
                    ui->send_encoded->setEnabled(false);
                    ui->record_encoded->setChecked(false);
                    ui->record_encoded->setEnabled(false);
                }
            }
            else
            {
                ui->HID_AudioCodec->setText("");
                ui->hidd_audio_groupBox->setEnabled(false);
            }

            if (m_device_capability_ull)
            {
                ui->ull_hid_groupBox->setEnabled(true);
                Log("Sending get ULL features command");
                SendWicedCommand(HCI_CONTROL_HIDD_COMMAND_GET_ULL_FEATURES, NULL, 0);
            }
            else
            {
                ui->ull_hid_groupBox->setEnabled(false);
            }

            Log("Device Capability: Audio=%02x Motion=%02x IR=%02x ULL=%02x", p_data[0], p_data[1], p_data[2], p_data[3]);
            break;

        case HCI_CONTROL_HIDD_EVENT_AUDIO_STOP:
            Log("Received audio stop event from the device");
            stop_AudioRawFileSend();
            break;
    }
}

void MainWindow::stop_AudioRawFileSend()
{
    fclose(m_hiddAudioRaw_fp);
    m_hiddAudioRaw_fp = NULL;
    ui->AudioRawDataSendProgressBar->setEnabled(false);
    ui->AudioRawDataSendProgressBar->setValue(0);
    ui->btnAudioRawFileSend->setText("Send");
}

#define MAX_AUDIO_SAMPLE_COUNT 512
void MainWindow::send_AudioRawData(unsigned int count)
{
    unsigned int size;
    UINT16 data[MAX_AUDIO_SAMPLE_COUNT];

    if ((m_hiddAudioRaw_fp != NULL) && count && m_audioRawDataSize)
    {
        if (count > MAX_AUDIO_SAMPLE_COUNT)
        {
            count = MAX_AUDIO_SAMPLE_COUNT;
        }

        size = fread(data, 1, count*2, m_hiddAudioRaw_fp);

        // if we have data, send it.
        if (size)
        {
            SendWicedCommand(HCI_CONTROL_HIDD_COMMAND_AUDIO_DATA, (unsigned char *) data, size);
            m_audioRawDataCurrentSize += size;
            ui->AudioRawDataSendProgressBar->setValue(100 * m_audioRawDataCurrentSize / m_audioRawDataSize);
        }

        // if we got less than we asked for, that means it is reaching the end of file
        if ((size/sizeof(UINT16)) < count)
        {
            SendWicedCommand(HCI_CONTROL_HIDD_COMMAND_AUDIO_STOP_REQ, NULL, 0);
            stop_AudioRawFileSend();
        }
    }
}

void MainWindow::send_audio_file(const char * filename)
{
    m_hiddAudioRaw_fp = fopen(filename, "rb");
    if (m_hiddAudioRaw_fp == NULL)
    {
        Log("Error opening file %s", filename);
        QMessageBox(QMessageBox::Information, filename, "File Open Error", QMessageBox::Ok).exec();
    }
    else
    {
        Log("Sending input audio data file from %s", filename);
        fseek(m_hiddAudioRaw_fp, 0, SEEK_END);
        m_audioRawDataSize = ftell(m_hiddAudioRaw_fp);
        rewind(m_hiddAudioRaw_fp);
        m_audioRawDataCurrentSize = 0;

        ui->AudioRawDataSendProgressBar->setEnabled(true);
        ui->btnAudioRawFileSend->setText("Cancel");
        uint8_t buff[2];
        buff[0] = 1;
        buff[1] = ui->send_encoded->isChecked() ? 1 : 0;
        SendWicedCommand(HCI_CONTROL_HIDD_COMMAND_AUDIO_START_REQ, buff, 2);
    }
}

QFile sourceFile;
QAudioOutput* audio; // class member.

void MainWindow::play_audio_file(const char * filename, uint32_t sample_rate )
{
    if (sourceFile.isOpen())
    {
        sourceFile.close();
    }
    sourceFile.setFileName(filename);
    if (!sourceFile.open(QIODevice::ReadOnly))
    {
        Log("Fail to open file %s", filename);
        return;
    }

    QAudioFormat format;
    // Set up the format, eg.
    format.setSampleRate(sample_rate);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format))
    {
        Log("Raw audio format not supported by backend, cannot play audio.");
        return;
    }

    Log("Playing file %s", filename);
    audio = new QAudioOutput(format, this);
//    connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
    audio->start(&sourceFile);
}

void MainWindow::on_btnAudioRawFileSend_clicked()
{
    if (!strcmp(ui->btnAudioRawFileSend->text().toStdString().c_str(),"Cancel"))
    {
        stop_AudioRawFileSend();
        SendWicedCommand(HCI_CONTROL_HIDD_COMMAND_AUDIO_STOP_REQ, NULL, 0);
    }
    else
    {
        send_audio_file(ui->edAudioRawFile->text().toStdString().c_str());
    }
}

void MainWindow::on_btnFindAudioRawFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Audio Data File"), "", tr("Raw Files (*.raw);;Audio Data Files (*.raw *.bin *.opus *.adpcm *.msbc);;All (*.*)"));
    if (!fileName.isEmpty())
    {
        ui->edAudioRawFile->setText(fileName);
    }
}

void MainWindow::on_cbAudioSelect_activated(int index)
{
    char filename[100];

    ui->cbAudioSelect->setCurrentIndex(index);
    sprintf(filename, "audio_%s.raw", ui->cbAudioSelect->currentText().toStdString().c_str());
    ui->edAudioRawFile->setText(filename);
//    send_audio_file(filename);
}


void MainWindow::on_cbBLEHIDDebug_currentIndexChanged(int index)
{
    (void) index;
    Log("Route debug message to %s", ui->cbBLEHIDDebug->currentText().toStdString().c_str());
    EnableAppTraces();
}


void MainWindow::on_btnFindAudioRawFile_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Audio Data File"), "", tr("Raw Files (*.raw);;Audio Data Files (*.raw *.bin *.opus *.adpcm *.msbc);;All (*.*)"));
    if (!fileName.isEmpty())
    {
        FILE * tempFile = fopen(fileName.toStdString().c_str(), "rb");
        if (tempFile)
        {
            fclose(tempFile);

            if (QMessageBox::Yes != QMessageBox(QMessageBox::Information, "File Exists", "Overwrite?", QMessageBox::Yes|QMessageBox::No).exec())
            {
                return;
            }
        }
        ui->edAudioRawFile_2->setText(fileName);
    }
}

void MainWindow::on_audioPlayButton1_clicked()
{
    if (m_hiddAudioRaw_fp != NULL)
    {
        fclose(m_hiddAudioRaw_fp);
    }

    if (ui->send_encoded->isChecked())
    {
        Log("Cannot play encoded file");
    }
    else
    {
        play_audio_file(ui->edAudioRawFile->text().toStdString().c_str(), ui->sampleRate16KHz_2->isChecked()? 16000 : 8000);
    }
}

void MainWindow::on_audioPlayButton2_clicked()
{
    if (m_hiddAudioRaw_write_fp != NULL)
    {
        fclose(m_hiddAudioRaw_write_fp);
    }
    if (ui->record_encoded->isChecked())
    {
        Log("Cannot play encoded file");
    }
    else
    {
        play_audio_file(ui->edAudioRawFile_2->text().toStdString().c_str(), ui->sampleRate16KHz->isChecked()? 16000 : 8000);
    }
}


void MainWindow::on_audioRecordutton_pressed()
{
    uint8_t cmd[3] = {1};

    cmd[0] = 1; // start
    cmd[1] = ui->sampleRate16KHz->isChecked() ? 1 : 0; // 16kHz or 8kHz
    cmd[2] = ui->record_encoded->isChecked() ? 1 : 0;         // save the file encoded or in RAW PCM.
    m_hiddAudioRaw_write_fp = fopen(ui->edAudioRawFile_2->text().toStdString().c_str(), "wb");

    if (m_hiddAudioRaw_write_fp == NULL)
    {
        QMessageBox(QMessageBox::Information, "Audio data output", "File Open Error", QMessageBox::Ok).exec();
        return;
    }
    Log("File %s opened", ui->edAudioRawFile_2->text().toStdString().c_str());
    m_audioRawDataWrSize = 0;
    SendWicedCommand(HCI_CONTROL_HIDD_COMMAND_AUDIO_MIC_START_STOP, cmd, 3);
}

void MainWindow::on_audioRecordutton_released()
{
    uint8_t cmd = 0;
    if (m_hiddAudioRaw_write_fp)
    {
        fclose(m_hiddAudioRaw_write_fp);
        m_hiddAudioRaw_write_fp = NULL;
        Log("File %s created, size = %d", ui->edAudioRawFile_2->text().toStdString().c_str(), m_audioRawDataWrSize);
    }
    SendWicedCommand(HCI_CONTROL_HIDD_COMMAND_AUDIO_MIC_START_STOP, &cmd, 1);
    if (ui->record_encoded->isChecked())
    {
        Log("Cannot play encoded file");
    }
    else
    {
        play_audio_file(ui->edAudioRawFile_2->text().toStdString().c_str(), ui->sampleRate16KHz->isChecked()? 16000 : 8000);
    }
}

int ull_build_rpt_data(uint8_t * cmd, unsigned int * len_p,
                       QLineEdit * rpt_id, QComboBox * rpt_type, QCheckBox * powerSaving, QCheckBox * repetition,
                       int output_index)
{
    int index = rpt_type->currentIndex();

    // make sure this report is not disabled.
    //  1. when ouput report index is 1 (This rpt_type only has index 0 & 1, no 'Not Used' entry. )  -- always enabled
    //  2. when ouput report index is 2 (This rpt_type has index 0, 1, & 2. Index 0 is used for 'Not Used'. ) -- it is disabled when index is 0
    if ((output_index != 2) || index)
    {
        unsigned int len = *len_p;

        cmd[len] = rpt_id->text().toUInt();

        if (!cmd[len++])
        {
            QMessageBox msgBox;
            char msg[80];

            sprintf(msg, "Report ID is 0. Are you sure you want to use it?");
            msgBox.setWindowTitle("Warning");
            msgBox.setText(msg);
            msgBox.setStandardButtons(QMessageBox::Yes);
            msgBox.addButton(QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            if(msgBox.exec() != QMessageBox::Yes)
            {
                return false;
            }
        }

        cmd[len++] = (index == output_index ? 1 : 0) |
                     (powerSaving->isChecked() ? 2 : 0) |
                     (repetition->isChecked() ? 4 : 0);
        *len_p = len;
    }
    return true;
}

void MainWindow::on_btn_ull_save_clicked()
{
    uint8_t cmd[21];
    unsigned int len = 0;

    Log("on_btn_ull_save_clicked");
    cmd[len++] = ui->ull_op_mode_checkBox->isChecked() ? 1 : 0; // start
    cmd[len++] = (ui->checkBox_1ms->isChecked() ? 1 : 0) |
                 (ui->checkBox_2ms->isChecked() ? 2 : 0) |
                 (ui->checkBox_3ms->isChecked() ? 4 : 0) |
                 (ui->checkBox_4ms->isChecked() ? 8 : 0) |
                 (ui->checkBox_5ms->isChecked() ? 16 : 0) |
                 (ui->checkBox_1_25ms->isChecked() ? 32 : 0) |
                 (ui->checkBox_2_5ms->isChecked() ? 64 : 0) |
                 (ui->checkBox_3_75ms->isChecked() ? 128 : 0);
    cmd[len++] = (ui->checkBox_7_5ms->isChecked() ? 1 : 0);
    cmd[len++] = ui->lineEdit_d2h_pdu_size->text().toUInt();
    cmd[len++] = ui->lineEdit_h2d_pdu_size->text().toUInt();

    if (!ull_build_rpt_data(cmd, &len, ui->lineEdit_ull_rpt_id0, ui->comboBox_ull_rpt_type0, ui->checkBox_power_saving0, ui->checkBox_repetition0, 1)) return;
    if (!ull_build_rpt_data(cmd, &len, ui->lineEdit_ull_rpt_id1, ui->comboBox_ull_rpt_type1, ui->checkBox_power_saving1, ui->checkBox_repetition1, 2)) return;
    if (!ull_build_rpt_data(cmd, &len, ui->lineEdit_ull_rpt_id2, ui->comboBox_ull_rpt_type2, ui->checkBox_power_saving2, ui->checkBox_repetition2, 2)) return;
    if (!ull_build_rpt_data(cmd, &len, ui->lineEdit_ull_rpt_id3, ui->comboBox_ull_rpt_type3, ui->checkBox_power_saving3, ui->checkBox_repetition3, 2)) return;
    if (!ull_build_rpt_data(cmd, &len, ui->lineEdit_ull_rpt_id4, ui->comboBox_ull_rpt_type4, ui->checkBox_power_saving4, ui->checkBox_repetition4, 2)) return;
    if (!ull_build_rpt_data(cmd, &len, ui->lineEdit_ull_rpt_id5, ui->comboBox_ull_rpt_type5, ui->checkBox_power_saving5, ui->checkBox_repetition5, 2)) return;
    if (!ull_build_rpt_data(cmd, &len, ui->lineEdit_ull_rpt_id6, ui->comboBox_ull_rpt_type6, ui->checkBox_power_saving6, ui->checkBox_repetition5, 2)) return;
    if (!ull_build_rpt_data(cmd, &len, ui->lineEdit_ull_rpt_id7, ui->comboBox_ull_rpt_type7, ui->checkBox_power_saving7, ui->checkBox_repetition7, 2)) return;

    char buff[500] = "";
    for (uint8_t i=0;i < len; i++)
    {
        char str[10];

        sprintf(str, "%02X ",cmd[i]);
        strcat(buff, str);
    }
    Log("Sending saving ull features command: %s",buff);
    SendWicedCommand(HCI_CONTROL_HIDD_COMMAND_SAVE_ULL_FEATURES, cmd, len);
}

void on_comboBox_ull_rpt_type_currentIndexChanged(QComboBox * next_rpt_type, QCheckBox * powerSaving, QCheckBox * repetition, int index)
{
    if (index)
    {
        powerSaving->setEnabled(true);
        repetition->setEnabled(true);
        if (next_rpt_type)
        {
            next_rpt_type->setEnabled(true);
        }
    }
    else
    {
        powerSaving->setChecked(false);
        powerSaving->setEnabled(false);
        repetition->setChecked(false);
        repetition->setEnabled(false);
        if (next_rpt_type)
        {
            next_rpt_type->setCurrentIndex(0);
            next_rpt_type->setEnabled(false);
        }
    }
}

void MainWindow::on_comboBox_ull_rpt_type1_currentIndexChanged(int index)
{
    on_comboBox_ull_rpt_type_currentIndexChanged(ui->comboBox_ull_rpt_type2, ui->checkBox_power_saving1, ui->checkBox_repetition1, index);
}

void MainWindow::on_comboBox_ull_rpt_type2_currentIndexChanged(int index)
{
    on_comboBox_ull_rpt_type_currentIndexChanged(ui->comboBox_ull_rpt_type3, ui->checkBox_power_saving2, ui->checkBox_repetition2, index);
}

void MainWindow::on_comboBox_ull_rpt_type3_currentIndexChanged(int index)
{
    on_comboBox_ull_rpt_type_currentIndexChanged(ui->comboBox_ull_rpt_type4, ui->checkBox_power_saving3, ui->checkBox_repetition3, index);
}

void MainWindow::on_comboBox_ull_rpt_type4_currentIndexChanged(int index)
{
    on_comboBox_ull_rpt_type_currentIndexChanged(ui->comboBox_ull_rpt_type5, ui->checkBox_power_saving4, ui->checkBox_repetition4, index);
}

void MainWindow::on_comboBox_ull_rpt_type5_currentIndexChanged(int index)
{
    on_comboBox_ull_rpt_type_currentIndexChanged(ui->comboBox_ull_rpt_type6, ui->checkBox_power_saving5, ui->checkBox_repetition5, index);
}

void MainWindow::on_comboBox_ull_rpt_type6_currentIndexChanged(int index)
{
    on_comboBox_ull_rpt_type_currentIndexChanged(ui->comboBox_ull_rpt_type7, ui->checkBox_power_saving6, ui->checkBox_repetition6, index);
}

void MainWindow::on_comboBox_ull_rpt_type7_currentIndexChanged(int index)
{
    on_comboBox_ull_rpt_type_currentIndexChanged(NULL, ui->checkBox_power_saving7, ui->checkBox_repetition7, index);
}
