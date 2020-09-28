//
// Created by Sichen Song on 7/26/2020.
//

#ifndef PROJECT_RULE_H
#define PROJECT_RULE_H

#include <list>
#include <map>
#include "Symbol.h"

typedef std::list<Symbol *> Rhs;
typedef std::list<Rhs> RhsList;
typedef Symbol *Lhs;
typedef std::map<Lhs, RhsList> rules_t;

class Rules {
    rules_t _rules;
public:
    /**
     * add a rule.
     * @param l lhs of the rule
     * @param r rhs of the rule
     */
    Rhs& addRule(const Lhs &l, const Rhs &r);

    /**
     * delete a rule from a list of rhs
     * @param rhslist
     * @param rule
     */
    void deleteRule(RhsList &rhslist, RhsList::iterator rule);

    /**
     * update a rule
     * @param rule
     * @param newRhs
     */
    void setRule(RhsList::iterator rule, const Rhs &newRhs);

    /**
     * lookup LHS for all rules
     * @return
     */
    RhsList &getProductionFor(const Lhs &);

    /**
     * remove all the productions of one LHS
     * @param l
     */
    void clearProductionFor(const Lhs &l);

    /**
     * lookup whether exist rule for l
     * @param l
     * @return
     */
    bool contains(const Lhs &l);

    /**
     * load from file
     * @param fname
     */
    void load(std::string fname);

    void print();
};

#endif //PROJECT_RULE_H
