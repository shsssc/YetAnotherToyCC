//
// Created by Sichen Song on 9/28/2020.
//

#ifndef PROJECT_LL1PARSER_H
#define PROJECT_LL1PARSER_H

#include "../ParseTree.h"
#include "../Token.h"
#include "../Grammar.h"
#include "../visitors/GetNullableVisitor.h"
#include "../visitors/RemoveUnreachableVisitor.h"
#include "../visitors/RemoveUnproducableVisitor.h"
#include <unordered_map>
#include <vector>

class LL1TableBuilder : public GrammarVisitor {
protected:
    Grammar _g;
    std::set<Symbol *> _nullables;
    std::unordered_map<Symbol *, std::set<Symbol *>> _first;
    std::unordered_map<Symbol *, std::set<Symbol *>> _follow;
    std::unordered_map<Symbol *, std::unordered_map<Symbol *, Rhs *> > _table;
    std::unordered_map<Symbol *, Rhs *> _nullableRule;
    enum ScanState {
        searchSymbol,
        scanUntilNotNullable
    };

    Rhs *findNullableRule(Symbol *s) {
        Rhs *result = nullptr;
        if (_nullables.find(s) != _nullables.end()) {
            for (auto &rule: _g.getRules().getProductionFor(s)) {
                bool isNullable = true;
                for (auto symbol:rule) {
                    if (symbol != Symbol::terminal("") && _nullables.find(symbol) == _nullables.end()) {
                        isNullable = false;
                    }
                }
                if (isNullable) {
                    result = &rule;
                    break;
                }
            }

        }
        if (result == nullptr)throw std::runtime_error("did not find nullable rhs");
        return result;
    }

    void printFirstSet() {
        std::cout << "The first set:\n";
        for (auto &x:_first) {
            std::cout << x.first << " :";
            for (auto y:x.second) {
                std::cout << y << ",";
            }
            std::cout << std::endl;
        }
    }


    void printFollowSet() {
        std::cout << "The follow set:\n";
        for (auto &x:_follow) {
            std::cout << x.first << " :";
            for (auto y:x.second) {
                std::cout << y << ",";
            }
            std::cout << std::endl;
        }
    }

    void printParseTable() {
        std::cout << "\nThe LL1 parse table:\n";
        for (auto &p1:_table) {
            auto nonterminal = p1.first;
            for (auto &p2:p1.second) {
                auto terminal = p2.first;
                std::cout << "For the non-terminal " << nonterminal->getName() << " and terminal "
                          << terminal->getName()
                          << "\n\t";
                for (auto s:*p2.second) {
                    std::cout << s << " ";
                }
                std::cout << std::endl;
            }
        }
        std::cout << "\n";
    }

    void insertRule(Symbol *nonterminal, Symbol *terminal, Rhs *rule) {
        if (_table.find(nonterminal) != _table.end()
            && _table[nonterminal].find(terminal) != _table[nonterminal].end()
            && _table[nonterminal][terminal] != rule) {
            std::cout << nonterminal << std::endl;
            throw std::runtime_error("not a ll1 grammar");
        } else
            _table[nonterminal][terminal] = rule;
    }

    void follow(Symbol *s, std::set<Symbol *> &visited) {
        std::set<Symbol *> &result = _follow[s];
        if (visited.find(s) != visited.end()) return;
        bool sIsNullable = _nullables.find(s) != _nullables.end();
        Rhs *nullableRule = sIsNullable ? findNullableRule(s) : nullptr;
        visited.insert(s);

        for (auto lhs:Symbol::nonTerminals()) {
            if (!_g.getRules().contains(lhs))continue;
            for (auto &rhs: _g.getRules().getProductionFor(lhs)) {
                ScanState state = searchSymbol;
                for (auto sym:rhs) {
                    if (state == searchSymbol && sym == s) {
                        state = scanUntilNotNullable;
                        continue;
                    }
                    if (state == scanUntilNotNullable) {
                        //follow by terminal
                        if (sym->isTerminal()) {
                            result.insert(sym);
                            if (sIsNullable) insertRule(s, sym, nullableRule);
                            state = searchSymbol;
                            continue;
                        }
                        //follow by non-terminal
                        result.insert(_first[sym].begin(), _first[sym].end());
                        if (sIsNullable) {
                            for (auto sym1 : _first[sym]) {
                                insertRule(s, sym1, nullableRule);
                            }
                        }
                        //if current is not nullable, we can end the search
                        if (_nullables.find(sym) == _nullables.end()) {
                            state = searchSymbol;
                            continue;
                        }
                        continue;
                    }
                }
                //at last symbol, must do recursion
                if (state == scanUntilNotNullable) {
                    follow(lhs, visited);
                    result.insert(_follow[lhs].begin(), _follow[lhs].end());
                    if (sIsNullable) {
                        for (auto sym1 : _follow[lhs]) {
                            insertRule(s, sym1, nullableRule);
                        }
                    }
                }
            }
        }
        visited.erase(s);
    }

    void first(Symbol *s, std::set<Symbol *> &visited) {
        std::set<Symbol *> &result = _first[s];
        if (visited.find(s) != visited.end()) return;
        visited.insert(s);
        for (auto &rule:_g.getRules().getProductionFor(s)) {
            for (auto sym:rule) {
                if (sym->isTerminal()) {
                    if (sym->getName().empty())break;
                    result.insert(sym);
                    insertRule(s, sym, &rule);
                    break;
                }
                first(sym, visited);
                result.insert(_first[sym].begin(), _first[sym].end());
                for (auto sym1 : _first[sym]) {
                    insertRule(s, sym1, &rule);
                }

                if (_nullables.find(sym) == _nullables.end()) {
                    break;
                }
            }
        }
        visited.erase(s);
    }

public:

    void visit(Grammar &g) override {
        //RemoveUnproducableVisitor().visit(g);
        //RemoveUnreachableVisitor().visit(g);
        GetNullableVisitor gnv;
        gnv.visit(g);
        _nullables = gnv.getResult();
        _g = g;
        auto lhs = Symbol::nonTerminal("Generated-Start");
        Rhs rhs;
        rhs.push_back(_g.getStartSymbol());
        rhs.push_back(Symbol::terminal("EOF"));
        _g.getRules().addRule(lhs, rhs);
        for (auto s : Symbol::nonTerminals()) {
            if (!g.getRules().contains(s))continue;
            std::set<Symbol *> visited;
            first(s, visited);
        }
        for (auto s : Symbol::nonTerminals()) {
            if (!g.getRules().contains(s))continue;
            std::set<Symbol *> visited;
            follow(s, visited);
        }
    }
};

class LL1Parser : public LL1TableBuilder {
    std::pair<int, std::shared_ptr<Node>> parse(Symbol *s, int start, std::vector<Token> &tokens) {
        auto result = std::make_shared<Node>();
        result->_symbol = s;
        //empty is matched
        if (s->isTerminal() && s->getName().empty()) {
            result->_terminal = true;
            result->_value = "";
            return {start, result};
        }
        if (start < 0 || start >= tokens.size()) {
            throw std::runtime_error("unexpected start");
        }
        if (s->isTerminal()) {
            if (s->getName() == tokens[start].type) {
                result->_terminal = true;
                result->_value = tokens[start].value;
                return {start + 1, result};
            } else
                throw std::runtime_error("failed to parse terminal" + s->getName());
        }
        if (_table.find(s) == _table.end()) throw std::runtime_error("unexpected symbol");
        const auto lookAhead = tokens[start];
        const auto lookAheadSym = Symbol::terminal(lookAhead.type);
        if (_table[s].find(lookAheadSym) == _table[s].end())
            throw std::runtime_error("unexpected terminal symbol" + lookAhead.type);
        auto &rule = *_table[s][lookAheadSym];
        result->_rule = rule;
        result->_terminal = false;
        int tmp = start;
        std::shared_ptr<Node> tmpnode;
        for (auto sym:rule) {
            std::tie(tmp, tmpnode) = parse(sym, tmp, tokens);
            result->_children.push_back(tmpnode);
        }
        return {tmp, result};
    }

public:
    LL1Parser(Grammar &g) : LL1TableBuilder() {
        g.accept(*this);
    }

    bool validate(std::vector<Token> &tokens) {
        try {
            return parse(_g.getStartSymbol(), 0, tokens).first == tokens.size();
        } catch (std::runtime_error e) {
            return false;
        }

    }

    std::shared_ptr<Node> parse(std::vector<Token> &input) {
        auto result = parse(_g.getStartSymbol(), 0, input);
        if (result.first != input.size())throw std::runtime_error("failed");
        return result.second;
    }

};


#endif //PROJECT_LL1PARSER_H
