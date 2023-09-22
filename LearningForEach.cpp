// LearningExceptionHandling
// compile With:
// g++ -Wall -pthread -std=c++11 -o LearningForEach LearningForEach.cpp

#include <algorithm>
#include <iostream>
#include <vector>
int other_val = 0;
 
int main()
{
    std::vector<int> v {3, -4, 2, -8, 15, 267};

    auto print = [](const int& n) { std::cout << n << ' ';};
 
    std::cout << "before:\t";
    std::for_each(v.cbegin(), v.cend(), print);
    std::cout << '\n';
 
    // increment elements in-place
    std::for_each(v.begin(), v.end(), [](int &n) { n++; other_val++; std::cout << "Increments: " << other_val << "\n";});
 
    std::cout << "after:\t";
    std::for_each(v.cbegin(), v.cend(), print);
    std::cout << '\n';
 
    struct Sum
    {
        void operator()(int n) { sum += n; }
        int sum {0};
    };
 
    // invoke Sum::operator() for each element
    Sum s = std::for_each(v.cbegin(), v.cend(), Sum());    
    std::cout << "sum:\t" << s.sum << '\n';
}