#include "tree.h"

#include <list>

bool isANode(char ch){
    if(ch == '|'){
        return false;
    }else{
        return true;
    }
}

void findBrackets(std::list<std::shared_ptr<Node>> &list, std::shared_ptr<Node> &start, std::shared_ptr<Node> &end){
    bool findBrackets = false;
    std::list<std::shared_ptr<Node>>:: iterator it;
    it = list.begin();
    do{                 //finding ( and )
        if (it->get()->getchar() == '(' && it->get()->gettype() == 0) {
            start = *it;
            findBrackets = true;
        } else if (it->get()->getchar() == ')' && findBrackets && it->get()->gettype() == 0) {
            end = *it;
            findBrackets = false;
        }
        it++;
    }while(it != list.end());
    /*start->print(std::cout);
    end->print(std::cout);
    std::cout << std::endl;*/
}
void SlashParse(std::list<std::shared_ptr<Node>> &list, std::shared_ptr<Node> &start, std::shared_ptr<Node> &end){
    std::list<std::shared_ptr<Node>>:: iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        if (*it == start) {
            break;
        }
    }
    bool Sl = false;
    it++;
    while(*it != end){
        //it->get()->print(std::cout);
        if(it->get()->getchar() == '\\' && !Sl){
            Sl = true;
        }else if(Sl){
            ANode Temp(it->get()->getchar());
            list.remove(*it--);
            list.remove(*it++);
            list.insert(it, std::make_shared<ANode>(Temp));
            Sl = false;
        }else{Sl = false;}
        it++;
        }

}//\ parse
void AParse(std::list<std::shared_ptr<Node>> &list, std::shared_ptr<Node> &start, std::shared_ptr<Node> &end){
    //*it = start;// was wrong
    std::list<std::shared_ptr<Node>>:: iterator it;
    for (it = list.begin(); it != list.end(); ++it) {//slide
        if (*it == start) {
            break;
        }
    }
    it++;
    while(*it != end){
        //it->get()->print(std::cout);
        if(isANode(it->get()->getchar()) && it->get()->gettype() == 0){
            ANode Temp(it->get()->getchar());
            list.remove(*it++);
            list.insert(it, std::make_shared<ANode>(Temp));
        }else{it++;}
    }
    //std::cout << std::endl;
}
void SelParse(std::list<std::shared_ptr<Node>> &list, std::shared_ptr<Node> &start, std::shared_ptr<Node> &end){//[abcdf]
    std::list<std::shared_ptr<Node>>:: iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        if (*it == start) {
            break;
        }
    }
    char Sel = 0;
    it++;
    while(*it != end) {
        if (it->get()->getchar() == '[' && Sel == 0) {
            Sel = 1;
        } else if (it->get()->getchar() == '[' && Sel != 0 || it->get()->getchar() == ']' && Sel != 2) {
            throw std::runtime_error("Wrong syntax of [r]");
        } else if (it->get()->getchar() != '[' && Sel == 1) {
            Sel = 2;
        } else if (it->get()->getchar() == ']' && Sel == 2) {
            list.remove(*it--);
            it--;
            list.remove(*it++);
            Sel = 0;
        } else if (it->get()->getchar() != '[' && Sel == 2 || it->get()->getchar() != ']' && Sel == 2) {//main
            auto right = *it;
            it--;
            auto left = *it;
            OrNode Temp(left, right);
            list.remove(*it++);
            list.remove(*it++);
            list.insert(it, std::make_shared<OrNode>(Temp));
            it--;
        }
        it++;
        if(*it == end && Sel != 0){
            throw std::runtime_error("Wrong syntax of [r");
        }
    }
}
void QuantParse(std::list<std::shared_ptr<Node>> &list, std::shared_ptr<Node> &start, std::shared_ptr<Node> &end){//kvantificator a{n}
    std::list<std::shared_ptr<Node>>:: iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        if (*it == start) {
            break;
        }
    }
    int Quant = 0;
    bool Bracket = false;
    int n;
    it++;
    while(*it != end){
        if(it->get()->getchar() != ')' && it->get()->getchar() != '(' && it->get()->getchar() != '{' && it->get()->getchar() != '}' && it->get()->gettype() == 0 && Quant == 0){
            Quant = 1;
            Bracket = false;
        }else if(it->get()->getchar() != ')' && it->get()->getchar() != '(' && it->get()->getchar() != '{' && it->get()->getchar() != '}' && it->get()->gettype() < 2 && Quant >= 1 && Quant != 2){
            Quant = 1;
            Bracket = false;
        }else if((it->get()->getchar() == ')'|| it->get()->getchar() == '(') && it->get()->getchar() != '{' && it->get()->getchar() != '}' && it->get()->gettype() == 0 && Quant < 2){
            Quant = 1;
            Bracket = true;
        }else if(it->get()->getchar() == '{' && it->get()->gettype() == 0 && Quant == 1 && Bracket){
            throw std::runtime_error("a{n}: a should be non bracket!!!");
        }else if(it->get()->getchar() == '{' && it->get()->gettype() == 0 && Quant == 1 && !Bracket){
            Quant = 2;
        }else if(it->get()->getchar() != ')' && it->get()->getchar() != '(' && it->get()->gettype() == 0 && Quant == 2){
            if(it->get()->getchar() > 47 && it->get()->getchar() < 58) {
                n = it->get()->getchar() - 48;
            }else{
                throw std::runtime_error("In quantifier r{n}: n need to be a number!");
            }
            Quant = 3;
        }else if(it->get()->getchar() == '}' && it->get()->gettype() == 0 && Quant == 3 && !Bracket){
            list.remove(*it--);
            list.remove(*it--);
            list.remove(*it--);
            int a = 0;
            char b = it->get()->getchar();
            list.remove(*it++);
            while(a < n){
                ANode Temp(b);
                list.insert(it, std::make_shared<ANode>(Temp));
                a++;
            }
            it--;
            Quant = 0;
        }else if(it->get()->getchar() == '}' && it->get()->gettype() == 0 && Quant == 3 && Bracket){
            throw std::runtime_error("a{n}: a should be non bracket!!!");
        }//else{Quant = 0;}
        it++;
    }
}
void OrParse(std::list<std::shared_ptr<Node>> &list, std::shared_ptr<Node> &start, std::shared_ptr<Node> &end){
    std::list<std::shared_ptr<Node>>:: iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        if (*it == start) {
            break;
        }
    }
    int Or = 0;
    it++;
    while(*it != end){
        if(it->get()->gettype() != 0 && Or != 2) {Or = 1;}
        else if(it->get()->gettype() == 0 && it->get()->getchar() == '|' && Or == 1){Or = 2;}
        else if(it->get()->gettype() == 0 && it->get()->getchar() == '|' && Or != 1){throw std::runtime_error("Wrong syntax of [x|b]");}
        else if(it->get()->gettype() != 0 && Or == 2){
            auto right = *it;
            it--; it--;
            auto left = *it;
            OrNode Temp(left, right);
            list.remove(*it++);
            list.remove(*it++);
            list.remove(*it++);
            list.insert(it, std::make_shared<OrNode>(Temp));
            it--;
            it--;
            Or = 0;
        }else{Or = 0;}
        it++;
        if(*it == end && Or == 2){
            throw std::runtime_error("Wrong syntax of (a|x)");
        }
        /*list.remove(*it++);
        list.insert(it, std::make_shared<ANode>(Temp));
    }else{it++;}*/
    }
}
void ConcatParse(std::list<std::shared_ptr<Node>> &list, std::shared_ptr<Node> &start, std::shared_ptr<Node> &end){
    std::list<std::shared_ptr<Node>>:: iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        if (*it == start) {
            break;
        }
    }
    bool Con = false;
    it++;
    while(*it != end) {
        if(it->get()->gettype() == 0) {//restart
            Con = false;
        }else if(it->get()->gettype() != 0 && !Con){//if false
            Con = true;
        }else if(it->get()->gettype() != 0 && Con){//if true
            auto right = *it;
            it--;
            auto left = *it;
            ConNode Temp(left, right);
            list.remove(*it++);
            list.remove(*it++);
            list.insert(it, std::make_shared<ConNode>(Temp));
            it--;it--;
            Con = false;
        }
        it++;
    }
}
void KleeneParse(std::list<std::shared_ptr<Node>> &list, std::shared_ptr<Node> &start, std::shared_ptr<Node> &end){
    std::list<std::shared_ptr<Node>>:: iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        if (*it == start) {
            break;
        }
    }
    char Kleene = 0;
    it++;
    while(*it != end) {
        if(it->get()->gettype() != 0 && it->get()->getchar() != '.'){Kleene = 1;}
        else if(it->get()->gettype() == 1 && it->get()->getchar() == '.' && Kleene < 3){Kleene++;}
        else if(it->get()->gettype() == 1 && it->get()->getchar() == '.' && Kleene == 3){
            std::advance(it, -3);
            auto op = *it;
            KleeneNode Temp(op);
            list.remove(*it++);
            list.remove(*it++);
            list.remove(*it++);
            list.remove(*it++);
            list.insert(it, std::make_shared<KleeneNode>(Temp));
            it--;it--;
            Kleene = 0;
        }else{Kleene = 0;}
        it++;
    }
}

std::shared_ptr<Node> makingTree(const std::string &regex){
    std::list<std::shared_ptr<Node>> list;
    std::shared_ptr<Node> start;
    std::shared_ptr<Node> end;
    std::list<std::shared_ptr<Node>>:: iterator it;//iterator for list

    for(char a : regex){// regex: string -> list<Node>
        Char Elem(a);
        list.push_back(std::make_shared<Char>(Elem));
    }
    Char Elem_s('(');//adding edge ( and )
    list.push_front(std::make_shared<Char>(Elem_s));
    Char Elem_ss('(');//adding edge ( and )
    list.push_front(std::make_shared<Char>(Elem_ss));
    Char Elem_ee(')');
    list.push_back(std::make_shared<Char>(Elem_ee));
    Char Elem_end('#');//adding end symbol, special symbol
    list.push_back(std::make_shared<Char>(Elem_end));
    Char Elem_e(')');
    list.push_back(std::make_shared<Char>(Elem_e));

    std::shared_ptr<Node> e = list.back();
    bool b = true;

    SlashParse(list, list.front(), list.back());//replacing \char form
    QuantParse(list,  list.front(), list.back());//replacing quantifier

    do{
    start = nullptr;
    end = nullptr;
    findBrackets(list, start, end);//finding start and end
    if(start == list.front() && end != list.back() || start != list.front() && end == list.back()){
        throw std::runtime_error("In brackets!!!");
    }
    if(end != nullptr) {

        AParse(list, start, end);//replacing simple symbols to ANode
        SelParse(list, start, end);//replacing [r]

        KleeneParse(list, start, end);//replacing Kleene syntax to KleeneNode
        ConcatParse(list, start, end);//replacing Concat syntax to ConNode
        OrParse(list, start, end);//replacing Or syntax to OrNode

        for (it = list.begin(); it != list.end(); ++it) {//deleting ( and )
            if (*it == start) {
                break;
            }
        }
        list.remove(*it++);
        it++;
        list.remove(*it--);
        /*it->get()->print(std::cout);
        std::cout << std::endl;*/
    }
    if(end == e){b = false;}
    }while(b);

    return list.front();
}

