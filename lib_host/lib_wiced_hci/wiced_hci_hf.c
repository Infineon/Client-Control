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

#include <string.h>
#include <memory.h>
#include "wiced_hci_hf.h"
#include "hci_control_api.h"

bool wiced_hci_hf_connect (wiced_hci_bt_hf_connect_data_t * data)
{
    uint8_t cmd[60];
    int      commandBytes = 0;
    int i = 0;

    for (i = 0; i < 6; i++)
    {
        cmd[commandBytes++] = data->m_address[5 - i];
    }
    return wiced_hci_send_command(HCI_CONTROL_HF_COMMAND_CONNECT, cmd, 6);
}

bool wiced_hci_hf_disconnect (wiced_hci_bt_hf_disconnect_data_t * data)
{
    uint8_t cmd[60];
    uint32_t      commandBytes = 0;

    uint16_t nHandle = data->handle;

    cmd[commandBytes++] = nHandle & 0xff;
    cmd[commandBytes++] = (nHandle >> 8) & 0xff;

    return wiced_hci_send_command(HCI_CONTROL_HF_COMMAND_DISCONNECT, cmd, commandBytes);
}

bool wiced_hci_hf_open_audio (wiced_hci_bt_hf_open_audio_data_t * data)
{
    uint8_t    cmd[60];
    uint32_t     commandBytes = 0;
    cmd[commandBytes++] = data->handle & 0xff;
    cmd[commandBytes++] = (data->handle >> 8) & 0xff;

    return wiced_hci_send_command(HCI_CONTROL_HF_COMMAND_OPEN_AUDIO, cmd, commandBytes);
}

bool wiced_hci_hf_close_audio (wiced_hci_bt_hf_close_audio_data_t * data)
{
    uint8_t    cmd[60];
    uint32_t     commandBytes = 0;
    cmd[commandBytes++] = data->handle & 0xff;
    cmd[commandBytes++] = (data->handle >> 8) & 0xff;

    return wiced_hci_send_command(HCI_CONTROL_HF_COMMAND_CLOSE_AUDIO, cmd, commandBytes);
}

bool wiced_hci_hf_at_command (wiced_hci_bt_hf_at_command_data_t * data)
{
    uint8_t cmd[300]={0};
    uint32_t     commandBytes = 0;
    bool rval = false;

    cmd[commandBytes++] = data->handle & 0xff;
    cmd[commandBytes++] = (data->handle >> 8) & 0xff;
    cmd[commandBytes++] = data->num & 0xff;
    cmd[commandBytes++] = (data->num >> 8) & 0xff;

    if (data->atStr)
    {
        strncpy((char *)&cmd[commandBytes], data->atStr, sizeof(cmd) - commandBytes-1);
       rval = wiced_hci_send_command(HCI_CONTROL_HF_AT_COMMAND_BASE + data->nAtCmd, cmd, commandBytes + strlen(data->atStr));
    }
    else
    {
    if(data->nAtCmd == HCI_CONTROL_HF_COMMAND_BUTTON_PRESS || data->nAtCmd == HCI_CONTROL_HF_COMMAND_LONG_BUTTON_PRESS)
	{
		rval = wiced_hci_send_command(data->nAtCmd, cmd, commandBytes);
	}
	else
	{
		rval = wiced_hci_send_command(HCI_CONTROL_HF_AT_COMMAND_BASE + data->nAtCmd, cmd, commandBytes);
	}
    }

    return rval;
}

bool wiced_hci_hf_audio_accept_conn (wiced_hci_bt_hf_audio_accept_data_t * data)
{
    uint8_t  cmd[3];
    uint8_t  *p = &cmd[0];

    //Accept the connection request
    UINT16_TO_STREAM(p, data->sco_index);
    UINT8_TO_STREAM(p, 1);// 1 : Accept, 0:Reject

    return wiced_hci_send_command(HCI_CONTROL_HF_COMMAND_AUDIO_ACCEPT_CONN, cmd, 3);
}
