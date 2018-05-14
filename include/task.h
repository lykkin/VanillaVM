#pragma once

#include <mutex>
#include <condition_variable>

class actor;
class task {
    static unsigned long long int id_counter;
    actor* handle;
    std::mutex _m;
    bool done = false;
    std::condition_variable _cv;
public:
    unsigned long long int id;

    task(actor* act) : handle(act), id(id_counter++) {}

    ~task() {}

    void wait();
    void execute();
};
