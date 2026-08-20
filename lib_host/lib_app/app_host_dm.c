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
#include <string.h>

bool app_host_dm_set_device_name(char * dev_nam)
{
    wiced_hci_dm_setdevname_data_t data;
    data.device_name = dev_nam;
    return wiced_hci_dm_setdevname(&data);
}

bool app_host_dm_set_device_addr(uint8_t * bda)
{
    wiced_hci_dm_setdevaddr_data_t data;
    data.device_addr = bda;
    return wiced_hci_dm_setdevaddr(&data);
}

bool app_host_dm_set_pairing_mode(bool mode)
{
    wiced_hci_dm_set_pairing_mode_data_t data;
    data.mode = mode;
    return wiced_hic_dm_set_pairing_mode(&data);
}

bool app_host_dm_set_vis(bool disc, bool connectable)
{
    wiced_hci_dm_set_vis_data_t data;
    data.connectable = connectable;
    data.discoverable = disc;
    return wiced_hci_dm_set_vis(&data);
}

bool app_host_dm_reset()
{
    return wiced_hci_dm_reset();
}

bool app_host_dm_delete_nvram_data(int nvram_id)
{
    wiced_hci_dm_delete_nvram_data_t data;
    data.nvram_id = nvram_id;
    return wiced_hci_dm_delete_nvram_data(&data);
}

bool app_host_dm_inquiry(bool start_stop)
{
    wiced_hci_dm_inquiry_data_t data;
    data.start_stop = start_stop;
    return wiced_hci_dm_inquiry(&data);
}

bool app_host_dm_le_scan(bool enable, bool filter_adv)
{
    wiced_hci_dm_le_set_scan_data_t data;
    data.enable = enable;
    data.filter_adv = filter_adv;
    return wiced_hci_dm_le_scan(&data);
}

bool app_host_dm_push_pairing_host_info(uint8_t * nvram_data, int len)
{
    wiced_hic_dm_push_host_info_data_t data;
    data.len = len;
    data.nvram_data = nvram_data;
    return wiced_hci_dm_push_pairing_host_info(&data);
}

bool app_host_dm_push_nvram_data(int nvram_id, uint8_t * nvram_data, int len)
{
    wiced_hci_dm_push_nvram_data_t data;
    data.len = len;
    data.nvram_data = nvram_data;
    data.nvram_id = nvram_id;
    return wiced_hci_dm_push_nvram_data(&data);
}

bool app_host_dm_hidh_add(int bda[BDA_LEN])
{
    wiced_hci_dm_bda_data_t data;
    memcpy(&(data.bda), bda, sizeof(data.bda));
    return wiced_hci_dm_hidh_add(&data);
}

bool app_host_dm_add_battery_client(int bda[BDA_LEN])
{
    wiced_hci_dm_bda_data_t data;
    memcpy(&(data.bda), bda, sizeof(data.bda));
    return wiced_hci_dm_add_battery_client(&data);
}

bool app_host_dm_add_findme_locator(int bda[BDA_LEN])
{
    wiced_hci_dm_bda_data_t data;
    memcpy(&(data.bda), bda, sizeof(data.bda));
    return wiced_hci_dm_add_findme_locator(&data);
}

bool app_host_dm_get_version_info()
{
    return wiced_hci_dm_get_version_info();
}

bool app_host_dm_set_app_traces(bool enable, uint8_t rt)
{
    wiced_hci_dm_set_app_traces_data_t data;
    data.enable = enable;
    data.route = rt;
    return wiced_hci_dm_set_app_traces(&data);
}

bool app_host_dm_user_confirm(uint8_t * bda, bool accept)
{
    wiced_hci_dm_user_confirm_data_t data;
    data.bda = bda;
    data.confirm = accept;
    return wiced_hci_dm_user_confirm(&data);
}

bool app_host_dm_unbond_device(uint8_t bda[BDA_LEN])
{
    wiced_hci_dm_bda_data_t data;
    data.bda[0] = bda[0];
    data.bda[1] = bda[1];
    data.bda[2] = bda[2];
    data.bda[3] = bda[3];
    data.bda[4] = bda[4];
    data.bda[5] = bda[5];
    return wiced_hci_dm_unbond_device(&data);
}
