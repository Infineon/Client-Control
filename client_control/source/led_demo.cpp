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
 * Sample MCU application for LED DEMO using WICED HCI protocol.
 */


#include "app_include.h"

// Initialize app
void MainWindow::InitLED_Demo()
{
    m_led_brightness_level = 0;
    ui->horizontalSliderLEDbrightness->setRange(0, 100);
    ui->horizontalSliderLEDbrightness->setSliderPosition(0);
}

// Slider moved for LED brightness setting
void MainWindow::on_horizontalSliderLEDbrightness_sliderMoved(int position)
{
    if (m_led_brightness_level != position)
    {
        BYTE    cmd[10];
        int     commandBytes = 0;
        m_led_brightness_level = position;

        cmd[commandBytes++] = m_led_brightness_level;
        SendWicedCommand(HCI_CONTROL_LED_COMMAND_SET_BRIGHTNESS, cmd, commandBytes);
        Log("LED Brightness Level : %d ", m_led_brightness_level);
        if (m_led_brightness_level == 0)
        {
            ui->pushButtonLEDONOFF->setText("Turn ON");
        }
        else if (m_led_brightness_level == 100)
        {
            ui->pushButtonLEDONOFF->setText("Turn OFF");
        }
    }
}

void MainWindow::on_pushButtonLEDONOFF_clicked()
{
    UINT16 commnd;
    if (m_led_brightness_level == 0)
    {
        m_led_brightness_level = 100;
        commnd = HCI_CONTROL_LED_COMMAND_TURN_ON;
        ui->pushButtonLEDONOFF->setText("Turn OFF");
        ui->horizontalSliderLEDbrightness->setSliderPosition(100);
        Log("Send LED Turn ON command ");
    }
    else
    {
        m_led_brightness_level = 0;
        commnd = HCI_CONTROL_LED_COMMAND_TURN_OFF;
        ui->pushButtonLEDONOFF->setText("TURN ON");
        ui->horizontalSliderLEDbrightness->setSliderPosition(0);
        Log("Send LED Turn OFF command ");
    }
    SendWicedCommand(commnd, NULL, 0);
}
