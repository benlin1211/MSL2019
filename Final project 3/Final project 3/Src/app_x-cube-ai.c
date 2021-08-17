
#ifdef __cplusplus
 extern "C" {
#endif
/**
  ******************************************************************************
  * @file           : app_x-cube-ai.c
  * @brief          : AI program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "app_x-cube-ai.h"
#include "bsp_ai.h"
#include "aiValidation.h"
#include "ai_datatypes_defines.h"

/* USER CODE BEGIN includes */
/* USER CODE END includes */

/*************************************************************************
  *
  */
void MX_X_CUBE_AI_Init(void)
{
    MX_UARTx_Init();
    aiValidationInit();
    /* USER CODE BEGIN 0 */
    /* Activation/working buffer is allocated as a static memory chunk
     * (bss section) */
    AI_ALIGNED(4)
    static ai_u8 activations[AI_HAND_RECOG_DATA_ACTIVATIONS_SIZE];

    aiInit(activations);
    /* USER CODE END 0 */
}

void MX_X_CUBE_AI_Process(void)
{
    aiValidationProcess();
    /* USER CODE BEGIN 1 */
	int nb_run = 20;
    int res;

    /* Example of definition of the buffers to store the tensor input/output */
    /*  type is dependent of the expected format                             */
    AI_ALIGNED(4)
    static ai_i8 in_data[AI_HAND_RECOG_IN_1_SIZE_BYTES];

    AI_ALIGNED(4)
    static ai_i8 out_data[AI_HAND_RECOG_OUT_1_SIZE_BYTES];

    /* Retrieve format/type of the first input tensor - index 0 */
    const ai_buffer_format fmt_ = AI_BUFFER_FORMAT(&ai_input[0]);
    const uint32_t type_ = AI_BUFFER_FMT_GET_TYPE(fmt_);

    /* Prepare parameters for float to Qmn conversion */
    const ai_i16 N_ = AI_BUFFER_FMT_GET_FBITS(fmt_);
    const ai_float scale_ = (0x1U << N_);
    const ai_i16 M_ =  AI_BUFFER_FMT_GET_BITS(fmt_)
                       - AI_BUFFER_FMT_GET_SIGN(fmt_) - N_;
    const ai_float max_ = (ai_float)(0x1U << M_);

    /* Perform nb_rub inferences (batch = 1) */
    while (--nb_run) {

        /* ---------------------------------------- */
        /* Data generation and Pre-Process          */
        /* ---------------------------------------- */
        /* - fill the input buffer with random data */
        for (ai_size i=0;  i < AI_HAND_RECOG_IN_1_SIZE; i++ ) {

            /* Generate random data in the range [-1, 1] */
            ai_float val = 2.0f * (ai_float)rand() / (ai_float)RAND_MAX - 1.0f;

            /* Convert the data if necessary */
            if (type_ == AI_BUFFER_FMT_TYPE_FLOAT) {
                ((ai_float *)in_data)[i] = val;
            } else { /* AI_BUFFER_FMT_TYPE_Q */
                /* Scale the values in the range [-2^M, 2^M] */
                val *= max_;
                /* Convert float to Qmn format */
                const ai_i32 tmp_ = AI_ROUND(val * scale_, ai_i32);
                in_data[i] =  AI_CLAMP(tmp_, -128, 127, ai_i8);
            }
        }

        /* Perform the inference */
        res = aiRun(in_data, out_data);
        if (res) {
            // ...
            return;
        }

        /* Post-Process - process the output buffer */
        // ...
    }
    /* USER CODE END 1 */
}
/* Multiple network support --------------------------------------------------*/

#include <string.h>
#include "ai_datatypes_defines.h"

static const ai_network_entry_t networks[AI_MNETWORK_NUMBER] = {
    {
        .name = (const char *)AI_HAND_RECOG_MODEL_NAME,
        .config = AI_HAND_RECOG_DATA_CONFIG,
        .ai_get_info = ai_hand_recog_get_info,
        .ai_create = ai_hand_recog_create,
        .ai_destroy = ai_hand_recog_destroy,
        .ai_get_error = ai_hand_recog_get_error,
        .ai_init = ai_hand_recog_init,
        .ai_run = ai_hand_recog_run,
        .ai_forward = ai_hand_recog_forward,
        .ai_data_weights_get_default = ai_hand_recog_data_weights_get,
        .params = { AI_HAND_RECOG_DATA_WEIGHTS(0),
                AI_HAND_RECOG_DATA_ACTIVATIONS(0)},
        .extActBufferStartAddr = AI_HAND_RECOG_DATA_ACTIVATIONS_START_ADDR,
        .actBufferSize = AI_HAND_RECOG_DATA_ACTIVATIONS_SIZE
    },
};

struct network_instance {
     const ai_network_entry_t *entry;
     ai_handle handle;
     ai_network_params params;
};

/* Number of instance is aligned on the number of network */
AI_STATIC struct network_instance gnetworks[AI_MNETWORK_NUMBER] = {0};

AI_DECLARE_STATIC
ai_bool ai_mnetwork_is_valid(const char* name,
        const ai_network_entry_t *entry)
{
    if (name && (strlen(entry->name) == strlen(name)) &&
            (strncmp(entry->name, name, strlen(entry->name)) == 0))
        return true;
    return false;
}

AI_DECLARE_STATIC
struct network_instance *ai_mnetwork_handle(struct network_instance *inst)
{
    for (int i=0; i<AI_MNETWORK_NUMBER; i++) {
        if ((inst) && (&gnetworks[i] == inst))
            return inst;
        else if ((!inst) && (gnetworks[i].entry == NULL))
            return &gnetworks[i];
    }
    return NULL;
}

AI_DECLARE_STATIC
void ai_mnetwork_release_handle(struct network_instance *inst)
{
    for (int i=0; i<AI_MNETWORK_NUMBER; i++) {
        if ((inst) && (&gnetworks[i] == inst)) {
            gnetworks[i].entry = NULL;
            return;
        }
    }
}

AI_API_ENTRY
const char* ai_mnetwork_find(const char *name, ai_int idx)
{
    const ai_network_entry_t *entry;

    for (int i=0; i<AI_MNETWORK_NUMBER; i++) {
        entry = &networks[i];
        if (ai_mnetwork_is_valid(name, entry))
            return entry->name;
        else {
            if (!idx--)
                return entry->name;
        }
    }
    return NULL;
}

AI_API_ENTRY
ai_error ai_mnetwork_create(const char *name, ai_handle* network,
        const ai_buffer* network_config)
{
    const ai_network_entry_t *entry;
    const ai_network_entry_t *found = NULL;
    ai_error err;
    struct network_instance *inst = ai_mnetwork_handle(NULL);

    if (!inst) {
        err.type = AI_ERROR_ALLOCATION_FAILED;
        err.code = AI_ERROR_CODE_NETWORK;
        return err;
    }

    for (int i=0; i<AI_MNETWORK_NUMBER; i++) {
        entry = &networks[i];
        if (ai_mnetwork_is_valid(name, entry)) {
            found = entry;
            break;
        }
    }

    if (!found) {
        err.type = AI_ERROR_INVALID_PARAM;
        err.code = AI_ERROR_CODE_NETWORK;
        return err;
    }

    if (network_config == NULL)
        err = found->ai_create(network, found->config);
    else
        err = found->ai_create(network, network_config);
    if ((err.code == AI_ERROR_CODE_NONE) && (err.type == AI_ERROR_NONE)) {
        inst->entry = found;
        inst->handle = *network;
        *network = (ai_handle*)inst;
    }

    return err;
}

AI_API_ENTRY
ai_handle ai_mnetwork_destroy(ai_handle network)
{
    struct network_instance *inn;
    inn =  ai_mnetwork_handle((struct network_instance *)network);
    if (inn) {
        ai_handle hdl = inn->entry->ai_destroy(inn->handle);
        if (hdl != inn->handle) {
            ai_mnetwork_release_handle(inn);
            network = AI_HANDLE_NULL;
        }
    }
    return network;
}

AI_API_ENTRY
ai_bool ai_mnetwork_get_info(ai_handle network, ai_network_report* report)
{
    struct network_instance *inn;
    inn =  ai_mnetwork_handle((struct network_instance *)network);
    if (inn)
        return inn->entry->ai_get_info(inn->handle, report);
    else
        return false;
}

AI_API_ENTRY
ai_error ai_mnetwork_get_error(ai_handle network)
{
    struct network_instance *inn;
    ai_error err;
    err.type = AI_ERROR_INVALID_PARAM;
    err.code = AI_ERROR_CODE_NETWORK;

    inn =  ai_mnetwork_handle((struct network_instance *)network);
    if (inn)
        return inn->entry->ai_get_error(inn->handle);
    else
        return err;
}

AI_API_ENTRY
ai_bool ai_mnetwork_init(ai_handle network, const ai_network_params* params)
{
    struct network_instance *inn;
    ai_network_params par;

    /* TODO: adding check ai_buffer activations/weights shape coherence */

    inn =  ai_mnetwork_handle((struct network_instance *)network);
    if (inn) {
        par = inn->entry->params;
        if (params->activations.n_batches)
            par.activations = params->activations;
        else
            par.activations.data = params->activations.data;
        if (params->params.n_batches)
            par.params = params->params;
        else
            par.params.data = inn->entry->ai_data_weights_get_default();
        return inn->entry->ai_init(inn->handle, &par);
    }
    else
        return false;
}

AI_API_ENTRY
ai_i32 ai_mnetwork_run(ai_handle network, const ai_buffer* input,
        ai_buffer* output)
{
    struct network_instance* inn;
    inn =  ai_mnetwork_handle((struct network_instance *)network);
    if (inn)
        return inn->entry->ai_run(inn->handle, input, output);
    else
        return 0;
}

AI_API_ENTRY
ai_i32 ai_mnetwork_forward(ai_handle network, const ai_buffer* input)
{
    struct network_instance *inn;
    inn =  ai_mnetwork_handle((struct network_instance *)network);
    if (inn)
        return inn->entry->ai_forward(inn->handle, input);
    else
        return 0;
}

AI_API_ENTRY
 int ai_mnetwork_get_private_handle(ai_handle network,
         ai_handle *phandle,
         ai_network_params *pparams)
 {
     struct network_instance* inn;
     inn =  ai_mnetwork_handle((struct network_instance *)network);
     if (inn && phandle && pparams) {
         *phandle = inn->handle;
         *pparams = inn->params;
         return 0;
     }
     else
         return -1;
 }

 AI_API_ENTRY
int ai_mnetwork_get_ext_data_activations(ai_handle network,
         ai_u32 *add,
         ai_u32 *size)
 {
     struct network_instance* inn;
     inn =  ai_mnetwork_handle((struct network_instance *)network);
     if (inn && add && size) {
         *add = inn->entry->extActBufferStartAddr;
         *size = inn->entry->actBufferSize;
         return 0;
     }
     else
         return -1;
 }

#ifdef __cplusplus
}
#endif
