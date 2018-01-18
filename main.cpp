#include <iostream>
#include <vector>
#include <regex>
#include <list>
#include <map>
#include <condition_variable>
#include <string>
#include <utility>
#include <mutex>
#include <fstream>
#include <thread>

#include "include/actor.h"
#include "include/error.h"
#include "include/task.h"
#include "include/task_pool.h"
#include "include/instruction.h"
#include "include/syncer.h"

int task::id_counter = 0;

using namespace std;

map<pair<int, int>, map<string, int>> memory;
map<pair<string, int>, syncer> sync_map;
vector<actor*> actors;

/* TEMPLATE
 * {
 *     "select", [](actor& ins_actor, vector<string> args) {
 *     }
 * }
 */

pair<string, instruction_producer> create_instruction_producer(string name, instruction_verifier verifier, string fail_message, instruction_fn ins_fn) {
    return {
        name,
        [=](actor& ins_actor, vector<string> args, pair<int, int> direction){
            if (!verifier(ins_actor, args)) {
                // is there a more graceful way to handle this?
                throw loading_error(name + " " + fail_message);
            }

            return instruction(direction, ins_fn, args, name);
        }
    };
}

map<string, instruction_producer> instruction_map {
    create_instruction_producer(
        "drop",
        [](actor& ins_actor, vector<string> args) {
            return args.size() == 0;
        },
        "TAKES NO ARGUMENTS",
        [](actor& ins_actor, vector<string> args){
            memory[ins_actor.get_coords()] = ins_actor.get_context();
            ins_actor.get_context().clear();
        }
    ),
    create_instruction_producer(
        "grab",
        [](actor& ins_actor, vector<string> args) {
            return args.size() == 0;
        },
        "TAKES NO ARGUMENTS",
        [](actor& ins_actor, vector<string> args){
            ins_actor.set_context(memory[ins_actor.get_coords()]);
            memory[ins_actor.get_coords()]; 
        }
    ),
    create_instruction_producer(
        "select",
        [](actor& ins_actor, vector<string> args) {
            return args.size() == 1;
        },
        "TAKES ONE ARGUMENT",
        [](actor& ins_actor, vector<string> args){
            auto context = memory[ins_actor.get_coords()];
            ins_actor.get_context()[args[0]] = context[args[0]];
        }
    ),
    create_instruction_producer(
        "write",
        [](actor& ins_actor, vector<string> args) {
            return args.size() == 1;
        },
        "TAKES ONE ARGUMENT",
        [](actor& ins_actor, vector<string> args){
            auto context = memory[ins_actor.get_coords()];
            context[args[0]] = ins_actor.get_context()[args[0]];
        }
    ),
    create_instruction_producer(
        "print",
        [](actor& ins_actor, vector<string> args) {
            return args.size() == 0;
        },
        "TAKES NO ARGUMENTS",
        [](actor& ins_actor, vector<string> args){
            ins_actor.print();
        }
    ),
    create_instruction_producer(
        "noop",
        [](actor& ins_actor, vector<string> args) {
            return args.size() == 0;
        },
        "TAKES NO ARGUMENTS",
        [](actor& ins_actor, vector<string> args){
        }
    ),
    create_instruction_producer(
        "jump",
        [](actor& ins_actor, vector<string> args) {
            return args.size() == 1 && ins_actor.has_label(args[0]);
        },
        "TAKES ONE ARGUMENT AND A LABEL DEFINED BEFORE THE JUMP STATEMENT",
        [](actor& ins_actor, vector<string> args){
            ins_actor.jump(args[0]);
        }
    ),
    create_instruction_producer(
        "sync",
        [](actor& ins_actor, vector<string> args) {
            auto size = args.size();
            return size == 1 || size == 2;
        },
        "TAKES ONE OR TWO ARGUMENTS",
        [](actor& ins_actor, vector<string> args){
            int num_to_sync = args.size() == 1 ? stoi(args[1]) : actors.size();
            pair<string, int> sync_key = {args[0], num_to_sync};
            auto map_iter = sync_map.find(sync_key);
            if (map_iter == sync_map.end()) {
                syncer s(num_to_sync);
                s.sync(&ins_actor);
                sync_map.emplace(sync_key, s);
            } else {
                map_iter->second.sync(&ins_actor);
            }
        }
    )
};

void print_fail_message(actor* act, int line_number, string message) {
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "FIRE REQUIRED" << endl;
        return 1;
    }

    map<pair<int, int>, vector<task*>> occupancy_graph;

    for (int i = 1; i < argc; i++) {
        fstream input(argv[i]);

        if (!input.is_open()) {
            cout << "FIRE DOESN'T EXIST: " << argv[i] << endl;
            return 1;
        }

        // Instruction parser
        string line;
        regex parser("(\\S+)");
        smatch results;

        actor* curr_actor = new actor(i, argv[i]);

        int line_number = 0;
        while (getline(input, line)) {
            ++line_number;
            auto words_begin = sregex_iterator(line.begin(), line.end(), parser);
            auto words_end = sregex_iterator();

            string possible_label = words_begin->str();
            bool labeled = false;
            string direction_word;
            if (possible_label[possible_label.length() - 1] == ':') {
                possible_label = possible_label.substr(0, possible_label.length() - 1);
                labeled = true;
                ++words_begin;
                direction_word = words_begin->str();
            } else {
                direction_word = possible_label;
            }

            // Pull out the movement direction
            pair<int, int> dir;

            if (direction_word == "up") {
                dir = {0, 1};
            } else if (direction_word == "down") {
                dir = {0, -1};
            } else if (direction_word == "left") {
                dir = {-1, 0};
            } else if (direction_word == "right") {
                dir = {1, 0};
            } else {
                cout << direction_word << " IS NOT A VALID DIRECTION" << endl;
                return 1;
            }

            // Pull out the instruction name
            ++words_begin;
            string ins_name = words_begin->str();
            instruction_producer producer = instruction_map[ins_name];

            if (!producer) {
                cout << ins_name << " IS NOT A VALID INSTRUCTION (" << curr_actor->get_name() << " line " << line_number << ")" << endl;
                return 1;
            }

            // Pull out the arguments
            ++words_begin;
            vector<string> args;
            for (sregex_iterator i = words_begin; i != words_end; ++i) {
                args.push_back(i->str());
            }

            // Register the instruction with the actor associated with
            // the file.
            try {
                instruction curr_ins = producer(*curr_actor, args, dir);
                curr_actor->add_instruction(curr_ins);
                if (labeled) {
                    curr_actor->label_last_instruction(possible_label);
                }
            } catch (loading_error e) {
                cout << "FAILED WHILE LOADING INSTRUCTION (" << curr_actor->get_name() << " line " << line_number << "): " << e.get_message() << endl;
                return 1;
            }
        }

        // Keep track of the constructed actor.
        actors.push_back(curr_actor);
    }

    list<vector<task*>> job_queue;
    vector<thread> workers;

    condition_variable worker_cv;
    mutex job_queue_mutex;

    for (int i = 0; i < 1; i++) {
        workers.emplace_back([&](){
            unique_lock<mutex> queue_lock(job_queue_mutex, defer_lock);
            while (true) {
                // Wait for work
                queue_lock.lock();
                worker_cv.wait(queue_lock, [&](){
                    cout << "WORKER: WAITING " << job_queue.size() << endl;
                    return !job_queue.empty();
                });
                cout << "WORKER: STARTING" << endl; 
                // Take a job and unlock the queue
                auto worker_tasks = job_queue.front(); 
                job_queue.pop_front();
                cout << "AFTER " << job_queue.size() << endl;
                queue_lock.unlock();

                for (task* curr_task : worker_tasks) {
                    cout << "WORKER: EXECUTING " << curr_task->id << endl;
                    curr_task->execute();
                    cout << "WORKER: EXECUTED " << curr_task->id << endl;
                }
            }
        });
    }

    unique_lock<mutex> queue_lock(job_queue_mutex, defer_lock);
    vector<task*> tick_tasks;
    while (true) {
        // Move the actors and mark their cells they land in.
        for (actor* curr_actor : actors) {
            if (!curr_actor->is_paused()) {
                curr_actor->move();
                auto cell_actors = &occupancy_graph[curr_actor->get_coords()];
                task* new_task = new task(curr_actor);
                tick_tasks.push_back(new_task);
                cell_actors->push_back(new_task);
            }
        }

        queue_lock.lock();
        for (auto i = occupancy_graph.begin(); i != occupancy_graph.end(); ++i) {
            job_queue.push_back(i->second);
        }
        cout << "MAIN: NOTIFY" << endl;
        worker_cv.notify_all();
        queue_lock.unlock();

        for (task* tick_task : tick_tasks) {
            cout << "DONE " << tick_task->id << endl;
            tick_task->wait();
            cout << "DELETE " << tick_task->id << endl;
            delete tick_task;
        }

        occupancy_graph.clear();
        tick_tasks.clear();

        cout << "MAIN: END" << endl;
    }

    return 0;
}
