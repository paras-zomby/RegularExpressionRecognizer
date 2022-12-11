#include "NFAutomata.h"
#include <string>

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

void NFAutomata::closure(const std::shared_ptr<State>& state, Set& set) {
    set.insert(state);
    if (state->has('\0')) {
        for (auto& newState : state->get('\0')) {
            if (set.count(newState) == 0) {
                closure(newState, set);
            }
        }
    }
}

void NFAutomata::closure(const Set& from, Set& to) {
    for (const auto& state : from) {
        closure(state, to);
    }
}

void NFAutomata::move(char key, const Set& from, Set& to) {
    for (auto& state : from) {
        if (state->has(key)) {
            for (auto& newState : state->get(key)) {
                to.insert(newState);
            }
        }
    }
}

bool NFAutomata::match(const std::string& key) const {
    Set s1;
    closure(start, s1);

    auto iter = key.begin();
    bool matched = false;
    while (iter != key.end() && !s1.empty()) {
        Set s2;
        move(*iter, s1, s2);
        s1.clear();
        closure(s2, s1);
        ++iter;
        if (s1.count(accept) != 0) {
            matched = true;
        }
    }
    return matched;
}
