//
// Created by Sichen Song on 9/6/2020.
//

#ifndef PROJECT_GETNULLABLEVISITOR_H
#define PROJECT_GETNULLABLEVISITOR_H


#include "../Grammar.h"
#include <map>

class GetNullableVisitor : public GrammarVisitor {

    std::set<Symbol *> result;
public:
    void visit(Grammar &g) override;

    std::set<Symbol *> getResult();

};


#endif //PROJECT_GETNULLABLEVISITOR_H
