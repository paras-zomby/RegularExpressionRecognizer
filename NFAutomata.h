#ifndef REGULAREXPRESSIONRECOGNIZER_NFAUTOMATA_H
#define REGULAREXPRESSIONRECOGNIZER_NFAUTOMATA_H

#include <memory>
#include <deque>
#include <map>
#include <unordered_set>
#include <queue>
#include "DFAutomata.h"

class NFAutomata {
private:
    struct State {
        using Ptr = std::shared_ptr<State>;
        using List = std::deque<Ptr>;
        using Map = std::map<char, List>;

        Map next;//一开始是用multimap的，然而写出来的代码格外丑，于是就改成了这样

        [[nodiscard]] bool has(char key) const;
        List& get(char key);
    };

private:
    using Set = std::unordered_set<std::shared_ptr<State>>;
    static void closure(const std::shared_ptr<State>& state, Set& set);
    static void closure(const Set& from, Set& to);
    static void move(char key, const Set& from, Set& to);
public:
    std::shared_ptr<State> start;
    std::shared_ptr<State> accept;

public:
    NFAutomata():
        start(new State()),
        accept(new State()) {}

    NFAutomata(
        std::shared_ptr<State>& start,
        std::shared_ptr<State>& accept
    ) : start(start), accept(accept) {}

    [[nodiscard]] bool match(const std::string& key) const;
    [[nodiscard]] DFAutomata compileDFA() const;
private:
    void traverse(std::map<std::shared_ptr<State>, int>& id_map, std::unordered_set<char>& alpha_table) const;
    static void push_all_child(const std::shared_ptr<State> &node_ptr, std::queue<std::shared_ptr<State>> &queue, std::unordered_set<char>& alpha_table);

    template<typename From, typename To>
    static std::unordered_set<To> mapping(const std::unordered_set<From> &from, const std::map<From, To>& key_map)
    {
        std::unordered_set<To> result;
        result.reserve(from.size());
        for (auto& f: from) {
            result.insert(key_map.find(f)->second);
        }
        return result;
    }
};

#endif //REGULAREXPRESSIONRECOGNIZER_NFAUTOMATA_H
