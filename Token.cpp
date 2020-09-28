//
// Created by Sichen Song on 9/26/2020.
//

#include "regular-expression/LongestReMatcher.h"
#include "Token.h"

Token::Token(std::string t, std::string v) : type(std::move(t)), value(std::move(v)) {

}