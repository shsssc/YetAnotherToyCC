#include "gtest/gtest.h"
#include "../NFA.h"

TEST(NFATest, simple) {
    auto a = newState(), b = newState(), start = newState(), accept = newState();
    start->addNext('a', a);
    a->addNext('b', b);
    b->addNext('c', accept);
    NFA::NFA n(start, accept);
    EXPECT_FALSE(n.accepted());
    n.take('a');
    EXPECT_EQ(n.getCurrentState().size(), 1);
    n.take('b');
    EXPECT_EQ(n.getCurrentState().size(), 1);
    n.take('c');
    EXPECT_EQ(n.getCurrentState().size(), 1);
    EXPECT_TRUE(n.accepted());
    n.take('d');
    EXPECT_TRUE(n.invalidPrefix());
    n.refresh();
    EXPECT_FALSE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take('a');
    EXPECT_EQ(n.getCurrentState().size(), 1);
    n.take('b');
    EXPECT_EQ(n.getCurrentState().size(), 1);
    n.take('c');
    EXPECT_EQ(n.getCurrentState().size(), 1);
    EXPECT_TRUE(n.accepted());
    n.take('d');
    EXPECT_TRUE(n.invalidPrefix());

}

TEST(NFATest, epsilon) {
    auto a = newState(), b = newState(), start = newState(), accept = newState();
    start->addNext(NFAState::EPSILON, a);
    a->addNext(NFAState::EPSILON, b);
    b->addNext(NFAState::EPSILON, accept);
    start->addNext('a', a);
    a->addNext('b', b);
    b->addNext('c', accept);
    NFA::NFA n(start, accept);
    EXPECT_TRUE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    EXPECT_EQ(n.getCurrentState().size(), 4);
    n.take('2');
    EXPECT_TRUE(n.invalidPrefix());
    n.refresh();
    EXPECT_TRUE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    EXPECT_EQ(n.getCurrentState().size(), 4);
    n.take('b');
    EXPECT_EQ(n.getCurrentState().size(), 2);
    EXPECT_TRUE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take('x');
    EXPECT_FALSE(n.accepted());
    EXPECT_TRUE(n.invalidPrefix());
}

TEST(NFAUtil, NFAFromStr) {
    auto n = NFA::nfaFromWords("abcde");
    n.take('a');
    EXPECT_FALSE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take('b');
    n.take('c');
    n.take('d');
    n.take('e');
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take('a');
    n.take('e');
    EXPECT_TRUE(n.invalidPrefix());
    n = NFA::nfaFromWords("a");
    n.take('a');
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take('d');
    EXPECT_TRUE(n.invalidPrefix());
}

TEST(NFAUtil, NFAFromCharSet) {
    auto n = NFA::nfaFromCharSet("abc1-5");
    for (auto c:std::set<char>{'a', 'b', 'c', '1', '2', '3', '4', '5'}) {
        n.take(c);
        EXPECT_TRUE(n.accepted());
        n.refresh();
    }
    for (auto c:std::set<char>{'d', 'e', 'f', '0', '7', '8', '6', '9'}) {
        n.take(c);
        EXPECT_FALSE(n.accepted());
        EXPECT_TRUE(n.invalidPrefix());
        n.refresh();
    }
    n = NFA::nfaFromCharSet(R"(\\\-)");
    for (auto c:std::set<char>{'\\', '-'}) {
        n.take(c);
        EXPECT_TRUE(n.accepted());
        n.refresh();
    }
}

TEST(NFAUtil, NFAConcat) {
    auto n = NFA::nfaFromWords("abc");
    auto n2 = NFA::nfaFromWords("de");
    NFA::nfaConcat(n, n2);
    n.take('a');
    EXPECT_FALSE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take('b');
    n.take('c');
    n.take('d');
    n.take('e');
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take('a');
    n.take('e');
    EXPECT_TRUE(n.invalidPrefix());
}

TEST(NFAUtil, NFAStar) {
    auto n = NFA::nfaFromWords("abc");
    NFA::nfaStar(n);
    EXPECT_TRUE(n.accepted());
    n.take('a');
    n.take('b');
    n.take('c');
    n.take('a');
    EXPECT_FALSE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take('b');
    n.take('c');
    EXPECT_TRUE(n.accepted());
    n.refresh();
    n.take('a');
    n.take('e');
    EXPECT_TRUE(n.invalidPrefix());
    n.refresh();
    EXPECT_TRUE(n.accepted());
}

TEST(NFAUtil, NFAQuestion) {
    auto n = NFA::nfaFromWords("a");
    auto n2 = NFA::nfaFromWords("a");
    auto n3 = NFA::nfaFromWords("a");
    NFA::nfaQuestion(n);
    NFA::nfaQuestion(n2);
    NFA::nfaQuestion(n3);
    NFA::nfaConcat(n, n2);
    NFA::nfaConcat(n, n3);
    EXPECT_TRUE(n.accepted());
    n.take('a');
    EXPECT_TRUE(n.accepted());
    n.take('a');
    EXPECT_TRUE(n.accepted());
    n.take('a');
    EXPECT_TRUE(n.accepted());
    n.take('a');
    EXPECT_FALSE(n.accepted());
    EXPECT_TRUE(n.invalidPrefix());
}

TEST(NFAUtil, NFAPlus) {
    auto n = NFA::nfaFromWords("a");
    NFA::nfaPlus(n);
    EXPECT_FALSE(n.accepted());
    n.take('a');
    EXPECT_TRUE(n.accepted());
    n.take('a');
    EXPECT_TRUE(n.accepted());
    n.take('a');
    EXPECT_TRUE(n.accepted());
    n.take('a');
    EXPECT_TRUE(n.accepted());
    EXPECT_FALSE(n.invalidPrefix());
    n.take('b');
    EXPECT_TRUE(n.invalidPrefix());
}

TEST(NFAUtil, NFAOr) {
    auto n = NFA::nfaFromWords("abc");
    auto n1 = NFA::nfaFromWords("abd");
    NFA::nfaOr(n, n1);
    EXPECT_FALSE(n.accepted());
    n.take('a');
    EXPECT_FALSE(n.invalidPrefix());
    n.take('b');
    EXPECT_FALSE(n.invalidPrefix());
    n.take('c');
    EXPECT_FALSE(n.invalidPrefix());
    EXPECT_TRUE(n.accepted());
    n.refresh();
    EXPECT_FALSE(n.accepted());
    n.take('a');
    EXPECT_FALSE(n.invalidPrefix());
    n.take('b');
    EXPECT_FALSE(n.invalidPrefix());
    n.take('d');
    EXPECT_FALSE(n.invalidPrefix());
    EXPECT_TRUE(n.accepted());
    n.refresh();
    EXPECT_FALSE(n.accepted());
    n.take('a');
    EXPECT_FALSE(n.invalidPrefix());
    n.take('b');
    EXPECT_FALSE(n.invalidPrefix());
    n.take('f');
    EXPECT_TRUE(n.invalidPrefix());
    EXPECT_FALSE(n.accepted());
}

