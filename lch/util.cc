#include "util.h"

pid_t lch::GetThreadId() {
    return syscall(SYS_gettid);
}

uint32_t lch::GetFiberId() {
    return 0;
}