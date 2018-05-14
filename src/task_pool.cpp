#include "task_pool.h"
#include "task.h"

task* task_pool::acquire(actor* act) {
    if (pool.size() == 0) {
        return new task(act);
    } else {
        task* old_task = pool.back();
        pool.pop_back();
        new(old_task) task(act);
        return old_task;
    }
}

void task_pool::release(task* old_task) {
    pool.push_back(old_task);
}
