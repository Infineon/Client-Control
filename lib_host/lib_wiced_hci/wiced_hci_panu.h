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
 * Definitions for WICED HCI
 */

#ifndef WICED_HCI_PANU_H
#define WICED_HCI_PANU_H

#include "wiced_types.h"
#include "wiced_hci.h"

// WICED HCI command
bool wiced_hci_send_command(uint16_t command, uint8_t * payload, uint32_t len);

typedef wiced_hci_bt_bda_t wiced_hci_bt_panu_connect_data_t ;
typedef wiced_hci_bt_handle_t wiced_hci_bt_panu_disconnect_data_t;

typedef struct
{
    uint16_t handle;
    uint8_t dst[BDA_LEN];
    uint8_t src[BDA_LEN];
    uint16_t protocol;
    uint16_t data_len;
    uint8_t  data[0];
}wiced_hci_bt_panu_send_data_t;

bool wiced_hci_panu_connect(wiced_hci_bt_panu_connect_data_t *data);
bool wiced_hci_panu_disconnect(wiced_hci_bt_panu_disconnect_data_t *data);
bool wiced_hci_panu_send_data(wiced_hci_bt_panu_send_data_t *data);


#endif // WICED_HCI_H
