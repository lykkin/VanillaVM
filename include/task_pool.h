#pragma once

#include <vector>

class task;
class actor;
class task_pool {
    std::vector<task*> pool;
public:

    task_pool()
    {}

    task* acquire(actor* act);

    void release(task* old_task);
};
