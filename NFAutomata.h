#ifndef REGULAREXPRESSIONRECOGNIZER_NFAUTOMATA_H
#define REGULAREXPRESSIONRECOGNIZER_NFAUTOMATA_H

#include <memory>
#include <deque>
#include <map>

class NFAutomata {
    struct State {
        using Ptr = std::shared_ptr<State>;
        using List = std::deque<Ptr>;
        using Map = std::map<char, List>;

        Map next;//一开始是用multimap的，然而写出来的代码格外丑，于是就改成了这样

        [[nodiscard]] bool has(char key) const;
        List& get(char key);
    };
public:
    NFAutomata():
        start(new State()),
        accept(new State()) {}

    NFAutomata(
        std::shared_ptr<State>& start,
        std::shared_ptr<State>& accept
    ) : start(start), accept(accept) {}

    std::shared_ptr<State> start;
    std::shared_ptr<State> accept;
};

#endif //REGULAREXPRESSIONRECOGNIZER_NFAUTOMATA_H
