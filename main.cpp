#include <iostream>
#include <memory>
#include <utility>
#include <deque>
#include <map>

template<typename T>
using Ptr = std::shared_ptr<T>;

class Automata;
//正则表达式类，只保存其对应的抽象语法树
class Regular {
public:
    struct Node;//抽象语法树的各个节点，具体实现先放在一边

    explicit Regular (Ptr<Node> node):
    node(std::move(node)) {}

    Regular operator+() const;//正闭包运算
    Regular operator*() const;//闭包运算

    Regular operator+(const Regular& other) const;//连接运算
    Regular operator|(const Regular& other) const;//或运算

    public:
    struct Node {
        virtual Automata make() const = 0;
    };
    Automata make() {
        return node->make();
    }
private:
    Ptr<Node> node;
};

//以下几个函数用来构造最常用的几种正则表达式
Regular nil();//空语言
Regular Char(char ch);//单个确定字符构成的语言
Regular digit();//0-9的数字
Regular alpha();//英文字母
Regular lower();//小写字母
Regular upper();//大写字母

struct Regular::Node {};//目前它不需要任何多余的属性和方法
//后面的几个类没必要暴露在全局，于是放在匿名namespace中隐藏掉
namespace {
    struct SingleChar final : public Regular::Node {
        char ch;
    };
    //正闭包，闭包*r == +(nil()|r)
    struct Pos final : public Regular::Node {
        Ptr<Node> node;
    };

    struct And final : public Regular::Node {
        Ptr<Node> node[2];
    };
    struct Or final : public Regular::Node {
        Ptr<Node> node[2];
    };
}

Regular Regular::operator+() const {
    auto node = std::make_shared<Pos>();
    node->node = this->node;

    return Regular(node);
}

Regular Regular::operator*() const {
    return nil() | +(*this);
}

Regular Regular::operator+(const Regular& other) const {
    auto node = std::make_shared<And>();
    node->node[0] = this->node;
    node->node[1] = other.node;

    return Regular(node);
}

Regular Regular::operator|(const Regular& other) const {
    auto node = std::make_shared<Or>();
    node->node[0] = this->node;
    node->node[1] = other.node;

    return Regular(node);
}

namespace {
    std::array<Ptr<SingleChar>, 128> makeCharTable() {
        std::array<Ptr<SingleChar>, 128> charTable;
        for (unsigned char i = 0; i < 128; ++i) {
            charTable[i] = std::make_shared<SingleChar>();
            charTable[i]->ch = i;
        }

        return charTable;
    }
    Regular range(char begin, char end) {
        Regular result = Char(begin);
        for (char i = begin + 1; i <= end; ++i) {
            result = result | Char(i);
        }
        return result;
    }
}

Regular Char(char ch) {
    static auto charTable = makeCharTable();
    return Regular(charTable[ch]);
}

Regular nil() {
    return Char('\0');
}

Regular digit() {
    static auto result = range('0', '9');
    return result;
}
Regular lower() {
    static auto result = range('a', 'z');
    return result;
}
Regular upper() {
    static auto result = range('A', 'Z');
    return result;
}
Regular alpha() {
    static auto result = upper() | lower();
    return result;
}


struct Automata {
    struct State {
        using Ptr = std::shared_ptr<State>;
        using List = std::deque<Ptr>;
        using Map = std::map<char, List>;

        Map next;//一开始是用multimap的，然而写出来的代码格外丑，于是就改成了这样

        bool has(char key) const {
            if (next.find(key) == next.end()) {
                return false;
            } else {
                return true;
            }
        }
        List& get(char key) {
            return next[key];
        }
    };

    Automata():
        start(new State()),
        accept(new State()) {}

    Automata(
        std::shared_ptr<State>& start,
        std::shared_ptr<State>& accept
    ) : start(start), accept(accept) {}

    std::shared_ptr<State> start;
    std::shared_ptr<State> accept;
};

struct SingleChar final : public Regular::Node {
    char ch;

    Automata make() const override {
        Automata result;
        result.start->get(ch).push_back(result.accept);
        return result;
    }
};
struct Pos final : public Regular::Node {
    Ptr<Node> node;

    Automata make() const override {
        Automata inner = node->make();
        Automata result;
        result.start = inner.start;

        auto& list = inner.accept->get('\0');
        list.push_back(result.accept);
        list.push_back(result.start);

        return result;
    }
};

struct And final : public Regular::Node {
    Ptr<Node> node[2];

    Automata make() const override {
        Automata inner[2];
        inner[0] = node[0]->make();
        inner[1] = node[1]->make();
        Automata result(
            inner[0].start,
            inner[1].accept
        );
        inner[0].accept->get('\0').push_back(inner[1].start);
        return result;
    }
};

struct Or final : public Regular::Node {
    Ptr<Node> node[2];

    Automata make() const override {
        Automata inner[2];
        inner[0] = node[0]->make();
        inner[1] = node[1]->make();

        Automata result;
        auto& list = result.start->get('\0');
        list.push_back(inner[0].start);
        list.push_back(inner[1].start);

        inner[0].accept->get('\0').push_back(result.accept);
        inner[1].accept->get('\0').push_back(result.accept);

        return result;
    }
};







int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
