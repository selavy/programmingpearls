#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

// NOTE(peter): stupid ADL
namespace pcl {

    template <class It>
    void print_range(It begin, It end) {
        for (It it = begin; it != end; ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }

    // Performs a left rotation on a range of elements.
    // Specifically, rotate swaps the elements in the range [first, last) in
    // such a way that the element n_first becomes the first element of the new range and
    // n_first - 1 becomes the last element
    template <class ForwardIt>
    void rotate(ForwardIt first, ForwardIt n_first, ForwardIt last) {
        int swaps = 0;
        
        auto rotations = n_first - first;
        auto end = std::prev(last);
        for (int i = 0; i < rotations; ++i) {
            auto curr = first;
            while (curr != end) {
                auto next = std::next(curr);
                std::swap(*curr, *next);
                std::cout << "swap\n";
                ++swaps;
                curr = next;
            }
        }
        std::cout << "swaps = " << swaps << "\n";
    }

    template <class ForwardIt>
    ForwardIt rotate2(ForwardIt first, ForwardIt n_first, ForwardIt last) {
        int swaps = 0;
        
        if (first == n_first) {
            return last;
        }
        if (n_first == last) {
            return first;
        }

        ForwardIt next = n_first;
        do {
            std::iter_swap(first++, next++);
            std::cout << "swap\n";
            ++swaps;
            if (first == n_first) {
                n_first = next;
            }
        } while (next != last);

        ForwardIt ret = first;
        for (next = n_first; next != last; ) {
            std::iter_swap(first++, next++);
            std::cout << "swap\n";
            ++swaps;            
            if (first == n_first) {
                n_first = next;
            } else if (next == last) {
                next = n_first;
            }
        }

        std::cout << "swaps = " << swaps << "\n";
        return ret;
    }

} // ~namespace pcl

template <class T>
void print(const std::vector<T>& vec) {
    for (auto&& e: vec) {
        std::cout << e << " ";
    }
    std::cout << "\n";
}

int main(int argc, char **argv) {
    // {
    //     // Input:  a b c d e f g h
    //     // rotate_left(3)
    //     // Output: d e f g h a b c
    //     std::vector<char> vec = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    //     print(vec);
    //     std::rotate(vec.begin(), vec.begin()+3, vec.end());
    //     print(vec);
    // }

    // {
    //     std::cout << "\n";
    //     std::vector<char> vec = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    //     print(vec);
    //     std::rotate(vec.begin()+3, vec.begin(), vec.end());
    //     print(vec);
    // }

    {
        std::vector<char> vec = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
        print(vec);
        pcl::rotate(vec.begin(), vec.begin()+3, vec.end());
        print(vec);
    }

    {
        std::vector<char> vec = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
        print(vec);
        pcl::rotate2(vec.begin(), vec.begin()+3, vec.end());
        print(vec);
    }
    
    return 0;
}
