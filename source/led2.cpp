#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"
#include "main-hw.h"

struct box_context {
};

static const UvisorBoxAclItem acl[] = {
};

void led2_main();

// UVISOR_BOX_NAMESPACE(NULL);
// UVISOR_BOX_HEAPSIZE(8192);
// UVISOR_BOX_MAIN(led2_main, osPriorityAboveNormal);
// UVISOR_BOX_CONFIG(box_led2, acl, UVISOR_BOX_STACK_SIZE, box_context);

void led2_main()
{
    DigitalOut led2(LED2);
    led2 = LED_OFF;

    while (1) {
        led2 = !led2;
        int32_t ii = 300000;
        while(ii-- > 0) osThreadYield();
        // Thread::wait(300);
    }
}
