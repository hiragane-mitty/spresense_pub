/****************************************************************************
 * modules/lte/altcom/include/api/mbedtls/apicmd_entropy.h
 *
 *   Copyright 2020 Sony Semiconductor Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Sony Semiconductor Solutions Corporation nor
 *    the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __MODULES_LTE_ALTCOM_INCLUDE_API_MBEDTLS_APICMD_ENTROPY_H
#define __MODULES_LTE_ALTCOM_INCLUDE_API_MBEDTLS_APICMD_ENTROPY_H

 /****************************************************************************
 * Included Files
 ****************************************************************************/

#include "apicmd.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define APISUBCMDID_TLS_ENTROPY_INIT (0x01)
#define APISUBCMDID_TLS_ENTROPY_FREE (0x02)

#define APICMD_TLS_ENTROPY_CMD_DATA_SIZE (sizeof(struct apicmd_entropycmd_s))
#define APICMD_TLS_ENTROPY_CMDRES_DATA_SIZE (sizeof(struct apicmd_entropycmdres_s))

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* This structure discribes the data structure of the API command */

begin_packed_struct struct apicmd_entropycmd_s
{
  uint32_t ctx;
  uint32_t subcmd_id;
} end_packed_struct;

begin_packed_struct struct apicmd_entropycmdres_s
{
  int32_t ret_code;
} end_packed_struct;


#endif /* __MODULES_LTE_ALTCOM_INCLUDE_API_MBEDTLS_APICMD_ENTROPY_H */