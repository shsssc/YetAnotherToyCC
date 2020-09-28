#include "gtest/gtest.h"
#include "../Rule.h"
#include "../utils.h"
#include "../Grammar.h"
#include "../visitors/MakeCnfVisitor.h"
#include <vector>

TEST(MakeCnf, Basic) {
    Symbol::clear();
    std::string fname = PROJ_ROOT R"(/test/configs/make-cnf-test)";
    std::string g_str = readfile(fname);
    Grammar g(g_str);

    MakeCnfVisitor grv;
    g.accept(grv);

    EXPECT_EQ(Symbol::nonTerminals().size(), 8);
}
