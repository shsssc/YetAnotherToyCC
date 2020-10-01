# YetAnotherToyCC

[![img](https://camo.githubusercontent.com/377a9d586c22dc8b43d6664d4a28c5c0aec503af/68747470733a2f2f6170692e7472617669732d63692e636f6d2f7368737373632f596574416e6f74686572546f7943432e7376673f6272616e63683d6d6173746572267374617475733d706173736564)](https://camo.githubusercontent.com/377a9d586c22dc8b43d6664d4a28c5c0aec503af/68747470733a2f2f6170692e7472617669732d63692e636f6d2f7368737373632f596574416e6f74686572546f7943432e7376673f6272616e63683d6d6173746572267374617475733d706173736564)

A toy compiler compiler with a lexer generator.

## Files

* `Google_tests` is the testing library
* `test` folders are test cases for each module
* `visitors` are grammar visitors that can access and transform grammar to validate/cleanup the grammar.
* `LL-Parsing` folder contains the LL parser generator.
* `Nondirectional-Parsing` folder contains non-directional parser (which is very slow).
* `regular-expression` folder contains a naive implementation of regular-expression together with a simple lexer generator.
* Files in the root folder are the shared data structures.

## Usage

An example LL1 parser can be programmed as the following:

```cpp
#include <iostream>
#include "Grammar.h"
#include "./Nondirectional-Parsing/UngerParser.h"
#include "./regular-expression/LongestReMatcher.h"
#include "./LL-Parsing/LL1Parser.h"

int main() {
    auto s = readfile(PROJ_ROOT
                      R"(/test/configs/ll1-first-test)");
    Grammar g(s);
    LL1Parser p(g);

    s = readfile(PROJ_ROOT
                 R"(/regular-expression/configs/simple)");
    LongestReMatcher l(s);

    auto tokens = l.match(" ({(abc);});");
    LongestReMatcher::removeTokenFromList(tokens, "SPACE");
    std::vector<Token> vtokens(tokens.begin(), tokens.end());
    if (p.validate(vtokens)) {
        std::cout << "Good!!" << std::endl;
    }else{
        std::cout << "bad!!" << std::endl;
    }
    return 0;
}
```

The lexer generator config is in the following format:

```
SPACE = ( )+
IF = if
ID = [a-zA-Z_][a-zA-Z_0-9]*
SC =       ;
LP = [(]
RP = [)]
LB =[{]
RB   =                    [}]
Quote   ="[a-z0-9A-Z ]*"
Plus= \+
Minus= [\-]
Number = [0-9]+
```

And the context free grammar can be configed use the format:

```
A := 'LP' B 'RP' C | 'IF' ;
B := 'LB' A 'RB' | '' | 'ID';
C := D 'SC';
D := B;
```

You can refer to `test` folder of each module for more details.

