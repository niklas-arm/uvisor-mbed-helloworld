#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"

struct box_context {
    Thread *thread;
};

int led2_val;

static const UvisorBoxAclItem acl[] = {
};

UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_CONFIG(box_led2, acl, UVISOR_BOX_STACK_SIZE, box_context);

static void led2_main(const void *)
{
    while (1) {
        led2_val = !led2_val;
        Thread::wait(300);
    }
}

UVISOR_EXTERN void __led2_init(void)
{
    uvisor_ctx->thread = new Thread(led2_main, NULL, osPriorityAboveNormal);
}

void led2_init(void)
{
    secure_gateway(box_led2, __led2_init);
}
