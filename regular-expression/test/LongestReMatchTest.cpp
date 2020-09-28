#include "gtest/gtest.h"
#include "../LongestReMatcher.h"
#include "../../utils.h"

TEST(LongestReMatch, Simple) {
    std::list<std::pair<std::string, std::string>> m;
    m.emplace_back("IF", "if");
    m.emplace_back("Plus", "\\+");
    m.emplace_back("PlusPlus", "\\+\\+");
    m.emplace_back("word", "[a-z]+");
    m.emplace_back("number", "[0-9]+");
    m.emplace_back("space", "( )+");
    m.emplace_back("LQuote", "\\(");
    m.emplace_back("RQuote", "\\)");
    m.emplace_back("Quote", "\"[0-9a-zA-Z !@#$%^&*()=]*\"");
    LongestReMatcher l(m);

    auto result = l.match("\"this is a test of the quote\"   1+1");
    std::list<std::string> gt = {"Quote", "space", "number", "Plus", "number"};
    EXPECT_EQ(result.size(), gt.size());
    auto a = gt.begin();
    auto b = result.begin();
    for (int i = 0; i < gt.size(); i++) {
        EXPECT_EQ(*a, b->type);
        a++;
        b++;
    }
}

TEST(LongestReMatch, Priority) {
    std::list<std::pair<std::string, std::string>> m;
    m.emplace_back("abc", "abc");
    m.emplace_back("abd", "abd");
    m.emplace_back("abe", "abe");
    m.emplace_back("word", "[a-z]+");
    m.emplace_back("abf", "abf");
    m.emplace_back("abg", "abg");
    m.emplace_back("abh", "abh");
    m.emplace_back("space", "( )+");
    LongestReMatcher l(m);

    auto result = l.match("abc abd abe xxx abf abg abh");
    std::list<std::string> gt = {"abc", "space", "abd", "space", "abe", "space", "word", "space", "word", "space",
                                 "word", "space", "word"};

    EXPECT_EQ(result.size(), gt.size());
    auto a = gt.begin();
    auto b = result.begin();
    for (int i = 0; i < gt.size(); i++) {
        EXPECT_EQ(*a, b->type);
        a++;
        b++;
    }

}

TEST(LongestReMatch, configFile) {
    auto s = readfile(PROJ_ROOT
                      R"(regular-expression/configs/simple)");
    LongestReMatcher l(s);
    auto result = l.match("int main(){3+33;}    \"w \"");
    std::list<std::string> gt = {"ID", "SPACE", "ID", "LP", "RP", "LB", "Number", "Plus", "Number", "SC", "RB", "SPACE",
                                 "Quote"};
    std::list<int> lens = {3, 1, 4, 1, 1, 1, 1, 1, 2, 1, 1, 4, 4};
    EXPECT_EQ(result.size(), gt.size());
    auto a = gt.begin();
    auto b = result.begin();
    auto c = lens.begin();
    for (int i = 0; i < gt.size(); i++) {
        EXPECT_EQ(*a, b->type);
        EXPECT_EQ(*c, b->value.length());
        a++;
        b++;
        c++;
    }
}