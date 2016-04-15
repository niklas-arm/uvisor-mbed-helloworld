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

const unsigned int kB = 1024;

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

static void dump(const UvisorPageTable *const table)
{
    int table_size = table->page_count;
    if (!table_size || !table->page_size) {
        printf("pages_dump: FAIL: Page table empty!\n");
        return;
    }
    printf("pages_dump: Table with %d pages of size %ukB\n", table_size, table->page_size/kB);
    for (void* const *page = table->page_origins; table_size > 0 && *page; page++, table_size--)
    {
        printf("pages_dump: Page #%u at %p\n", page - table->page_origins, *page);
    }
    printf("\n");
}

int main(void)
{
    bool verified;
    extern serial_t stdio_uart;
    serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
    serial_baud(&stdio_uart, 115200);

    printf("\r\n***** uvisor-helloworld example *****\r\n");

    /* I want to see some page fragmentation happening here */
    /* array for keeping track of page tables */
    UVISOR_PAGE_TABLE(7) request[10];
    /* allocate 1 page */
    request[0].page_size = UVISOR_PAGE_SIZE;
    request[0].page_count = 1;
    uvisor_page_malloc((UvisorPageTable*)&request[0]); dump((UvisorPageTable*)&request[0]);
    /* allocate 1 page */
    request[1].page_size = UVISOR_PAGE_SIZE;
    request[1].page_count = 1;
    uvisor_page_malloc((UvisorPageTable*)&request[1]); dump((UvisorPageTable*)&request[1]);
    /* allocate 2 pages */
    request[2].page_size = UVISOR_PAGE_SIZE;
    request[2].page_count = 2;
    uvisor_page_malloc((UvisorPageTable*)&request[2]); dump((UvisorPageTable*)&request[2]);
    /* allocate 1 page */
    request[3].page_size = UVISOR_PAGE_SIZE;
    request[3].page_count = 1;
    uvisor_page_malloc((UvisorPageTable*)&request[3]); dump((UvisorPageTable*)&request[3]);
    /* allocate 2 page */
    request[4].page_size = UVISOR_PAGE_SIZE;
    request[4].page_count = 1;
    uvisor_page_malloc((UvisorPageTable*)&request[4]); dump((UvisorPageTable*)&request[4]);
    /* free 1 page */
    uvisor_page_free((UvisorPageTable*)&request[1]);
    /* free 1 page */
    uvisor_page_free((UvisorPageTable*)&request[3]);

    UvisorAllocator thread_allocator = uvisor_allocator_create_with_pages(40*kB, 8*kB);
    uint16_t *mem = (uint16_t*)uvisor_malloc(thread_allocator, 1360);

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
