#include "Regular.h"
#include <iostream>

int main() {
    Regular reg = *(Regular::character('_') | Regular::alpha());
    auto nfa = reg.make();
    auto dfa = nfa.compileDFA();
    std::cout << nfa.match("4_asd") << std::endl;
    std::cout << nfa.match("_asd") << std::endl;
    std::cout << nfa.match("45678") << std::endl;
    std::cout << dfa.match("4_asd") << std::endl;
    std::cout << dfa.match("_asd") << std::endl;
    std::cout << dfa.match("45678") << std::endl;
    return 0;
}
