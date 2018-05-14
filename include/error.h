#pragma once

#include <string>

class vanilla_error {
    std::string message;
public:
    vanilla_error(std::string message) : message(message) {}

    std::string get_message() {
        return message;
    }
};

class loading_error : public vanilla_error {
public:
    loading_error(std::string message) : vanilla_error(message) {}
};
