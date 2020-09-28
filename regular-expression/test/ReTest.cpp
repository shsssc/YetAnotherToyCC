#include "gtest/gtest.h"
#include "../Re2NFA.h"

TEST(RE, Simple) {
    Re2NFA a("abc123");
    auto n = a.getNFA();
    n.take("abc");
    EXPECT_FALSE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take("123");
    EXPECT_TRUE(n.accepted());

    a = Re2NFA(R"(abc\\123\[\])");
    n = a.getNFA();
    n.take("abc");
    EXPECT_FALSE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take("123");
    EXPECT_TRUE(n.invalidPrefix());
    n.refresh();
    n.take("abc");
    EXPECT_FALSE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take("\\123[]");
    EXPECT_TRUE(n.accepted());
}

TEST(RE, Group) {
    Re2NFA a("abc(123)*");
    auto n = a.getNFA();
    n.take("abc");
    EXPECT_TRUE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take("123");
    EXPECT_TRUE(n.accepted());
    n.take("12");
    EXPECT_FALSE(n.accepted());
    n.take("3");
    EXPECT_TRUE(n.accepted());
    n.take("123");
    n.take("123");
    n.take("123");
    n.take("123");
    EXPECT_TRUE(n.accepted());
    n.take("4");
    EXPECT_TRUE(n.invalidPrefix());
}

TEST(RE, Modifier) {
    Re2NFA a("abc(123)*");
    auto n = a.getNFA();
    n.take("abc");
    EXPECT_TRUE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take("123");
    EXPECT_TRUE(n.accepted());
    n.take("12");
    EXPECT_FALSE(n.accepted());
    n.take("3");
    EXPECT_TRUE(n.accepted());
    n.take("123");
    n.take("123");
    n.take("123");
    n.take("123");
    EXPECT_TRUE(n.accepted());
    n.take("4");
    EXPECT_TRUE(n.invalidPrefix());

    a = Re2NFA(R"(abc(123)+)");
    n = a.getNFA();
    n.take("abc");
    EXPECT_FALSE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take("123");
    EXPECT_FALSE(n.invalidPrefix());
    n.take("12");
    EXPECT_FALSE(n.accepted());
    n.take("3");
    EXPECT_TRUE(n.accepted());
    n.take("123");
    n.take("123");
    n.take("123");
    n.take("123");
    EXPECT_TRUE(n.accepted());
    n.take("4");
    EXPECT_TRUE(n.invalidPrefix());

    a = Re2NFA(R"(abc(123)?)");
    n = a.getNFA();
    n.take("abc");
    EXPECT_TRUE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take("123");
    EXPECT_TRUE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take("123");
    EXPECT_TRUE(n.invalidPrefix());
}

TEST(RE, CharSet) {
    Re2NFA a("[1-9_\\]]+");
    auto n = a.getNFA();
    EXPECT_FALSE(n.accepted());
    n.take("123123321");
    EXPECT_TRUE(n.accepted());
    n.take("_");
    EXPECT_TRUE(n.accepted());
    n.take("1_2]");
    EXPECT_TRUE(n.accepted());
}

TEST(RE, RecursiveGrouping1) {
    Re2NFA a("(2((1)+)?)+(3)?");
    auto n = a.getNFA();
    n.take("2");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("21111111111");
    EXPECT_TRUE(n.accepted());
    n.take("221212112121212121");
    EXPECT_TRUE(n.accepted());
    n.take("3");
    EXPECT_TRUE(n.accepted());
    n.take("3");
    EXPECT_FALSE(n.accepted());
}

TEST(RE, RecursiveGrouping2) {
    Re2NFA a("[a-z]([1-9]*[A-Z]*)*");
    auto n = a.getNFA();
    n.take("aZ");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("a123123123Z");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("a123A123B5688756C");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("a123A123B5688756Ca");
    EXPECT_FALSE(n.accepted());
    n.refresh();
    n.take("a123A123B5688756C123123CCCCCC123CCCCCCCCCCCCCCCCCCC1CCC");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("a123A123B5688756C123123CCCCCC123CCCCCCCCCCCCCCCCCCC1CCC1111111111111d");
    EXPECT_FALSE(n.accepted());
}

TEST(RE, RecursiveGrouping3) {
    Re2NFA a("(l*r*)*");
    auto n = a.getNFA();
    n.take("l");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("r");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("lrlllrllllllrllllllllrrrrrr");
    EXPECT_TRUE(n.accepted());
    n.refresh();
}

TEST(RE, OrSimple) {
    Re2NFA a("abc|def(d)*");
    auto n = a.getNFA();
    n.take("abc");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("def");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("defddddddddddddddddddddddddd");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("abcddddddddddddddddddddddddd");
    EXPECT_TRUE(n.accepted());
}

TEST(RE, Or1) {
    Re2NFA a("x+abc|def(d)*");
    auto n = a.getNFA();
    n.take("xabc");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("xxdef");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("xxxxdefddddddddddddddddddddddddd");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("xxxxxabcddddddddddddddddddddddddd");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("abcddddddddddddddddddddddddd");
    EXPECT_FALSE(n.accepted());
    n.refresh();
    n.take("abcddddddddaddddddddddddddddd");
    EXPECT_FALSE(n.accepted());
}

TEST(RE, Or2) {
    Re2NFA a("l*((abc)|(def))|(123|456)r*");
    auto n = a.getNFA();
    n.take("123");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("456");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("def");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("abc");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("l123r");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("456rr");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("def");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("lllabc");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("lr");
    EXPECT_FALSE(n.accepted());
}

TEST(Re, concatination) {
    Re2NFA a("1(a)+1(a)*");
    auto n = a.getNFA();
    n.take("1a1");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("1aaaaaaaaaaaaaaaaaaa1");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("11");
    EXPECT_FALSE(n.accepted());
    n.refresh();
    n.take("1a1aaaaaaaaaaaaaaaaaaaaa");
    EXPECT_TRUE(n.accepted());
    n.refresh();
}

TEST(Re, escape) {
    Re2NFA a("[()][()](\\(|\\))");
    auto n = a.getNFA();
    n.take("())");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("()(");
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take("()");
    EXPECT_FALSE(n.accepted());
}