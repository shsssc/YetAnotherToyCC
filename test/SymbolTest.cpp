#include "gtest/gtest.h"
#include "../Symbol.h"
#include "../Rule.h"

TEST(Symbol_Test, Interface) {
    EXPECT_EQ(1, 1 + 1 - 1);
    auto terminal = Symbol::terminal("terminal");
    EXPECT_TRUE(terminal->isTerminal());
    EXPECT_EQ(terminal->getName(), "terminal");
    auto nonterminal = Symbol::nonTerminal("nonterminal");
    EXPECT_FALSE(nonterminal->isTerminal());
    EXPECT_EQ(nonterminal->getName(), "nonterminal");
}

TEST(Symbol_Test, Flyweight) {
    auto terminal1 = Symbol::terminal("aaa");
    auto terminal2 = Symbol::terminal("aaa");
    auto terminal3 = Symbol::terminal("bbb");
    auto terminal4 = Symbol::terminal("bbb");
    EXPECT_EQ(terminal1, terminal2);
    EXPECT_EQ(terminal3, terminal4);
    EXPECT_NE(terminal1, terminal3);

    auto non_terminal1 = Symbol::nonTerminal("aaa");
    auto non_terminal2 = Symbol::nonTerminal("aaa");
    auto non_terminal3 = Symbol::nonTerminal("bbb");
    auto non_terminal4 = Symbol::nonTerminal("bbb");
    EXPECT_EQ(non_terminal1, non_terminal2);
    EXPECT_EQ(non_terminal3, non_terminal4);
    EXPECT_NE(non_terminal1, non_terminal3);

    EXPECT_NE(terminal1, non_terminal1);
    EXPECT_NE(terminal3, non_terminal3);
}

