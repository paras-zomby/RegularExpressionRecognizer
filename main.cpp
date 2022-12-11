#include "Regular.h"
#include <iostream>

int main() {
    Regular reg = *(Regular::character('_') | Regular::alpha()) + *Regular::digit();
    auto automata = reg.make();
    std::cout << automata.match("4_asd") << std::endl;
    std::cout << automata.match("_asd") << std::endl;
    std::cout << automata.match("45678") << std::endl;
    return 0;
}
