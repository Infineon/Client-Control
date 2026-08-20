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
#include "wiced_bt_defs.h"

void wiced_hci_gatt_db_util_uuid_to_stream(uint8_t **p, wiced_hci_bt_uuid_t *p_uuid)
{
    int i = 0;
    if(p_uuid->uuid_type_16 == true )
    {
        UINT8_TO_STREAM(*p, LEN_UUID_16);      // UUID length
        UINT16_TO_STREAM(*p, p_uuid->u.uuid16);          // UUID 16-bit
    }
    else
    {
        UINT8_TO_STREAM(*p, LEN_UUID_128);         // UUID length
        for(i = 0; i < 16; i++)          // UUID 128-bit
        {
            UINT8_TO_STREAM(*p, p_uuid->u.uuid128[16 -1 - i]);
        }
    }
}

bool wiced_hci_gatt_db_primary_service(wiced_hci_bt_primary_service_data_t *p_data)
{
    uint8_t    cmd[32] = { 0 };
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);  // Service handle

    wiced_hci_gatt_db_util_uuid_to_stream(&p_cmd, &p_data->uuid);

    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_DB_PRIMARY_SERVICE_ADD, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_gatt_db_secondary_service(wiced_hci_bt_secondary_service_data_t *p_data)
{
    uint8_t    cmd[32] = { 0 };
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->handle);  // Service handle

    wiced_hci_gatt_db_util_uuid_to_stream(&p_cmd, &p_data->uuid);

    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_DB_SECONDARY_SERVICE_ADD, cmd, (uint32_t)(p_cmd - cmd));
}

bool wiced_hci_gatt_db_included_service(wiced_included_service_data_t *p_data)
{
    uint8_t    cmd[32] = { 0 };
    uint8_t     *p_cmd = cmd;

    UINT16_TO_STREAM(p_cmd, p_data->included_svc.handle);  // handle
    UINT16_TO_STREAM(p_cmd, p_data->svc_handle);  // Service handle
    UINT16_TO_STREAM(p_cmd, p_data->end_grp);  // end group

    wiced_hci_gatt_db_util_uuid_to_stream(&p_cmd, &p_data->included_svc.uuid);

    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_DB_INCLUDED_SERVICE_ADD, cmd, (uint32_t)(p_cmd - cmd));

}

bool wiced_hci_gatt_db_characteristic(wiced_characteristic_data_t *p_data)
{
    uint8_t    cmd[32] = { 0 };
    uint8_t     *p = cmd;

    UINT16_TO_STREAM(p, p_data->handle);  // handle
    UINT16_TO_STREAM(p, p_data->handle_val);  // handle value
    UINT8_TO_STREAM(p, p_data->prop);  // property
    UINT8_TO_STREAM(p, p_data->perm);  // permission

    wiced_hci_gatt_db_util_uuid_to_stream(&p, &p_data->uuid);

    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_DB_CHARACTERISTIC_ADD, cmd, (uint32_t)(p - cmd));

}

bool wiced_hci_gatt_db_descriptor(wiced_descriptor_data_t *p_data)
{
    uint8_t    cmd[32] = { 0 };
    uint8_t     *p = cmd;

    UINT16_TO_STREAM(p, p_data->handle);  // handle
    UINT8_TO_STREAM(p, p_data->perm);  // permission

    wiced_hci_gatt_db_util_uuid_to_stream(&p, &p_data->uuid);

    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_DB_DESCRIPTOR_ADD, cmd, (uint32_t)(p - cmd));

}

bool wiced_hci_gatt_db_set_advert_data(uint8_t *p_data, uint8_t size)
{
    return wiced_hci_send_command(HCI_CONTROL_LE_COMMAND_SET_RAW_ADVERTISE_DATA, p_data, size);
}

bool wiced_hci_gatt_db_init()
{
    return wiced_hci_send_command(HCI_CONTROL_GATT_COMMAND_DB_INIT, 0, 0);
}

bool wiced_hci_gatt_start_stop_advert(bool start, uint8_t pairing_method , uint8_t tx_power)
{

    uint8_t    cmd[6] = { 0 };
    uint8_t     *p_cmd = cmd;

    UINT8_TO_STREAM(p_cmd, start);
    UINT8_TO_STREAM(p_cmd, tx_power);
    UINT8_TO_STREAM(p_cmd, pairing_method); // swift pair
    UINT16_TO_STREAM(p_cmd, 0xffff);        // adv options for now settign to 0xffff

    return wiced_hci_send_command(HCI_CONTROL_LE_COMMAND_ADVERTISE, cmd, (uint32_t)(p_cmd - cmd));

}
