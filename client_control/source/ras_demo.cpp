/*
 * (c) 2024-2026, Infineon Technologies AG, or an affiliate of Infineon
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
 * Sample MCU application for RAS/RAP using WICED HCI protocol.
 */
#include "app_include.h"
extern "C"
{
#include "app_host.h"
#include "wiced_hci_ras.h"
}
#include "QDebug"
#include "mainwindow.h"
#include <QByteArray>
#include <QMessageBox>
#include <QFileInfo>

Q_DECLARE_METATYPE(CBtDevice *)

#define TAG "RAS"
#define NO_SWIFT_PAIR FALSE
#define RAS_TX_POWER 0

void MainWindow::on_btn_le_ras_start_adv_clicked()
{
    QString text = ui->btn_le_ras_start_adv->text();

    if (text == STRING_START_ADVERTISEMENT)
    {
        app_host_gatt_start_stop_advert(TRUE, NO_SWIFT_PAIR , RAS_TX_POWER);
        ui->btn_le_ras_start_adv->setText(STRING_STOP_ADVERTISEMENT);
    }
    else if (text == STRING_STOP_ADVERTISEMENT)
    {
        app_host_gatt_start_stop_advert(FALSE, NO_SWIFT_PAIR, RAS_TX_POWER);
        ui->btn_le_ras_start_adv->setText(STRING_START_ADVERTISEMENT);
    }

}

void MainWindow::on_btn_le_ras_connect_clicked()
{
    on_LeConnectPressed(ui->btn_le_ras_connect->text());
}

void MainWindow::on_btn_le_ras_set_cs_params_clicked()
{

}

void MainWindow::ras_reset_ui()
{
    ui->btn_le_ras_connect->setText(STRING_CONNECT);
    ui->btn_le_ras_start_adv->setText(STRING_START_ADVERTISEMENT);
}

uint8_t get_phy_value_from_cb(QString phy_string)
{
    uint8_t cs_phy = 1;
    if(phy_string == "LE 1M Phy"){
        cs_phy = 1;
    }else if(phy_string == "LE 2M Phy"){
        cs_phy = 2;
    }else if(phy_string == "LE Coded, S=8"){
        cs_phy = 3;
    }else {
        cs_phy = 4;
    }

    return cs_phy;
}

void MainWindow::on_btn_le_ras_start_cs_clicked()
{
    CBtDevice *p_device = GetSelectedLEDevice();

    Log("%s Starting CS %s",TAG, p_device? p_device->get_bdaddr_string():"none");
    if(!p_device)
    {
        return;
    }

    if(p_device->is_app_status_ready() == 0){
        Log("%s %s not ready, returning",TAG, p_device->get_bdaddr_string());
        return;
    }

    ras_rpc_cs_cmd_procedure_params_t params;

    uint16_t conn_id = p_device->get_connection_handle();

    params.cs_role = ui->cs_role_cb->currentText() == "Initiator"? 0:1;
    params.mode_0_steps = ui->cs_mode_0_steps->text().toInt();
    params.max_procedure_len = ui->cs_max_procedure_len->text().toInt();
    params.min_procedure_interval = ui->cs_min_procedure_interval->text().toInt();
    params.max_procedure_interval = ui->cs_max_procedure_interval->text().toInt();
    params.max_procedure_count = ui->cs_max_procedure_count->text().toInt();
    params.min_subevent_len = ui->cs_min_subevent_len->text().toInt();
    params.max_subevent_len = ui->cs_max_subevent_len->text().toInt();
    params.cs_sync_phy = get_phy_value_from_cb(ui->cs_sync_phy_cb->currentText());
    params.cs_procedure_phy = get_phy_value_from_cb(ui->cs_procedure_phy_cb->currentText());
    params.tx_power_delta = ui->cs_tx_power_delta->text().toInt();

    Log("Set CS Params role %d m0_steps %d proc len %d proc interval (%d %d)",
        params.cs_role, params.mode_0_steps, params.max_procedure_len, params.min_procedure_interval, params.max_procedure_interval);
    Log(".. count %d subevent (%d %d) phy (%d %d) pwr_delta %d",
        params.max_procedure_count, params.min_subevent_len, params.max_subevent_len, params.cs_sync_phy, params.cs_procedure_phy, params.tx_power_delta);

     wiced_hci_le_ras_start_cs_procedure(conn_id, &params);
}

static const name_val_t ras_event_strings[]  = {
    { "security enabled", HCI_CONTROL_RAS_CS_EVENT_SECURITY_ENABLE },
    { "read remote capabilities complete", HCI_CONTROL_RAS_CS_EVENT_READ_REMOTE_CAPABILITIES_COMPLETE },
    { "read fae table complete", HCI_CONTROL_RAS_CS_EVENT_READ_REMOTE_FAE_TABLE_COMPLETE },
    { "config complete", HCI_CONTROL_RAS_CS_EVENT_CS_CONFIG_COMPLETE },
    { "procedure enable", HCI_CONTROL_RAS_CS_EVENT_PROCEDURE_ENABLE}
};

void MainWindow::ras_handle_ranging_data_events(DWORD opcode, BYTE *rx_buf, DWORD len)
{
    uint16_t conn_id;
    uint8_t status;
    uint8_t *p = rx_buf;
    const char * p_ras_str = get_name_for_val(ras_event_strings, sizeof(ras_event_strings)/sizeof(ras_event_strings[0]), opcode);

    UNUSED(len);

    STREAM_TO_UINT16(conn_id, p);
    STREAM_TO_UINT8(status, p);

    switch(opcode)
    {
    case HCI_CONTROL_RAS_CS_EVENT_SECURITY_ENABLE:
    {
        Log("[%s] event [%ld] %s status 0x%x",TAG, opcode,p_ras_str, status );

    }break;
    case HCI_CONTROL_RAS_CS_EVENT_READ_REMOTE_CAPABILITIES_COMPLETE:
    {
        Log("[%s] event [%ld] %s status 0x%x ",TAG, opcode,p_ras_str, status);
    }break;
    case HCI_CONTROL_RAS_CS_EVENT_READ_REMOTE_FAE_TABLE_COMPLETE:
    {
        Log("[%s] event [%ld] %s status 0x%x",TAG, opcode, p_ras_str, status);
    }break;
    case HCI_CONTROL_RAS_CS_EVENT_CS_CONFIG_COMPLETE:
    {
        wiced_ble_cs_config_complete_t cfg, *p_cfg = &cfg;
        STREAM_TO_UINT8( p_cfg->config_id, p);
        STREAM_TO_UINT8( p_cfg->action, p);
        STREAM_TO_UINT8( p_cfg->main_mode_type, p);
        STREAM_TO_UINT8( p_cfg->sub_mode_type, p);
        STREAM_TO_UINT8( p_cfg->min_main_mode_steps, p);
        STREAM_TO_UINT8( p_cfg->max_main_mode_steps, p);
        STREAM_TO_UINT8( p_cfg->main_mode_repetition, p);
        STREAM_TO_UINT8( p_cfg->mode_0_steps, p);
        STREAM_TO_UINT8( p_cfg->role, p);

        Log("[%s] event [%ld] %s status 0x%x role %s",TAG, opcode, p_ras_str, status, p_cfg->role == 0? "initator": "reflector");

    }break;
    case HCI_CONTROL_RAS_CS_EVENT_PROCEDURE_ENABLE:
    {
        wiced_ble_cs_procedure_enable_complete_t proc, *p_proc = &proc;

        STREAM_TO_UINT8( p_proc->config_id, p);
        STREAM_TO_UINT8( p_proc->state, p);
        STREAM_TO_UINT8( p_proc->tone_antenna_config_selection, p);
        STREAM_TO_UINT8( p_proc->selected_tx_power, p);
        STREAM_TO_UINT24( p_proc->subevent_len, p);
        STREAM_TO_UINT8( p_proc->subevents_per_event, p);
        STREAM_TO_UINT16( p_proc->subevent_interval, p);
        STREAM_TO_UINT16( p_proc->event_interval, p);
        STREAM_TO_UINT16( p_proc->procedure_interval, p);
        STREAM_TO_UINT16( p_proc->procedure_count, p);
        STREAM_TO_UINT16( p_proc->max_procedure_len, p);

        Log("[%s] event [%ld] %s status 0x%x count %d",TAG, opcode,p_ras_str, status,p_proc->procedure_count);
    }break;
    }
}

void MainWindow::ras_handle_le_events(DWORD opcode, BYTE *rx_buf, DWORD len)
{
    UNUSED(rx_buf);
    UNUSED(len);

    Log("[%s] event %ld len %ld", TAG, opcode, len);
    switch(opcode)
    {
    case HCI_CONTROL_LE_EVENT_CONNECTED:
        Log("[%s] %sconnected",TAG,rx_buf[10]? "":"dis");
        ui->btn_le_ras_connect->setText(STRING_DISCONNECT);
        break;
    case HCI_CONTROL_LE_EVENT_DISCONNECTED:
        Log("[%s] disconnected",TAG);
        ui->btn_le_ras_connect->setText(STRING_CONNECT);
        break;
    }
}


// Handle WICED HCI events
void MainWindow::onHandleWicedEventRas(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    if(!ui->tabRas->isEnabled())
    {
        return;
    }

    switch (HCI_CONTROL_GROUP(opcode))
    {
    case HCI_CONTROL_GROUP_RAS:
        ras_handle_ranging_data_events(opcode, p_data, len);
        break;
    case HCI_CONTROL_GROUP_LE:
        ras_handle_le_events(opcode, p_data, len);
        break;
    }
}
