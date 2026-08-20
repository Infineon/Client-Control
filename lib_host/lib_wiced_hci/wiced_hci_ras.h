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

/*
 * Definitions for WICED HCI RAS
 */


#ifndef WICED_HCI_RAS_H
#define WICED_HCI_RAS_H

#include "wiced_types.h"
#define WICED_BLE_CS_FAE_TABLE_SIZE 72 /**< Size of the CS FAE (Frequency Actuation Error) Table */
#define WICED_BLE_CS_CHANNEL_MAP_SIZE 10 /**< Size of the CS Channel Map */

/** CS FAE (Frequency Actuation Error) table  */
typedef uint8_t wiced_ble_cs_fae_table_t[WICED_BLE_CS_FAE_TABLE_SIZE];

/** CS Channel map
* 10 octet value to set the CS channel map, This parameter contains 80 1-bit fields. The nth such field (in the
* range 0 to 78) contains the value for CS  channel index n.
* Channel n is enabled for CS procedure = 1
* Channel n is disabled for CS procedure = 0
* Channels n = 0, 1, 23, 24, 25, 77, and 78 shall be ignored and shall be set to zero.
* At least 15 channels shall be enabled.
* The most significant bit (bit 79) is reserved for future use.
*/
typedef uint8_t wiced_ble_cs_channel_map_t[WICED_BLE_CS_CHANNEL_MAP_SIZE];

typedef struct
{
    /**
     * CS Config Id identifying CS procedure
     */
    uint8_t config_id;
    /**
     * Value 0x00 CS configuration is removed
     * Value 0x01 CS configuration is created
     */
    uint8_t action;
    /**
     * If set to 0x1: Mode-1, 0x2: Mode-2, 0x3:Mode-3
     */
    uint8_t main_mode_type;
    /**
     * If set to 0x1: Mode-1, 0x2: Mode-2, 0x3:Mode-3
     */
    uint8_t sub_mode_type;
    /** Valid values (0x2 to 0xFF) : Minimum number of CS main mode steps to be executed before a submode
     * step is executed
     */
    uint8_t min_main_mode_steps;
    /**
     * Valid values (0x2 to 0xFF) : Maximum number of CS main mode steps to be
     * executed before a submode step is executed
     */
    uint8_t max_main_mode_steps;
    /** Valid values (0x0 to 0x3) : The number of main mode steps taken from the end of the last CS subevent
     * to be repeated at the beginning of the current CS subevent directly after the last mode-0 step of
     * that event
     */
    uint8_t main_mode_repetition;
    /**
     * Valid values (0x1 to 0x3) : Number of CS mode-0 steps to be included at the beginning of each CS subevent
     */
    uint8_t mode_0_steps;
    /**
     * Set Value 0 - Initiator, Value 1 - Reflector
     */
    uint8_t role;
    /**
     * 0x00 RTT AA-only, 0x01 RTT with 32-bit sounding sequence, 0x02 RTT with 96-bit sounding sequence,
     * 0x03 RTT with 32-bit random sequence, 0x04 RTT with 64-bit random sequence,
     * 0x05 RTT with 96-bit random sequence, 0x06 RTT with 128-bit random sequence
     * All other values Reserved for future use
     */
    uint8_t rtt_type;
    /**
     * 0x01 LE 1M PHY, 0x02 LE 2M PHY, 0x03 LE 2M 2BT PHY
     */
    uint8_t cs_sync_phy;
    /**
     * CS Channel map
     */
    wiced_ble_cs_channel_map_t channel_map;
    /**
     * The number of times the map represented by the channel_map field is to be cycled through for non-mode-0
     * steps within a CS procedure
     * Valid value 0x1-0xFF
     */
    uint8_t channel_map_repetition;
    /**
     * Value 0 : Use Channel Selection Algorithm #3b for non-mode-0 CS steps
     * Value 1: Use Channel Selection Algorithm #3c for non-mode-0 CS steps
     */
    uint8_t channel_selection_type;
    /**
     * Value 0: Use Hat shape for user-specified channel sequence
     * Value 1: Use X shape for user-specified channel sequence
     */
    uint8_t ch3c_shape;
    /**
     * Values (0x2 - 0x8) Number of channels skipped in each rising and falling sequence
     */
    uint8_t ch3c_jump;
    /**
     * Reserved, shall be set to 0
     */
    uint8_t reserved;
    /**
     * Interlude time in microseconds between the RTT packets,
     * Values : 0x0A, 0x14, 0x1E, 0x28, 0x32, 0x3C, 0x50, or 0x91
     */
    uint8_t t_ip1_time;
    /**
     * Interlude time in microseconds between the CS tones
     * Values : 0x0A, 0x14, 0x1E, 0x28, 0x32, 0x3C, 0x50, or 0x91
     */
    uint8_t t_ip2_time;
    /**
     * Time in microseconds for frequency changes
     * Values : 0x0F, 0x14, 0x1E, 0x28, 0x32, 0x3C, 0x50, 0x64, 0x78, or 0x96
     */
    uint8_t t_fcs_time;
    /**
     * Time in microseconds for the phase measurement period of the CS tones Values: 0x0A, 0x14, or 0x28
     */
    uint8_t t_pm_time;
} wiced_ble_cs_config_complete_t;

/** Structure to return the CS Procedure enable complete event */
typedef struct
{
    /**
     * CS Config Id identifying CS procedure
     */
    uint8_t config_id;
    /**
     * CS Procedure State
     * Value 0: CS procedures are disabled
     * Value 1: CS procedures are enabled
     */
    uint8_t state;
    /**
     * Antenna Configuration Index as described in BT Core Specification ver 6.0, [Vol 6] Part A, Section 5.3,
     * Values : 0x00 - 0x07
     */
    uint8_t tone_antenna_config_selection;
    /**
     * Transmit power level used for CS procedure, Range: -127 to 20, Units: dBm,
     * If value = 0x7F - Transmit power level is unavailable
     */
    uint8_t selected_tx_power;
    /**
     * Duration for each CS subevent in microseconds, Range: 1250 us to 4 s
     */
    uint32_t subevent_len;
    /**
     * Number of CS subevents anchored off the same ACL connection event
     */
    uint8_t subevents_per_event;
    /**
     * Time between consecutive CS subevents anchored off the same ACL connection event. Units: 0.625 ms
     */
    uint16_t subevent_interval;
    /**
     * Number of ACL connection events between consecutive CS event anchor points
     */
    uint16_t event_interval;
    /**
     * Number of ACL connection events between consecutive CS procedure anchor points
     */
    uint16_t procedure_interval;
    /**
     * Number of CS procedures to be scheduled, Range: 0x0001 to 0xFFFF
     * If set to 0 - CS procedures to continue until disabled, else,
     */
    uint16_t procedure_count;
    /**
     * Maximum duration for each CS procedure,
     * Range: 0x0001 to 0xFFFF, Time = N × 0.625 ms, Time range: 0.625 ms to 40.959375 s
     */
    uint16_t max_procedure_len;
} wiced_ble_cs_procedure_enable_complete_t;


typedef struct {
    uint8_t cs_role;
    uint8_t mode_0_steps;
    uint16_t max_procedure_len;
    uint16_t min_procedure_interval;
    uint16_t max_procedure_interval;
    uint16_t max_procedure_count;
    uint32_t min_subevent_len;
    uint32_t max_subevent_len;
    uint8_t  cs_sync_phy;
    uint8_t  cs_procedure_phy;
    uint8_t  tx_power_delta;
} ras_rpc_cs_cmd_procedure_params_t;

bool wiced_hci_le_ras_start_cs_procedure(uint16_t conn_id, ras_rpc_cs_cmd_procedure_params_t *p_params);

#endif // WICED_HCI_RAS_H
