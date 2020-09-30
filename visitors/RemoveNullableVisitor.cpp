//
// Created by Sichen Song on 9/8/2020.
//

#include "RemoveNullableVisitor.h"

void RemoveNullableVisitor::recursiveRemoveNullable(Rhs &rule, Rhs::iterator currentLocation,
                                                    const std::set<Symbol *> &nullables, RhsList &result,
                                                    Rhs &pendingRhs) {
    if (rule.end() == currentLocation) {
        if (pendingRhs.size() == rule.size() || pendingRhs.empty())return;
        //remove duplicate
        for (auto &rhs:result) {
            if (rhs == pendingRhs)return;
        }
        result.push_back(pendingRhs);
        return;
    }
    if (nullables.find(*currentLocation) == nullables.end()) {
        pendingRhs.push_back(*currentLocation);
        currentLocation++;
        recursiveRemoveNullable(rule, currentLocation, nullables, result, pendingRhs);
        pendingRhs.pop_back();
        return;
    }
    //use: use this, goto next
    pendingRhs.push_back(*currentLocation);
    currentLocation++;
    recursiveRemoveNullable(rule, currentLocation, nullables, result, pendingRhs);
    currentLocation--;
    pendingRhs.pop_back();
    //not use: not use this, go to next different
    Symbol *currentSym = *currentLocation;
    while (currentLocation != rule.end() && *currentLocation == currentSym)++currentLocation;
    recursiveRemoveNullable(rule, currentLocation, nullables, result, pendingRhs);
}

void RemoveNullableVisitor::visit(Grammar &g) {

    //find all nullables
    GetNullableVisitor gnv;
    g.accept(gnv);
    auto nullables = gnv.getResult();

    //find all empty rules and remove them
    for (auto &lhs:nullables) {
        for (auto rule = g.getRules().getProductionFor(lhs).begin();
             rule != g.getRules().getProductionFor(lhs).end();) {
            if (rule->empty() || rule->size() == 1 && *rule->begin() == Symbol::terminal("")) {
                auto oldIterator = rule++;
                g.getRules().getProductionFor(lhs).erase(oldIterator);
                continue;
            } else {
                ++rule;
            }
        }
    }

    for (auto lhs:g.getNonTerminal()) {
        if (!g.getRules().contains(lhs))continue;
        auto &rules = g.getRules().getProductionFor(lhs);
        RhsList ruleToAdd;
        Rhs pendingRhs;
        for (auto &rule:rules) {
            //find all productions that nullable is (partially) removed.
            recursiveRemoveNullable(rule, rule.begin(),
                                    nullables, ruleToAdd, pendingRhs);
        }
        rules.insert(rules.end(), ruleToAdd.begin(), ruleToAdd.end());
    }

}
