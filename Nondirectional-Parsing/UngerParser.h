//
// Created by Sichen Song on 9/26/2020.
//

#ifndef PROJECT_UNGERPARSER_H
#define PROJECT_UNGERPARSER_H

#include <vector>
#include  "../Grammar.h"
#include "../Token.h"
#include "../ParseTree.h"

class UngerParser {
    static void divideLength(int length, int parts, std::vector<int> &tmp, std::vector<std::vector<int>> &result) {
        if (parts == 1 && length > 0) {
            tmp.push_back(length);
            result.push_back(tmp);
            tmp.pop_back();
            return;
        }
        for (int current = 1; current <= length - parts + 1; current++) {
            tmp.push_back(current);
            divideLength(length - current, parts - 1, tmp, result);
            tmp.pop_back();
        }
    }

    std::shared_ptr<Node> recValidate(const std::vector<Token> &input, int l, int r, Symbol *s) {
        //should never happen. Only for nullable
        if (r < l)return nullptr;
        //terminal match
        if (s->isTerminal()) {
            //std::cout << input[l].type << s->getName() << std::endl;
            if (l == r && input[l].type == s->getName()) {
                auto result = std::make_shared<Node>();
                result->_symbol = s;
                result->_terminal = true;
                result->_value = input[l].value;
                return result;
            }
            return nullptr;
        }
        //no production
        if (!_g.getRules().contains(s))return nullptr;

        //each production
        auto result = std::make_shared<Node>();
        result->_symbol = s;
        result->_terminal = false;
        for (auto &rule : _g.getRules().getProductionFor(s)) {
            if (rule.size() > r - l + 1)continue;//skip too long rules
            result->_rule = rule;
            auto divideResult = divideLength(r - l + 1, rule.size());
            for (auto &divide:divideResult) {
                bool accepted = true;
                int left = l;
                auto currentSymbol = rule.begin();
                for (int currentLen : divide) {
                    int right = left + currentLen - 1;
                    auto recResult = recValidate(input, left, right, *currentSymbol);
                    if (!recResult) {
                        accepted = false;
                        break;
                    }
                    result->_children.push_back(recResult);
                    left = right + 1;
                    currentSymbol++;
                }
                if (accepted)return result;
                else {
                    result->_children.clear();
                }
            }
        }
        //all failed
        return nullptr;
    }

    Grammar _g;
public:
    explicit UngerParser(Grammar &g) : _g(g) {
    }

    bool validate(const std::vector<Token> &input) {
        return recValidate(input, 0, input.size() - 1, _g.getStartSymbol()) != nullptr;
    }

    std::shared_ptr<Node> parse(const std::vector<Token> &input) {
        return recValidate(input, 0, input.size() - 1, _g.getStartSymbol());
    }

    static std::vector<std::vector<int>> divideLength(int length, int parts) {
        std::vector<std::vector<int>> result;
        std::vector<int> tmp;
        divideLength(length, parts, tmp, result);
        return result;
    }

};


#endif //PROJECT_UNGERPARSER_H
