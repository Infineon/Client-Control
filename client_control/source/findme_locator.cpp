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
 * Sample MCU application for BLE FindMe Locator (FINDMEL) profile using WICED HCI protocol.
 */


#include "app_include.h"

// Initialize app
void MainWindow::InitFINDMEL(void)
{
    ui->cbFINDMELLevel->addItem("No Alert", 0);
    ui->cbFINDMELLevel->addItem("Mild Alert", 0);
    ui->cbFINDMELLevel->addItem("High Alert", 0);
    ui->cbFINDMELLevel->setCurrentIndex(0);
}

// connect to peer deivce
void MainWindow::on_btnFINDMELConnect_clicked(void)
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

    if(pDev->m_findmel_handle != NULL_HANDLE)
    {
        Log("FINDMEL already connected for selected device");
        return;
    }

    for (int i = 0; i < 6; i++)
        cmd[commandBytes++] = pDev->m_address[5 - i];

    Log("FINDMEL Connect Command BDA: %02x:%02x:%02x:%02x:%02x:%02x",
           pDev->m_address[0], pDev->m_address[1], pDev->m_address[2], pDev->m_address[3], pDev->m_address[4], pDev->m_address[5]);

    SendWicedCommand(HCI_CONTROL_FINDME_LOCATOR_COMMAND_CONNECT, cmd, 6);
}

// disconnect from peer deivce
void MainWindow::on_btnFINDMELDisconnect_clicked(void)
{
    BYTE   cmd[2];
    int    commandBytes = 0;
    USHORT nHandle;

    CBtDevice * pDev = GetConnectedFINDMELDevice();
    if (pDev == NULL)
        return;

    nHandle = pDev->m_findmel_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    Log("FINDMEL Disconnect Command, Handle: %d", nHandle);
    SendWicedCommand(HCI_CONTROL_FINDME_LOCATOR_COMMAND_DISCONNECT, cmd, commandBytes);

    pDev->m_findmel_handle = NULL_HANDLE;
    pDev->m_conn_type &= ~CONNECTION_TYPE_FINDMEL;
}

void MainWindow::on_cbFINDMELLevel_currentIndexChanged(int index)
{
    BYTE   cmd[3];
    int    commandBytes = 0;
    USHORT nHandle;

    if (m_CommPort == NULL)
        return;

    if (!m_bPortOpen)
    {
        return;
    }

    CBtDevice * pDev = GetConnectedFINDMELDevice();
    if (pDev == NULL)
        return;

    nHandle = pDev->m_findmel_handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;
    cmd[commandBytes++] = index;

    Log("FINDMEL Command Handle:%d Level:%d", nHandle, index);
    SendWicedCommand(HCI_CONTROL_FINDME_LOCATOR_COMMAND_WRITE, cmd, commandBytes);
}

// Handle WICED HCI events
void MainWindow::onHandleWicedEventFINDMEL(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    switch (HCI_CONTROL_GROUP(opcode))
    {
    case HCI_CONTROL_GROUP_FINDME_LOCATOR:
        HandleFINDMELHEvents(opcode, p_data, len);
        break;
    }
}

// Handle WICED HCI events for FINDMEL
void MainWindow::HandleFINDMELHEvents(DWORD opcode, LPBYTE p_data, DWORD len)
{
    char      trace[1024];
    BYTE       bda[6];
    CBtDevice *device;
    UINT16  handle;

    UNUSED(len);

    switch (opcode)
    {
    case HCI_CONTROL_FINDME_LOCATOR_EVENT_CONNECTED:
        for (int i = 0; i < 6; i++)
            bda[5 - i] = p_data[i + 1];
        sprintf(trace, "FINDMEL Connected status:%d address %02x:%02x:%02x:%02x:%02x:%02x handle:%d",
            p_data[0], bda[0], bda[1], bda[2], bda[3], bda[4], bda[5], p_data[7] + (p_data[8] << 8));
        Log(trace);
        handle = p_data[7] + (p_data[8] << 8);
        if (p_data[0] == 0)
        {
            if ((device = FindInList(bda,ui->cbBLEDeviceList)) == NULL)
                device = AddDeviceToList(bda, ui->cbBLEDeviceList, NULL);
            device->m_findmel_handle = handle;
            device->m_conn_type |= CONNECTION_TYPE_FINDMEL;
            FindMeLocatorDeviceAdd(bda);
            sprintf(trace, "FINDMEL Add Device address %02x:%02x:%02x:%02x:%02x:%02x",
                bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
            Log(trace);
            SendWicedCommand(HCI_CONTROL_FINDME_LOCATOR_COMMAND_ADD, &p_data[1], BDA_LEN);
        }
        SelectDevice(ui->cbBLEDeviceList, bda);
        break;

    case HCI_CONTROL_FINDME_LOCATOR_EVENT_DISCONNECTED:
        handle = p_data[0] | (p_data[1] << 8);
        sprintf(trace, "FINDMEL Connection Closed handle:%d reason:%d ", handle, p_data[2]);
        CBtDevice * pDev;

        pDev = FindInList(CONNECTION_TYPE_FINDMEL, handle, ui->cbBLEDeviceList);

        if (pDev && (pDev->m_findmel_handle == handle))
        {
            pDev->m_findmel_handle = NULL_HANDLE;
            pDev->m_conn_type &= ~CONNECTION_TYPE_FINDMEL;
        }
        Log(trace);
        break;

    case HCI_CONTROL_FINDME_LOCATOR_EVENT_STATUS:
        sprintf(trace, "FINDMEL Cmd Status:%d ", p_data[0]);
        Log(trace);
        break;

    default:
        sprintf(trace, "Rcvd Unknown FINDMEL OpCode: %d", (int)opcode);
        Log(trace);
        break;
    }
}

// Get selected device from BR/EDR combo box
CBtDevice* MainWindow::GetConnectedFINDMELDevice(void)
{
    CBtDevice * pDev = (CBtDevice *)GetSelectedLEDevice();
    if (pDev == NULL)
    {
        Log("No device selected");
        return NULL;
    }

    if(pDev->m_findmel_handle == NULL_HANDLE)
    {
        Log("Selected device is not connected as FINDMEL");
        return NULL;
    }

    return pDev;
}
