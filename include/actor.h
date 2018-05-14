#pragma once

#include <map>
#include <vector>
#include <string>

#include "instruction.h"

class actor {
    // id variables
    std::string name;
    //int id;

    // memory vars
    int x;
    int y;

    bool paused = false;

    std::map<std::string, int> context;
    std::map<std::string, int> labels;

    // instruction vars
    std::vector<instruction> instructions;
    int curr_ins;
public:
    actor(const actor& old) = delete;

    actor(int /*id*/, std::string name, int x = 0, int y = 0) :
        name(name),
        //id(id),
        x(x),
        y(y),
        curr_ins(0)
    {}

    ~actor()
    {}

    std::pair<int, int> get_coords();
    std::string get_name();

    bool is_paused();
    void pause();
    void unpause();

    int get_current_index();
    instruction get_current_instruction();
    void add_instruction(instruction ins);

    void label_last_instruction(std::string label);
    void jump(std::string label);
    bool has_label(std::string label);

    void move();

    std::map<std::string, int> get_context();
    void set_context(std::map<std::string, int> ctx);

    void execute();

    void print();
};
