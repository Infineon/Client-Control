
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


#ifndef APP_HOST_DM_H
#define APP_HOST_DM_H

#include "wiced_hci_dm.h"
#include "hci_control_api.h"
#include "app_host.h"

// device manager
extern bool app_host_dm_set_device_name(char * dev_nam);
extern bool app_host_dm_set_device_addr(uint8_t * bda);
extern bool app_host_dm_set_pairing_mode(bool mode);
extern bool app_host_dm_set_vis(bool disc, bool connectable);
extern bool app_host_dm_reset();
extern bool app_host_dm_delete_nvram_data(int nvram_id);
extern bool app_host_dm_inquiry(bool start_stop);
extern bool app_host_dm_le_scan(bool enable, bool filter_adv);
extern bool app_host_dm_push_pairing_host_info(uint8_t * nvram_data, int len);
extern bool app_host_dm_push_nvram_data(int nvram_id, uint8_t * nvram_data, int len);
extern bool app_host_dm_hidh_add(int bda[BDA_LEN]);
extern bool app_host_dm_add_battery_client(int bda[BDA_LEN]);
extern bool app_host_dm_add_findme_locator(int bda[BDA_LEN]);
extern bool app_host_dm_get_version_info();
extern bool app_host_dm_set_app_traces(bool enable, uint8_t route);
extern bool app_host_dm_user_confirm(uint8_t * bda, bool accept);
extern bool app_host_dm_unbond_device(uint8_t * bda);

// device management API
void app_host_remove_device(uint8_t bda[6]);
void app_host_remove_all_devices();
wiced_hci_bt_device_t *app_host_find_device_by_connection(uint16_t conn_type, uint16_t handle);
wiced_hci_bt_device_t *app_host_find_device_by_handle(uint16_t handle);
extern wiced_hci_bt_device_t* app_host_find_device(uint8_t bda[6]);
extern wiced_hci_bt_device_t*  app_host_add_device(uint8_t bda[6]);

#endif
