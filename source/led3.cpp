#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"

struct box_context {
    Thread *thread;
};

int led3_val;

static const UvisorBoxAclItem acl[] = {
};

UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(4*1024);
UVISOR_BOX_CONFIG(box_led3, acl, UVISOR_BOX_STACK_SIZE, box_context);

static void led3_main(const void *)
{
    while (1) {
        led3_val = !led3_val;
        Thread::wait(500);
    }
}

UVISOR_EXTERN void __led3_init(void)
{
    uvisor_ctx->thread = new Thread(led3_main, NULL, osPriorityAboveNormal);
}

void led3_init(void)
{
    secure_gateway(box_led3, __led3_init);
}
