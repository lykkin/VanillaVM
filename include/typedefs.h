#pragma once

#include <vector>
#include <string>

using namespace std;

class instruction;
class actor;
typedef function<void(actor&, vector<string>)> instruction_fn;
typedef function<instruction(actor&, vector<string>, pair<int, int>)> instruction_producer;

typedef function<bool(vector<string>)> instruction_verifier;
