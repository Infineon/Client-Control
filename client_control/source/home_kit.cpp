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
 * Sample MCU application for implementing Apple HomeKit protocol using WICED HCI protocol.
 */

#include "app_include.h"
#include <QTimer>

const char* door_state[] = {
    "Open",
    "Closed",
    "Opening",
    "Closing",
    "Stopped"
};

const char* lock_state[] = {
    "Unsecured",
    "Secured",
    "Jammed",
    "Unknown"
};

const char* lock_target_state[] = {
    "Unsecured",
    "Secured"
};

#define HK_FMNA_BASE64_DEBUG 1

#define AFM_NVDATA_SIZE                             3000
#define AFM_FACTORY_KEYS_SIZE                       600
#define HCI_TRANSFER_MAX_DATA_SIZE                  600

#define FMNA_NVRAM_STATE_UNINITIALIZE               0
#define FMNA_NVRAM_STATE_INITIALIZED                1
#define FMNA_NVRAM_STATE_NEED_UPDATE                2
#define FMNA_NVRAM_STATE_UPDAET_COMPL               3

#define WICED_BT_FACTORY_CONFIG_ITEM_UUID                         0xE0
#define WICED_BT_FACTORY_CONFIG_ITEM_OOB_STATIC_DATA              0xE1
#define WICED_BT_FACTORY_CONFIG_ITEM_SW_AUTH_TOKEN                0xE2
#define WICED_BT_FACTORY_CONFIG_ITEM_PRIVATE_KEY                  0xE3
#define WICED_BT_FACTORY_CONFIG_ITEM_SERIAL_NUM                   0xE4
#define WICED_BT_FACTORY_CONFIG_ITEM_SW_AUTH_TOKEN_UUID           0xE5
#define WICED_BT_FACTORY_CONFIG_ITEM_FM_SVR_ENCRY_KEY             0xE6      // Find My server encryption key
#define WICED_BT_FACTORY_CONFIG_ITEM_FM_SIG_VERIF_KEY             0xE7      // Find My signature verification key
#define WICED_BT_FACTORY_CONFIG_ITEM_LAST                         0xEF

#define SOFTWARE_AUTH_TOKEN_BLEN                      1024
#define SOFTWARE_AUTH_UUID_BLEN                       16
#define APPLE_SERVER_ENCRYPTION_KEY_BLEN              65
#define APPLE_SERVER_SIG_VERIFICATION_KEY_BLEN        65

static unsigned char fmna_nvram_data[AFM_NVDATA_SIZE] = {0};
static unsigned char fmna_factory_data[AFM_FACTORY_KEYS_SIZE] = {0};

unsigned char software_token[1024];
char software_token_encode[1400];
unsigned char *p_token_buf;
int software_token_size;

#define HAP_CHARACTERISTIC_FORMAT_BOOL                      0x01
#define HAP_CHARACTERISTIC_FORMAT_UINT8                     0x04
#define HAP_CHARACTERISTIC_FORMAT_UINT16                    0x06
#define HAP_CHARACTERISTIC_FORMAT_UINT32                    0x08
#define HAP_CHARACTERISTIC_FORMAT_UINT64                    0x0a
#define HAP_CHARACTERISTIC_FORMAT_INT32                     0x10
#define HAP_CHARACTERISTIC_FORMAT_FLOAT                     0x14
#define HAP_CHARACTERISTIC_FORMAT_STRING                    0x19
#define HAP_CHARACTERISTIC_FORMAT_TLV8                      0x1b

const char* TypeString(BYTE type)
{
    QString str;
    switch (type)
    {
    case HAP_CHARACTERISTIC_FORMAT_BOOL:
        return "boolean";
    case HAP_CHARACTERISTIC_FORMAT_UINT8:
        return "uint8";
    case HAP_CHARACTERISTIC_FORMAT_UINT16:
        return "uint16";
    case HAP_CHARACTERISTIC_FORMAT_UINT32:
        return "uint32";
    case HAP_CHARACTERISTIC_FORMAT_UINT64:
        return "uint64";
    case HAP_CHARACTERISTIC_FORMAT_INT32:
        return "int32";
    case HAP_CHARACTERISTIC_FORMAT_FLOAT:
        return "float";
    case HAP_CHARACTERISTIC_FORMAT_STRING:
        return "string";
    case HAP_CHARACTERISTIC_FORMAT_TLV8:
        return "tlv8";
    default:
        return "unknown";
    }
}

#define FMNA_FILE_LINE_MAX 1024

#ifdef HK_FMNA_BASE64_DEBUG
static int clean_text_space(char *buf)
{
    int line_len = 0;

    if(buf)
    {
        line_len = strlen(buf);
        if ('\n' == buf[line_len - 1])
        {
            buf[line_len - 1] = '\0';
            line_len--;
            if (0 == line_len)
            {
                return 0;
            }
        }

        if ('\r' == buf[line_len - 1])
        {
            buf[line_len - 1] = '\0';
            line_len--;
            if (0 == line_len)
            {
                return 0;
            }
        }
    }

    return line_len;
}

static int read_effect_tokens_data(FILE *fp, char *buf)
{
    int buf_len = 0;

    if(buf && fp)
    {
        do
        {
            if(fgets(buf, FMNA_FILE_LINE_MAX, fp))
            {
                buf_len = clean_text_space(buf);
            }
            else
                buf_len = -1;
        }
        while(buf_len == 0);
    }

    return buf_len;
}

static int b64invs[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
    59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
    6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49, 50, 51 };

static size_t b64_decoded_size(const char *in)
{
    size_t len;
    size_t ret;
    size_t i;

    if (in == NULL)
        return 0;

    len = strlen(in);
    ret = len / 4 * 3;

    for (i=len; i-->0; ) {
        if (in[i] == '=') {
            ret--;
        } else {
            break;
        }
    }

    return ret;
}

static int b64_isvalidchar(char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;
    if (c >= 'a' && c <= 'z')
        return 1;
    if (c == '+' || c == '/' || c == '=')
        return 1;
    return 0;
}

static int b64_decode(const char *in, unsigned char *out, size_t outlen)
{
    size_t len;
    size_t i;
    size_t j;
    int    v;

    if (in == NULL || out == NULL)
        return 0;

    len = strlen(in);
    if (outlen < b64_decoded_size(in) || len % 4 != 0)
        return 0;

    for (i=0; i<len; i++) {
        if (!b64_isvalidchar(in[i])) {
            return 0;
        }
    }

    for (i=0, j=0; i<len; i+=4, j+=3) {
        v = b64invs[in[i]-43];
        v = (v << 6) | b64invs[in[i+1]-43];
        v = in[i+2]=='=' ? v << 6 : (v << 6) | b64invs[in[i+2]-43];
        v = in[i+3]=='=' ? v << 6 : (v << 6) | b64invs[in[i+3]-43];

        out[j] = (v >> 16) & 0xFF;
        if (in[i+2] != '=')
            out[j+1] = (v >> 8) & 0xFF;
        if (in[i+3] != '=')
            out[j+2] = v & 0xFF;
    }

    return 1;
}

static uint16_t fmna_nvram_factory_config_read(uint8_t item_type, uint8_t *buffer, uint16_t read_length, uint16_t read_offset, uint16_t *record_size)
{
    uint16_t i = 0;
    uint16_t copy_len = 0;

    *record_size = 0;
    while(i < sizeof(fmna_factory_data))
    {
        uint8_t type = fmna_factory_data[i];
        uint16_t length = (fmna_factory_data[i+2] << 8) | fmna_factory_data[i+1];

        if(type == 0)
            break;

        if(item_type == type)
        {
            if( read_offset + copy_len <= read_length )
            {
                *record_size = copy_len = length;
                memcpy(buffer + read_offset, fmna_factory_data + i + 3, copy_len);
            }
            break;
        }
        i += length + 3;
    }

    return copy_len;
}

static uint16_t fmna_nvram_factory_config_write(uint8_t item_type, uint8_t *buffer, uint16_t length)
{
    uint16_t i = 0;

    while(i < sizeof(fmna_factory_data))
    {
        uint8_t type = fmna_factory_data[i];
        uint16_t sec_length = (fmna_factory_data[i+2] << 8) | fmna_factory_data[i+1];

        if(type == 0)
            break;

        i += sec_length + 3;
    }

    if( buffer)
    {
        fmna_factory_data[i] = item_type;
        fmna_factory_data[i+2] = (length >>8) & 0xFF;
        fmna_factory_data[i+1] = length & 0xFF;

        memcpy(fmna_factory_data + i + 3, buffer, length);
        i += length + 3;
    }

    return i;
}

static void fmna_read_factory_config(uint8_t id, uint8_t *pFC, uint8_t length)
{
    uint16_t record_size;

    fmna_nvram_factory_config_read(id, pFC, length, 0, &record_size);
}

static void fmna_write_factory_config(uint8_t id, uint8_t *pFC, uint8_t length)
{
    fmna_nvram_factory_config_write(id, pFC, length);
}

static void fmna_connection_platform_get_serial_number(uint8_t *pSN, uint8_t length)
{
    fmna_read_factory_config(WICED_BT_FACTORY_CONFIG_ITEM_SERIAL_NUM, pSN, length);
}

static void fmna_connection_platform_set_serial_number(uint8_t *pSN, uint8_t length)
{
    fmna_write_factory_config(WICED_BT_FACTORY_CONFIG_ITEM_SERIAL_NUM, pSN, length);
}

static void fmna_connection_platform_get_token_uuid(uint8_t *pTU, uint8_t length)
{
    fmna_read_factory_config(WICED_BT_FACTORY_CONFIG_ITEM_SW_AUTH_TOKEN_UUID, pTU, length);
}

static void fmna_connection_platform_set_token_uuid(uint8_t *pTU, uint8_t length)
{
    fmna_write_factory_config(WICED_BT_FACTORY_CONFIG_ITEM_SW_AUTH_TOKEN_UUID, pTU, length);
}

static void fmna_connection_platform_get_token(uint8_t *pTK, uint16_t *pLen)
{
    uint16_t record_size;

    fmna_nvram_factory_config_read(WICED_BT_FACTORY_CONFIG_ITEM_SW_AUTH_TOKEN, pTK, *pLen, 0, &record_size);

    *pLen = record_size;
}

static void fmna_connection_platform_set_token(uint8_t *pTK, uint16_t Len)
{
    fmna_nvram_factory_config_write(WICED_BT_FACTORY_CONFIG_ITEM_SW_AUTH_TOKEN, pTK, Len);
}


static void fmna_connection_platform_get_server_encry_key(uint8_t *pSEK, uint8_t length)
{
    fmna_read_factory_config(WICED_BT_FACTORY_CONFIG_ITEM_FM_SVR_ENCRY_KEY, pSEK, length);
}

static void fmna_connection_platform_set_server_encry_key(uint8_t *pSEK, uint8_t length)
{
    fmna_write_factory_config(WICED_BT_FACTORY_CONFIG_ITEM_FM_SVR_ENCRY_KEY, pSEK, length);
}

static void fmna_connection_platform_get_signature_verif_key(uint8_t *pSVK, uint8_t length)
{
    fmna_read_factory_config(WICED_BT_FACTORY_CONFIG_ITEM_FM_SIG_VERIF_KEY, pSVK, length);
}

static void fmna_connection_platform_set_signature_verif_key(uint8_t *pSVK, uint8_t length)
{
    fmna_write_factory_config(WICED_BT_FACTORY_CONFIG_ITEM_FM_SIG_VERIF_KEY, pSVK, length);
}
#endif


// Initialize app
void MainWindow::InitHK()
{
    m_bLightOn = false;
    m_nLightBrightness = 0;
    m_nDoorState = 1;
    m_nLockState = 1;
    m_nLockTargetState = 1;
    m_nIdentifyTimerCounter = 0;

    SetLightOnOff(m_bLightOn);

    char strBrightness[20];
    sprintf(strBrightness, "%d", m_nLightBrightness);

    ui->lineEditHKBrightness->setText(strBrightness);

    for (int i = 0; i < 5; i++)
    {
        ui->cbDoorState->addItem(door_state[i]);
    }
    ui->cbDoorState->setCurrentIndex(m_nDoorState);

    for (int i = 0; i < 4; i++)
    {
        ui->cbLockState->addItem(lock_state[i]);
    }
    ui->cbLockState->setCurrentIndex(m_nLockState);

    for (int i = 0; i < 2; i++)
    {
        ui->cbLockTargetState->addItem(lock_target_state[i]);
    }
    ui->cbLockTargetState->setCurrentIndex(m_nLockTargetState);

    p_timer = new QTimer(this);

    connect(p_timer, SIGNAL(timeout()), this, SLOT(on_timer()));
}

void MainWindow::StartHK()
{
    on_btnHKList_clicked();
}

// Send read or write WICED HCI command
void MainWindow::SendHciCommand(UINT16 command, USHORT handle, LPBYTE p, DWORD dwLen)
{
    if (m_CommPort == NULL)
        return;

    BYTE buffer[32];
    char trace[1024];

    buffer[0] = handle & 0xff;
    buffer[1] = (handle >> 8) & 0xff;
    if (p)
        memcpy(&buffer[2], p, dwLen);

    SendWicedCommand(command, buffer, dwLen + 2);

    switch (command)
    {
    case HCI_CONTROL_HK_COMMAND_READ:
        sprintf(trace, "Read characteristic [%02x]", handle);
        Log(trace);
        break;
    case HCI_CONTROL_HK_COMMAND_WRITE:
        sprintf(trace, "Write characteristic [%02x] : ", handle);
        if(p)
        {
            for (int i = 0; i < (int)dwLen; i++)
                sprintf(&trace[strlen(trace)], "%02x", p[i]);
        }
        Log(trace);
        break;
    }
}

// read value
void MainWindow::on_btnHKRead_clicked()
{
    QString str = ui->lineEditHKHandle->text();
    USHORT handle = GetHandle(str);

    SendHciCommand(HCI_CONTROL_HK_COMMAND_READ, handle, NULL, 0);
}

// write value
void MainWindow::on_btnHKWrite_clicked()
{
    USHORT handle;
    BYTE  buffer[32];
    DWORD num_bytes;

    QString strHandle = ui->lineEditHKHandle->text();
    handle = GetHandle(strHandle);

    QString strHex = ui->lineEditHKHexVal->text();
    num_bytes = GetHexValue(buffer, sizeof(buffer), strHex);

    SendHciCommand(HCI_CONTROL_HK_COMMAND_WRITE, handle, buffer, num_bytes);
}

// List characteristics
void MainWindow::on_btnHKList_clicked()
{
    m_hIdentify = 0;
    m_hLightOn = 0;
    m_hLightBrightness = 0;
    m_hDoorState = 0;
    m_hLockState = 0;
    m_hLockTargetState = 0;

    SendWicedCommand(HCI_CONTROL_HK_COMMAND_LIST, NULL, 0);
    Log("List characteristics");
}

// Bulb on/off
void MainWindow::on_btnHKSwitch_clicked()
{
    m_bLightOn = !m_bLightOn;
    SetLightOnOff(m_bLightOn);
    SendHciCommand(HCI_CONTROL_HK_COMMAND_WRITE, m_hLightOn, (LPBYTE)&m_bLightOn, 1);
}

void MainWindow::ShowMessage()
{
    QMessageBox msgBox;
    msgBox.setText("Please enter a value between 0 and 100");
    msgBox.exec();
}

// Set brightness
void MainWindow::on_btnHKSet_clicked()
{
    QString str = ui->lineEditHKBrightness->text();
    if(str.length() == 0 || str.length() > 3)
    {
        ShowMessage();
        return;
    }

    for(int i = 0; i < str.length(); i++)
    {
        QChar c = str.at(i);
        if(!c.isDigit())
        {
            ShowMessage();
            return;
        }
    }

    uint brightness = ui->lineEditHKBrightness->text().toUInt();

    if (brightness > 100)
    {
        ShowMessage();
        return;
    }

    m_nLightBrightness = brightness;
    SendHciCommand(HCI_CONTROL_HK_COMMAND_WRITE, m_hLightBrightness,(LPBYTE)&m_nLightBrightness, 4);

}

// Door state set
void MainWindow::on_cbDoorState_currentIndexChanged(int index)
{
    if (m_CommPort == NULL)
        return;

    m_nDoorState = index;
    SendHciCommand(HCI_CONTROL_HK_COMMAND_WRITE, m_hDoorState, (LPBYTE)&m_nDoorState, 1);

}

// Lock state set
void MainWindow::on_cbLockState_currentIndexChanged(int index)
{
    m_nLockState = index;
    SendHciCommand(HCI_CONTROL_HK_COMMAND_WRITE, m_hLockState, (LPBYTE)&m_nLockState, 1);
}

void MainWindow::on_cbLockTargetState_currentIndexChanged(int index)
{
    m_nLockTargetState = index;
    SendHciCommand(HCI_CONTROL_HK_COMMAND_WRITE, m_hLockTargetState, (LPBYTE)&m_nLockTargetState, 1);
}


// Device factory reset
void MainWindow::on_btnHKFactoryReset_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Warning");
    msgBox.setText("Are you sure you want to factory reset?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        SendHciCommand(HCI_CONTROL_HK_COMMAND_FACTORY_RESET, 0, NULL, 0);
    }
}

// Get software authentication token from device
void MainWindow::on_btnHKGetToken_clicked()
{
    SendHciCommand(HCI_CONTROL_HK_COMMAND_GET_TOKEN, 0, NULL, 0);
}

void MainWindow::on_btnHKFMNAGetNvData_clicked()
{
    if (m_CommPort == NULL)
        return;

    BYTE buffer[32];
    BYTE *p_data = buffer;

    UINT16_TO_STREAM(p_data, 0);
    UINT16_TO_STREAM(p_data, AFM_NVDATA_SIZE);

    SendWicedCommand(HCI_CONTROL_HK_COMMAND_GET_FMNA_INFO, buffer, 4);
}

void MainWindow::on_btnHKFMNAGetFacData_clicked()
{
    if (m_CommPort == NULL)
        return;

    BYTE buffer[32];
    BYTE *p_data = buffer;

    UINT16_TO_STREAM(p_data, 0);
    UINT16_TO_STREAM(p_data, AFM_FACTORY_KEYS_SIZE);

    SendWicedCommand(HCI_CONTROL_HK_COMMAND_GET_FACTORY_DATA, buffer, 4);
}

void MainWindow::on_btnHKFMNASetNvData_clicked()
{
    FILE *fp = NULL;

    uint16_t file_size = 0;

    if (m_CommPort == NULL)
        return;

    fp = fopen("./fmna/fmna_nv_data.bin", "rb");

    if (fp)
    {
        file_size = fread(fmna_nvram_data, 1, sizeof(fmna_nvram_data), fp);

        Log("Update FMNA NVRAM Data length: %d", file_size);

        if(file_size > 0)
        {
            uint8_t section_buf[HCI_TRANSFER_MAX_DATA_SIZE + 4];
            uint8_t section_count =  file_size/HCI_TRANSFER_MAX_DATA_SIZE;

            if(file_size % HCI_TRANSFER_MAX_DATA_SIZE)
            {
                section_count++;
            }

            for(uint8_t i = 0;i < section_count;i++)
            {
                uint8_t *p_data = section_buf;
                uint16_t section_offset = i*HCI_TRANSFER_MAX_DATA_SIZE;
                uint16_t section_len = HCI_TRANSFER_MAX_DATA_SIZE;

                if(section_len + section_offset > file_size)
                    section_len = file_size - section_offset;

                UINT16_TO_STREAM(p_data, section_offset);
                if(i == section_count - 1)
                {
                    UINT16_TO_STREAM(p_data, (section_len | 0x8000));
                }
                else
                {
                    UINT16_TO_STREAM(p_data, section_len);
                }
                memcpy(p_data, fmna_nvram_data + section_offset, section_len);

                SendWicedCommand(HCI_CONTROL_HK_COMMAND_UPDATE_FMNA_INFO, section_buf, section_len + 4);
                QThread::msleep(100);
            }
        }
        fclose(fp);
    }
    else
        Log("NO FMNA NVRAM Data File !!!");


}

void MainWindow::on_btnHKFMNASetFacData_clicked()
{
    FILE *fp = NULL;
    uint16_t file_size = 0;

    if (m_CommPort == NULL)
        return;

    fp = fopen("./fmna/fmna_factory_data.bin", "rb");

    if (fp)
    {
        file_size = fread(fmna_factory_data, 1, sizeof(fmna_factory_data), fp);

        Log("Update FMNA Factory Tokens length: %d", file_size);

        if(file_size > 0)
        {
            uint8_t section_buf[HCI_TRANSFER_MAX_DATA_SIZE + 4];
            uint8_t section_count =  file_size/HCI_TRANSFER_MAX_DATA_SIZE;

            if(file_size % HCI_TRANSFER_MAX_DATA_SIZE)
            {
                section_count++;
            }

            for(uint8_t i = 0;i < section_count;i++)
            {
                uint8_t *p_data = section_buf;
                uint16_t section_offset = i*HCI_TRANSFER_MAX_DATA_SIZE;
                uint16_t section_len = HCI_TRANSFER_MAX_DATA_SIZE;

                if(section_len + section_offset > file_size)
                    section_len = file_size - section_offset;

                UINT16_TO_STREAM(p_data, section_offset);
                if(i == section_count - 1)
                {
                    UINT16_TO_STREAM(p_data, (section_len | 0x8000));
                }
                else
                {
                    UINT16_TO_STREAM(p_data, section_len);
                }

                memcpy(p_data, fmna_factory_data + section_offset, section_len);

                SendWicedCommand(HCI_CONTROL_HK_COMMAND_UPDATE_FACTORY_DATA, section_buf, section_len + 4);
                QThread::msleep(100);
            }
        }
        fclose(fp);
    }
    else
    {
        Log("No FMNA Factory Tokens bin files, Try to generate it from fmna/fmna_factory_token.txt !!!");
#ifdef HK_FMNA_BASE64_DEBUG
        fp = fopen("./fmna/fmna_factory_token.txt", "r");
        bool b_tokens_complete = false;

        if (fp)
        {
            char buf[FMNA_FILE_LINE_MAX] = {0};

            memset(fmna_factory_data, 0 , sizeof(fmna_factory_data));

            while(fgets(buf, FMNA_FILE_LINE_MAX, fp))
            {
                if (0 == clean_text_space(buf))
                {
                    continue;
                }

                if(0 == strcmp(buf, "SERIAL_NUMBER"))
                {
                    if(read_effect_tokens_data(fp, buf) > 0)
                    {
                        Log("%s\n", buf);
                        fmna_connection_platform_set_serial_number((uint8_t *)buf, strlen(buf));
                    }
                    else
                        break;
                }
                else if(0 == strcmp(buf, "TOKEN_UUID"))
                {
                    if(read_effect_tokens_data(fp, buf) > 0)
                    {
                        uint8_t uuid[16] = {0}, i = 0, j = 0;
                        Log("%s\n", buf);

                        while(i < strlen(buf))
                        {
                            if(buf[i] != '-')
                            {
                                if(isdigit(buf[i]))
                                    uuid[j] = buf[i] - '0';
                                else if((buf[i] >= 'A') && (buf[i] <= 'Z'))
                                    uuid[j] = buf[i] - 'A' + 0xA;
                                else if((buf[i] >= 'a') && (buf[i] <= 'z'))
                                    uuid[j] = buf[i] - 'a' + 0xA;

                                uuid[j] *= 16;
                                i++;

                                if(isdigit(buf[i]))
                                    uuid[j] += buf[i] - '0';
                                else if((buf[i] >= 'A') && (buf[i] <= 'Z'))
                                    uuid[j] += buf[i] - 'A' + 0xA;
                                else if((buf[i] >= 'a') && (buf[i] <= 'z'))
                                    uuid[j] += buf[i] - 'a' + 0xA;

                                j++;
                            }
                            i++;
                        }
                        fmna_connection_platform_set_token_uuid(uuid, 16);
                    }
                    else
                        break;
                }
                else if(0 == strcmp(buf, "FM_SVR_ENCRY_KEY"))
                {
                    if(read_effect_tokens_data(fp, buf) > 0)
                    {
                        char       *out;
                        size_t      out_len;

                        Log("%s\n", buf);

                        /* +1 for the NULL terminator. */
                        out_len = b64_decoded_size(buf);
                        out = (char *)malloc(out_len + 1);

                        if (b64_decode((const char *)buf, (unsigned char *)out, out_len+1))
                        {
                            out[out_len] = '\0';
                            fmna_connection_platform_set_server_encry_key((uint8_t *)out, out_len);
                            free(out);
                        }
                        else
                        {
                            Log("Decode FM_SVR_ENCRY_KEY Failure\n");
                            free(out);
                            break;
                        }
                    }
                    else
                        break;
                }
                else if(0 == strcmp(buf, "FM_SIG_VERIF_KEY"))
                {
                    if(read_effect_tokens_data(fp, buf) > 0)
                    {
                        char       *out;
                        size_t      out_len;

                        Log("%s\n", buf);

                        /* +1 for the NULL terminator. */
                        out_len = b64_decoded_size(buf);
                        out = (char *)malloc(out_len + 1);

                        if (b64_decode((const char *)buf, (unsigned char *)out, out_len+1))
                        {
                            out[out_len] = '\0';
                            fmna_connection_platform_set_signature_verif_key((uint8_t *)out, out_len);
                            free(out);
                        }
                        else
                        {
                            Log("Decode FM_SIG_VERIF_KEY Failure\n");
                            free(out);
                            break;
                        }
                    }
                    else
                        break;
                }
                else if(0 == strcmp(buf, "SW_AUTH_TOKEN"))
                {
                    if(read_effect_tokens_data(fp, buf) > 0)
                    {
                        char       *out;
                        size_t      out_len;

                        Log("%s\n", buf);

                        /* +1 for the NULL terminator. */
                        out_len = b64_decoded_size(buf);
                        out = (char *)malloc(out_len + 1);

                        if (b64_decode((const char *)buf, (unsigned char *)out, out_len+1))
                        {
                            out[out_len] = '\0';
                            fmna_connection_platform_set_token((uint8_t *)out, out_len);
                            free(out);
                            b_tokens_complete = true;
                        }
                        else
                        {
                            Log("Decode SW_AUTH_TOKEN Failure\n");
                            free(out);
                            break;
                        }
                    }
                    else
                        break;
                }
                else
                    break;
            }

            fclose(fp);

            if(b_tokens_complete)
            {
                fp = fopen("./fmna/fmna_factory_data.bin", "ab");

                if (fp)
                {
                    fwrite(fmna_factory_data, 1, sizeof(fmna_factory_data), fp);
                    fclose(fp);
                }
                else
                    Log("Fail to generate factory token file: fmna/fmna_factory_token.txt !!!");
            }
        }
        else
            Log("No FMNA Factory Tokens File: fmna/fmna_factory_token.txt !!!");
#endif
    }
}


void MainWindow::on_btnHKFMNAEnable_clicked()
{
    SendHciCommand(HCI_CONTROL_HK_COMMAND_ENABLE_FMNA, 0, NULL, 0);
}

void MainWindow::on_btnHKFMNAPairingMode_clicked()
{
    uint8_t pairing = 1;
    SendWicedCommand(HCI_CONTROL_HK_COMMAND_FMNA_ENTER_PAIRING, &pairing, 1);
}

void MainWindow::on_btnHKFMNAMotionDetected_clicked()
{
    SendHciCommand(HCI_CONTROL_HK_COMMAND_FMNA_MOTION_DETECT, 0, NULL, 0);
}

void MainWindow::on_btnHKFMNAGetDevState_clicked()
{
    SendHciCommand(HCI_CONTROL_HK_COMMAND_GET_FMNA_STATE, 0, NULL, 0);
}


// event to set on off builb state
void MainWindow::SetLightOnOff(BOOL on)
{

    if (on)
        ui->labelHKLight->setText("ON");
    else
        ui->labelHKLight->setText("OFF");
}

// Update UI on event
void MainWindow::UpdateUI(USHORT handle, LPBYTE p, DWORD dwLen)
{
    if(dwLen < 1)
    {
        Log("UpdateUI bad length: %ld", dwLen);
        return;
    }

    char str[10];
    if (handle == m_hLightOn)
    {
        m_bLightOn = p[0];
        Log("UpdateUI Lightbulb On : %d", m_bLightOn);
        SetLightOnOff(m_bLightOn);
    }
    else if (handle == m_hLightBrightness)
    {
        m_nLightBrightness = p[0];
        sprintf(str, "%d", m_nLightBrightness);
        Log("UpdateUI Lightbulb Brightness : %d", m_nLightBrightness);
        ui->lineEditHKBrightness->setText(str);
    }
    else if (handle == m_hLockState)
    {
        m_nLockState = p[0];
        Log("UpdateUI Lock State : %d", m_nLockState);
        ui->cbLockState->setCurrentIndex(m_nLockState);
    }
    else if (handle == m_hLockTargetState)
    {
        m_nLockTargetState = p[0];
        Log("UpdateUI Lock Target State : %d", m_nLockTargetState);
        ui->cbLockTargetState->setCurrentIndex(m_nLockTargetState);
    }
}

void MainWindow::on_timer()
{
    if (m_nIdentifyTimerCounter > 0)
    {
        if (--m_nIdentifyTimerCounter > 0)
        {
            m_bLightOn = !m_bLightOn;
            SetLightOnOff(m_bLightOn);
        }
        else
        {
            p_timer->stop();
        }
    }
}

// Handle WICED HCI events
void MainWindow::onHandleWicedEventHK(unsigned int opcode, unsigned char *p_data, unsigned int len)
{
    switch (HCI_CONTROL_GROUP(opcode))
        {
            case HCI_CONTROL_GROUP_HK:
            HandleHkEvent(opcode, p_data, len);
            break;
        }
}

int base64encode(const void* data_buf, size_t dataLength, char* result, size_t resultSize)
{
    const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const uint8_t *data = (const uint8_t *)data_buf;
    size_t resultIndex = 0;
    size_t x;
    uint32_t n = 0;
    int padCount = dataLength % 3;
    uint8_t n0, n1, n2, n3;

    /* increment over the length of the string, three characters at a time */
    for (x = 0; x < dataLength; x += 3)
    {
        /* these three 8-bit (ASCII) characters become one 24-bit number */
        n = ((uint32_t)data[x]) << 16; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0

        if ((x + 1) < dataLength)
            n += ((uint32_t)data[x + 1]) << 8;//parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0

        if ((x + 2) < dataLength)
            n += data[x + 2];

        /* this 24-bit number gets separated into four 6-bit numbers */
        n0 = (uint8_t)(n >> 18) & 63;
        n1 = (uint8_t)(n >> 12) & 63;
        n2 = (uint8_t)(n >> 6) & 63;
        n3 = (uint8_t)n & 63;

        /*
        * if we have one byte available, then its encoding is spread
        * out over two characters
        */
        if (resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
        result[resultIndex++] = base64chars[n0];
        if (resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
        result[resultIndex++] = base64chars[n1];

        /*
        * if we have only two bytes available, then their encoding is
        * spread out over three chars
        */
        if ((x + 1) < dataLength)
        {
            if (resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
            result[resultIndex++] = base64chars[n2];
        }

        /*
        * if we have all three bytes available, then their encoding is spread
        * out over four characters
        */
        if ((x + 2) < dataLength)
        {
            if (resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
            result[resultIndex++] = base64chars[n3];
        }
    }

    /*
    * create and add padding that is required if we did not have a multiple of 3
    * number of characters available
    */
    if (padCount > 0)
    {
        for (; padCount < 3; padCount++)
        {
            if (resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
            result[resultIndex++] = '=';
        }
    }
    if (resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
    result[resultIndex] = 0;
    return 0;   /* indicate success */
}



// Handle WICED HCI events for Homekit
void MainWindow::HandleHkEvent(DWORD opcode, LPBYTE p_data, DWORD len)
{
    char trace[1024];
    uint handle;
    char strhandle[20];
    FILE *fp = NULL;

    Log("CLightbulbControlDlg::HandleHkEvent  Rcvd Op Code: 0x%04lx, len: %ld", opcode, len);

    switch (opcode)
    {
    case HCI_CONTROL_HK_EVENT_READ_RESPONSE:
        sprintf(trace, "Read Response : ");
        for (int i = 0; i < (int)len - 2; i++)
            sprintf(&trace[strlen(trace)], "%02x", p_data[2 + i]);
        ui->lineEditHKHexVal->setText(&trace[strlen("Read Response : ")]);
        Log(trace);
        break;
    case HCI_CONTROL_HK_EVENT_UPDATE:
        handle = p_data[0] + (p_data[1] << 8);
        sprintf(strhandle, "%04x", handle);
        ui->lineEditHKHandle->setText(strhandle);

        if (handle == m_hIdentify && p_data[2])
        {
            Log("Received Identify");
            m_nIdentifyTimerCounter = 5;

            p_timer->start(1000); // start a 1 sec timer to flicker the light
            break;
        }
        else if (handle == m_hLockTargetState)
        {
            sprintf(trace, "Characteristic update [%s] : %02x", strhandle, p_data[2]);
            ui->lineEditHKHexVal->setText(&trace[strlen("Characteristic update [0000] : ")]);
            Log(trace);
            if (len > 3)
            {
                Log("Additional authorization data : ");
                for (int i = 0; i < (int)len - 3; i++)
                    sprintf(&trace[strlen(trace)], "%02x", p_data[3 + i]);
               Log(trace);
            }
        }
        else
        {
            sprintf(trace, "Characteristic update [%s] : ", strhandle);
            for (int i = 0; i < (int)len - 2; i++)
                sprintf(&trace[strlen(trace)],"%02x", p_data[2 + i]);
            ui->lineEditHKHexVal->setText(&trace[strlen("Characteristic update [0000] : ")]);
            Log(trace);
        }

        UpdateUI(handle, &p_data[2], len - 2);
        break;
    case HCI_CONTROL_HK_EVENT_LIST_ITEM:
        handle = p_data[0] + (p_data[1] << 8);
        if (strcmp((char *)&p_data[3], "Identify") == 0)
            m_hIdentify = handle;
        else if (strcmp((char *)&p_data[3], "Lightbulb On") == 0)
            m_hLightOn = handle;
        else if (strcmp((char *)&p_data[3], "Lightbulb Brightness") == 0)
            m_hLightBrightness = handle;
        else if (strcmp((char *)&p_data[3], "Current Door State") == 0)
            m_hDoorState = handle;
        else if (strcmp((char *)&p_data[3], "Lock Current State") == 0)
            m_hLockState = handle;
        else if (strcmp((char *)&p_data[3], "Lock Target State") == 0)
            m_hLockTargetState = handle;
        sprintf(trace, "0x%04x    %s     %s", handle, TypeString(p_data[2]), &p_data[3]);
        Log(trace);
        break;
    case HCI_CONTROL_HK_EVENT_TOKEN_DATA:
        if (p_data[0] & HCI_TOKEN_DATA_FLAG_UUID)
        {
            fp = fopen("token.txt", "w");
            if (fp)
            {
                fprintf(fp, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
                    p_data[16], p_data[15], p_data[14], p_data[13], p_data[12], p_data[11], p_data[10], p_data[9],
                    p_data[8], p_data[7], p_data[6], p_data[5], p_data[4], p_data[3], p_data[2], p_data[1]);
                fclose(fp);
            }
            break;
        }

        if (p_data[0] & HCI_TOKEN_DATA_FLAG_START)
        {
            p_token_buf = software_token;
            software_token_size = 0;
        }

        memcpy(p_token_buf, &p_data[1], len - 1);
        p_token_buf += len - 1;
        software_token_size += len - 1;

        if (p_data[0] & HCI_TOKEN_DATA_FLAG_END)
        {
            fp = fopen("token.txt", "a");
            if (fp)
            {
                base64encode(software_token, software_token_size, software_token_encode, 1400);
                fprintf(fp, "%s\n", software_token_encode);
                fclose(fp);
            }
        }
        break;

    case HCI_CONTROL_HK_EVENT_FMNA_DEV_DATA:
    {
        uint16_t offset;
        uint16_t data_len;

        STREAM_TO_UINT16(offset, p_data);
        STREAM_TO_UINT16(data_len, p_data);

        data_len &= 0x7FFF;

        Log("Find My NVRAM data update : %d, %d", offset, data_len);

        if(data_len > 0)
        {
            if(offset == 0)
                fp = fopen("./fmna/fmna_nv_data.bin", "wb");
            else
                fp = fopen("./fmna/fmna_nv_data.bin", "ab");

            if (fp)
            {
                fseek(fp, 0, SEEK_END);
                fwrite(p_data, 1, data_len, fp);
                fclose(fp);
            }
        }
    }
    break;

    case HCI_CONTROL_HK_EVENT_FMNA_FACTORY_DATA:
    {
        uint16_t offset;
        uint16_t data_len;

        STREAM_TO_UINT16(offset, p_data);
        STREAM_TO_UINT16(data_len, p_data);

        bool b_data_complete =(data_len & 0x8000);
        data_len &= 0x7FFF;

        if(data_len > 0)
        {
            memcpy(fmna_factory_data + offset, p_data, data_len);

            Log("Find My Factory tokens update : %d, %d", offset, data_len);

            if(offset == 0)
                fp = fopen("./fmna/fmna_factory_data.bin", "wb");
            else
                fp = fopen("./fmna/fmna_factory_data.bin", "ab");

            if (fp)
            {
                fseek(fp, 0, SEEK_END);
                fwrite(p_data, 1, data_len, fp);
                fclose(fp);
            }

            if(b_data_complete)
            {
    #ifdef HK_FMNA_BASE64_DEBUG
                char FMNA_token_file[255] =  "./fmna/fmna_factory_token.txt";

                time_t timep;
                struct tm *p;
                time(&timep);
                p = localtime(&timep);
                sprintf(FMNA_token_file, "./fmna/fmna_factory_token_%04d%02d%02d_%02d%02d%02d.txt"
                    ,p->tm_year+1900
                    ,p->tm_mon+1
                    ,p->tm_mday
                    ,p->tm_hour
                    ,p->tm_min
                    ,p->tm_sec);

                fp = fopen(FMNA_token_file, "w");
                if (fp)
                {
                    uint16_t length = 0;

                    fmna_connection_platform_get_serial_number(software_token, 8);
                    software_token[8] = 0;
                    Log("FMNA Serial Number : %s", software_token);
                    fprintf(fp, "%s\n", "SERIAL_NUMBER");
                    fprintf(fp, "%s\n\r\n", software_token);

                    fmna_connection_platform_get_server_encry_key(software_token, APPLE_SERVER_ENCRYPTION_KEY_BLEN);
                    base64encode(software_token, APPLE_SERVER_ENCRYPTION_KEY_BLEN, software_token_encode, 1400);
                    fprintf(fp, "%s\n", "FM_SVR_ENCRY_KEY");
                    fprintf(fp, "%s\n\r\n", software_token_encode);


                    fmna_connection_platform_get_signature_verif_key(software_token, APPLE_SERVER_SIG_VERIFICATION_KEY_BLEN);
                    base64encode(software_token, APPLE_SERVER_SIG_VERIFICATION_KEY_BLEN, software_token_encode, 1400);
                    fprintf(fp, "%s\n", "FM_SIG_VERIF_KEY");
                    fprintf(fp, "%s\n\r\n", software_token_encode);

                    fmna_connection_platform_get_token_uuid(software_token, SOFTWARE_AUTH_UUID_BLEN);
                    fprintf(fp, "%s\n", "TOKEN_UUID");
                    fprintf(fp, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n\r\n",
                        software_token[0], software_token[1], software_token[2], software_token[3], software_token[4], software_token[5], software_token[6], software_token[7],
                        software_token[8], software_token[9], software_token[10], software_token[11], software_token[12], software_token[13], software_token[14], software_token[15]);

                    length = SOFTWARE_AUTH_TOKEN_BLEN;

                    fmna_connection_platform_get_token(software_token, &length);
                    base64encode(software_token, length, software_token_encode, 1400);
                    fprintf(fp, "%s\n", "SW_AUTH_TOKEN");
                    fprintf(fp, "%s\n\r\n", software_token_encode);

                    fclose(fp);
                }
    #endif
                Log("Got complete updatedfactory tokens data len: %d", data_len);
            }
        }
    }
    break;

    case HCI_CONTROL_HK_EVENT_FMNA_SOUND_PLAY:
        Log("Find My device play sound : %d", p_data[0]);
        break;

    case HCI_CONTROL_HK_EVENT_FMNA_PAIRING_COMP:
        Log("Find My device pairing complete, BD_ADDR: %02x:%02x:%02x:%02x:%02x:%02x",  p_data[0],  p_data[1],  p_data[2],  p_data[3], p_data[4], p_data[5]);
#ifdef HK_FMNA_BASE64_DEBUG
        //Get update factory tokens after pairing complete
        on_btnHKFMNAGetFacData_clicked();
#endif
        break;

    case HCI_CONTROL_HK_EVENT_FMNA_STATE_REPORT:
        Log("Find My device state  : %d BD_ADDR: %02x:%02x:%02x:%02x:%02x:%02x", p_data[0], p_data[1],  p_data[2],  p_data[3], p_data[4], p_data[5], p_data[6]);
        break;

    case HCI_CONTROL_HK_EVENT_FMNA_NVRAM_REPORT:
    {
        uint8_t nvram_type = p_data[0];
        uint8_t nvram_state = p_data[1];
        if(nvram_state == FMNA_NVRAM_STATE_INITIALIZED)
        {
            Log("Update FMNA NVRAM or Factory Data complete  : %d", nvram_type);
        }
        else if(nvram_state == FMNA_NVRAM_STATE_NEED_UPDATE)
        {
            Log("Need to Update FMNA NVRAM Data : %d", nvram_type);
            on_btnHKFMNAGetNvData_clicked();
        }
    }
        break;

    default:
        Log("HandleHkEvent  Rcvd Unsupported Op Code: 0x%04lx", opcode);
        break;
    }
}

