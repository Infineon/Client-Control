/*
 * (c) 2024-2026, Infineon Technologies AG, or an affiliate of Infineon
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
//#include <malloc.h>
#include "wiced_hci.h"
#include "hci_control_api.h"
#include "wiced_hci_ras.h"
#include "wiced_bt_defs.h"

bool wiced_hci_le_ras_start_cs_procedure(uint16_t conn_id, ras_rpc_cs_cmd_procedure_params_t *p_params)
{
    uint8_t    cmd[30];
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, conn_id);
    UINT8_TO_STREAM(p_cmd, p_params->cs_role);
    UINT8_TO_STREAM(p_cmd, p_params->mode_0_steps);
    UINT16_TO_STREAM(p_cmd, p_params->max_procedure_len);
    UINT16_TO_STREAM(p_cmd, p_params->min_procedure_interval);
    UINT16_TO_STREAM(p_cmd, p_params->max_procedure_interval);
    UINT16_TO_STREAM(p_cmd, p_params->max_procedure_count);
    UINT32_TO_STREAM(p_cmd, p_params->min_subevent_len);
    UINT32_TO_STREAM(p_cmd, p_params->max_subevent_len);
    UINT8_TO_STREAM(p_cmd, p_params->cs_sync_phy);
    UINT8_TO_STREAM(p_cmd, p_params->cs_procedure_phy);
    UINT8_TO_STREAM(p_cmd, p_params->tx_power_delta);


    return wiced_hci_send_command(HCI_CONTROL_RAS_START_CS_PROCEDURE, cmd, (uint32_t)(p_cmd - cmd));
}
