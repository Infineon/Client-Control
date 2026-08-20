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
 * Sample MCU application for BLE Battery Client (BATTC) profile using WICED HCI protocol.
 */


#include "app_include.h"
#include "app_host.h"

// Initialize app
void MainWindow::InitBATTC(void)
{

}

// connect to peer deivce
void MainWindow::on_btnBATTCConnect_clicked(void)
{
    BYTE    cmd[6];
    int     commandBytes = 0;

    if (m_CommPort == NULL)
        return;

    if (!m_bPortOpen)
    {
        return;
    }

    CBtDevice * pDev =(CBtDevice *)GetSelectedLEDevice() ;
    if (NULL == pDev)
        return;

    if(pDev->m_battc_handle != NULL_HANDLE)
    {
        Log("BATTC already connected for selected device");
        return;
    }

    for (int i = 0; i < 6; i++)
        cmd[commandBytes++] = pDev->m_address[5 - i];

    Log("BATTC Connect Command, BDA: %02x:%02x:%02x:%02x:%02x:%02x",
           pDev->m_address[0], pDev->m_address[1], pDev->m_address[2], pDev->m_address[3], pDev->m_address[4], pDev->m_address[5]);

    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_CONNECT, cmd, 6);
}

// disconnect from peer deivce
void MainWindow::on_btnBATTCDisconnect_clicked(void)
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;
    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Disconnect Command, Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_DISCONNECT, cmd, commandBytes);

    pDev->m_battc_handle = NULL_HANDLE;
    pDev->m_conn_type &= ~CONNECTION_TYPE_BATTC;
}

// disconnect from peer deivce
void MainWindow::on_btnBATTCReadLevel_clicked(void)
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;
    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Read Level Command, Handle: %d (%02X%02X)", nHandle, cmd[1], cmd[0]);
    //SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_READ, cmd, commandBytes);
}


void MainWindow::on_btnBroadcastEnable_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Enable Broadcast Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_ENABLE_BROADCAST, cmd, commandBytes);
}

void MainWindow::on_btnDisableBroadcast_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Disabl Broadcast Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_DISABLE_BROADCAST, cmd, commandBytes);
}


void MainWindow::on_btnLevelModify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify Level Status Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_LEVEL_STATUS_MODIFY, cmd, commandBytes);
}


void MainWindow::on_btnLevelNotify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify/Indicate Status Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_LEVEL_STATUS_SIGNAL, cmd, commandBytes);
}


void MainWindow::on_btnServicedateModify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify/Indicate Serial number Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_SERVICE_DATE_MODIFY, cmd, commandBytes);
}


void MainWindow::on_btnServicedateNotify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify/Indicate Serial number Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_SERVICE_DATE_SIGNAL, cmd, commandBytes);
}


void MainWindow::on_btnCriticalStatusModify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Modify critical status Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_CRITICAL_STATUS_MODIFY, cmd, commandBytes);
}


void MainWindow::on_btnCriticalStatusSignal_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify/Indicate critical status Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_CRITICAL_STATUS_SIGNAL, cmd, commandBytes);
}


void MainWindow::on_btnEnergyStatusModify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify Energy status Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_ENERGY_STATUS_MODIFY, cmd, commandBytes);
}


void MainWindow::on_btnEnergyStatusSignal_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify/Indicate Energy Status Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_ENERGY_STATUS_SIGNAL, cmd, commandBytes);
}

void MainWindow::on_btnTimeStatusModify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify TIME Status Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_TIME_STATUS_MODIFY, cmd, commandBytes);
}


void MainWindow::on_btnTimeStatusNotify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify/Indicate time Status Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_TIME_STATUS_SIGNAL, cmd, commandBytes);
}



void MainWindow::on_btnHealthStatusModify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify Health Status Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_HEALTH_STATUS_MODIFY, cmd, commandBytes);
}


void MainWindow::on_btnHealthStatusSignal_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify/Indicate Health Status Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_HEALTH_STATUS_SIGNAL, cmd, commandBytes);
}


void MainWindow::on_btnHealthInfoModify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify Health Info Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_HEALTH_INFO_MODIFY, cmd, commandBytes);
}


void MainWindow::on_btnHealthInfoSignal_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify/Indicate Health Info Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_HEALTH_INFO_SIGNAL, cmd, commandBytes);
}


void MainWindow::on_btnBatteryInfoModify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify Battery Info Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_BATTERY_INFO_MODIFY, cmd, commandBytes);
}


void MainWindow::on_btnBatteryInfoSignal_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify/Indicate Battery Info Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_BATTERY_INFO_SIGNAL, cmd, commandBytes);
}


void MainWindow::on_btnNameModify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Modify manufacturer name Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_NAME_MODIFY, cmd, commandBytes);
}



void MainWindow::on_btnNameNotify_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Signal manufacturer name Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_NAME_SIGNAL, cmd, commandBytes);
}


void MainWindow::on_btnModelChange_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Modify model number Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_MODEL_MODIFY, cmd, commandBytes);
}



void MainWindow::on_btnModelSignal_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify model number Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_MODEL_SIGNAL, cmd, commandBytes);
}


void MainWindow::on_btnNumberMofiy_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Modify Serial number Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_NUMBER_MODIFY, cmd, commandBytes);
}


void MainWindow::on_btnNumberSignal_clicked()
{
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Notify/Indicate Serial number Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_NUMBER_SIGNAL, cmd, commandBytes);
}

void MainWindow::on_btnBATTCAdvStart_clicked()
{
    /*
    BYTE   cmd[60];
    int    commandBytes = 0;

    CBtDevice * pDev = GetConnectedBATTCDevice();
    if (pDev == NULL)
        return;

    USHORT nHandle = pDev->m_battc_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("BATTC Start advertisements: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_START_ADV, cmd, commandBytes);
    */
}


// Handle WICED HCI events
void MainWindow::onHandleWicedEventBATTC(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    switch (HCI_CONTROL_GROUP(opcode))
    {
    case HCI_CONTROL_GROUP_BATT_CLIENT:
        HandleBATTCHEvents(opcode, p_data, len);
        break;
    }
}

// Handle WICED HCI events for Battery Client
void MainWindow::HandleBATTCHEvents(DWORD opcode, LPBYTE p_data, DWORD len)
{
    char      trace[1024];
    BYTE       bda[6];
    CBtDevice *device;
    UINT16  handle;

    UNUSED(len);

    switch (opcode)
    {
    case HCI_CONTROL_BATT_CLIENT_EVENT_CONNECTED:
        for (int i = 0; i < 6; i++){
            bda[5 - i] = p_data[i + 1];
            //sprintf(trace, "BATTC Connected p_data:%d i:%d", p_data[i], i);
            //Log(trace);
        }
        sprintf(trace, "BATTC Connected status:%d address %02x:%02x:%02x:%02x:%02x:%02x handle:%d",
            p_data[0], bda[0], bda[1], bda[2], bda[3], bda[4], bda[5], p_data[7] + (p_data[8] << 8));
        Log(trace);
        handle = p_data[7] + (p_data[8] << 8);
        if (p_data[0] != 0)
        {
            sprintf(trace, "Trying to add device to the list");
            Log(trace);
            if ((device = FindInList(bda,ui->cbBLEDeviceList)) == NULL)
                device = AddDeviceToList(bda, ui->cbBLEDeviceList, NULL);
            device->m_battc_handle = handle;
            device->m_conn_type |= CONNECTION_TYPE_BATTC;
            BattCDeviceAdd(bda);
            sprintf(trace, "BATTC Add Device address %02x:%02x:%02x:%02x:%02x:%02x",
                bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
            Log(trace);
            SendWicedCommand(HCI_CONTROL_BATT_CLIENT_COMMAND_ADD, &p_data[1], BDA_LEN);
        }
        SelectDevice(ui->cbBLEDeviceList, bda);
        break;

    case HCI_CONTROL_BATT_CLIENT_EVENT_DISCONNECTED:
        handle = p_data[0] | (p_data[1] << 8);
        sprintf(trace, "BATTC Connection Closed handle:%d reason:%d ", handle, p_data[2]);
        CBtDevice * pDev;

        pDev = FindInList(CONNECTION_TYPE_BATTC, handle, ui->cbBLEDeviceList);

        if (pDev && (pDev->m_battc_handle == handle))
        {
            pDev->m_battc_handle = NULL_HANDLE;
            pDev->m_conn_type &= ~CONNECTION_TYPE_BATTC;
        }
        Log(trace);
        break;

    case HCI_CONTROL_BATT_CLIENT_EVENT_LEVEL:
        sprintf(trace, "BATTC handle:%d status:%d level:%d", p_data[0] + (p_data[1] << 8), p_data[2], p_data[3]);
        Log(trace);
        break;

    case HCI_CONTROL_BATT_CLIENT_EVENT_STATUS:
        //sprintf(trace, "BATTC Cmd Status:%d ", p_data[0]);
        //Log(trace);
        break;

    default:
        sprintf(trace, "Rcvd Unknown BATTC OpCode: %d", (int)opcode);
        Log(trace);
        break;
    }
}

// Get selected device from BR/EDR combo box
CBtDevice* MainWindow::GetConnectedBATTCDevice(void)
{
    CBtDevice * pDev = (CBtDevice *)GetSelectedLEDevice();
    if (pDev == NULL)
    {
        Log("No device selected");
        return NULL;
    }

    if(pDev->m_battc_handle == NULL_HANDLE)
    {
        Log("Selected device is not connected as BATTC");
        return NULL;
    }

    return pDev;
}
