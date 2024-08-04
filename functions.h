#ifndef LAB2_FUNCTIONS_H
#define LAB2_FUNCTIONS_H

#include <algorithm>
#include "tree/tree.h"

class DFA {
private:
    int startState;
    std::vector<char> alphabet;
    std::map<int, bool> acceptStates;
    std::map<int, std::map<char, int>> transitionFunction;

    bool sup;

public:
    DFA() {startState = 1; sup = false;}
    DFA(int start, std::vector<char> &alp, std::map<int, bool> &accept, std::map<int, std::map<char, int>> &transition)
            : startState(start), alphabet(std::move(alp)), acceptStates(std::move(accept)), transitionFunction(std::move(transition)) {sup = false;}

    void setAlp(auto &pos){
        for(auto a : pos){
            bool find = false;
            for(auto b : alphabet){
                if(b == a){find = true;}
            }
            if(!find){
                alphabet.push_back(a);
            }
        }
    }
    auto &getStart(){return startState;}
    auto &getAlp(){return alphabet;}
    auto &getStates(){return acceptStates;}
    auto &getTransit(){return transitionFunction;}

    void addTransit(int s, char a, int e){
        transitionFunction[s][a] = e;
    }

    void printTransit();
    void printAlph();
    void printStates();

    void printDFA(){
        std::cout << "Starting State: " << startState << std::endl;
        printAlph();
        printStates();
        printTransit();
    }

    bool accepts(const std::string &input) {
        int currentState = startState;
        for (char symbol : input) {
            int find = false;
            for(auto a : alphabet){
                if(a == symbol){find = true;}
            }
            if(find){
                currentState = transitionFunction[currentState][symbol];
            }else{
                currentState = transitionFunction[currentState]['~'];
            }
        }
        return acceptStates[currentState];
    }
    bool income(std::string &str, int &b);//непересекающиеся вхождения
    void supplement();
    bool DFAtoRegEx(std::string &res);
};

class RegEx {
private:
    std::string re;
    DFA Automata;
    std::shared_ptr<Node> myTree;

    bool compiled;
public:
    explicit RegEx(std::string &r){
        re = r;
        compiled = false;
    }
    explicit RegEx(std::string &r, DFA &dfa){
        re = r;
        Automata = dfa;
    }
    DFA &getDFA(){return Automata;}
    void setDFA(int start, std::vector<char> &alp, std::map<int, bool> &accept, std::map<int, std::map<char, int>> &transition){
        Automata.getTransit() = transition;
        Automata.getStates() = accept;
        Automata.getAlp() = alp;
        Automata.getStart() = 1;
    }
    std::string findall(std::string &str);
    void compile(bool pr);
    void minimizeDFA(bool prdiv);
    bool match(const std::string &re);
    void supplement();
    bool DFAtoRegEx(std::string &res);
};

void printFP(auto &FP){
    for (auto & it : FP) {
        std::cout << "Key: " << it.first << ", Value: ";
        for (int i : it.second) {
            std::cout << i << ' ';
        }
        std::cout << std::endl;
    }
}

void sortFP(auto &FP){
    for (auto & it : FP) {
        std::sort(it.second.begin(), it.second.end());
    }
}

//DFA &intersectDFA(DFA &dfa1, DFA dfa2);
#endif //LAB2_FUNCTIONS_H
