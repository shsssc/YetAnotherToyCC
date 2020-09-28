#include "gtest/gtest.h"
#include "../Rule.h"
#include "../utils.h"
#include "../Grammar.h"
#include "../visitors//GetProducableVisitor.h"
#include "../visitors//RemoveUnproducableVisitor.h"
#include <vector>

TEST(Producable, Basic) {
    std::string fname = PROJ_ROOT R"(/test/configs/producible-test)";
    std::string g_str = readfile(fname);
    Grammar g(g_str);

    GetProducableVisitor grv;
    g.accept(grv);

    std::vector<std::string> correct = {"S", "G", "G1", "G2", "G21", "G11"};

    EXPECT_EQ(correct.size(), grv.getResult().size());
    for (auto &s:correct) {
        auto reachableSet = grv.getResult();
        EXPECT_TRUE(reachableSet.find(Symbol::nonTerminal(s)) != reachableSet.end());
    }
}

TEST(RemoveProducable, Basic) {
    std::string fname = PROJ_ROOT R"(/test/configs/producible-test)";
    std::string g_str = readfile(fname);
    Grammar g(g_str);

    RemoveUnproducableVisitor grv;
    g.accept(grv);

    std::set<std::string> correct = {"S", "G", "G1", "G2", "G21", "G11"};

    for (auto &s:Symbol::nonTerminals()) {
        if(correct.find(s->getName())!=correct.end()){
            EXPECT_TRUE(g.getRules().contains(s));
        }else{
            EXPECT_FALSE(g.getRules().contains(s));
        }
    }
}