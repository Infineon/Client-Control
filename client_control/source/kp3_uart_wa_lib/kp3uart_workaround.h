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
 *  Workaround BTSDK-4891 -
 *  KP3_RTS (BT_UART_CTS) stays high when Clientcontrol com port is enabled
 *  Do addtional UART control flow handling, specific for the KP3 serial device on Windows.
 *  For FTDI based devices this call does nothing.
 */
#ifndef KP3UARTRTSWORKAROUND_H
#define KP3UARTRTSWORKAROUND_H
#include <QString>
#include <windows.h>

#ifndef Q_OS_WIN
#error This header can be included only for windows platform
#endif

namespace Cypress {
namespace KitProg3Sepcifics {

#define LOG_SERIAL_TRACE_TO_FILE 0
typedef enum _USB_UART_CHIP_TYPE {
    USB_UART_UNKNOWN = 0,
    USB_UART_KP3 = 1,
    USB_UART_RP2040 = 2,
} USB_UART_CHIP_TYPE;

//
// This module allows to handle
// the UART RTS/DTR lines of KitProg3 device in a specific way via 'usbser' driver on Windows.
//
// Workaround for
// 1. BTSDK-4891
// KP3_RTS (BT_UART_CTS) stays high when Clientcontrol com port is enabled
// 2. CYBLUETOOL-369
// KP3_RTS (BT_UART_CTS) stays high when Bluetool com port is enabled
//
class Kp3UartWorkaround
{
    Kp3UartWorkaround() = delete;
    static void Log(const char * fmt, ...);
    static void GetSerialDeviceDetailsUsingSetupAPI(const QString& portNameToFind, QString& driver_name_out, QString& hardware_id_out);
public:
    // This static method activates RTS/DTR control lines for KitProg3 UART device
    // via 'usbser' driver on Windows platform.
    // For FTDI based devices this call does nothing
    static void assertRtsDtrLinesForKP3OnWindows(const QString& com_port_name_or_path, HANDLE com_port_handle);
    static USB_UART_CHIP_TYPE getUsbUartChipTypeOnWindows(const QString& com_port_name_or_path);
    static void assertRtsDtrLinesOnWindows(USB_UART_CHIP_TYPE chip_type, HANDLE com_port_handle);
};

}
}

#endif // KP3UARTRTSWORKAROUND_H
