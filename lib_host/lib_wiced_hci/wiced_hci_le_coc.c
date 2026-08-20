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

#include "wiced_hci.h"
#include "hci_control_api.h"

bool wiced_hci_le_coc_send_phy(uint16_t le2m_enable)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, le2m_enable);
    return wiced_hci_send_command(HCI_CONTROL_LE_COC_COMMAND_ENABLE_LE2M, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_coc_send_psm(uint16_t psm)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, psm);
    return wiced_hci_send_command(HCI_CONTROL_LE_COC_COMMAND_SET_PSM, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_coc_send_mtu(uint16_t mtu)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, mtu);
    return wiced_hci_send_command(HCI_CONTROL_LE_COC_COMMAND_SET_MTU, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_coc_connect(wiced_hci_bt_bda_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    BDADDR_TO_STREAM(p_cmd, p_data->bda);

    return wiced_hci_send_command(HCI_CONTROL_LE_COC_COMMAND_CONNECT, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_coc_disconnect(wiced_hci_bt_bda_t *p_data)
{
    uint8_t    cmd[10];
    uint8_t     *p_cmd = cmd;

    BDADDR_TO_STREAM(p_cmd, p_data->bda);

    return wiced_hci_send_command(HCI_CONTROL_LE_COC_COMMAND_DISCONNECT, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_le_coc_start_adv(bool start)
{
    return wiced_hci_send_command(HCI_CONTROL_LE_COMMAND_ADVERTISE, (uint8_t*)&start, 1);
}

bool wiced_hci_le_coc_send_data(uint8_t *p_data, uint16_t len)
{
    return wiced_hci_send_command(HCI_CONTROL_LE_COC_COMMAND_SEND_DATA, p_data, len);
}
