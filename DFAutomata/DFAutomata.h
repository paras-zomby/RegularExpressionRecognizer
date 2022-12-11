#ifndef REGULAREXPRESSIONRECOGNIZER_DFAUTOMATA_H
#define REGULAREXPRESSIONRECOGNIZER_DFAUTOMATA_H

#include <map>
#include <memory>
#include <unordered_set>
#include <vector>

class DFAutomata {
private:
    struct State {
        std::map<char, int> next;

        std::unordered_set<int> node_id;
        State() = default;
        State(const std::unordered_set<int> &node_ids);
    };
    std::vector<State> nodes;
    std::unordered_set<int> accept_nodes;

public:

    DFAutomata() = default;
    [[nodiscard]] bool match(const std::string& key) const;
    bool make_node(const std::unordered_set<int>& old_nodes_id, int& node_id);
    void make_connection(int from_id, char character, int to_id);
    void check(int end_node_id, const std::unordered_set<char> &alpha_table);
};


#endif //REGULAREXPRESSIONRECOGNIZER_DFAUTOMATA_H
