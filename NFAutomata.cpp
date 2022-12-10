#include "NFAutomata.h"

bool NFAutomata::State::has(char key) const  {
    if (next.find(key) == next.end()) {
        return false;
    } else {
        return true;
    }
}

NFAutomata::State::List &NFAutomata::State::get(char key)  {
    return next[key];
}
