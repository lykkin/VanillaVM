#pragma once

#include <string>

class vanilla_error {
    string message;
public:
    vanilla_error(string message) : message(message) {}

    string get_message() {
        return message;
    }
};

class loading_error {
    string message;
public:
    loading_error(string message) : message(message) {}

    string get_message() {
        return message;
    }
};
