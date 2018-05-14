#include <iostream>

#include "instruction.h"
#include "actor.h"
#include "typedefs.h"

using namespace std;

void instruction::execute(actor& ins_actor) {
    fn(ins_actor, args);
}

int instruction::get_x_delta() {
    return direction.first;
}

string instruction::get_name() {
    return name;
}

int instruction::get_y_delta() {
    return direction.second;
}

void instruction::print() {
    cout << "  Instruction(" << name << "): " << endl;
    cout << "    <" << direction.first << ", " << direction.second << ">" << endl;
}
