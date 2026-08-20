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
 * Demo applications.
 */

#include "app_include.h"


// Initialize app
void MainWindow::InitDemo()
{

}


// Handle WICED HCI events
void MainWindow::onHandleWicedEventDemo(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    switch (HCI_CONTROL_GROUP(opcode))
    {
    case HCI_CONTROL_GROUP_DEMO:
        HandleDemoEvents(opcode, p_data, len);
        break;
    }
}

// Handle WICED HCI events for demo
void MainWindow::HandleDemoEvents(DWORD opcode, LPBYTE p_data, DWORD len)
{
    BYTE   ssid[30];
    BYTE   password[30];
    memset(ssid, 0, 30);
    memset(password, 0, 30);
    uint8_t sssid_len;
    uint8_t passwd_len;


    switch (opcode)
    {
        case HCI_CONTROL_DEMO_EVENT_SSID_PASSWD:
        {
            /* Buffer format: ssid len, password len, SSID, password */

            if(len < 2)
            {
                Log("HandleDemoEvents, invalid data, packet size %ld", len);
                break;
            }
            sssid_len = p_data[0];
            passwd_len = p_data[1];

            if(len < (sssid_len + passwd_len))
            {
                Log("HandleDemoEvents, invalid data, SSID len %d, passd len %d, total packet size %ld",
                    sssid_len, passwd_len, len);
                break;
            }

            Log("HandleDemoEvents, SSID len %d, passd len %d, total packet size %ld",
                sssid_len, passwd_len, len);

            if((sssid_len > 30) || (passwd_len > 30))
            {
                Log("HandleDemoEvents, invalid data, SSID len %d, passd len %d",
                    sssid_len, passwd_len);
                break;
            }

            memcpy(ssid, &p_data[2], sssid_len);
            memcpy(password, &p_data[sssid_len+2], passwd_len);

            Log("HandleDemoEvents, SSID %s, password %s", ssid, password);

            ui->labelSSID->setText((char *)ssid);
            ui->labelPassword->setText((char *)password);

            ConnectWiFi(ssid, password);

        }
        break;
    }
}

static void addDelay()
{
    QThread::sleep(5); // Sleep for 5 seconds
}

void MainWindow::ConnectWiFi(BYTE *ssid, BYTE *password)
{
    char command[100];
    int result = 0;

    Log("ConnectWiFi");

    sprintf(command, "wl mpc 0");
    result = system(command);
    Log("%s res %d", command, result);

    addDelay();

    sprintf(command, "wl PM 0");
    result = system(command);
    Log("%s res %d", command, result);

    addDelay();

    sprintf(command, "wl down");
    result = system(command);
    Log("%s res %d", command, result);

    addDelay();

    sprintf(command, "wl wsec 4");
    result = system(command);
    Log("%s res %d", command, result);

    addDelay();

    sprintf(command, "wl wpa_auth 0x80");
    result = system(command);
    Log("%s res %d", command, result);

    addDelay();

    sprintf(command, "wl sup_wpa 1");
    result = system(command);
    Log("%s res %d", command, result);

    addDelay();

    sprintf(command, "wl set_pmk %s", password);
    result = system(command);
    Log("%s res %d", command, result);

    addDelay();

    sprintf(command, "wl up");
    result = system(command);
    Log("%s res %d", command, result);

    addDelay();

    sprintf(command, "wl join %s imode bss amode wpa2psk", ssid);
    result = system(command);
    Log("%s res %d", command, result);

    addDelay();
}

