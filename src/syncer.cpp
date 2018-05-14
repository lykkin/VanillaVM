#include "actor.h"
#include "instruction.h"
#include "syncer.h"

void syncer::sync(actor* syncing_actor) {
    --num_to_sync;
    if (!num_to_sync) {
        for (actor* sync_actor : waiting_actors) {
            sync_actor->unpause();
        }
    } else {
        waiting_actors.push_back(syncing_actor);
        syncing_actor->pause();
    }
}
