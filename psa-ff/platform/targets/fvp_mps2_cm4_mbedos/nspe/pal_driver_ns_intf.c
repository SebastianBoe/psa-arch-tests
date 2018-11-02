/** @file
 * Copyright (c) 2018, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "pal_common.h"
#include "pal_client_api_intf.h"
#include "pal_sid.h"

/**
    @brief    - This function initializes the UART
    @param    - uart base addr
    @return   - SUCCESS/FAILURE
**/
int pal_uart_init_ns(uint32_t uart_base_addr)
{
    psa_handle_t            print_handle = 0;
    psa_status_t            status_of_call = PSA_SUCCESS;
    uart_fn_type_t          uart_fn = UART_INIT;

    psa_invec data[3] = {{&uart_fn, sizeof(uart_fn)},
                         {&uart_base_addr, sizeof(uart_base_addr)},
                         {NULL, 0}};

    print_handle = pal_ipc_connect(DRIVER_UART_SID, 0);
    if (print_handle < 0)
    {
        return(PAL_STATUS_ERROR);
    }

    status_of_call = pal_ipc_call(print_handle, data, 3, NULL, 0);
    if (status_of_call != PSA_SUCCESS)
    {
        return(PAL_STATUS_ERROR);
    }

    pal_ipc_close(print_handle);
    return PAL_STATUS_SUCCESS;
}

/**
    @brief    - This function parses the input string and writes bytes into UART TX FIFO
    @param    - str      : Input String
              - data     : Value for format specifier
    @return   - SUCCESS/FAILURE
**/

int pal_print_ns(char *str, uint32_t data)
{
    int             string_len = 0;
    char            *p = str;
    psa_handle_t    print_handle = 0;
    psa_status_t    status_of_call = PSA_SUCCESS;
    pal_status_t    status = PAL_STATUS_SUCCESS;
    uart_fn_type_t  uart_fn = UART_PRINT;

    while (*p != '\0')
    {
        string_len++;
        p++;
    }

    psa_invec data1[3] = {{&uart_fn, sizeof(uart_fn)},
                          {str, string_len+1},
                          {&data, sizeof(data)}};
    print_handle = pal_ipc_connect(DRIVER_UART_SID, 0);

    if (print_handle < 0)
    {
        return PAL_STATUS_ERROR;
    }
    else
    {
        status_of_call = pal_ipc_call(print_handle, data1, 3, NULL, 0);
        if (status_of_call != PSA_SUCCESS)
        {
            status = PAL_STATUS_ERROR;
        }
    }
    pal_ipc_close(print_handle);
    return status;
}

/**
    @brief           - Initializes an hardware watchdog timer
    @param           - base_addr       : Base address of the watchdog module
                     - time_us         : Time in micro seconds
                     - timer_tick_us   : Number of ticks per micro second
    @return          - SUCCESS/FAILURE
**/
int pal_wd_timer_init_ns(addr_t base_addr, uint32_t time_us, uint32_t timer_tick_us)
{
    wd_param_t              wd_param;
    psa_handle_t            handle = 0;
    psa_status_t            status_of_call = PSA_SUCCESS;

    wd_param.wd_fn_type = WD_INIT_SEQ;
    wd_param.wd_base_addr = base_addr;
    wd_param.wd_time_us = time_us;
    wd_param.wd_timer_tick_us = timer_tick_us;
    psa_invec invec[1] = {{&wd_param, sizeof(wd_param)}};

    handle = pal_ipc_connect(DRIVER_WATCHDOG_SID, 0);
    if (handle < 0)
    {
        return PAL_STATUS_ERROR;
    }
    else
    {
        status_of_call = pal_ipc_call(handle, invec, 1, NULL, 0);
        if (status_of_call != PSA_SUCCESS)
        {
            pal_ipc_close(handle);
            return PAL_STATUS_ERROR;
        }
    }
   pal_ipc_close(handle);
   return PAL_STATUS_SUCCESS;
}

/**
    @brief           - Enables a hardware watchdog timer
    @param           - base_addr       : Base address of the watchdog module
    @return          - SUCCESS/FAILURE
**/
int pal_wd_timer_enable_ns(addr_t base_addr)
{
    wd_param_t              wd_param;
    psa_handle_t            handle = 0;
    psa_status_t            status_of_call = PSA_SUCCESS;

    wd_param.wd_fn_type = WD_ENABLE_SEQ;
    wd_param.wd_base_addr = base_addr;
    wd_param.wd_time_us = 0;
    wd_param.wd_timer_tick_us = 0;
    psa_invec invec[1] = {{&wd_param, sizeof(wd_param)}};

    handle = pal_ipc_connect(DRIVER_WATCHDOG_SID, 0);
    if (handle < 0)
    {
        return PAL_STATUS_ERROR;
    }
    else
    {
        status_of_call = pal_ipc_call(handle, invec, 1, NULL, 0);
        if (status_of_call != PSA_SUCCESS)
        {
            pal_ipc_close(handle);
            return PAL_STATUS_ERROR;
        }
    }
   pal_ipc_close(handle);
   return PAL_STATUS_SUCCESS;
}

/**
    @brief           - Disables a hardware watchdog timer
    @param           - base_addr  : Base address of the watchdog module
    @return          - SUCCESS/FAILURE
**/
int pal_wd_timer_disable_ns(addr_t base_addr)
{
    wd_param_t              wd_param;
    psa_handle_t            handle = 0;
    psa_status_t            status_of_call = PSA_SUCCESS;

    wd_param.wd_fn_type = WD_DISABLE_SEQ;
    wd_param.wd_base_addr = base_addr;
    wd_param.wd_time_us = 0;
    wd_param.wd_timer_tick_us = 0;
    psa_invec invec[1] = {{&wd_param, sizeof(wd_param)}};

    handle = pal_ipc_connect(DRIVER_WATCHDOG_SID, 0);
    if (handle < 0)
    {
        return PAL_STATUS_ERROR;
    }
    else
    {
        status_of_call = pal_ipc_call(handle, invec, 1, NULL, 0);
        if (status_of_call != PSA_SUCCESS)
        {
            pal_ipc_close(handle);
            return PAL_STATUS_ERROR;
        }
    }
   pal_ipc_close(handle);
   return PAL_STATUS_SUCCESS;
}

/**
    @brief    - Reads from given non-volatile address.
    @param    - base    : Base address of nvmem
                offset  : Offset
                buffer  : Pointer to source address
                size    : Number of bytes
    @return   - SUCCESS/FAILURE
**/
int pal_nvmem_read_ns(addr_t base, uint32_t offset, void *buffer, int size)
{
    nvmem_param_t   nvmem_param;
    psa_handle_t    handle = 0;
    psa_status_t    status_of_call = PSA_SUCCESS;

    nvmem_param.nvmem_fn_type = NVMEM_READ;
    nvmem_param.base = base;
    nvmem_param.offset = offset;
    nvmem_param.size = size;
    psa_invec invec[1] = {{&nvmem_param, sizeof(nvmem_param)}};
    psa_outvec outvec[1] = {{buffer, size}};

    handle = pal_ipc_connect(DRIVER_NVMEM_SID, 0);
    if (handle < 0)
    {
        return PAL_STATUS_ERROR;
    }
    else
    {
        status_of_call = pal_ipc_call(handle, invec, 1, outvec, 1);
        if (status_of_call != PSA_SUCCESS)
        {
            pal_ipc_close(handle);
            return PAL_STATUS_ERROR;
        }
    }
   psa_close(handle);
   return PAL_STATUS_SUCCESS;
}

/**
    @brief    - Writes into given non-volatile address.
    @param    - base    : Base address of nvmem
                offset  : Offset
                buffer  : Pointer to source address
                size    : Number of bytes
    @return   - SUCCESS/FAILURE
**/
int pal_nvmem_write_ns(addr_t base, uint32_t offset, void *buffer, int size)
{
    nvmem_param_t   nvmem_param;
    psa_handle_t    handle = 0;
    psa_status_t    status_of_call = PSA_SUCCESS;

    nvmem_param.nvmem_fn_type = NVMEM_WRITE;
    nvmem_param.base = base;
    nvmem_param.offset = offset;
    nvmem_param.size = size;
    psa_invec invec[2] = {{&nvmem_param, sizeof(nvmem_param)}, {buffer, size}};

    handle = pal_ipc_connect(DRIVER_NVMEM_SID, 0);
    if (handle < 0)
    {
        return PAL_STATUS_ERROR;
    }
    else
    {
        status_of_call = pal_ipc_call(handle, invec, 2, NULL, 0);
        if (status_of_call != PSA_SUCCESS)
        {
            pal_ipc_close(handle);
            return PAL_STATUS_ERROR;
        }
    }
   pal_ipc_close(handle);
   return PAL_STATUS_SUCCESS;
}

/**
 *   @brief    - This function will read peripherals using SPI commands
 *   @param    - addr : address of the peripheral
 *               data : read buffer
 *               len  : length of the read buffer in bytes
 *   @return   - error status
**/
int pal_spi_read(addr_t addr, uint8_t *data, uint32_t len)
{
    return 0xFF;
}
