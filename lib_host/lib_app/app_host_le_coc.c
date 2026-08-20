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


#include "app_host.h"

extern void updateAdvBtn(bool sts);
extern void setPhy( void );
extern void recvData(char *p_data, unsigned int len);
extern void txComplete(void);

bool app_host_le_coc_set_phy(uint16_t le2m_enable)
{
    return wiced_hci_le_coc_send_phy(le2m_enable);
}

bool app_host_le_coc_send_psm(uint16_t psm)
{
    return wiced_hci_le_coc_send_psm(psm);
}

bool app_host_le_coc_send_mtu(uint16_t mtu)
{
    return wiced_hci_le_coc_send_mtu(mtu);
}

bool app_host_le_coc_connect(uint8_t bda[6])
{
    app_host_log("Sending LE COC Connect");

    return wiced_hci_le_coc_connect((wiced_hci_bt_bda_t *)bda);
}

bool app_host_le_coc_disconnect(uint8_t bda[6])
{
    app_host_log("Disconnecting LE COC ");

    return wiced_hci_le_coc_disconnect((wiced_hci_bt_bda_t *)bda);
}

bool app_host_le_coc_start_adv(bool start)
{
    return wiced_hci_le_coc_start_adv(start);
}

bool app_host_le_coc_send_data(uint8_t *p_data, uint16_t len)
{
    return wiced_hci_le_coc_send_data(p_data, len);
}

void app_host_le_coc_event(uint16_t opcode, uint8_t * p_data, uint32_t len)
{
    uint8_t bda[6], i=0;

    switch (opcode)
    {

    case HCI_CONTROL_LE_COC_EVENT_CONNECTED:
        /* For now receiving NULL bd address can be treat as connection failure. Todo: proper error code handshake */
        if (!p_data[0] && !p_data[1] && !p_data[2] && !p_data[3] && !p_data[4] && !p_data[5])
        {
            app_host_log("LE COC connection failed! \n");
        }
        else
        {
            for (i = 0; i < 6; i++)
                bda[5 - i] = p_data[i];
            app_host_log("LE COC connected to %02x:%02x:%02x:%02x:%02x:%02x ", bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);

            //set PHY to 2M if enabled
            setPhy();
        }
        break;

    case HCI_CONTROL_LE_COC_EVENT_DISCONNECTED:
        for (i = 0; i < 6; i++)
            bda[5 - i] = p_data[i];

        app_host_log("LE COC disconnected from %02x:%02x:%02x:%02x:%02x:%02x ", bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
        break;

    case HCI_CONTROL_LE_COC_EVENT_RX_DATA:
        //app_host_log("received %d bytes from device", len);
        recvData((char*)p_data, len);
        break;

    case HCI_CONTROL_LE_COC_EVENT_TX_COMPLETE:
        //app_host_log("received tx complete");
        txComplete();
        break;

    case HCI_CONTROL_LE_COC_EVENT_ADV_STS:
        updateAdvBtn(p_data[0]);
        break;

    default:
        app_host_log("Unknown LE COC event:%d", opcode);
        break;
    }
}
