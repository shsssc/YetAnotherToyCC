#include <iostream>
#include "gtest/gtest.h"
#include "../Rule.h"
#include "../utils.h"
#include "../Grammar.h"
#include "../visitors/RemoveUnitVisitor.h"

TEST(RemoveUnitRule, simple) {
    std::string fname = PROJ_ROOT R"(/test/configs/unit-rule-test)";
    std::string g_str = readfile(fname);
    Grammar g(g_str);
    //g.print();

    RemoveUnitVisitor ruv;
    g.accept(ruv);
    EXPECT_EQ(g.getRules().getProductionFor(Symbol::nonTerminal("Number")).size(), 3);
    EXPECT_EQ(g.getRules().getProductionFor(Symbol::nonTerminal("Expression")).size(), 6);
    EXPECT_EQ(g.getRules().getProductionFor(Symbol::nonTerminal("A")).size(), 4);
    EXPECT_EQ(g.getRules().getProductionFor(Symbol::nonTerminal("B")).size(), 4);
    EXPECT_EQ(g.getRules().getProductionFor(Symbol::nonTerminal("C")).size(), 4);
    EXPECT_EQ(g.getRules().getProductionFor(Symbol::nonTerminal("D")).size(), 4);
    EXPECT_EQ(g.getRules().getProductionFor(Symbol::nonTerminal("E")).size(), 5);
}