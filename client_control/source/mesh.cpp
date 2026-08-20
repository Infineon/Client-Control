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
 * Sample MCU application for implementing BLE mesh application using WICED HCI protocol.
 */

#include "app_include.h"

// Initialize app
void MainWindow::InitMesh()
{

}

// Gemeric ON OFF model, On clicked
void MainWindow::on_btnMeshOn_clicked()
{

}


// Gemeric ON OFF model, off clicked
void MainWindow::on_btnMeshOff_clicked()
{

}


// Gemeric level model, level changed
void MainWindow::on_ctrlMeshSlider_valueChanged(int value)
{

}

// Handle WICED HCI events
void MainWindow::onHandleWicedEventMesh(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    switch (HCI_CONTROL_GROUP(opcode))
    {
    case HCI_CONTROL_GROUP_MESH:
        HandleMeshEvents(opcode, p_data, len);
        break;
    }
}

// Handle WICED HCI events for Mesh
void MainWindow::HandleMeshEvents(DWORD opcode, LPBYTE p_data, DWORD len)
{
    char   trace[1024];
    CBtDevice *device;
    BYTE    bda[6];

    UINT16  handle, features;


    switch (opcode)
    {
        case HCI_CONTROL_MESH_EVENT_ONOFF_SET:
        {

        }
        break;
        case HCI_CONTROL_MESH_EVENT_LEVEL_SET:
        {
        }
        break;
    }
}
