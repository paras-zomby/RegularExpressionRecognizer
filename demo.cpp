#include "Regular.h"
#include <iostream>

int main() {
    auto ch0 = Regular::character('0');
    auto ch1 = Regular::character('1');
    Regular reg = (*(ch0 | ch1) + ch0) | *(ch0 + ch0);
    auto nfa = reg.compileNFA();
    auto dfa = nfa.compileDFA();
    std::cout << "[NFA]0000 : " << nfa.match("0000") << std::endl;
    std::cout << "[NFA]10010: " << nfa.match("10010") << std::endl;
    std::cout << "[NFA]00110: " << nfa.match("00110") << std::endl;
    std::cout << "[NFA]1111 : " << nfa.match("1111") << std::endl;
    std::cout << "[DFA]0000 : " << dfa.match("0000") << std::endl;
    std::cout << "[DFA]10010: " << dfa.match("10010") << std::endl;
    std::cout << "[DFA]00110: " << dfa.match("00110") << std::endl;
    std::cout << "[DFA]1111 : " << dfa.match("1111") << std::endl;
    return 0;
}
