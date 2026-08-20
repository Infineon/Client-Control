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
 * Sample MCU application for Bluetooth Serial Over Gatt (BSG) using WICED HCI protocol.
 * BSG is Cypress proprietary protocol. Local and peer side device needs to implement this protocol.
 */


#include "app_include.h"

Q_DECLARE_METATYPE( CBtDevice* )


#define WICED_BT_RFCOMM_SUCCESS 0
#define WICED_BT_RFCOMM_NO_MEM  5

extern MainWindow *g_pMainWindow;

// Bluetooth Serial over GATT

// Initialize app
void MainWindow::InitBSG()
{
    m_bsg_bytes_sent = 0;
    m_bsg_total_to_send = 0;
    m_bsg_tx_complete_result = 0;
    m_bsg_receive_file = NULL;
    g_pMainWindow = this;
}

// send data/file to peer device
void MainWindow::on_btnBSGSend_clicked()
{
    char buf[1030] = { 0 };
    CBtDevice * pDev = GetConnectedBSGDevice();
    if (pDev == NULL)
        return;
    USHORT nHandle = pDev->m_bsg_handle;

    buf[0] = nHandle & 0xff;
    buf[1] = (nHandle >> 8) & 0xff;

    // Send data from text box
    if (!ui->cbBSGSendFile->isChecked())
    {
        QString str = ui->lineEditBSGSend->text();
        strncpy(&buf[2], str.toStdString().c_str(), sizeof(buf) - 2);
        m_bsg_total_to_send = 0;
        m_bsg_bytes_sent = 0;
        SendWicedCommand(HCI_CONTROL_BSG_COMMAND_DATA, (LPBYTE)buf, 2 + strlen(&buf[2]));
    }
    // send data from file. create a thread and send file data
    else
    {
        ui->btnBSGSend->setDisabled(true);

        QThread* thread = new QThread;
        Worker* worker = new Worker();
        worker->moveToThread(thread);

        connect(thread, SIGNAL(started()), worker, SLOT(process_bsg()));
        connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
        connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        connect(worker, SIGNAL(finished()), this, SLOT(on_cbBSGThreadComplete()));

        thread->start();
    }
}

// Open a file to send data
void MainWindow::on_btnBSGBrowseSend_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open File"),"","");

    ui->lineEditBSGSendFile->setText(file);
}

// open a file to receive data
void MainWindow::on_btnBSGBrowseReceive_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Save File"),"","");

    ui->lineEditBSGReceiveFile->setText(file);

    on_cbBSGReceiveFile_clicked(true);
}

// send data from file instead of text box
void MainWindow::on_cbBSGSendFile_clicked(bool checked)
{
    UNUSED(checked);
}

// receive data to file instead of text box
void MainWindow::on_cbBSGReceiveFile_clicked(bool checked)
{
    UNUSED(checked);
    if (m_bsg_receive_file){
        fclose(m_bsg_receive_file);
        m_bsg_receive_file = NULL;
    }

    if (ui->cbBSGReceiveFile->isChecked())
    {
        QString strfile = ui->lineEditBSGReceiveFile->text();


        m_bsg_receive_file = fopen(strfile.toStdString().c_str(), "wb");

        if (!m_bsg_receive_file){
            Log("Error: could not open bsg receive file %s \n", strfile.toStdString().c_str());
        }
        else{
            Log("Opened bsg receive file %s \n", strfile.toStdString().c_str());
        }
    }
    else
    {
        if (m_bsg_receive_file){
            fclose(m_bsg_receive_file);
            m_bsg_receive_file = NULL;
        }
    }
}

// Get selected device from BLE combo box
CBtDevice* MainWindow::GetConnectedBSGDevice()
{
    CBtDevice * pDev = GetSelectedLEDevice();
    if (pDev == NULL)
    {
        Log("No device selected");
        return NULL;
    }

    /* currently there is no BSG handle
    if(pDev->m_bsg_handle == NULL_HANDLE)
    {
        Log("Selected device is not connected as BSG");
        return NULL;
    }
    */

    return pDev;
}

// Handle WICED HCI events
void MainWindow::onHandleWicedEventBSG(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    switch (HCI_CONTROL_GROUP(opcode))
    {

    case HCI_CONTROL_GROUP_BSG:
        HandleBSGEvents(opcode, p_data, len);
        break;
    }
}

// Handle WICED HCI events for BSG
void MainWindow::HandleBSGEvents(DWORD opcode, LPBYTE p_data, DWORD len)
{
    CHAR    trace[1024];
    uint16_t handle;

    switch (opcode)
    {
    // connected with peer
    case HCI_CONTROL_BSG_EVENT_CONNECTED:
        Log("BSG Connected address %02x:%02x:%02x:%02x:%02x:%02x handle:%d",
            p_data[5], p_data[4], p_data[3], p_data[2], p_data[1], p_data[0], p_data[6] + (p_data[7] << 8));
        break;

        // diconnected from peer
    case HCI_CONTROL_BSG_EVENT_DISCONNECTED:
        Log("BSG Disconnected handle:%d", p_data[0] + (p_data[1] << 8));
        break;

        // send data complete
    case HCI_CONTROL_BSG_EVENT_TX_COMPLETE:
        handle = p_data[0] | (p_data[1] << 8);
        m_bsg_tx_complete_result = p_data[2];

        if (!ui->cbBSGSendFile->isChecked())
        {
            sprintf(trace, "BSG tx complete handle:%d result:%d", handle, m_bsg_tx_complete_result);
            Log(trace);
        }
        else
        {
            qDebug("BSG tx complete handle:%u result:%u %lu of %lu",
                handle, m_bsg_tx_complete_result, m_bsg_bytes_sent, m_bsg_total_to_send);
        }
        bsg_tx_wait.wakeAll();

        break;

        // receive data complete
    case HCI_CONTROL_BSG_EVENT_RX_DATA:
    {
        handle = p_data[0] | (p_data[1] << 8);

        qDebug("BSG data rx handle:%u len:%lu", handle, len - 2);

        // NOTE: Since this method is called directly from read thread
        // there should be no UI manipulation in this if block below or code above in this case statement.
        // See dm.cpp - Worker::read_serial_port_thread()
        if (m_bsg_receive_file)
        {
            fwrite((char*)&p_data[2], len - 2, 1, m_bsg_receive_file);
        }
        else
        {
            char data[512];
            memset(data, 0, sizeof(data));

            for (DWORD i = 0; (i < len - 2) && (i < 100); i++)
                snprintf(&data[strlen(data)], sizeof(data) / sizeof(char) - strlen(data), "%02x ", p_data[2+i]);

            ui->lineEditBSGreceive->setText(data);
        }
    }
    break;

    default:
        Log("Unknown BSG event:%ld", opcode);
        break;
    }
}


// Send data from a file using a thread
DWORD MainWindow::SendFileThreadBSG()
{
    FILE *fp = NULL;
    char buf[1030] = { 0 };
    QString strfile = ui->lineEditBSGSendFile->text();

    fp = fopen(strfile.toStdString().c_str(), "rb");
    if (!fp)
    {
        Log("Failed to open file %s", strfile.toStdString().c_str());

        return 0;
    }

    CBtDevice * pDev = GetConnectedBSGDevice();
    if (pDev == NULL)
    {
        fclose(fp);
        return 0;
    }

    int nHandle = pDev->m_bsg_handle;
    buf[0] = nHandle & 0xff;
    buf[1] = (nHandle >> 8) & 0xff;

    fseek(fp, 0, SEEK_END);
    m_bsg_total_to_send = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    m_bsg_bytes_sent = 0;

    int read_bytes;
    QMutex mutex;

     m_uart_tx_size = m_settings.value("UartTxSize",512).toInt();

     Log("UartTxSize set to %ld", m_uart_tx_size);


    while ((read_bytes = fread(&buf[2], 1, m_uart_tx_size, fp)) != 0)
    {
        mutex.lock();

        SendWicedCommand(HCI_CONTROL_BSG_COMMAND_DATA, (LPBYTE)buf, 2 + read_bytes);
        m_bsg_bytes_sent += read_bytes;

        if(bsg_tx_wait.wait(&mutex, 5000) == false)
        {
            Log("Wait failed");
            //break;
        }


        if ((m_bsg_tx_complete_result != WICED_BT_RFCOMM_SUCCESS) ||
            (!ui->cbBSGSendFile->isChecked()))
        {
            m_bsg_total_to_send = 0;
            mutex.unlock();
            break;
        }
        mutex.unlock();
    }
    fclose(fp);

    return 0;
}

void MainWindow::on_cbBSGThreadComplete()
{
    Log("on_cbBSGThreadComplete");
    ui->btnBSGSend->setDisabled(false);
}


/* bsg thread */
void Worker::process_bsg()
{
    g_pMainWindow->SendFileThreadBSG();
    emit finished();
}
