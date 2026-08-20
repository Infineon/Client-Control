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

#ifndef HCI_LOOPBACK_H
#define HCI_LOOPBACK_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtSerialPort/QSerialPort>
#include "mainwindow.h"

#define WAIT_TIMEOUT    3000

class HciLoopbackThread : public QThread
{
    Q_OBJECT

public:
    HciLoopbackThread(QObject *parent = 0);
  //  ~HciLoopbackThread();
  //  void run();
    void loopback(MainWindow *pParent, QString &fn);
    void stop()
    {
        bStop = true;
    }
    bool getSuccess()
    {
        return success;
    }

signals:
    void lbProgress(QString* msg,int pkt_cnt, int byte_cnt);
    void lbDone(const QString &s);

private:
    bool SendRecvCmd(BYTE * txbuf,int txbuf_sz, BYTE * rxbuf, int rxbuf_sz,QSerialPort & serial, int ms_to=WAIT_TIMEOUT);
    bool send_miniport(QSerialPort & serial);
    void Log(const char *);
    void SetupSerialPort(QSerialPort &serial);

    MainWindow * parent;
    bool bStop;
    bool success;
    int nPktCnt, nByteCnt;
};

#endif // HCI_LOOPBACK_H
