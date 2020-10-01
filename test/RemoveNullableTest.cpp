#include <iostream>
#include "gtest/gtest.h"
#include "../Rule.h"
#include "../utils.h"
#include "../Grammar.h"
#include "../visitors/RemoveNullableVisitor.h"

TEST(RemoveNullable, simple) {
    std::string fname = PROJ_ROOT R"(/test/configs/remove-nullable-test)";
    std::string g_str = readfile(fname);
    Grammar g(g_str);
    //g.print();

    RemoveNullableVisitor ruv;
    g.accept(ruv);
    EXPECT_EQ(g.getRules().getProductionFor(Symbol::nonTerminal("B1")).size(), 6);
    EXPECT_EQ(g.getRules().getProductionFor(Symbol::nonTerminal("B")).size(), 2 * 3 * 4);
}