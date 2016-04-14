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

void led1_main(const void *)
{
    led1 = LED_OFF;

    while (1) {
        led1 = !led1;
        Thread::wait(200);
    }
}

void led2_main(const void *)
{
    led2 = LED_OFF;

    while (1) {
        led2 = !led2;
        Thread::wait(300);
    }
}

void led3_main(const void *)
{
    led3 = LED_OFF;

    while (1) {
        led3 = !led3;
        Thread::wait(500);
    }
}

int main(void)
{
    printf("\r\n***** stupid uvisor-rtos example *****\r\n");

    Thread led1_thread(led1_main);
    Thread led2_thread(led2_main);
    Thread led3_thread(led3_main);

    while (1);

    return 0;
}
