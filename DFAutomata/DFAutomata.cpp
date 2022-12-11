#include "DFAutomata.h"
#include <string>

bool DFAutomata::match(const std::string &key) const {
    int node_id = 0;
    for (auto &ch: key) {
        auto iter = nodes[node_id].next.find(ch);
        if (iter == nodes[node_id].next.end() || iter->second == nodes.size() - 1)
            return false;
        node_id = iter->second;
    }
    return accept_nodes.find(node_id) != accept_nodes.end();
}

bool DFAutomata::make_node(const std::unordered_set<int> &old_nodes_id, int &node_id) {
    for (auto i = 0; i < nodes.size(); ++i) {
        if (nodes[i].node_id == old_nodes_id) {
            node_id = i;
            return false;
        }
    }

    node_id = nodes.size();
    nodes.emplace_back(old_nodes_id);
    return true;
}

void DFAutomata::make_connection(int from_id, char character, int to_id) {
    nodes[from_id].next.insert({character, to_id});
}

void DFAutomata::check(int end_node_id, const std::unordered_set<char> &alpha_table) {
    for (int i = 0; i < nodes.size() - 1; ++i) {
        if (nodes[i].node_id.find(end_node_id) != nodes[i].node_id.end())
            accept_nodes.emplace(i);
    }
}

DFAutomata::State::State(const std::unordered_set<int> &node_ids) : node_id(node_ids) {

}
