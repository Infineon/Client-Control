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
#ifndef PROTOBUF_RPC_H
#define PROTOBUF_RPC_H

#ifdef __cplusplus
extern "C" {
#endif
typedef bool BOOLEAN;
typedef unsigned short USHORT;
typedef unsigned short uint16_t;
#define FALSE 0
//#define TRUE    ~FALSE
#define TRUE    1
#define WICED_SUCCESS 0
#define WICED_ERROR   1001
#define HCI_CONTROL_GROUP_SCRIPT                              0x25
#define HCI_CONTROL_SCRIPT_EVENT_RET_CODE                   ( ( HCI_CONTROL_GROUP_SCRIPT << 8 ) | 0x01 )   /* Script command return code */
#define HCI_CONTROL_SCRIPT_EVENT_UNKNOWN_CMD                ( ( HCI_CONTROL_GROUP_SCRIPT << 8 ) | 0x02 )   /* Unknown Script command */
#define HCI_CONTROL_SCRIPT_EVENT_CALLBACK                   ( ( HCI_CONTROL_GROUP_SCRIPT << 8 ) | 0x03 )   /* Async script callback */

typedef uint16_t wiced_result_t;

typedef struct  __protobuf_param {
    RPC_HEADER *header;
    pb_istream_t *stream;
    void* pRet;
    USHORT* ret_size;
} PROTOBUF_PARAM;

typedef BOOLEAN tPROTOBUF_RPC_FUNC_PROXY(PROTOBUF_PARAM* parm);
typedef BOOLEAN _rpc_dispatch(PROTOBUF_PARAM *parm);

extern bool read_bytes(pb_istream_t * stream, const pb_field_t *field, void **arg);
extern BOOLEAN SendStreamedResponse(PROTOBUF_PARAM* parm, pb_ostream_t *postream);
extern BOOLEAN SendBooleanResponse(PROTOBUF_PARAM* parm, RPC_BooleanResponse* response);
extern BOOLEAN SendUint32Response(PROTOBUF_PARAM* parm, RPC_Uint32Response* response);
extern void* get_protobuf_buffer(bool bInitialize);
#ifdef __cplusplus
}
#endif
#endif // PROTOBUF_RPC_H
