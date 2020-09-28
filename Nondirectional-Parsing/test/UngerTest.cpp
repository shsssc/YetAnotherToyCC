#include "gtest/gtest.h"
#include "../UngerParser.h"
#include "../../regular-expression/LongestReMatcher.h"


int evaluate(std::shared_ptr<Node> tree) {
    if (tree->_terminal) {
        if (tree->_symbol->getName() == "Number") {
            return atoi(tree->_value.c_str());
        } else {
            throw std::runtime_error("unexpected non-number terminal");
        }
    }
    if (tree->_children[0]->_symbol->getName() == "LP") {
        return evaluate(tree->_children[1]);
    }
    if (tree->_children.size() == 1) {
        return evaluate(tree->_children[0]);
    }
    return evaluate(tree->_children[0]) + evaluate(tree->_children[2]);
}


bool validateString(UngerParser &p, LongestReMatcher &l, const std::string &input) {
    auto tokens = l.match(input);
    LongestReMatcher::removeTokenFromList(tokens, "SPACE");
    std::vector<Token> vtokens(tokens.begin(), tokens.end());
    return p.validate(vtokens);
}



int evalString(UngerParser &p, LongestReMatcher &l, const std::string &input) {
    auto tokens = l.match(input);
    LongestReMatcher::removeTokenFromList(tokens, "SPACE");
    std::vector<Token> vtokens(tokens.begin(), tokens.end());
    return evaluate(p.parse(vtokens));
}


TEST(Unger, simple) {
    auto s = readfile(PROJ_ROOT
                      R"(/test/configs/plus-exp-simple)");
    Grammar g(s);
    UngerParser parser(g);

    s = readfile(PROJ_ROOT
                 R"(/regular-expression/configs/simple)");
    LongestReMatcher l(s);

    EXPECT_TRUE(validateString(parser, l, "1+1+12"));
    EXPECT_TRUE(validateString(parser, l, "1"));
    EXPECT_TRUE(validateString(parser, l, "(1)+1+1"));
    EXPECT_TRUE(validateString(parser, l, "1+(1)+31"));
    EXPECT_TRUE(validateString(parser, l, "1+1+(1)"));
    EXPECT_FALSE(validateString(parser, l, "1 2 3+1+(1)"));
    EXPECT_FALSE(validateString(parser, l, "()"));
    EXPECT_FALSE(validateString(parser, l, "3+(3))"));
//EXPECT_TRUE(validateString(parser, l, "(3+(3+(3))+3)"));
}



TEST(Unger, Eval) {
    auto s = readfile(PROJ_ROOT
                      R"(/test/configs/plus-exp-simple)");
    Grammar g(s);
    UngerParser parser(g);

    s = readfile(PROJ_ROOT
                 R"(/regular-expression/configs/simple)");
    LongestReMatcher l(s);

    EXPECT_EQ(evalString(parser,l,"(5+ 1 + 2 + 5+1000)"),1013);
    EXPECT_EQ(evalString(parser,l,"(1)"),1);
    EXPECT_EQ(evalString(parser,l,"((5+ 1) +15 +(0))"),5+1+15);
}