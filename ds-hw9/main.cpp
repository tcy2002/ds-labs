#include "test.h"
#include <windows.h>
#include "graph.h"

int main() {
    test obj("../input.txt", "../output.txt");
    obj.report();
    graph grf;

    return 0;
}
