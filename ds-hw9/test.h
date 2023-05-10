#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

#include "graph.h"

class test {
private:
    std::ifstream input;
    std::ifstream output;

public:
    explicit test(const std::string &in, const std::string &out):
    input(in), output(out) {}
    ~test() { input.close(); output.close(); }

    void report() {
        graph grf;
        int total{}, correct{}, start, end, stu, ans;
        char delim;
        while (input.good()) {
            input >> start >> delim >> end;
            grf.insert(start, end, 0);
        }

        std::stringstream out;
        std::streambuf *oldBuf = std::cout.rdbuf(out.rdbuf());
        grf.topological();
        std::cout.rdbuf(oldBuf);

        while (output.good()) {
            output >> ans >> delim;
            out >> stu;
            total++;
            std::cout << ans << " || " << stu;
            if (ans != stu) std::cout << "!";
            else correct++;
            std::cout << std::endl;
        }

        if (total == correct) std::cout << "correct!" << std::endl;
        else std::cout << "false!" << std::endl;
    }
};

