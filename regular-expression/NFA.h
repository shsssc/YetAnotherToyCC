//
// Created by Sichen Song on 9/16/2020.
//

#ifndef PROJECT_NFA_H
#define PROJECT_NFA_H

#include "NFAState.h"
#include <stdexcept>

namespace NFA {
    typedef NFAState *State;

    class NFA {
        static void findEpsilonClosure(State s, std::unordered_set<State> &result);

        static std::unordered_set<State> findEpsilonClosure(const std::unordered_set<State> &states);

        State _start;
        State _accept;//we require only one accept state
        std::unordered_set<State> _current_location;
    public:
        NFA(State start, State accept);

        NFA();

        /**
         * refresh the current state. Must be used after manual modification of the states.
         */
        void refresh();

        void setStartState(State s);

        void setAcceptState(State s);

        State getStartState() const;

        State getAcceptState() const;

        std::unordered_set<State> getCurrentState() const;

        bool accepted() const;

        bool invalidPrefix() const;

        void take(char c);

        void take(const std::string &s);

    };

    /*
     * note that the following utilities WILL BREAK ITS INPUT
     */

    NFA nfaFromWords(const std::string &w);

    NFA nfaFromCharSet(const std::string &charSet);

    void nfaConcat(NFA &n1, const NFA &n2);

    void nfaStar(NFA &n);

    void nfaQuestion(NFA &n);

    void nfaPlus(NFA &n);

    void nfaOr(NFA &n1, const NFA &n2);
}


#endif //PROJECT_NFA_H
