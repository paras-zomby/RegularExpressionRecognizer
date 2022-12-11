#include "Regular.h"

std::array<std::shared_ptr<Regular::Char>, 128> Regular::makeCharTable() {
    std::array<std::shared_ptr<Char>, 128> charTable;
    for (unsigned char i = 0; i < 128; ++i) {
        charTable[i] = std::make_shared<Char>();
        charTable[i]->ch = i;
    }

    return charTable;
}

Regular Regular::range(char begin, char end) {
    Regular result = character(begin);
    for (char i = begin + 1; i <= end; ++i) {
        result = result | character(i);
    }
    return result;
}

Regular Regular::operator+() const {
    auto nodet = std::make_shared<PClosure>();
    nodet->node = this->node;
    return Regular(nodet);
}

Regular Regular::operator*() const {
    return nil() | +(*this);
}

Regular Regular::operator+(const Regular &other) const {
    auto nodet = std::make_shared<And>();
    nodet->node[0] = this->node;
    nodet->node[1] = other.node;

    return Regular(nodet);
}

Regular Regular::operator|(const Regular &other) const {
    auto nodet = std::make_shared<Or>();
    nodet->node[0] = this->node;
    nodet->node[1] = other.node;

    return Regular(nodet);
}

Regular Regular::character(char ch) {
    static auto charTable = makeCharTable();
    return Regular(charTable[ch]);
}
Regular Regular::nil() {
    return character('\0');
}
Regular Regular::digit() {
    static auto result = range('0', '9');
    return result;
}
Regular Regular::lower() {
    static auto result = range('a', 'z');
    return result;
}
Regular Regular::upper() {
    static auto result = range('A', 'Z');
    return result;
}
Regular Regular::alpha() {
    static auto result = upper() | lower();
    return result;
}

Regular &Regular::operator+=(const Regular &other) {
    return *this = *this + other;
}

Regular &Regular::operator|=(const Regular &other) {
    return *this = *this | other;
}

NFAutomata Regular::Char::make() const {
    NFAutomata result;
    result.start->get(ch).push_back(result.accept);
    return result;
}

NFAutomata Regular::PClosure::make() const {
    NFAutomata inner = node->make();
    NFAutomata result;
    result.start = inner.start;

    auto& list = inner.accept->get('\0');
    list.push_back(result.accept);
    list.push_back(result.start);

    return result;
}

NFAutomata Regular::And::make() const {
    NFAutomata inner[2];
    inner[0] = node[0]->make();
    inner[1] = node[1]->make();
    NFAutomata result(
        inner[0].start,
        inner[1].accept
    );
    inner[0].accept->get('\0').push_back(inner[1].start);
    return result;
}

NFAutomata Regular::Or::make() const {
    NFAutomata inner[2];
    inner[0] = node[0]->make();
    inner[1] = node[1]->make();

    NFAutomata result;
    auto& list = result.start->get('\0');
    list.push_back(inner[0].start);
    list.push_back(inner[1].start);

    inner[0].accept->get('\0').push_back(result.accept);
    inner[1].accept->get('\0').push_back(result.accept);

    return result;
}
