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
#include "wiced_hci_hidd.h"
#include <string.h>


bool app_host_hidd_disconnect()
{
    return wiced_hci_hidd_disconnect();
}

bool app_host_hidd_connect()
{
    return wiced_hci_hidd_connect();
}

bool app_host_hidd_send_report(uint8_t channel, uint8_t report_type, uint8_t *report, uint8_t report_len)
{
	wiced_hci_bt_hidd_report_t data;
	data.channel = channel;
	data.report_type = report_type;
	data.report_len = report_len;
	data.report = report;

	return wiced_hci_hidd_send_report(&data);
}

bool app_host_hidd_pairing_mode(uint8_t m_pairing_mode_active)
{
    wiced_hci_bt_hidd_paring_mode_data_t data;
	data.pairing_mode = m_pairing_mode_active;
	return wiced_hci_hidd_pairing_mode(&data);
}

bool app_host_hidd_key(uint8_t key, uint8_t keyDown)
{
	return wiced_hci_hidd_key(key, keyDown);
}

bool app_host_hidd_send_key(uint8_t cap_lock, uint8_t ctrl_key, uint8_t alt_key, char * buffer, uint8_t btn_up)
{
	wiced_hci_bt_hidd_send_key_data_t data;

	data.cap_lock = cap_lock;
	data.ctrl_key = ctrl_key;
	data.alt_key = alt_key;
	memcpy(data.buffer, buffer,6);
	data.btn_up = btn_up;

	return wiced_hci_bt_hidd_send_key(&data);
}

bool app_host_hidd_cap_lock(uint8_t cap_lock, uint8_t ctrl_key, uint8_t alt_key)
{
	wiced_hci_bt_hidd_cap_lock_data_t data;

	data.cap_lock = cap_lock;
	data.ctrl_key = ctrl_key;
	data.alt_key = alt_key;

	return wiced_hci_bt_hidd_cap_lock(&data);
}

bool app_host_hidd_get_host_info()
{
	return wiced_hci_hidd_get_host_info();
}

bool app_host_hidd_virtual_unplug()
{
	return wiced_hci_bt_hidd_virtual_unplug();
}
