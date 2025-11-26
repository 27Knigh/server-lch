#include "lch/lch.h"

lch::Logger::ptr g_logger = LCH_LOG_ROOT();

void fun1() {
    LCH_LOG_INFO(g_logger)  << "name: " << lch::Thread::GetName()
                            << " this.name: " << lch::Thread::GetThis()->getName()
                            << " id: " << lch::GetThreadId()
                            << " this.id: " << lch::Thread::GetThis()->getId();
    sleep(1000);
}

void fun2() {
}

int main(int argc, char** argv) {
    LCH_LOG_INFO(g_logger) << "thread test begin";
    std::vector<lch::Thread::ptr> thrs;
    for (int i = 0; i < 5; ++i) {
        lch::Thread::ptr thr(new lch::Thread(&fun1, "name_" + std::to_string(i)));
        thrs.push_back(thr);
    }

    for (int i = 0; i < 5; ++i) {
        thrs[i]->join();
    }
    LCH_LOG_INFO(g_logger) << "thread test end";
    return 0;
}