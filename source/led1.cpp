#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"
#include "main-hw.h"

struct box_context {
};

static const UvisorBoxAclItem acl[] = {
};

void led1_main();

// UVISOR_BOX_NAMESPACE(NULL);
// UVISOR_BOX_HEAPSIZE(8192);
// UVISOR_BOX_MAIN(led1_main, osPriorityAboveNormal);
// UVISOR_BOX_CONFIG(box_led1, acl, UVISOR_BOX_STACK_SIZE, box_context);

void led1_main()
{
    DigitalOut led1(LED1);
    led1 = LED_OFF;

    while (1) {
        led1 = !led1;
        uint8_t *p = new uint8_t[rand()%500];
        Thread::wait(200);
        delete p;
    }
}
