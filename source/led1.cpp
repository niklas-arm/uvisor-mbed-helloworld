#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"

struct box_context {
    Thread *thread;
};

int led1_val;

static const UvisorBoxAclItem acl[] = {
};

UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(0);
UVISOR_BOX_CONFIG(box_led1, acl, UVISOR_BOX_STACK_SIZE, box_context);

static void led1_main(const void *)
{
    while (1) {
        led1_val = !led1_val;
        Thread::wait(200);
    }
}

UVISOR_EXTERN void __led1_init(void)
{
    uvisor_ctx->thread = new Thread(led1_main, NULL, osPriorityAboveNormal);
}

void led1_init(void)
{
    secure_gateway(box_led1, __led1_init);
}
