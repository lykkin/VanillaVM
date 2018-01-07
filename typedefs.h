#pragma once

#include <vector>
#include <string>

using namespace std;

class actor;
typedef function<void(actor&, vector<string>)> instruction_fn;
