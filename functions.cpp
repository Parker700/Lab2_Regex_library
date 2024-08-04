#include "functions.h"

#include <algorithm>

int getIndex(const auto &mas, const auto &a){
    auto it = std::find(mas.begin(), mas.end(), a);
    int i;
    if (it != mas.end()) {
        i = (int)std::distance(mas.begin(), it);
        return i;
    } else {
        return -1;
    }
}

void DFA::printTransit(){
    std::cout << "All transitions: " << std::endl;
    for (const auto &outerPair : transitionFunction) {
        int outerKey = outerPair.first;
        std::map<char, int> innerMap = outerPair.second;
        for (const auto &innerPair : innerMap) {
            char innerKey = innerPair.first;
            int value = innerPair.second;
            std::cout << "Start state: " << outerKey << ", Char: " << innerKey << ", New state: " << value << std::endl;
        }
    }
}

void DFA::printAlph() {
    std::cout << "Alphabet of DFA : ";
    for(auto a : alphabet){
        std::cout << a << ' ';
    }
    std::cout << std::endl;
}

void DFA::printStates(){
    std::cout << "Printing our states: " << std::endl;
    int n = 0;
    for (auto &a: acceptStates) {
        std::cout << a.first << " - ";
        if(a.second){
            std::cout << "acceptable";
        }else{
            std::cout << "not";
        }
        std::cout << std::endl;
        n++;
    }
}

void makeDFA(DFA &Automata, std::shared_ptr<Node> &tree, auto &FP, std::vector<char> &positions, bool pr){
    std::vector<std::vector<int>> states;
    std::vector<std::vector<int>> pStates;//processing states
    states.emplace_back();
    std::sort(tree->F.begin(), tree->F.end());
    states.push_back(tree->F);
    pStates.push_back(tree->F);
    while(!pStates.empty()){
        auto curState = pStates.front();
        for(auto a : Automata.getAlp()){
            std::vector<int> cur;
            for(auto b : curState){
                if(a == positions.at(b)) {//we found a transition
                    std::vector<int> vec2 = FP[b];
                    cur.insert(cur.end(), vec2.begin(), vec2.end());
                }
            }
            std::sort(cur.begin(), cur.end());
            if(!cur.empty()){//we found a transition
                bool find = false;
                for(const auto &d : states){
                    if(d == cur){find = true;}
                }
                if(!find){
                    states.push_back(cur);
                    pStates.push_back(cur);
                }
                int s = getIndex(states, curState);
                int e = getIndex(states, cur);
                Automata.addTransit(s, a, e);
            }
        }
        //Automata.getTransit()[curState] =
        int starting = getIndex(states, curState);
        for(auto a : Automata.getAlp()) {
            if (Automata.getTransit().count(starting) > 0 && Automata.getTransit()[starting].count(a) > 0) {
            }else{
                Automata.addTransit(starting, a, 0);
            }
        }

        if (!pStates.empty()) {
            pStates.erase(pStates.begin());
        }
    }

    int s = 0;//setting acceptable for states
    int i = getIndex(positions, '#');
    for(auto &a : states){
        bool find = false;
        for(const auto &d : a){
            if(d == i){find = true;}
        }
        if(find){
            Automata.getStates().insert(std::make_pair(s, true));
        }else{
            Automata.getStates().insert(std::make_pair(s, false));
        }
        s++;
    }

    for(auto a: Automata.getAlp()){//adding combinations from zero to zero
        Automata.addTransit(0, a, 0);
    }

    for(auto a: Automata.getStates()){//adding transitions to zero from every state
        Automata.addTransit(a.first, '~', 0);
    }

    if(pr) {
        std::cout << "Printing raw states: " << std::endl;
        int n = 0;
        for (const auto &a: states) {
            std::cout << n << ": ";
            for (auto b: a) {
                std::cout << b << ' ';
            }
            std::cout << std::endl;
            n++;
        }
    }
}

void RegEx::compile(bool pr) {
    try {
        std::vector<char> positions; //positions
        std::map<int, std::vector<int>> FP;//FP table
        auto tree = makingTree(re);
        myTree = tree;

        tree->setNumb(positions);
        tree->setN();
        tree->setF();
        tree->setL();

        for(int i = 0; i < positions.size(); i++){//initialization of FP table
            std::vector<int> T;
            FP.insert(std::make_pair(i, T));
        }

        tree->confFP(FP);
        sortFP(FP);

        Automata.setAlp(positions);//setting alphabet
        makeDFA(Automata, tree, FP, positions, pr);//pr - printing of raw states;
        minimizeDFA(pr);
        //Automata.printTransit();


        if(pr) {
            printFP(FP);
            tree->print(std::cout);//printing our tree
            std::cout << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    compiled = true;
}
void RegEx::minimizeDFA(bool prdiv){
    DFA &dfa = Automata;
    std::vector<std::vector<int>> P;
    std::vector<int> Accept;
    std::vector<int> notAccept;
    for(auto a : dfa.getStates()){
        if(a.second){
            Accept.push_back(a.first);
        }else{
            notAccept.push_back(a.first);
        }
    }
    P.push_back(notAccept);
    P.push_back(Accept);
    std::sort(P.begin(), P.end());
    std::vector<std::vector<int>> newP = P;
    std::vector<std::vector<int>> tempP = newP;
    std::vector<std::vector<int>> checkP;
    if(prdiv) {
        std::cout << "Minimizing DFA:\n";
    }
    do{
        checkP = tempP;
        if(prdiv) {
            for (const auto &a: tempP) {//print dividing
                std::cout << "[";
                for (auto b: a) {
                    std::cout << b;
                }
                std::cout << "] ";
            }
            std::cout << std::endl;
        }

        for (auto c: dfa.getAlp()) {
            tempP.clear();
            for (const auto &Gi: newP) {
                std::multimap<int, int> temp;
                for (auto s: Gi) {
                    int p = dfa.getTransit()[s][c];
                    int i = 0;
                    for (int k = 0; k < (int) newP.size(); k++) {
                        bool find = false;
                        for (auto b: newP.at(k)) {
                            if (b == p) { find = true; }
                        }
                        if (find) {
                            i = k;
                            k = (int) newP.size();
                        }
                    }
                    temp.insert(std::make_pair(i, s));
                }

                int currentKey = temp.begin()->first;
                std::vector<int> tempVec;
                for (const auto &pair: temp) {
                    if (pair.first != currentKey) {
                        tempP.push_back(tempVec);
                        tempVec.clear();
                        currentKey = pair.first;
                    }
                    tempVec.push_back(pair.second);
                }
                tempP.push_back(tempVec);
            }
            newP = tempP;
        }
        std::sort(newP.begin(), newP.end());
    }while(newP != checkP);
    newP = tempP;//finally

    int startStateN;
    std::vector<char> alphabetN;
    std::map<int, bool> acceptStatesN;
    std::map<int, std::map<char, int>> transitionFunctionN;

    for(int i = 0; i < (int)newP.size(); i++){
        if(!newP.at(i).empty()) {
            for(auto b : newP.at(i)){
                if(b == dfa.getStart()){
                    startStateN = i;
                }
            }
            bool accept = dfa.getStates()[newP.at(i).at(0)];
            acceptStatesN[i] = accept;
            for (auto alp: dfa.getAlp()){
                int s = dfa.getTransit()[newP.at(i).at(0)][alp];
                int e = 0;
                for(int k = 0; k < (int)newP.size(); k++){
                    for(auto b : newP.at(k)){
                        if(b == s){e = k;}
                    }
                }
                transitionFunctionN[i][alp] = e;
            }
        }
    }
    alphabetN = dfa.getAlp();

    DFA Temp(startStateN, alphabetN, acceptStatesN, transitionFunctionN);
    Automata = Temp;
    if(prdiv) {
        std::cout << "Minimizing end.\n";
    }
}

bool RegEx::match(const std::string &r) {
    if(!compiled){
        compile(false);
    }
    return Automata.accepts(r);
}

bool DFA::income(std::string &str, int &b){
    int currentState = startState;
    int lastState = currentState;
    b = 0;
    bool last = false;
    bool found = false;
    for (int i = 0; i < str.size(); i++) {
        int find = false;
        for(auto a : alphabet){
            if(a == str.at(i)){find = true;}
        }
        lastState = currentState;
        if(find){
            currentState = transitionFunction[currentState][str.at(i)];
        }else{
            currentState = transitionFunction[currentState]['~'];
        }
        if(currentState == 0){
            if(sup){
                b = i;
            }else{
                i = (int)str.size();
            }
            last = true;
        }else if(acceptStates[currentState]){
            b = i;
            b++;
            found = true;
        }
    }
    if(sup){
        b++;
    }
    if(currentState != 0 && acceptStates[currentState]){
        b = (int)str.size();
    }
    if(found){
        return true;
    }else if(!last){
        return acceptStates[currentState];
    }else{
        if(sup){
            return true;
        }else{
            return acceptStates[lastState];
        }
    }
}

void DFA::supplement() {
    for(auto& pair : acceptStates) {
        pair.second = !pair.second;
    }
    if(!sup){
        sup = true;
    }else{
        sup = false;
    }
}

bool DFA::DFAtoRegEx(std::string &res) {
    std::map<int, bool> acceptStatesN = acceptStates;
    std::map<int, std::map<std::string, int>> transitionFunctionN;
    for (const auto &outerPair : transitionFunction) {
        int outerKey = outerPair.first;
        for (const auto &innerPair : outerPair.second) {
            char innerKey = innerPair.first;
            int value = innerPair.second;
            std::cout << "Start state: " << outerKey << ", Char: " << innerKey << ", New state: " << value << std::endl;
        }
    }
    return false;
}

std::string RegEx::findall(std::string &str) {
    if(!compiled){
        compile(false);
    }
    int b;
    std::string newStr = str;
    int plusInd = 0;
    for(int i = 0; i < str.size(); i++) {
        std::string sub = str.substr(i, str.size() - i);
        bool a = Automata.income(sub, b);
        if(a) {
            newStr.insert(i + plusInd, "\033[32m");
            plusInd += 5;
            newStr.insert(i + plusInd + b, "\033[0m");
            plusInd += 4;
            //std::cout << a << ' ' << b << ' ';
        }
        i += b;
    }
    //std::cout << newStr << std::endl;
    return newStr;
}

void RegEx::supplement() {
    Automata.supplement();
}

bool RegEx::DFAtoRegEx(std::string &res) {
    return Automata.DFAtoRegEx(res);
}

/*DFA &intersectDFA(DFA &dfa1, DFA &dfa2){

}*/
