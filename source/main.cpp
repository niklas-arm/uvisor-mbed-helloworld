/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "led1.h"
#include "led2.h"
#include "led3.h"
#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"
#include "main-hw.h"

/* Create ACLs for main box. */
MAIN_ACL(g_main_acl);

/* Enable uVisor. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, g_main_acl);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

extern int led1_val;
extern int led2_val;
extern int led3_val;

void led_blinker(const void *)
{
    led1 = LED_OFF;
    led2 = LED_OFF;
    led3 = LED_OFF;

    while (1) {
        led1 = led1_val;
        led2 = led2_val;
        led3 = led3_val;
        Thread::wait(100);
    }
}

void malloc_battle(const void*)
{
    while(1) {
        void *ptr = malloc_p(rand() % 10000);
        free_p(ptr);
    }
}

int main(void)
{
    extern serial_t stdio_uart;
    serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
    serial_baud(&stdio_uart, 115200);
    printf("\r\n***** stupid uvisor-rtos example *****\r\n");

    // Thread led_blinker_thread(led_blinker);
    BaseThread led_blinker_thread(led_blinker, 40*1024, 8*1024);
    led_blinker_thread.start(osPriorityHigh);

    Thread mthread(malloc_battle);
    Thread mthread2(malloc_battle);

    led1_init();
    led2_init();
    led3_init();


    while (1);

    return 0;
}
