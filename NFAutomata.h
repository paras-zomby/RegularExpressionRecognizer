#ifndef REGULAREXPRESSIONRECOGNIZER_NFAUTOMATA_H
#define REGULAREXPRESSIONRECOGNIZER_NFAUTOMATA_H

#include <memory>
#include <deque>
#include <map>
#include <unordered_set>

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
};

#endif //REGULAREXPRESSIONRECOGNIZER_NFAUTOMATA_H
