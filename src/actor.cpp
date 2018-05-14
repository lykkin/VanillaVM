#include <iostream>

#include "actor.h"
#include "instruction.h"
#include "typedefs.h"

pair<int, int> actor::get_coords() {
    return {x, y};
}

string actor::get_name() {
    return name;
}

bool actor::is_paused() {
    return paused;
}

void actor::pause() {
    paused = true;
}

void actor::unpause() {
    paused = false;
}

int actor::get_current_index() {
    return curr_ins;
}

instruction actor::get_current_instruction() {
    return instructions[curr_ins];
}

void actor::add_instruction(instruction ins) {
    instructions.push_back(ins);
}

void actor::label_last_instruction(string label) {
    labels[label] = instructions.size() - 1;
}

void actor::jump(string label) {
    curr_ins = labels[label];
}

bool actor::has_label(string label) {
    return labels.find(label) != labels.end();
}

void actor::move() {
    auto ins = instructions[curr_ins];
    x += ins.get_x_delta();
    y += ins.get_y_delta();
    //cout << "MOVING: " << id << ": " << ins.get_x_delta() << ", " << ins.get_y_delta() << endl;
    //cout << "HERE: " << id << ": " << x << ", " << y << endl;
}

map<string, int> actor::get_context() {
    return context;
}

void actor::set_context(map<string, int> ctx) {
    context = ctx;
}

void actor::execute() {
    auto ins = instructions[curr_ins++];
    curr_ins %= instructions.size();
    ins.execute(*this);
}

void actor::print() {
    auto ins = instructions[curr_ins];
    //cout << "Actor " << id << ": <" << x << ", " << y << ">:" << endl;
    ins.print();
    //cout << endl;
}
