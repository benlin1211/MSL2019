/**
  ******************************************************************************
  * @file    hand_recog.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Wed Jan 15 13:05:33 2020
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */


#include "hand_recog.h"

#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "layers.h"

#undef AI_TOOLS_VERSION_MAJOR
#undef AI_TOOLS_VERSION_MINOR
#undef AI_TOOLS_VERSION_MICRO
#define AI_TOOLS_VERSION_MAJOR 5
#define AI_TOOLS_VERSION_MINOR 0
#define AI_TOOLS_VERSION_MICRO 0


#undef AI_TOOLS_API_VERSION_MAJOR
#undef AI_TOOLS_API_VERSION_MINOR
#undef AI_TOOLS_API_VERSION_MICRO
#define AI_TOOLS_API_VERSION_MAJOR 1
#define AI_TOOLS_API_VERSION_MINOR 3
#define AI_TOOLS_API_VERSION_MICRO 0

#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_hand_recog
 
#undef AI_HAND_RECOG_MODEL_SIGNATURE
#define AI_HAND_RECOG_MODEL_SIGNATURE     "7f7a2d53e2334725e1c2bb8933eb52ad"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     "(rev-5.0.0)"
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Wed Jan 15 13:05:33 2020"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_HAND_RECOG_N_BATCHES
#define AI_HAND_RECOG_N_BATCHES         (1)

/**  Forward network declaration section  *************************************/
AI_STATIC ai_network AI_NET_OBJ_INSTANCE;


/**  Forward network array declarations  **************************************/
AI_STATIC ai_array conv2d_92_scratch0_array;   /* Array #0 */
AI_STATIC ai_array dense_46_bias_array;   /* Array #1 */
AI_STATIC ai_array dense_46_weights_array;   /* Array #2 */
AI_STATIC ai_array dense_45_bias_array;   /* Array #3 */
AI_STATIC ai_array dense_45_weights_array;   /* Array #4 */
AI_STATIC ai_array dense_44_bias_array;   /* Array #5 */
AI_STATIC ai_array dense_44_weights_array;   /* Array #6 */
AI_STATIC ai_array conv2d_92_bias_array;   /* Array #7 */
AI_STATIC ai_array conv2d_92_weights_array;   /* Array #8 */
AI_STATIC ai_array input_0_output_array;   /* Array #9 */
AI_STATIC ai_array conv2d_92_output_array;   /* Array #10 */
AI_STATIC ai_array dense_44_output_array;   /* Array #11 */
AI_STATIC ai_array dense_44_nl_output_array;   /* Array #12 */
AI_STATIC ai_array dense_45_output_array;   /* Array #13 */
AI_STATIC ai_array dense_45_nl_output_array;   /* Array #14 */
AI_STATIC ai_array dense_46_output_array;   /* Array #15 */
AI_STATIC ai_array dense_46_nl_output_array;   /* Array #16 */


/**  Forward network tensor declarations  *************************************/
AI_STATIC ai_tensor conv2d_92_scratch0;   /* Tensor #0 */
AI_STATIC ai_tensor dense_46_bias;   /* Tensor #1 */
AI_STATIC ai_tensor dense_46_weights;   /* Tensor #2 */
AI_STATIC ai_tensor dense_45_bias;   /* Tensor #3 */
AI_STATIC ai_tensor dense_45_weights;   /* Tensor #4 */
AI_STATIC ai_tensor dense_44_bias;   /* Tensor #5 */
AI_STATIC ai_tensor dense_44_weights;   /* Tensor #6 */
AI_STATIC ai_tensor conv2d_92_bias;   /* Tensor #7 */
AI_STATIC ai_tensor conv2d_92_weights;   /* Tensor #8 */
AI_STATIC ai_tensor input_0_output;   /* Tensor #9 */
AI_STATIC ai_tensor conv2d_92_output;   /* Tensor #10 */
AI_STATIC ai_tensor conv2d_92_output0;   /* Tensor #11 */
AI_STATIC ai_tensor dense_44_output;   /* Tensor #12 */
AI_STATIC ai_tensor dense_44_nl_output;   /* Tensor #13 */
AI_STATIC ai_tensor dense_45_output;   /* Tensor #14 */
AI_STATIC ai_tensor dense_45_nl_output;   /* Tensor #15 */
AI_STATIC ai_tensor dense_46_output;   /* Tensor #16 */
AI_STATIC ai_tensor dense_46_nl_output;   /* Tensor #17 */


/**  Forward network tensor chain declarations  *******************************/
AI_STATIC_CONST ai_tensor_chain conv2d_92_chain;   /* Chain #0 */
AI_STATIC_CONST ai_tensor_chain dense_44_chain;   /* Chain #1 */
AI_STATIC_CONST ai_tensor_chain dense_44_nl_chain;   /* Chain #2 */
AI_STATIC_CONST ai_tensor_chain dense_45_chain;   /* Chain #3 */
AI_STATIC_CONST ai_tensor_chain dense_45_nl_chain;   /* Chain #4 */
AI_STATIC_CONST ai_tensor_chain dense_46_chain;   /* Chain #5 */
AI_STATIC_CONST ai_tensor_chain dense_46_nl_chain;   /* Chain #6 */


/**  Forward network layer declarations  **************************************/
AI_STATIC ai_layer_conv2d_nl_pool conv2d_92_layer; /* Layer #0 */
AI_STATIC ai_layer_dense dense_44_layer; /* Layer #1 */
AI_STATIC ai_layer_nl dense_44_nl_layer; /* Layer #2 */
AI_STATIC ai_layer_dense dense_45_layer; /* Layer #3 */
AI_STATIC ai_layer_nl dense_45_nl_layer; /* Layer #4 */
AI_STATIC ai_layer_dense dense_46_layer; /* Layer #5 */
AI_STATIC ai_layer_nl dense_46_nl_layer; /* Layer #6 */


/**  Array declarations section  **********************************************/
AI_ARRAY_OBJ_DECLARE(
    conv2d_92_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 7936,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    dense_46_bias_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 10,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    dense_46_weights_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 1280,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    dense_45_bias_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    dense_45_weights_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 16384,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    dense_44_bias_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    dense_44_weights_array, AI_ARRAY_FORMAT_LUT4_FLOAT,
    NULL, NULL, 1228800,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    conv2d_92_bias_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    conv2d_92_weights_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 512,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    input_0_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
    NULL, NULL, 384,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    conv2d_92_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 9600,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    dense_44_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    dense_44_nl_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    dense_45_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    dense_45_nl_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    dense_46_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 10,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    dense_46_nl_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
    NULL, NULL, 10,
     AI_STATIC)




/**  Tensor declarations section  *********************************************/
AI_TENSOR_OBJ_DECLARE(
  conv2d_92_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 31, 2), AI_STRIDE_INIT(4, 4, 4, 512, 15872),
  1, &conv2d_92_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_46_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 10, 1, 1), AI_STRIDE_INIT(4, 4, 4, 40, 40),
  1, &dense_46_bias_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_46_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 128, 10, 1, 1), AI_STRIDE_INIT(4, 4, 512, 5120, 5120),
  1, &dense_46_weights_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_45_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &dense_45_bias_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_45_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 128, 128, 1, 1), AI_STRIDE_INIT(4, 4, 512, 65536, 65536),
  1, &dense_45_weights_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_44_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &dense_44_bias_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_44_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 9600, 128, 1, 1), AI_STRIDE_INIT(4, 1, 4800, 614400, 614400),
  1, &dense_44_weights_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  conv2d_92_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv2d_92_bias_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  conv2d_92_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 2, 2, 128), AI_STRIDE_INIT(4, 4, 4, 8, 16),
  1, &conv2d_92_weights_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  input_0_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 1, 32, 12), AI_STRIDE_INIT(4, 4, 4, 4, 128),
  1, &input_0_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  conv2d_92_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 15, 5), AI_STRIDE_INIT(4, 4, 4, 512, 7680),
  1, &conv2d_92_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  conv2d_92_output0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 9600, 1, 1), AI_STRIDE_INIT(4, 4, 4, 38400, 38400),
  1, &conv2d_92_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_44_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &dense_44_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_44_nl_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &dense_44_nl_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_45_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &dense_45_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_45_nl_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &dense_45_nl_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_46_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 10, 1, 1), AI_STRIDE_INIT(4, 4, 4, 40, 40),
  1, &dense_46_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_46_nl_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 10, 1, 1), AI_STRIDE_INIT(4, 4, 4, 40, 40),
  1, &dense_46_nl_output_array, NULL)


/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_92_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&input_0_output),
  AI_TENSOR_LIST_ENTRY(&conv2d_92_output),
  AI_TENSOR_LIST_ENTRY(&conv2d_92_weights, &conv2d_92_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&conv2d_92_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_92_layer, 0,
  OPTIMIZED_CONV2D_TYPE,
  conv2d_nl_pool, forward_conv2d_nl_pool,
  &AI_NET_OBJ_INSTANCE, &dense_44_layer, AI_STATIC,
  .tensors = &conv2d_92_chain, 
  .groups = 1, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .pool_func = pool_func_mp_array_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_44_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&conv2d_92_output0),
  AI_TENSOR_LIST_ENTRY(&dense_44_output),
  AI_TENSOR_LIST_ENTRY(&dense_44_weights, &dense_44_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_44_layer, 4,
  DENSE_TYPE,
  dense, forward_dense,
  &AI_NET_OBJ_INSTANCE, &dense_44_nl_layer, AI_STATIC,
  .tensors = &dense_44_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_44_nl_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&dense_44_output),
  AI_TENSOR_LIST_ENTRY(&dense_44_nl_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_44_nl_layer, 4,
  NL_TYPE,
  nl, forward_relu,
  &AI_NET_OBJ_INSTANCE, &dense_45_layer, AI_STATIC,
  .tensors = &dense_44_nl_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_45_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&dense_44_nl_output),
  AI_TENSOR_LIST_ENTRY(&dense_45_output),
  AI_TENSOR_LIST_ENTRY(&dense_45_weights, &dense_45_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_45_layer, 5,
  DENSE_TYPE,
  dense, forward_dense,
  &AI_NET_OBJ_INSTANCE, &dense_45_nl_layer, AI_STATIC,
  .tensors = &dense_45_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_45_nl_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&dense_45_output),
  AI_TENSOR_LIST_ENTRY(&dense_45_nl_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_45_nl_layer, 5,
  NL_TYPE,
  nl, forward_relu,
  &AI_NET_OBJ_INSTANCE, &dense_46_layer, AI_STATIC,
  .tensors = &dense_45_nl_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_46_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&dense_45_nl_output),
  AI_TENSOR_LIST_ENTRY(&dense_46_output),
  AI_TENSOR_LIST_ENTRY(&dense_46_weights, &dense_46_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_46_layer, 6,
  DENSE_TYPE,
  dense, forward_dense,
  &AI_NET_OBJ_INSTANCE, &dense_46_nl_layer, AI_STATIC,
  .tensors = &dense_46_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_46_nl_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&dense_46_output),
  AI_TENSOR_LIST_ENTRY(&dense_46_nl_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_46_nl_layer, 6,
  NL_TYPE,
  nl, forward_sm,
  &AI_NET_OBJ_INSTANCE, &dense_46_nl_layer, AI_STATIC,
  .tensors = &dense_46_nl_chain, 
)


AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 688744, 1,
                     NULL),
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 70144, 1,
                     NULL),
  AI_TENSOR_LIST_IO_ENTRY(AI_FLAG_NONE, AI_HAND_RECOG_IN_NUM, &input_0_output),
  AI_TENSOR_LIST_IO_ENTRY(AI_FLAG_NONE, AI_HAND_RECOG_OUT_NUM, &dense_46_nl_output),
  &conv2d_92_layer, 0, NULL)



AI_DECLARE_STATIC
ai_bool hand_recog_configure_activations(
  ai_network* net_ctx, const ai_buffer* activation_buffer)
{
  AI_ASSERT(net_ctx &&  activation_buffer && activation_buffer->data)

  ai_ptr activations = AI_PTR(AI_PTR_ALIGN(activation_buffer->data, 4));
  AI_ASSERT(activations)
  AI_UNUSED(net_ctx)

  {
    /* Updating activations (byte) offsets */
    conv2d_92_scratch0_array.data = AI_PTR(activations + 0);
    conv2d_92_scratch0_array.data_start = AI_PTR(activations + 0);
    input_0_output_array.data = AI_PTR(NULL);
    input_0_output_array.data_start = AI_PTR(NULL);
    conv2d_92_output_array.data = AI_PTR(activations + 31744);
    conv2d_92_output_array.data_start = AI_PTR(activations + 31744);
    dense_44_output_array.data = AI_PTR(activations + 0);
    dense_44_output_array.data_start = AI_PTR(activations + 0);
    dense_44_nl_output_array.data = AI_PTR(activations + 0);
    dense_44_nl_output_array.data_start = AI_PTR(activations + 0);
    dense_45_output_array.data = AI_PTR(activations + 512);
    dense_45_output_array.data_start = AI_PTR(activations + 512);
    dense_45_nl_output_array.data = AI_PTR(activations + 512);
    dense_45_nl_output_array.data_start = AI_PTR(activations + 512);
    dense_46_output_array.data = AI_PTR(activations + 0);
    dense_46_output_array.data_start = AI_PTR(activations + 0);
    dense_46_nl_output_array.data = AI_PTR(NULL);
    dense_46_nl_output_array.data_start = AI_PTR(NULL);
    
  }
  return true;
}



AI_DECLARE_STATIC
ai_bool hand_recog_configure_weights(
  ai_network* net_ctx, const ai_buffer* weights_buffer)
{
  AI_ASSERT(net_ctx &&  weights_buffer && weights_buffer->data)

  ai_ptr weights = AI_PTR(weights_buffer->data);
  AI_ASSERT(weights)
  AI_UNUSED(net_ctx)

  {
    /* Updating weights (byte) offsets */
    
    dense_46_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_46_bias_array.data = AI_PTR(weights + 688704);
    dense_46_bias_array.data_start = AI_PTR(weights + 688704);
    dense_46_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_46_weights_array.data = AI_PTR(weights + 683584);
    dense_46_weights_array.data_start = AI_PTR(weights + 683584);
    dense_45_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_45_bias_array.data = AI_PTR(weights + 683072);
    dense_45_bias_array.data_start = AI_PTR(weights + 683072);
    dense_45_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_45_weights_array.data = AI_PTR(weights + 617536);
    dense_45_weights_array.data_start = AI_PTR(weights + 617536);
    dense_44_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_44_bias_array.data = AI_PTR(weights + 617024);
    dense_44_bias_array.data_start = AI_PTR(weights + 617024);
    dense_44_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_44_weights_array.data = AI_PTR(weights + 2624);
    dense_44_weights_array.data_start = AI_PTR(weights + 2560);
    conv2d_92_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_92_bias_array.data = AI_PTR(weights + 2048);
    conv2d_92_bias_array.data_start = AI_PTR(weights + 2048);
    conv2d_92_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_92_weights_array.data = AI_PTR(weights + 0);
    conv2d_92_weights_array.data_start = AI_PTR(weights + 0);
  }

  return true;
}


/**  PUBLIC APIs SECTION  *****************************************************/

AI_API_ENTRY
ai_bool ai_hand_recog_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if ( report && net_ctx )
  {
    ai_network_report r = {
      .model_name        = AI_HAND_RECOG_MODEL_NAME,
      .model_signature   = AI_HAND_RECOG_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = {AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR,
                            AI_TOOLS_API_VERSION_MICRO, 0x0},

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 1503638,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .activations       = AI_STRUCT_INIT,
      .params            = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if ( !ai_platform_api_get_network_report(network, &r) ) return false;

    *report = r;
    return true;
  }

  return false;
}

AI_API_ENTRY
ai_error ai_hand_recog_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}

AI_API_ENTRY
ai_error ai_hand_recog_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}

AI_API_ENTRY
ai_handle ai_hand_recog_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}

AI_API_ENTRY
ai_bool ai_hand_recog_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if ( !net_ctx ) return false;

  ai_bool ok = true;
  ok &= hand_recog_configure_weights(net_ctx, &params->params);
  ok &= hand_recog_configure_activations(net_ctx, &params->activations);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_hand_recog_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}

AI_API_ENTRY
ai_i32 ai_hand_recog_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}

#undef AI_HAND_RECOG_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_VERSION_MAJOR
#undef AI_TOOLS_VERSION_MINOR
#undef AI_TOOLS_VERSION_MICRO
#undef AI_TOOLS_API_VERSION_MAJOR
#undef AI_TOOLS_API_VERSION_MINOR
#undef AI_TOOLS_API_VERSION_MICRO
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

