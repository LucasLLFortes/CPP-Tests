// LearningChrono

// compile With:
// g++ -Wall -pthread -std=c++17 -o LearningChrono LearningChrono.cpp Logger.cc

// Header declarations

// #define TEST_CASE_0 // TESTES DE METODOS    
// #define TEST_CASE_1 // FALHAS_DE_COMUNICACAO    
#define TEST_CASE_2 // THREADED_EXECUTION       
// #define TEST_CASE_3 // PERFORMANCE_ANALYSIS     

// #include "SystemCommand.hpp"
#include "Logger.h"
#include <bits/stdc++.h>

#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <chrono>

//------------------------------------------------------------------------------
//Functions
void threadTests();

//------------------------------------------------------------------------------
int main(int argc, char** argv){
    std::cout << "\n -------------------- main() ---------------------------------- \n\n";

    // Logger::setSystemLevel(SILENT);
    // Logger::setSystemLevel(QUIET);
    // Logger::setSystemLevel(NORMAL);
    // Logger::setSystemLevel(VERBOSE);
    Logger::setSystemLevel(LOCAL_DEBUG);
    
    threadTests();

    return EXIT_SUCCESS;
}

void threadTests(){
    std::cout << "\n -------------------- Casos de teste 2 - THREADED EXECUTION ---------------------------------- \n\n";
    
    std::chrono::time_point time_system = std::chrono::system_clock::now();
    std::chrono::time_point time_steady = std::chrono::steady_clock::now();
 
    int delay = 1333, loops = 10;

    for(int i = 0; i < loops; i++){
        std::chrono::time_point current_system = std::chrono::system_clock::now();
        std::chrono::time_point current_steady = std::chrono::steady_clock::now();

        Logger::lprintf(NORMAL, "Test time time_point - System: %ld\n",std::chrono::time_point_cast<std::chrono::milliseconds>(current_system));
        Logger::lprintf(NORMAL, "Test time time_point - Steady: %ld\n",std::chrono::time_point_cast<std::chrono::milliseconds>(current_steady));
        Logger::lprintf(NORMAL, "Test time duration_cast - System: %f\n", std::chrono::duration_cast<std::chrono::milliseconds>(current_system - time_system).count()*1e-3);
        Logger::lprintf(NORMAL, "Test time duration_cast - Steady: %f\n",std::chrono::duration_cast<std::chrono::milliseconds>(current_steady - time_steady)*1e-3);
        // chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        usleep(delay*1000);
    }


}

