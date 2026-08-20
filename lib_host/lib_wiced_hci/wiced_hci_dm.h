
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

#ifndef WICED_HCI_DM_H
#define WICED_HCI_DM_H

#include "wiced_types.h"

typedef struct
{
    char * device_name;
} wiced_hci_dm_setdevname_data_t;

typedef struct
{
    unsigned char * device_addr;
} wiced_hci_dm_setdevaddr_data_t;

typedef struct
{
    bool mode;
} wiced_hci_dm_set_pairing_mode_data_t ;

typedef struct
{
    bool enable;
    uint8_t route;
} wiced_hci_dm_set_app_traces_data_t;

typedef struct
{
    int bda[6];
} wiced_hci_dm_bda_data_t;

typedef struct
{
    bool start_stop;
} wiced_hci_dm_inquiry_data_t;

typedef struct
{
    bool enable;
    bool filter_adv;
} wiced_hci_dm_le_set_scan_data_t;

typedef struct
{
    int nvram_id;
} wiced_hci_dm_delete_nvram_data_t;

bool wiced_hci_dm_delete_nvram_data(wiced_hci_dm_delete_nvram_data_t *data);


// WICED HCI command
bool wiced_hci_send_command(uint16_t command, uint8_t * payload, uint32_t len);

typedef struct
{
    bool discoverable;
    bool connectable;
} wiced_hci_dm_set_vis_data_t;

bool wiced_hci_dm_set_vis(wiced_hci_dm_set_vis_data_t* data);

typedef struct
{
    uint8_t * nvram_data;
    int len;
} wiced_hic_dm_push_host_info_data_t;

bool wiced_hci_dm_push_pairing_host_info(wiced_hic_dm_push_host_info_data_t *data);

typedef struct
{
    int nvram_id;
    uint8_t * nvram_data;
    int len;
} wiced_hci_dm_push_nvram_data_t;

bool wiced_hci_dm_push_nvram_data(wiced_hci_dm_push_nvram_data_t *data);

bool wiced_hci_dm_add_battery_client(wiced_hci_dm_bda_data_t * bda);
bool wiced_hci_dm_add_findme_locator(wiced_hci_dm_bda_data_t * bda);

typedef struct
{
    uint8_t * bda;
    bool confirm;
} wiced_hci_dm_user_confirm_data_t;

bool wiced_hci_dm_user_confirm(wiced_hci_dm_user_confirm_data_t * data);
bool wiced_hci_dm_setdevname(wiced_hci_dm_setdevname_data_t * dev_name);
bool wiced_hci_dm_setdevaddr(wiced_hci_dm_setdevaddr_data_t *device_addr);
bool wiced_hic_dm_set_pairing_mode(wiced_hci_dm_set_pairing_mode_data_t * data);
bool wiced_hci_dm_set_vis(wiced_hci_dm_set_vis_data_t* data);
bool wiced_hci_dm_reset();
bool wiced_hci_dm_inquiry(wiced_hci_dm_inquiry_data_t * data);
bool wiced_hci_dm_le_scan(wiced_hci_dm_le_set_scan_data_t * data);
bool wiced_hci_dm_push_pairing_host_info(wiced_hic_dm_push_host_info_data_t *data);
bool wiced_hci_dm_push_nvram_data(wiced_hci_dm_push_nvram_data_t *data);
bool wiced_hci_dm_hidh_add(wiced_hci_dm_bda_data_t * data);
bool wiced_hci_dm_add_battery_client(wiced_hci_dm_bda_data_t * data);
bool wiced_hci_dm_add_findme_locator(wiced_hci_dm_bda_data_t * data);
bool wiced_hci_dm_get_version_info();
bool wiced_hci_dm_set_app_traces(wiced_hci_dm_set_app_traces_data_t * data);
bool wiced_hci_dm_unbond_device(wiced_hci_dm_bda_data_t * data);

#endif
