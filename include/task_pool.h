#pragma once

#include <vector>

#include "task.h"

using namespace std;
class task_pool {
    vector<task*> pool;
public:

    task_pool()
    {}

    task* acquire(actor* act) {
        if (pool.size() == 0) {
            return new task(act);
        } else {
            task* old_task = pool.back();
            pool.pop_back();
            new(&old_task) task(act);
            return old_task;
        }
    }

    void release(task* old_task) {
        pool.push_back(old_task);
    }
};
