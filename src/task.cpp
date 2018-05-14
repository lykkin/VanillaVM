#include "task.h"
#include "actor.h"

unsigned long long int task::id_counter = 0;

using namespace std;
void task::wait() {
    unique_lock<mutex> lock(_m);
    _cv.wait(lock, [&](){
        return done;
    });
}

void task::execute() {
    unique_lock<mutex> lock(_m);
    handle->execute();
    done = true;
    _cv.notify_one();
}
