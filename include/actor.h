#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "instruction.h"
#include "typedefs.h"

using namespace std;

class actor {
    // id variables
    string name;
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

    actor(int id, string name, int x = 0, int y = 0) : name(name), id(id), x(x), y(y), curr_ins(0)
    {}

    ~actor()
    {}

    pair<int, int> get_coords() {
        return {x, y};
    }

    string get_name() {
        return name;
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

    int get_current_index() {
        return curr_ins;
    }

    instruction get_current_instruction() {
        return instructions[curr_ins];
    }

    void add_instruction(instruction ins) {
        instructions.push_back(ins);
    }

    void move() {
        auto ins = instructions[curr_ins];
        x += ins.get_x_delta();
        y += ins.get_y_delta();
        cout << "MOVING: " << id << ": " << ins.get_x_delta() << ", " << ins.get_y_delta() << endl;
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
