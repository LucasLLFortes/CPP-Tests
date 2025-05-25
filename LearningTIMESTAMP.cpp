// LearningTIMESTAMP

// compile With:
// g++ -Wall -pthread -std=c++17 -o LearningTIMESTAMP LearningTIMESTAMP.cpp Logger.cc ssitdefs.cc

// Header declarations

// #define TEST_CASE_0 // TESTES DE METODOS    
// #define TEST_CASE_1 // FALHAS_DE_COMUNICACAO    
// #define TEST_CASE_2 // THREADED_EXECUTION       
// #define TEST_CASE_3 // PERFORMANCE_ANALYSIS     

// #include "SystemCommand.hpp"
#include "Logger.h"
#include "ssitdefs.h"
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

constexpr int NOTIFICATION_TIME_WINDOW = 30; // janela de tempo em minutos no qual as notificacoes serao consideradas (exemplo, apenas notificacoes dos ultimos 30 minutos)

using namespace SSIT;

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

void threadTests()
{   
    
    TIMESTAMP current_timestamp, custom_timestamp(24,01,2025,10,1,1), diff;

    current_timestamp.getLocalTime();


    // Test 2
    custom_timestamp = current_timestamp - 25 * 60;
    Logger::lprintf(NORMAL, "Time: current %s custom %s\n", current_timestamp.toString().c_str(), custom_timestamp.toString().c_str());
    Logger::lprintf(NORMAL, "Diferenca: %.2f\n", current_timestamp - custom_timestamp);
    if(current_timestamp - custom_timestamp < NOTIFICATION_TIME_WINDOW * 60)
        Logger::lprintf(NORMAL, "Notify. Less than %ld mins\n", NOTIFICATION_TIME_WINDOW);        
    else
        Logger::lprintf(NORMAL, "Alarm will not notify. More than %ld mins\n", NOTIFICATION_TIME_WINDOW);        



    // Test 1
    custom_timestamp = current_timestamp - NOTIFICATION_TIME_WINDOW * 60;
    Logger::lprintf(NORMAL, "Time: current %s custom %s\n", current_timestamp.toString().c_str(), custom_timestamp.toString().c_str());
    Logger::lprintf(NORMAL, "Diferenca: %.2f\n", current_timestamp - custom_timestamp);
    if(current_timestamp - custom_timestamp < NOTIFICATION_TIME_WINDOW * 60)
        Logger::lprintf(NORMAL, "Notify. Less than %ld mins\n", NOTIFICATION_TIME_WINDOW);        
    else
        Logger::lprintf(NORMAL, "Alarm will not notify. More than %ld mins\n", NOTIFICATION_TIME_WINDOW);        

    Logger::lprintf(NORMAL, "recalculate tests!\n");

    int RECALCULATE_TIME_DELAY_IN_HOURS = 9;

    // Test A.1
    TIMESTAMP current_timestamp_recalc  (24,01,2025,20,00,50); 
    TIMESTAMP last_timestamp_recalc     (24,01,2025,19,59,58);

    if(!(current_timestamp_recalc - RECALCULATE_TIME_DELAY_IN_HOURS * 3600).isSameDay(last_timestamp_recalc - RECALCULATE_TIME_DELAY_IN_HOURS * 3600))
    {
        Logger::lprintf(NORMAL, "Running recalculate 1!\n");
    }

    // Test A.2
    TIMESTAMP current_timestamp_recalc2  (24,01,2025,23,58,01); 
    TIMESTAMP last_timestamp_recalc2     (24,01,2025,20,01,01);

    if(!(current_timestamp_recalc2 - RECALCULATE_TIME_DELAY_IN_HOURS * 3600).isSameDay(last_timestamp_recalc2 - RECALCULATE_TIME_DELAY_IN_HOURS * 3600))
    {
        Logger::lprintf(NORMAL, "Running recalculate 2!\n");
    }

    // Test A.3
    TIMESTAMP current_timestamp_recalc3  (25,01,2025,02,01,01); 
    TIMESTAMP last_timestamp_recalc3     (25,01,2025,00,59,01);

    if(!(current_timestamp_recalc3 - RECALCULATE_TIME_DELAY_IN_HOURS * 3600).isSameDay(last_timestamp_recalc3 - RECALCULATE_TIME_DELAY_IN_HOURS * 3600))
    {
        Logger::lprintf(NORMAL, "Running recalculate 3!\n");
    }
    // current_timestamp = current_timestamp - 60*20;
    // Logger::lprintf(NORMAL, "Time: current %s custom %s\n", current_timestamp.toString().c_str(), custom_timestamp.toString().c_str());

    // Test A.4
    TIMESTAMP current_timestamp_recalc4  (24,01,2025, 9,00,50); 
    TIMESTAMP last_timestamp_recalc4     (24,01,2025, 5,00,58);

    if(!(current_timestamp_recalc4 - RECALCULATE_TIME_DELAY_IN_HOURS * 3600).isSameDay(last_timestamp_recalc4 - RECALCULATE_TIME_DELAY_IN_HOURS * 3600))
    {
        Logger::lprintf(NORMAL, "Running recalculate 4!\n");
    }


    time_t curTime;
    time(&curTime);

    // Se variacao for 0, ou ter passado X minutos desde o ultimo armazenamento, insere de qualquer forma o dado    
    
    Logger::lprintf(NORMAL, "WebSSS::SetTeleObject() - curr time: %d \n", curTime);


    usleep(1000);

}

