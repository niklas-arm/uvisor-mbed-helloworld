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
#include "btn.h"
#include "main-hw.h"
#include "box-challenge.h"

InterruptIn btn(MAIN_BTN);
Timeout timeout;

static void btn_onpress(void)
{
    /* Attempt to copy from box_challenge context. We know the context is
     * properly aligned so we try to obtain a carbon copy of its memory
     * location. */
    printf("Attempting to read the secret... ");
    memcpy(&g_challenge, g_box_context, sizeof(g_challenge));
    printf("Access granted!\r\n");
}

void btn_init(void)
{
    /* Configure the pushbutton. */
    btn.mode(MAIN_BTN_PUPD);

    wait(0.2);
    btn.fall(&btn_onpress);

    /* Set a callback to do the equivalent of pushing the button for us. Every
     * period, press the button automatically. Pressing the button
     * automatically after a certain period of time allows us to trigger button
     * presses even for platforms without physically pressable buttons. */
    timeout.attach(&btn_onpress, 10.0);
}
