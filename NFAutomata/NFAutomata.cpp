#include "NFAutomata.h"
#include <string>

bool NFAutomata::State::has(char key) const {
    if (next.find(key) == next.end()) {
        return false;
    } else {
        return true;
    }
}

NFAutomata::State::List &NFAutomata::State::get(char key) {
    return next[key];
}

void NFAutomata::closure(const std::shared_ptr<State> &state, Set &set) {
    set.insert(state);
    if (state->has('\0')) {
        for (auto &newState: state->get('\0')) {
            if (set.count(newState) == 0) {
                closure(newState, set);
            }
        }
    }
}

void NFAutomata::closure(const Set &from, Set &to) {
    for (const auto &state: from) {
        closure(state, to);
    }
}

void NFAutomata::move(char key, const Set &from, Set &to) {
    for (auto &state: from) {
        if (state->has(key)) {
            for (auto &newState: state->get(key)) {
                to.insert(newState);
            }
        }
    }
}

bool NFAutomata::match(const std::string &key) const {
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

DFAutomata NFAutomata::compileDFA() const {
    DFAutomata result;
    std::map<std::shared_ptr<State>, int> id_map;
    std::unordered_set<char> alphaTable;
    traverse(id_map, alphaTable);
    Set start_set, end_set, end_set_closure;
    closure(start, start_set);
    int start_id;
    result.make_node(mapping(start_set, id_map), start_id);
    std::queue<std::pair<int, Set>> queue;
    queue.emplace(start_id, start_set);
    while (!queue.empty()) {
        auto [node_id, node_set] = queue.front();
        queue.pop();
        for (auto &key: alphaTable) {
            end_set.clear();
            end_set_closure.clear();
            move(key, start_set, end_set);
            closure(end_set, end_set_closure);
            int end_id;
            if(result.make_node(mapping(end_set_closure, id_map), end_id))
            {
                queue.emplace(end_id, end_set_closure);
            }
            result.make_connection(node_id, key, end_id);
        }
    }
    result.check(id_map.find(accept)->second, alphaTable);
    return result;
}

void NFAutomata::traverse(std::map<std::shared_ptr<State>, int> &id_map, std::unordered_set<char> &alpha_table) const {
    std::queue<std::shared_ptr<State>> queue;
    int id = 0;
    id_map.clear();
    alpha_table.clear();
    id_map.insert(std::make_pair(start, ++id));
    push_all_child(start, queue, alpha_table);
    while (!queue.empty()) {
        auto ptr = queue.front();
        queue.pop();
        if (id_map.find(ptr) == id_map.end()) {
            push_all_child(ptr, queue, alpha_table);
            id_map.insert(std::make_pair(ptr, ++id));
        }
    }
}

void NFAutomata::push_all_child(const std::shared_ptr<State> &node_ptr, std::queue<std::shared_ptr<State>> &queue,
                                std::unordered_set<char> &alpha_table) {
    for (auto &pair: node_ptr->next) {
        alpha_table.insert(pair.first);
        for (auto &child: pair.second) {
            queue.emplace(child);
        }
    }
}
