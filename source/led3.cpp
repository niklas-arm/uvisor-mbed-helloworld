#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"
#include "main-hw.h"

struct box_context {
};

static const UvisorBoxAclItem acl[] = {
};

void led3_main();

UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(8192);
UVISOR_BOX_MAIN(led3_main, osPriorityLow);
UVISOR_BOX_CONFIG(box_led3, acl, UVISOR_BOX_STACK_SIZE, box_context);

void led3_main()
{
    DigitalOut led3(LED3);
    led3 = LED_OFF;

    while (1) {
        led3 = !led3;
        Thread::wait(500);
    }
}
