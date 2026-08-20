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
 * Sample MCU application for sending trace to BTSpy application on Windows OS.
 */


#include <QObject>
#include <WinSock2.h>
#include <QDebug>

static int wiced_trace_to_spy_trace[] = { 0, 4, 3, 6, 7 };

#ifdef Q_OS_WIN32
#include <WinSock2.h>

static SOCKET log_sock = INVALID_SOCKET;

void TraceHciPkt(BYTE type, BYTE *buffer, USHORT length, USHORT serial_port_index, int iSpyInstance)
{
    SOCKADDR_IN socket_addr;
    static int initialized = FALSE;
    BYTE buf[1100];
    USHORT *p = (USHORT*)buf;

    if (!initialized)
    {
        initialized = TRUE;

        WSADATA wsaData;
        int err = WSAStartup(MAKEWORD(2, 0), &wsaData);
        if (err != 0)
            return;
        log_sock = socket(AF_INET, SOCK_DGRAM, 0);

        if (log_sock == INVALID_SOCKET)
            return;

        memset(&socket_addr, 0, sizeof(socket_addr));
        socket_addr.sin_family = AF_INET;
        socket_addr.sin_addr.s_addr = ADDR_ANY;
        socket_addr.sin_port = 0;

        err = bind(log_sock, (SOCKADDR *)&socket_addr, sizeof(socket_addr));
        if (err != 0)
        {
            closesocket(log_sock);
            log_sock = INVALID_SOCKET;
            return;
        }
    }
    if (log_sock == INVALID_SOCKET)
        return;

    if (length > 1024)
        length = 1024;

    *p++ = wiced_trace_to_spy_trace[type];
    *p++ = length;
    *p++ = 0;
    *p++ = serial_port_index;
    memcpy(p, buffer, length);

    memset(&socket_addr, 0, sizeof(socket_addr));
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = ntohl(0x7f000001);
    socket_addr.sin_port = htons(9876 + iSpyInstance);

    if(sendto(log_sock, (const char *)buf, length + 8, 0, (SOCKADDR *)&socket_addr, sizeof(SOCKADDR_IN)) < 0)
    {
        qDebug("sendto socket failed");
    }
}
#else
void TraceHciPkt(BYTE type, BYTE *buffer, USHORT length, USHORT serial_port_index, int iSpyInstance)
{
}
#endif
