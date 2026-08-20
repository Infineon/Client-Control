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
 * Sample MCU application for testing.
 */
#include "app_include.h"
extern "C"
{
#include "app_host.h"
}

extern MainWindow *g_pMainWindow;

void MainWindow::InitHciLoopbackTest()
{
    g_pMainWindow = this;
    m_hci_test_is_active = false;
}

void MainWindow::StartHciLoopbackTest()
{
    //Log("StartHciLoopbackTest");
    ui->btnTest->setText("Stop");
    m_hci_test_is_active = true;
    HciLoopbackSendCmd(1, NULL, 0);
}

void MainWindow::StopHciLoopbackTest()
{
    //Log("StopHciLoopbackTest");
    ui->btnTest->setText("Start");
    m_hci_test_is_active = false;
    HciLoopbackSendCmd(2, NULL, 0);
}

void MainWindow::ConfigHciLoopbackTest()
{
    unsigned char cfg[4] = { 0, 0, 0, 0 };
    cfg[0] = ui->comboBox_test_packet_size->currentIndex();
    cfg[1] = ui->comboBox_test_packet_pattern->currentIndex();
    cfg[2] = ui->comboBox_test_packets_in_loop->currentIndex();
    cfg[3] = ui->comboBox_test_error_response->currentIndex();
    Log("ConfigHciLoopbackTest payload size %d pattern: %d packets: %d error response: %d", cfg[0], cfg[1], cfg[2], cfg[3]);
    SendWicedCommand(HCI_CONTROL_HCITEST_CONFIGURE, cfg, 4);
}

void MainWindow::on_btnTest_clicked()
{
    //Log("on_btnTest_clicked");
    if(m_hci_test_is_active)
    {
        StopHciLoopbackTest();
    }
    else
    {
        ConfigHciLoopbackTest();
        StartHciLoopbackTest();
    }
}

void MainWindow::HciLoopbackSendCmd(UINT8 cmd, void * p_data, unsigned int len)
{
    UINT8 buf[1030];

    buf[0] = cmd;
    if(p_data && len)
    {
        memcpy(&buf[1], p_data, len);
    }
    SendWicedCommand(HCI_CONTROL_HCITEST_COMMAND, buf, len + 1);
}


void MainWindow::onHandleWicedEventHciLoopback(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    if(m_hci_test_is_active)
    {
        if(opcode == HCI_CONTROL_HCITEST_EVENT_PACKET)
        {
            // return packet
            HciLoopbackSendCmd(3, p_data, len);
        }
        else if(opcode != HCI_CONTROL_EVENT_WICED_TRACE)
        {
            Log("onHandleWicedEventHciLoopback opcode %x len %d", opcode, len);
        }
    }
}

