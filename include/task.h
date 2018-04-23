#pragma once

#include <mutex>
#include <condition_variable>

#include "actor.h"

using namespace std;
class task {
    static unsigned long long int id_counter;
    actor* handle;
    mutex _m;
    bool done = false;
    condition_variable _cv;
public:
    unsigned long long int id;

    task(actor* act) : handle(act), id(id_counter++) {
    }

    ~task() {
    }

    void wait() {
        unique_lock<mutex> lock(_m);
        _cv.wait(lock, [&](){
            return done;
        });
    }

    void execute() {
        unique_lock<mutex> lock(_m);
        handle->execute();
        done = true;
        _cv.notify_one();
    }
};
