#pragma once

#include <vector>
#include <map>

#include "actor.h"

class syncer {
    int num_to_sync;
    vector<actor*> waiting_actors;
public:
    syncer(int num_to_sync) : num_to_sync(num_to_sync) {}

    void sync(actor* syncing_actor) {
        if (!--num_to_sync) {
            for (actor* sync_actor : waiting_actors) {
                sync_actor->unpause();
            }
        } else {
            waiting_actors.push_back(syncing_actor);
            syncing_actor->pause();
        }
    }
};
