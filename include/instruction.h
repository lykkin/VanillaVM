#pragma once

#include <vector>
#include <string>

#include "typedefs.h"

class actor;

class instruction {
    std::string name;
    std::vector<std::string> args;
    std::pair<int, int> direction;
    instruction_fn fn;
public:
    instruction(std::pair<int, int> direction, instruction_fn fn, std::vector<std::string> args, std::string name):
        name(name),
        args(args),
        direction(direction),
        fn(fn)
    {}

    void execute(actor& ins_actor);
    int get_x_delta();
    std::string get_name();

    int get_y_delta();
    void print();
};
