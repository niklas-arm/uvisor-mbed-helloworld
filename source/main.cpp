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

typedef struct {
    uint8_t type;
    uint8_t reserved[3];
} UvisorBoxAclType;

void led_main(const void *num)
{
    DigitalOut led((PinName)((int)num));
    led = LED_OFF;

    while (1) {
        led = !led;
        uint8_t *p = new uint8_t[rand()%500];
        Thread::wait(200);
        delete p;
    }
}

int main(void)
{
    printf("\r\n***** stupid uvisor-rtos example *****\r\n");

    rtos::Thread t(led_main, (void *)LED1);
    Thread::wait(200);
    rtos::Thread t2(led_main, (void *)LED2);

    while (1) {
        // osThreadYield();
        osDelay(10000);
        // int32_t ii = 300000;
        // while(ii-- > 0) ;
    }

    return 0;
}
