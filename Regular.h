#ifndef REGULAREXPRESSIONRECOGNIZER_REGULAR_H
#define REGULAREXPRESSIONRECOGNIZER_REGULAR_H

#include "NFAutomata.h"

class Regular {
private:
    struct Node {
            [[nodiscard]] virtual NFAutomata make() const = 0;
        };

    struct Char final : public Regular::Node {
        char ch = 0;
        Char() = default;
        [[nodiscard]] NFAutomata make() const override;
    };
    struct PClosure final : public Regular::Node {
        std::shared_ptr<Node> node;

        PClosure() = default;
        [[nodiscard]] NFAutomata make() const override;
    };

    struct And final : public Regular::Node {
        std::shared_ptr<Node> node[2];

        And() = default;
        [[nodiscard]] NFAutomata make() const override;
    };

    struct Or final : public Regular::Node {
        std::shared_ptr<Node> node[2];

        Or() = default;
        [[nodiscard]] NFAutomata make() const override;
    };
public:

    explicit Regular (std::shared_ptr<Node> node):
    node(std::move(node)) {}

    inline NFAutomata make() const {return node->make(); };

    Regular operator+() const;//正闭包运算
    Regular operator*() const;//闭包运算

    Regular operator+(const Regular& other) const;//连接运算
    Regular& operator+=(const Regular& other);//连接运算
    Regular operator|(const Regular& other) const;//或运算
    Regular& operator|=(const Regular& other);//或运算

private:
    std::shared_ptr<Node> node;

private:
    static std::array<std::shared_ptr<Char>, 128> makeCharTable();
    static Regular range(char begin, char end);

public:
    //以下几个函数用来构造最常用的几种正则表达式
    static Regular nil();//空语言
    static Regular character(char ch);//单个确定字符构成的语言
    static Regular digit();//0-9的数字
    static Regular alpha();//英文字母
    static Regular lower();//小写字母
    static Regular upper();//大写字母
};


#endif //REGULAREXPRESSIONRECOGNIZER_REGULAR_H
