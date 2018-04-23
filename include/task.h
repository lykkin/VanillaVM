#pragma once

#include <mutex>
#include <condition_variable>

#include "actor.h"

using namespace std;
class task {
    static int id_counter;
    actor* handle;
    mutex _m;
    bool done = false;
    condition_variable _cv;
public:
    int id;

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
        handle->execute();
        done = true;
        unique_lock<mutex> lock(_m);
        _cv.notify_one();
    }
};
