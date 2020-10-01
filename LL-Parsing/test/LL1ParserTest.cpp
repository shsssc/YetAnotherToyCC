#include "gtest/gtest.h"
#include "../LL1Parser.h"
#include "../../regular-expression/LongestReMatcher.h"

bool validateString(LL1Parser &p, LongestReMatcher &l, const std::string &input) {
    auto tokens = l.match(input);
    LongestReMatcher::removeTokenFromList(tokens, "SPACE");
    std::vector<Token> vtokens(tokens.begin(), tokens.end());
    return p.validate(vtokens);
}

TEST(LL1,simple){
    auto s = readfile(PROJ_ROOT
                      R"(/test/configs/ll1-first-test)");
    Grammar g(s);
    LL1Parser p(g);

    s = readfile(PROJ_ROOT
                 R"(/regular-expression/configs/simple)");
    LongestReMatcher l(s);
    EXPECT_TRUE(validateString(p,l," ({(abc);});"));
    EXPECT_TRUE(validateString(p,l," if "));
    EXPECT_TRUE(validateString(p,l,"({if}) id ;"));
    EXPECT_TRUE(validateString(p,l,"({if}) {({if}) id ;} ;"));

}