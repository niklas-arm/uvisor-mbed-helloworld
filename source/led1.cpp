#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"
#include "main-hw.h"

struct box_context {
    Thread *thread;
    uint32_t heartbeat;
};

static const UvisorBoxAclItem acl[] = {
};

UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(8192);
UVISOR_BOX_CONFIG(box_led1, acl, UVISOR_BOX_STACK_SIZE, box_context);

static void led1_main(const void *)
{
    DigitalOut led1(LED1);
    led1 = LED_OFF;

    while (1) {
        led1 = !led1;
        ++uvisor_ctx->heartbeat;
        Thread::wait(200);
    }
}

/*
 * The *_init functions will look different when uVisor has the box init
 * feature.
 */
UVISOR_EXTERN void __led1_init(void)
{
    uvisor_ctx->thread = new Thread(led1_main, NULL, osPriorityBelowNormal);
}

void led1_init(void)
{
    secure_gateway(box_led1, __led1_init);
}
