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

#include "wiced_hci_panu.h"
#include "hci_control_api.h"
#include "string.h"

uint8_t gu8PanuData[2000];
extern uint8_t *scru_dump_hex(uint8_t *p, char *p_title, uint32_t len, uint32_t trace_layer, uint32_t trace_type);

bool wiced_hci_panu_connect(wiced_hci_bt_panu_connect_data_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    BDADDR_TO_STREAM(p_cmd, p_data->bda);

    return wiced_hci_send_command(HCI_CONTROL_PANU_COMMAND_CONNECT, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_panu_disconnect(wiced_hci_bt_panu_disconnect_data_t * p_data)
{
    uint8_t   cmd[60];
    uint32_t    commandBytes = 0;

    cmd[commandBytes++] = p_data->handle & 0xff;
    cmd[commandBytes++] = (p_data->handle >> 8) & 0xff;

    return wiced_hci_send_command(HCI_CONTROL_PANU_COMMAND_DISCONNECT, cmd, commandBytes);
}

bool wiced_hci_panu_send_data(wiced_hci_bt_panu_send_data_t *p_data)
{
    uint32_t    commandBytes = 0;
    uint16_t    bytesPerPacket = p_data->data_len + 18;
    uint8_t    *p_cmd = gu8PanuData;

    gu8PanuData[0] = HCI_WICED_PKT;
    gu8PanuData[1] = (uint8_t)(HCI_CONTROL_PANU_COMMAND_SEND_DATA & 0xff);
    gu8PanuData[2] = (uint8_t)((HCI_CONTROL_PANU_COMMAND_SEND_DATA >> 8) & 0xff);
    gu8PanuData[3] = (uint8_t)(bytesPerPacket & 0xff);
    gu8PanuData[4] = (uint8_t)((bytesPerPacket >> 8) & 0xff);
    commandBytes += 5;
    p_cmd += 5;

    UINT16_TO_STREAM(p_cmd, p_data->handle);
    BDADDR_TO_STREAM(p_cmd, p_data->dst);
    BDADDR_TO_STREAM(p_cmd, p_data->src);
    UINT16_TO_STREAM(p_cmd, p_data->protocol);
    UINT16_TO_STREAM(p_cmd, p_data->data_len);
    commandBytes += 18;

    memcpy(p_cmd, p_data->data, p_data->data_len);
    commandBytes += p_data->data_len;

    return wiced_hci_send_command(0, gu8PanuData, commandBytes);
}
