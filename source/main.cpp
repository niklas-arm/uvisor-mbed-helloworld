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
#include "main-hw.h"
#include "box-challenge.h"
#include "box-debug.h"
#include "btn.h"

/* Create ACLs for main box. */
MAIN_ACL(g_main_acl);

/* Enable uVisor. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, g_main_acl);

DigitalOut led(MAIN_LED);

uint8_t g_challenge[CHALLENGE_SIZE];

static void toggle_led(void)
{
    led = !led;
}

static bool retry_secret(void)
{
    /* Check the secret. */
    printf("Verifying secret... ");
    bool verified = challenge_verify(g_challenge, sizeof(g_challenge));
    printf("%s\r\n", verified ? "Match" : "Mismatch");

    return verified;
}

int main(void)
{
    bool verified;

    printf("\r\n***** uvisor-helloworld example *****\r\n");

    /* Initialize the debug box. */
    box_debug::init();

    /* Reset the challenge. */
    memset(&g_challenge, 0, sizeof(g_challenge));

    /* Turn the LED off. */
    led = LED_OFF;

    /* Initialize the challenge. */
    challenge_init();

    /* Configure the pushbutton. */
    btn_init();

    printf("Main unprivileged box configured\r\n");

    while (1) {
        verified = retry_secret();
        if (verified) {
            while (1) {
                toggle_led();
                wait(0.1);
            }
        } else {
            toggle_led();
            wait(0.5);
        }
    }

    return 0;
}
