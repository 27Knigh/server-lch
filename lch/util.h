#ifndef __LCH_UTIL_H__
#define __LCH_UTIL_H__

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdint.h>

namespace lch {

pid_t GetThreadId();

uint32_t GetFiberId();

}


#endif // !__LCH_UTIL_H__