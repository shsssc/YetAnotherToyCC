//
// Created by Sichen Song on 9/8/2020.
//

#ifndef PROJECT_REMOVENULLABLEVISITOR_H
#define PROJECT_REMOVENULLABLEVISITOR_H

#include "../Grammar.h"
#include "GetNullableVisitor.h"

class RemoveNullableVisitor : public GrammarVisitor {

    static void
    recursiveRemoveNullable(Rhs &rule,
                            Rhs::iterator currentLocation,
                            const std::set<Symbol *> &nullables,
                            RhsList &result,
                            Rhs &pendingRhs);

public:


    void visit(Grammar &g) override;
};


#endif //PROJECT_REMOVENULLABLEVISITOR_H
