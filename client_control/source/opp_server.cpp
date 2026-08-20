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
 * Sample MCU application for OPP server using WICED HCI protocol.
 */

#include "app_include.h"
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
//#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    WICED_BT_OP_ACCESS_ALLOW = 0,    /* Allow the requested operation */
    WICED_BT_OP_ACCESS_FORBID,       /* Disallow the requested operation */
    WICED_BT_OP_ACCESS_NONSUP        /* Requested operation is not supported */
};

enum
{
    WICED_BT_OP_OPER_PUSH = 1,    /* Allow the requested operation */
    WICED_BT_OP_OPER_PULL       /* Disallow the requested operation */
};

// Initialize app
void MainWindow::InitOPS()
{
    ui->progressBarOPS->setValue(0);
    ui->btnOPSDisconnect->setEnabled(false);
}

// Diconnect from peer
void MainWindow::on_btnOPSDisconnect_clicked()
{
    CBtDevice * pDev = GetConnectedOPSDevice();
    if (pDev == NULL)
        return;

    SendWicedCommand(HCI_CONTROL_OPS_COMMAND_DISCONNECT, NULL, 0);

    ui->labelOPSStatus->setText("Status : Abort");

    if (m_opp_receive_file)
    {
        fclose(m_opp_receive_file);
        m_opp_receive_file = NULL;
        received_size = 0;
    }

    pDev->m_conn_type &= ~CONNECTION_TYPE_OPS;
    ui->btnOPSDisconnect->setEnabled(false);
}

// Handle WICED HCI events
void MainWindow::onHandleWicedEventOPS(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    switch (HCI_CONTROL_GROUP(opcode))
    {

    case HCI_CONTROL_GROUP_OPS:
        HandleOPSEvents(opcode, p_data, len);
        break;
    }
}

// Handle WICED HCI events for OPP server
void MainWindow::HandleOPSEvents(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    uint8_t     bda[6];
    UINT8       status = 0;
    uint8_t     *p = p_data;

    switch (opcode)
    {
    case HCI_CONTROL_OPS_EVENT_CONNECTED:
    {
        CBtDevice   *device;

        if (status == 0)
        {
            STREAM_TO_BDADDR(bda, p);
            STREAM_TO_UINT8(status, p);

            Log("Rcvd HCI_CONTROL_OPS_EVENT_CONNECTED   BDA: %02x:%02x:%02x:%02x:%02x:%02x  Status: %d",
                bda[0], bda[1], bda[2], bda[3], bda[4], bda[5], status);

            // find device in the list with received address and save the connection handle
            if ((device = FindInList(bda,ui->cbDeviceList)) == NULL)
                device = AddDeviceToList(bda, ui->cbDeviceList, NULL);

            device->m_conn_type |= CONNECTION_TYPE_OPS;
            SelectDevice(ui->cbDeviceList, bda);
            ui->btnOPSDisconnect->setEnabled(true);
        }
    }
    break;

    case HCI_CONTROL_OPS_EVENT_PROGRESS:
    {
        uint32_t    obj_size;
        uint16_t    bytes;
        uint8_t     oper;

        STREAM_TO_UINT32(obj_size, p);
        STREAM_TO_UINT16(bytes, p);
        STREAM_TO_UINT8(oper, p);

        UNUSED(oper);

        received_size += bytes;

        ui->labelOPSStatus->setText("Status : Receiving");
        ui->progressBarOPS->setValue( (received_size*100) / obj_size );
    }
    break;

    case HCI_CONTROL_OPS_EVENT_OBJECT:
    {
        uint8_t     format;
        uint8_t     obj_name_len = 0;
        char        obj_name_str[255] = {0};

        STREAM_TO_UINT8(format, p);
        STREAM_TO_UINT8(obj_name_len, p);
        memcpy(obj_name_str, p, obj_name_len);

        ui->labelOPSStatus->setText("Status : Complete");
        UNUSED(obj_name_len);
        //UNUSED(obj_name_str);
        UNUSED(format);

        if (m_opp_receive_file)
        {
            fclose(m_opp_receive_file);
            m_opp_receive_file = NULL;
            received_size = 0;
        }
        QString strPath = QDir::tempPath();
        Log("File: (%s) was received and saved to directory: (%s) ", obj_name_str, strPath.toStdString().c_str());

    }
    break;

    case HCI_CONTROL_OPS_EVENT_CLOSE:
    {
        CBtDevice * pDev = GetConnectedOPSDevice();

        STREAM_TO_BDADDR(bda, p);
        Log("Rcvd HCI_CONTROL_OPS_EVENT_CLOSE   BDA: %02x:%02x:%02x:%02x:%02x:%02x",
            bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);

        if (pDev == NULL)
            return;
        pDev->m_conn_type &= ~CONNECTION_TYPE_OPS;
        ui->btnOPSDisconnect->setEnabled(false);
    }
    break;

    case HCI_CONTROL_OPS_EVENT_ACCESS:
    {
        uint8_t     oper;
        uint8_t     format;
        uint32_t    obj_size;
        uint8_t     obj_name_len = 0;
        char        obj_name_str[255] = {0};
        uint8_t     obj_type_len = 0;
        char        obj_type_str[255] = {0};

        BYTE        cmd[60];
        int         commandBytes = 0;

        STREAM_TO_BDADDR(bda, p);
        STREAM_TO_UINT8(oper, p);
        STREAM_TO_UINT8(format, p);
        STREAM_TO_UINT32(obj_size, p);
        STREAM_TO_UINT8(obj_name_len, p);
        memcpy(obj_name_str, p, obj_name_len);
        p = p + obj_name_len;
        STREAM_TO_UINT8(obj_type_len, p);
        memcpy(obj_type_str, p, obj_type_len);

        QString strPath = QDir::tempPath();


        ui->labelOPSName->setText("File Name : " + QString(obj_name_str) + " (" + strPath + ") ");
        ui->labelOPSType->setText("Type : " + QString(obj_type_str));
        ui->labelOPSSize->setText("Size : " + QString::number(obj_size) +" bytes");

        QString strFilePath = strPath + "/" + obj_name_str;

        m_opp_receive_file = fopen(strFilePath.toStdString().c_str(), "wb");
        if (!m_opp_receive_file) {
            Log("Error: could not open OPP receive file %s \n", strFilePath.toStdString().c_str());
        }
        received_size = 0;

        if ( oper == WICED_BT_OP_OPER_PUSH )
        {
            /* send acccess response as allow */
            cmd[commandBytes++] = oper;
            cmd[commandBytes++] = WICED_BT_OP_ACCESS_ALLOW;
            SendWicedCommand(HCI_CONTROL_OPS_COMMAND_ACCESS_RSP, cmd, commandBytes);
        }
        else
        {
            /* send acccess response as disallow not to support OPP pull*/
            cmd[commandBytes++] = oper;
            cmd[commandBytes++] = WICED_BT_OP_ACCESS_FORBID;
            SendWicedCommand(HCI_CONTROL_OPS_COMMAND_ACCESS_RSP, cmd, commandBytes);
        }
        UNUSED(format);
    }
    break;

    case HCI_CONTROL_OPS_EVENT_PUSH_DATA:
        if (m_opp_receive_file)
        {
            fwrite(p_data, len , 1, m_opp_receive_file);
        }
        break;

    default:
        /* Unhandled */
        Log("Rcvd unknown event:0x%X", opcode);
        break;
    }
}

// Get selected device from BR/EDR combo box
CBtDevice* MainWindow::GetConnectedOPSDevice()
{
    CBtDevice * pDev = GetSelectedDevice();
    if (pDev == NULL)
    {
        Log("No device selected");
        return NULL;
    }

    return pDev;
}

