
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
 * Definitions for WICED HCI AG
 */

#ifndef WICED_HCI_GATT_DB_H
#define WICED_HCI_GATT_DB_H

// GATT DB
typedef struct
{
    bool uuid_type_16;
    union
    {
        uint16_t uuid16;
        uint8_t uuid128[16];
    } u;
} wiced_hci_bt_uuid_t;

// GATT DB
typedef struct
{
    uint16_t handle;
    wiced_hci_bt_uuid_t uuid;
} wiced_hci_bt_service_data_t;


typedef struct
{
    wiced_hci_bt_service_data_t included_svc;
    uint16_t svc_handle;
    uint16_t end_grp;
} wiced_included_service_data_t;

typedef struct
{
    uint16_t handle;
    uint16_t handle_val;
    uint8_t prop;
    uint8_t perm;
    wiced_hci_bt_uuid_t uuid;
} wiced_characteristic_data_t;

typedef struct
{
    uint16_t handle;
    uint8_t perm;
    wiced_hci_bt_uuid_t uuid;
} wiced_descriptor_data_t;

typedef wiced_hci_bt_service_data_t wiced_hci_bt_primary_service_data_t;
typedef wiced_hci_bt_service_data_t wiced_hci_bt_secondary_service_data_t;

bool wiced_hci_gatt_db_primary_service(wiced_hci_bt_primary_service_data_t *p_data);
bool wiced_hci_gatt_db_secondary_service(wiced_hci_bt_secondary_service_data_t *p_data);
bool wiced_hci_gatt_db_included_service(wiced_included_service_data_t *p_data);
bool wiced_hci_gatt_db_characteristic(wiced_characteristic_data_t *p_data);
bool wiced_hci_gatt_db_descriptor(wiced_descriptor_data_t *p_data);
bool wiced_hci_gatt_db_set_advert_data(uint8_t *p_data, uint8_t size);
bool wiced_hci_gatt_db_init();

#endif
