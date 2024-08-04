#include <iostream>
#include "work/functions.h"

int main() {
    std::string re = "int|short|long"; // regular expression
    RegEx FA(re);
    FA.compile(false);//bool, print working objects
    std::cout << "\033[33mRegEx: " << re << "\033[0m" << std::endl;

    std::cout << "Print your text for finding with RegEx: " << std::endl;//Non-overlapping occurrences
    std::string line;
    std::getline(std::cin, line);
    while(line != "exit") {
        line = FA.findall(line);
        std::cout << line << std::endl;
        std::cout << "Print your next to find in: " << std::endl;
        std::getline(std::cin, line);
    }

    std::cout << "Print your text for matching to RegEx: " << std::endl;
    std::getline(std::cin, line);
    while(line != "exit") {
        std::cout << "Your sentence is ";
        if(FA.match(line)){
            std::cout << "\033[32macceptable!\033[0m" << std::endl;
        }else{
            std::cout << "\033[31mnot acceptable!!!\033[0m" << std::endl;
        }
        std::cout << "Print your text: " << std::endl;
        std::getline(std::cin, line);
    }
    return 0;
}

//reserved symbols: (, ), {, }, |, #, ~(126)

//(int|short|long) [qwertyuiopasdfghjklzxcvbnm][0123456789qwertyuiopasdfghjklzxcvbnm]... = [0123456789]...