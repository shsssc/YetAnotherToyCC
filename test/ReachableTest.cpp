#include "gtest/gtest.h"
#include "../Rule.h"
#include "../utils.h"
#include "../Grammar.h"
#include "../visitors//GetReachableVisitor.h"
#include "../visitors/RemoveUnreachableVisitor.h"
#include <vector>

TEST(GetReachable, Basic) {
    std::string fname = PROJ_ROOT R"(/test/configs/reachable-test)";
    std::string g_str = readfile(fname);
    Grammar g(g_str);

    GetReachableVisitor grv;
    g.accept(grv);

    std::vector<std::string> reachable = {"A", "S", "B", "A1", "B2", "A2", "B1"};

    EXPECT_EQ(reachable.size(), grv.getReachable().size());
    for (auto &s:reachable) {
        auto reachableSet = grv.getReachable();
        EXPECT_TRUE(reachableSet.find(Symbol::nonTerminal(s)) != reachableSet.end());
    }
}

TEST(RemoveReachable, Basic) {
    std::string fname = PROJ_ROOT R"(/test/configs/reachable-test)";
    std::string g_str = readfile(fname);
    Grammar g(g_str);
    EXPECT_TRUE(g.getRules().contains(Symbol::nonTerminal("Unreachable")));

    RemoveUnreachableVisitor ruv;
    g.accept(ruv);
    EXPECT_FALSE(g.getRules().contains(Symbol::nonTerminal("Unreachable")));
}