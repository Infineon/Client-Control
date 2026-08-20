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
#include <string.h>

#define WICED_HCI_DATA_MAX 1024

// extern API, Implemented by application
extern int app_host_port_write(uint8_t *data, uint32_t len);

// Send WICED HCI command
bool wiced_hci_send_command(uint16_t command, uint8_t * payload, uint32_t len)
{
    if(!command){
        return (app_host_port_write( payload, len) > 0 ? true : false);
    }

    // For all commands set the header bytes
    {
        uint8_t    data[WICED_HCI_DATA_MAX];
        uint16_t    header  = 0;

        data[header++] = HCI_WICED_PKT;
        data[header++] = command & 0xff;
        data[header++] = (command >> 8) & 0xff;
        data[header++] = len & 0xff;
        data[header++] = (len >> 8) & 0xff;

        if(len){
            memcpy(&data[header], payload, len);
        }

        return (app_host_port_write( data,  header+len) > 0 ? true : false);
    }
}
