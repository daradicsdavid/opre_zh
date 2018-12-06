#include <stdio.h>
#include "print/print.h"
#include "struct/struct.h"
#include "array/array.h"
#include "pipe/pipe.h"
#include "signal/zh_example/zh_example_signal.h"
#include "read/read.h"
#include "message_queue/zh/zh_example_signal.h"
#include "shared_memory/zh/zh_example_shared_memory.h"
#include "semaphore/zh/zh_example_semaphore.h"

int main() {
//    printExample();
//    structExample();
//    arrayExample();
//    pipeExample();
//    signalExample();
//    readExample();
//    messageQueueExampe();
//    signalExample();
//    sharedMemoryExample();
    semaphoreExample();
    return 0;
}