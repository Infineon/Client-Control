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
 * Sample MCU application for sending trace to BTSpy application on Linux/Mac OS.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "wiced_types.h"
#include <QDebug>

#define INVALID_SOCKET  -1
typedef unsigned char BYTE;
typedef unsigned short USHORT;

static int log_sock = INVALID_SOCKET;
static int wiced_trace_to_spy_trace[] = { 0, 4, 3, 6, 7 };

void TraceHciPkt(BYTE type, BYTE *buffer, USHORT length, USHORT serial_port_index, int iSpyInstance)
{
    struct sockaddr_in socket_addr;
    BYTE buf[1100];
    USHORT *p = (USHORT*)buf;

    if (log_sock == -1)
    {
        log_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (log_sock == INVALID_SOCKET)
            return;

        memset(&socket_addr, 0, sizeof(socket_addr));
        socket_addr.sin_family = AF_INET;
        socket_addr.sin_addr.s_addr = 0;
        socket_addr.sin_port = 0;

        int err = bind(log_sock, (sockaddr *)&socket_addr, sizeof(socket_addr));
        if (err != 0)
        {
            close(log_sock);
            log_sock = INVALID_SOCKET;
            return;
        }
    }

    if (length > 1024)
        length = 1024;

    memset(&socket_addr, 0, sizeof(socket_addr));
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = ntohl(0x7f000001);
    socket_addr.sin_port = htons(9876 + iSpyInstance);

    if ((char)type == -1)
    {
        if(sendto(log_sock, (const char *)buffer, length, 0, (sockaddr *)&socket_addr, sizeof(socket_addr)) < 0)
        {
            qDebug("sendto socket failed");
        }
    }
    else
    {
        *p++ = wiced_trace_to_spy_trace[type];
        *p++ = length;
        *p++ = 0;
        *p++ = serial_port_index;
        memcpy(p, buffer, length);
        if(sendto(log_sock, (const char *)buf, length + 8, 0, (sockaddr *)&socket_addr, sizeof(socket_addr)) < 0)
        {
            qDebug("sendto socket failed");
        }
    }
}
