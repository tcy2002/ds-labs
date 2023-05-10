//
// Created by Shen on 2022/2/27.
//

#include "SkipList.h"
#include "AVLTree.h"
#include "Container.h"
#include <iostream>
#include <fstream>
#include <chrono>

#define length 10000

std::string path = "../input/create_input";
std::string path1[3] = {"../input/random", "../input/order", "../input/reverse"};

void _create(Container *container, std::string input_file_path) {
    std::ifstream inputData;
    inputData.open(input_file_path, std::ios::in);
    if (!inputData) {
        std::cout << "[error]: file " << input_file_path << " not found." << std::endl;
        inputData.close();
        return;
    }

    std::string line;
    while (inputData >> line) {
        //cout<<line<<endl;
        int bracketPos = line.find('(');
        std::string op = line.substr(0, bracketPos);
        std::string param = line.substr(bracketPos + 1, line.size() - bracketPos - 2);
        if (op == "Insert") {
            int commaPos = param.find(',');
            int key = atoi(param.substr(0, commaPos).c_str());
            int val = atoi(param.substr(commaPos + 1).c_str());
            container->Insert(key, val);
        }
    }
    inputData.close();
};

double asl[2][3] = {0};
double cost[2][3] = {0};
double info[2][3]={0};
Container *create(int type, int index) {
    Container *container = nullptr;
    if (type == 1) {
        container = new AVLTree();
    } else {
        container = new SkipList();
    }
//    _create(container, path + std::to_string(length));
//    std::cout << path1[index] << std::endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    _create(container, path1[index] + std::to_string(length));
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
//    std::string t = type == 0 ? "Skiplist:" : "AVL Tree:";
//    std::cout << t << duration << "us asl:" << container->getASL() << std::endl;
    cost[type][index] += duration;
    asl[type][index] += container->getASL();
    info[type][index] += container->getInfo();
    return container;
}


int main() {
    srand((unsigned) time(NULL));
    Container *avltree;
    Container *skipList;
    int repeat = 100;
    for (int i = 0; i < 3; i++) {
        for (int t = 0; t < repeat; t++) {
            avltree = create(1, i);
            skipList = create(0, i);
            delete avltree;
            delete skipList;
        }
        std::cout<<path1[i]<<" avl: cost "<<cost[1][i]/repeat<<"us asl "<<asl[1][i]/repeat<<"step rotate "<<info[1][i]/repeat<<std::endl;
        std::cout<<path1[i]<<" skiplist: cost "<<cost[0][i]/repeat<<"us asl "<<asl[0][i]/repeat<<"step move "<<info[0][i]/repeat<<std::endl;
    }

//    skipList->Display();
//    avltree->Display();

}
