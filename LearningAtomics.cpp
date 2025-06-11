// LearningAtomics
// compile With:
// g++ -Wall -pthread -std=c++14 -o LearningAtomics LearningAtomics.cpp


#include <algorithm>
#include <atomic>
#include <cstddef>
#include <iostream>
// #include <syncstream>
#include <thread>
#include <vector>
 
int main()
{
    constexpr int thread_count{5};
    constexpr int sum{5};
 
    std::atomic<int> atom{0};
    std::atomic<int> counter{0};
 
    auto increment_to_sum = [&](const int id)
    {
        for (int next = 0; next < sum;)
        {
            // each thread is writing a value from its own knowledge
            const int current = atom.exchange(next);
            counter++;
            // sync writing to prevent from interrupting by other threads
            std::cout
                << "Thread #" << id << " (id=" << std::this_thread::get_id()
                << ") wrote " << next << " replacing the old value "
                << current << ".\n";
            next = std::max(current, next) + 1;
        }
    };
 
    std::vector<std::thread> v;
    for (std::size_t i = 0; i < thread_count; ++i)
        v.emplace_back(increment_to_sum, i);
 
    for (auto& tr : v)
        tr.join();
 
    std::cout << thread_count << " threads take "
              << counter << " times in total to "
              << "increment 0 to " << sum << ".\n";
}