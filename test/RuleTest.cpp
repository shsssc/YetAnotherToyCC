#include "gtest/gtest.h"
#include "../Rule.h"
#include "./testUtils.h"
#include "../utils.h"
#include <vector>

TEST(Rules, interface) {
    Rules rules;
    auto LHS = Symbol::nonTerminal("LHS");
    auto RHS = std::list<Lhs>({LHS, LHS, Symbol::terminal("terminal")});
    rules.addRule(LHS, RHS);
    EXPECT_TRUE(rules.contains(LHS));
    auto rule = *rules.getProductionFor(LHS).begin();
    EXPECT_RULE_EQUAL(RHS, rule);
    RHS.clear();
    auto &rhs = *rules.getProductionFor(LHS).begin();
    RHS.push_back(Symbol::terminal("New_Terminal"));
    rules.setRule(rules.getProductionFor(LHS).begin(), RHS);
    rule = *rules.getProductionFor(LHS).begin();
    EXPECT_RULE_EQUAL(RHS, rule);
    EXPECT_EQ(1, rules.getProductionFor(LHS).size());
    rules.deleteRule(rules.getProductionFor(LHS), rules.getProductionFor(LHS).begin());
    EXPECT_EQ(0, rules.getProductionFor(LHS).size());
    Symbol::clear();
}

TEST(Rules, parse_rules) {
    std::string fname = PROJ_ROOT R"(/test/configs/test-grammar)";
    std::string grammar = readfile(fname);
    std::vector<std::string> terminals = {"+", "++", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ""};
    std::vector<std::string> non_terminals = {"Digit", "Number", "Stmt"};
    Rules r;
    r.load(grammar);
    EXPECT_EQ(terminals.size(), Symbol::terminals().size());
    EXPECT_EQ(non_terminals.size(), Symbol::nonTerminals().size());

    const auto &terminal_set = Symbol::terminals();
    for (auto &t:terminals) {
        EXPECT_TRUE(terminal_set.find(Symbol::terminal(t)) != terminal_set.end());
    }

    const auto &nonterminal_set = Symbol::nonTerminals();
    for (auto &t:non_terminals) {
        EXPECT_TRUE(nonterminal_set.find(Symbol::nonTerminal(t)) != nonterminal_set.end());
    }
    //Symbol::print();    r.print();
    EXPECT_EQ(r.getProductionFor(Symbol::nonTerminal("Number")).size(), 2);
    EXPECT_EQ(r.getProductionFor(Symbol::nonTerminal("Stmt")).size(), 3);
    EXPECT_EQ(r.getProductionFor(Symbol::nonTerminal("Digit")).size(), 11);
}