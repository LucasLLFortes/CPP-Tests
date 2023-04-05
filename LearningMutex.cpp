// compile With:
// g++ -pthread -std=c++17 -o LearningMutex LearningMutex.cpp

#include <iostream>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <chrono>

using namespace std::chrono;
// or, the line below, both namespaces are defined in the <chrono> lib
// using namespace std::chrono_literals;
 
std::condition_variable cv;
std::mutex cv_m;
int i;
 
void waits(int idx)
{
    std::unique_lock<std::mutex> lk(cv_m);
    if(cv.wait_for(lk, idx*std::chrono::milliseconds(100), []{return i == 1;})) 
        std::cerr << "Thread " << idx << " finished waiting. i == " << i << '\n';
    else
        std::cerr << "Thread " << idx << " timed out. i == " << i << '\n';
}
 
void signals()
{
    std::this_thread::sleep_for(120ms);
    std::cerr << "Notifying...\n";
    i = 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cerr << "Waiting...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    i = 0;
    {
        std::lock_guard<std::mutex> lk(cv_m);
    }
    std::cerr << "Notifying again...\n";
    cv.notify_all();
}
 
int main()
{
    int i =1, option = 0;
    std::cerr << "Hello! Simulation example:\n";
    std::cin >> option;

    std::thread t1(waits, 1), t2(waits, 2), t3(waits, 4), t4(waits, 10), t5(signals);
    switch(option){
        case 1:
            t1.join();
            t2.join();
            t3.join();
            t4.join();
            t5.join();
            break;
        
        case 2:
            break;
        
        case 3:
            break;
        
        case 4:
            break;
        
        case 5:
            break;
        
        default:
            break;
    }
}