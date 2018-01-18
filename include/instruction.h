#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "actor.h"
#include "typedefs.h"

using namespace std;

class instruction {
    string name;
    vector<string> args;
    pair<int, int> direction;
    instruction_fn fn;
public:
    instruction(pair<int, int> direction, instruction_fn fn, vector<string> args, string name):
        name(name),
        args(args),
        direction(direction),
        fn(fn)
    {}

    void execute(actor& ins_actor) {
        fn(ins_actor, args);
    }

    int get_x_delta() {
        return direction.first;
    }

    string get_name() {
        return name;
    }

    int get_y_delta() {
        return direction.second;
    }

    void print() {
        cout << "  Instruction(" << name << "): " << endl;
        cout << "    <" << direction.first << ", " << direction.second << ">" << endl;
    }
};
