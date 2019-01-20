#include <cstdio>
#include <random>

int main(int argc, char** argv)
{
    const int k = 10000000;
    const int n = 1000000;

    std::vector<int> range;
    range.reserve(k);
    for (int i = 0; i < k; ++i) {
        range.push_back(i);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, k-1);
    for (int i = 0; i < k / 2; ++i) {
        int p1 = dis(gen);
        int p2 = dis(gen);
        std::iter_swap(&range[p1], &range[p2]);
    }

    for (int i = 0; i < n; ++i) {
        printf("%d\n", range[i]);
    }

    return 0;
}
