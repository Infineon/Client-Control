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
 * Sample MCU application for downloading firmware .hcd file on embedded device using WICED HCI protocol.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hci_control_api.h"
#include "wiced_types.h"
#include <QTimer>

QT_USE_NAMESPACE

Q_DECLARE_METATYPE( CBtDevice* )

/* Command definitions for the OTA FW upgrade */
#define WICED_OTA_UPGRADE_COMMAND_PREPARE_DOWNLOAD               1
#define WICED_OTA_UPGRADE_COMMAND_DOWNLOAD                       2
#define WICED_OTA_UPGRADE_COMMAND_VERIFY                         3
#define WICED_OTA_UPGRADE_COMMAND_FINISH                         4
#define WICED_OTA_UPGRADE_COMMAND_GET_STATUS                     5 /* Not currently used */
#define WICED_OTA_UPGRADE_COMMAND_CLEAR_STATUS                   6 /* Not currently used */
#define WICED_OTA_UPGRADE_COMMAND_ABORT                          7
/* Command definitions for the HCI FW upgrade */
#define WICED_HCI_UPGRADE_COMMAND_PREPARE_DOWNLOAD              WICED_OTA_UPGRADE_COMMAND_PREPARE_DOWNLOAD
#define WICED_HCI_UPGRADE_COMMAND_DOWNLOAD                      WICED_OTA_UPGRADE_COMMAND_DOWNLOAD
#define WICED_HCI_UPGRADE_COMMAND_VERIFY                        WICED_OTA_UPGRADE_COMMAND_VERIFY
#define WICED_HCI_UPGRADE_COMMAND_FINISH                        WICED_OTA_UPGRADE_COMMAND_FINISH
#define WICED_HCI_UPGRADE_COMMAND_GET_STATUS                    WICED_OTA_UPGRADE_COMMAND_GET_STATUS /* Not currently used */
#define WICED_HCI_UPGRADE_COMMAND_CLEAR_STATUS                  WICED_OTA_UPGRADE_COMMAND_CLEAR_STATUS /* Not currently used */
#define WICED_HCI_UPGRADE_COMMAND_ABORT                         WICED_OTA_UPGRADE_COMMAND_ABORT


bool MainWindow::FirmwareDownloadStart(QString filename)
{
    m_fpDownload = fopen(filename.toStdString().c_str(), "rb");
    if (m_fpDownload == NULL)
    {
        Log("Error opening file %s", filename.toStdString().c_str());
        return false;
    }
    fseek(m_fpDownload, 0, SEEK_END);
    m_nFirmwareSize = ftell(m_fpDownload);
    rewind(m_fpDownload);

    SendWicedCommand(HCI_CONTROL_DFU_COMMAND_READ_CONFIG, NULL, 0);

    m_pDownloadTimer = new QTimer(this);
    connect(m_pDownloadTimer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::FirmwareDownloadTimeout));
    m_pDownloadTimer->start(1000);

    return true;
}

void MainWindow::FirmwareDownloadStop()
{
    FirmwareDownloadSendCmd(WICED_HCI_UPGRADE_COMMAND_ABORT, NULL, 0);
}

void MainWindow::onHandleWicedEventHciDfu(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    switch (opcode)
    {
    case HCI_CONTROL_DFU_EVENT_CONFIG:
        m_pDownloadTimer->stop();
        if (len != 4)
            return;
        m_nDownloadSectorSize = p_data[0] + (p_data[1] << 8);
        FirmwareDownloadSendCmd(WICED_HCI_UPGRADE_COMMAND_PREPARE_DOWNLOAD, NULL, 0);
        break;
    case HCI_CONTROL_DFU_EVENT_STARTED:
        FirmwareDownloadSendCmd(WICED_HCI_UPGRADE_COMMAND_DOWNLOAD, &m_nFirmwareSize, sizeof(m_nFirmwareSize));

        m_nFirmwareSentSize = 0;
        m_nDownloadCRC = 0xFFFFFFFF;
        FirmwareDownloadSendData();
        break;
    case HCI_CONTROL_DFU_EVENT_DATA:
        onDlProgress(NULL, m_nFirmwareSentSize, m_nFirmwareSize);
        if (m_nFirmwareSentSize == m_nFirmwareSize)
        {
            m_nDownloadCRC = m_nDownloadCRC ^ 0xFFFFFFFF;
            FirmwareDownloadSendCmd(WICED_HCI_UPGRADE_COMMAND_VERIFY, &m_nDownloadCRC, sizeof(m_nDownloadCRC));
        }
        else
            FirmwareDownloadSendData();
        break;
    case HCI_CONTROL_DFU_EVENT_VERIFICATION:
        // Device is verifying firmware, do nothing
        break;
    case HCI_CONTROL_DFU_EVENT_VERIFIED:
        FirmwareDownloadCleanUp();
        onDlDone("Firmware download finished successfully.");
        break;
    case HCI_CONTROL_DFU_EVENT_ABORTED:
        FirmwareDownloadCleanUp();
        break;
    }
}

void MainWindow::FirmwareDownloadSendCmd(UINT8 cmd, void * p_data, int len)
{
    UINT8 buf[100];

    buf[0] = cmd;
    memcpy(&buf[1], p_data, len);
    SendWicedCommand(HCI_CONTROL_DFU_COMMAND_WRITE_COMMAND, buf, len + 1);
}

extern UINT32 update_crc(UINT32 crc, UINT8 *buf, UINT32 len);

void MainWindow::FirmwareDownloadSendData()
{
    UINT8 * p_buf;
    UINT32 bytes_read;

    p_buf = (UINT8 *)malloc(m_nDownloadSectorSize);
    if (p_buf == NULL)
        return;

    bytes_read = fread(p_buf, 1, m_nDownloadSectorSize, m_fpDownload);
    SendWicedCommand(HCI_CONTROL_DFU_COMMAND_WRITE_DATA, p_buf, bytes_read);
    m_nDownloadCRC = update_crc(m_nDownloadCRC, p_buf, bytes_read);
    m_nFirmwareSentSize += bytes_read;
    free(p_buf);
}

void MainWindow::FirmwareDownloadCleanUp()
{
    if (m_fpDownload)
    {
        fclose(m_fpDownload);
        m_fpDownload = NULL;
    }
    if (m_pDownloadTimer)
    {
        delete m_pDownloadTimer;
        m_pDownloadTimer = NULL;
    }
}

void MainWindow::FirmwareDownloadTimeout()
{
    FirmwareDownloadCleanUp();
    onDlDone("Device does not support HCI Firmware Download.");
}
