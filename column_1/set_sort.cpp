#include <cstdio>
#include <cstdlib>
#include <set>

int main(int argc, char** argv) {
    std::set<int> vs;
    int val;
    char input[256];
    while (fgets(&input[0], sizeof(input), stdin) != NULL) {
        val = atoi(&input[0]);
        vs.insert(val);
    }
    for (int v: vs) {
        printf("%d\n", v);
    }
    return 0;
}
