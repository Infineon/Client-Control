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
 * Serial port read/write
 */

#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <stdio.h>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

// Serial port class for read/read to serial port.
// This class wraps QSerialPort for Linux
// and OS specific implementation for Windows and MAC
// as built in QT class has limitations
class WicedSerialPort
{
public:
    WicedSerialPort (bool hostmode = false);
    virtual ~WicedSerialPort ();

    virtual qint64 read(char *data, qint64 maxlen);
    virtual qint64 readline(char *data, qint64 maxlen);

    virtual qint64 write(char *data, qint64 len);

    virtual bool open(const char *str_port_name, qint32 baudRate, bool bFlowControl);
    virtual void close();

    virtual void indicate_close();

    virtual bool isOpen() const;

    virtual void flush();

    virtual bool waitForBytesWritten(int iMilisec);
    virtual int errorNum();
    virtual void handleReadyRead();
};

class WicedSerialPortHostmode : public WicedSerialPort
{
public:
WicedSerialPortHostmode (QString str_cmd_ip_addr , int iSpyInstance);
    ~WicedSerialPortHostmode (){}


    virtual qint64 read(char *data, qint64 maxlen);
    virtual qint64 write(char *data, qint64 len);

    virtual bool open(const char *str_port_name, qint32 baudRate, bool bFlowControl);
    virtual void close();

    virtual bool isOpen() const;

    virtual void flush();

    virtual bool waitForBytesWritten(int iMilisec);
    virtual int errorNum();
    virtual void handleReadyRead();

    int m_ClientSocket;
    bool OpenSocket();
    QString str_ip_addr;
    int SpyInstance;
};

#endif // SERIAL_PORT_H
