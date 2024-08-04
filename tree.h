#ifndef LAB2_TREE_H
#define LAB2_TREE_H

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <utility>


class Node {
public:
    bool N = false;
    int position = 0;
    std::vector<int> F;
    std::vector<int> L;

    virtual char &getchar() = 0;
    virtual char &gettype() = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual void setN() = 0;
    virtual void setNumb(std::vector<char>& positions) = 0;
    virtual void setF() = 0;
    virtual void setL() = 0;
    virtual void confFP(std::map<int, std::vector<int>> &FP) = 0;
    //friend std::ostream& operator<<(std::ostream& os, const Node& n);
    virtual ~Node() = default;
};

class Char : public Node {
public:
    char ch;
    char type = 0;//type of node
    explicit Char(char a):ch(a){}
    ~Char() override = default;
    char &getchar() override {return ch;}
    char &gettype() override {return type;}
    void setN() override {}
    void setNumb(std::vector<char>& positions) override {}
    void setF() override {}
    void setL() override {}
    void confFP(std::map<int, std::vector<int>> &FP) override {}
    void print(std::ostream& os) const override{
        os << this->ch;
    }
};

class ANode : public Node {
    char type = 1;
public:
    char ch;
    explicit ANode(char a):ch(a){}
    char &gettype() override {return type;}
    char &getchar() override {return ch;}
    void setN() override {N = true;}
    void setNumb(std::vector<char>& positions) override {
        position = (int)positions.size();
        positions.push_back(ch);
    }
    void setF() override {
        F.clear();
        F.push_back(position);
    }
    void setL() override {
        L.clear();
        L.push_back(position);
    }
    void confFP(std::map<int, std::vector<int>> &FP) override {}

    void print(std::ostream& os) const override{
        os << this->ch;
    }
};

class OrNode : public Node {
    char type = 5;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
public:
    char ch = 0;
    explicit OrNode(std::shared_ptr<Node> &l, std::shared_ptr<Node> &r):left(l), right(r){}
    char &gettype() override {return type;}
    char &getchar() override {return ch;}
    void setN() override {
        left->setN();
        right->setN();
        if(!left->N && !right->N){
            N = false;
        }else{
            N = true;
        }
    }
    void setNumb(std::vector<char>& positions) override {
        left->setNumb(positions);
        right->setNumb(positions);
    }
    void setF() override {
        left->setF();
        right->setF();
        F.clear();
        F.insert(F.end(), left->F.begin(), left->F.end());
        F.insert(F.end(), right->F.begin(), right->F.end());
    }
    void setL() override {
        left->setL();
        right->setL();
        L.clear();
        L.insert(L.end(), left->L.begin(), left->L.end());
        L.insert(L.end(), right->L.begin(), right->L.end());
    }
    void confFP(std::map<int, std::vector<int>> &FP) override {
        left->confFP(FP);
        right->confFP(FP);
    }

    void print(std::ostream& os) const override{
        os << "(";
        left->print(os);
        os << "|";
        right->print(os);
        os << ")";
    }
};

class ConNode : public Node {
    char type = 4;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
public:
    char ch = 0;

    explicit ConNode(std::shared_ptr<Node> &l, std::shared_ptr<Node> &r):left(l), right(r){}

    char &gettype() override {return type;}
    char &getchar() override {return ch;}
    void setN() override {
        left->setN();
        right->setN();
        if(left->N || right->N){
            N = true;
        }else{N = false;}
    }
    void setNumb(std::vector<char>& positions) override {
        left->setNumb(positions);
        right->setNumb(positions);
    }
    void setF() override {
        left->setF();
        right->setF();
        F.clear();
        if(!left->N){
            F.insert(F.end(), left->F.begin(), left->F.end());
            F.insert(F.end(), right->F.begin(), right->F.end());
        }else{
            F.insert(F.end(), left->F.begin(), left->F.end());
        }
    }
    void setL() override {
        left->setL();
        right->setL();
        L.clear();
        if(!right->N){
            L.insert(L.end(), left->L.begin(), left->L.end());
            L.insert(L.end(), right->L.begin(), right->L.end());
        }else{
            L.insert(L.end(), right->L.begin(), right->L.end());
        }
    }
    void confFP(std::map<int, std::vector<int>> &FP) override {
        left->confFP(FP);
        right->confFP(FP);
        for(int a : left->L){
            if (FP.count(a) > 0) {
                for(int b : right->F){
                    FP[a].push_back(b);
                }
            } else {
                throw std::runtime_error("Not found a Position!!!");
            }
        }
    }

    void print(std::ostream& os) const override{
        left->print(os);
        right->print(os);
    }
};

class KleeneNode : public Node {
    char type = 2;
    std::shared_ptr<Node> Operand;
public:
    char ch = 0;
    explicit KleeneNode(std::shared_ptr<Node> &op):Operand(op){}
    char &gettype() override {return type;}
    char &getchar() override {return ch;}
    void setN() override {
        Operand->setN();
        N = false;
    }
    void setNumb(std::vector<char>& positions) override {
        Operand->setNumb(positions);
    }
    void setF() override {
        Operand->setF();
        F.clear();
        F.insert(F.end(), Operand->F.begin(), Operand->F.end());
    }
    void setL() override {
        Operand->setL();
        L.clear();
        L.insert(L.end(), Operand->L.begin(), Operand->L.end());
    }
    void confFP(std::map<int, std::vector<int>> &FP) override {
        Operand->confFP(FP);
        for(int a : Operand->L){
            if (FP.count(a) > 0) {
                for(int b : Operand->F){
                    FP[a].push_back(b);
                }
            } else {
                throw std::runtime_error("Not found a Position!!!");
            }
        }
    }

    void print(std::ostream& os) const override{
        os << '(';
        Operand->print(os);
        os << ")*";
    }
};
bool isANode(char ch);
std::shared_ptr<Node> makingTree(const std::string &regex);

#endif //LAB2_TREE_H
