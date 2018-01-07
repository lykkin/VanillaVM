#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "actor.h"
#include "typedefs.h"

using namespace std;

class instruction {
    pair<int, int> direction;
    instruction_fn fn;
    vector<string> args;
    string name;
public:
    instruction(pair<int, int> direction, instruction_fn fn, vector<string> args, string name):
        direction(direction),
        fn(fn),
        args(args),
        name(name)
    {}

    void execute(actor& ins_actor) {
        fn(ins_actor, args);
    }

    int getXDelta() {
        return direction.first;
    }

    int getYDelta() {
        return direction.second;
    }

    void print() {
        cout << "  Instruction(" << name << "): " << endl;
        cout << "    <" << direction.first << ", " << direction.second << ">" << endl;
    }
};
