#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

constexpr int Limit = 1000000;
constexpr int MaxValue = 10000000;
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <FILE>\n", argv[0]);
        exit(1);
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp)
    {
        fprintf(stderr, "Unable to open file: '%s'\n", argv[1]);
        exit(1);
    }

    uint32_t bitmap[Limit/32];
    char input[256];
    int val;
    for (int base = 0; base < MaxValue; base += Limit)
    {
        rewind(fp);
        memset(&bitmap[0], 0, sizeof(bitmap));
        const int lo = base;
        const int hi = base + Limit;

        while (fgets(&input[0], sizeof(input), fp) != NULL)
        {
            val = atoi(&input[0]);
            if (!(val >= lo && val < hi))
                continue;
            val -= base;
            int idx = val / 32;
            int off = val % 32;
            bitmap[idx] |= static_cast<uint32_t>(1) << off;
        }

        for (int i = 0; i < Limit; ++i)
        {
            int idx = i / 32;
            int off = i % 32;
            if ((bitmap[idx] & (static_cast<uint32_t>(1) << off)) != 0)
            {
                printf("%d\n", i + base);
            }
        }
    }

    fclose(fp);
    return 0;
}
