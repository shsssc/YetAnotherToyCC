#include "gtest/gtest.h"
#include "../Rule.h"
#include "../utils.h"
#include "../Grammar.h"
#include "../visitors/GetNullableVisitor.h"
#include <vector>

TEST(GetNullable, Basic) {
    std::string fname = PROJ_ROOT R"(/test/configs/nullable-test)";
    std::string g_str = readfile(fname);
    Grammar g(g_str);

    GetNullableVisitor grv;
    g.accept(grv);

    std::vector<std::string> reachable = {"A", "E", "D"};

    EXPECT_EQ(reachable.size(), grv.getResult().size());
    for (auto &s:reachable) {
        auto reachableSet = grv.getResult();
        EXPECT_TRUE(reachableSet.find(Symbol::nonTerminal(s)) != reachableSet.end());
    }
}
