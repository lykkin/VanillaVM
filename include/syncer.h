#pragma once

#include <vector>

class actor;
class syncer {
    int num_to_sync;
    std::vector<actor*> waiting_actors;
public:
    syncer(int num_to_sync) : num_to_sync(num_to_sync) {}

    void sync(actor* syncing_actor);
};
