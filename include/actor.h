#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "instruction.h"
#include "typedefs.h"

using namespace std;

class actor {
    int id;
    // memory vars
    int x;
    int y;

    bool paused = false;

    map<string, int> context;

    // instruction vars
    vector<instruction> instructions;
    int curr_ins;
public:
    actor(const actor& old) = delete;

    actor(int id,  int x = 0, int y = 0) : id(id), x(x), y(y)
    {curr_ins = 0;}

    ~actor()
    {}

    pair<int, int> get_coords() {
        return {x, y};
    }

    bool is_paused() {
        return paused;
    }

    void pause() {
        paused = true;
    }

    void unpause() {
        paused = false;
    }

    void add_instruction(pair<int, int> direction, instruction_fn fn, vector<string> args, string name) {
        instructions.push_back(instruction(direction, fn, args, name));
    }

    void move() {
        auto ins = instructions[curr_ins];
        x += ins.getXDelta();
        y += ins.getYDelta();
        cout << "MOVING: " << id << ": " << ins.getXDelta() << ", " << ins.getYDelta() << endl;
        cout << "HERE: " << id << ": " << x << ", " << y << endl;
    }

    map<string, int> getContext() {
        return context;
    }

    void setContext(map<string, int> ctx) {
        context = ctx;
    }

    void execute() {
        auto ins = instructions[curr_ins++];
        curr_ins %= instructions.size();
        ins.execute(*this);
    }

    void print() {
        auto ins = instructions[curr_ins];
        cout << "Actor " << id << ": <" << x << ", " << y << ">:" << endl;
        ins.print();
        cout << endl;
    }
};
