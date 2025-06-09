// compile With:
// g++ -Wall -pthread -std=c++17 -o LearningTaskScheduler LearningTaskScheduler.cpp Logger.cc TaskScheduler.cpp


#include "TaskScheduler.h"
#include "Logger.h"
#include <iostream>

void timedRoutineTest();
void timedRoutine2Test();
void slowTimedRoutineTest();
void slowTimedRoutine2Test();

void startCheckSystem() {
    
    // TaskScheduler::registerTask(WebSSS::CheckSSSAlive, CHECK_SYSTEM_INTERVAL);
    TaskScheduler::registerTask(timedRoutineTest        , 10, "timedRoutineTest");
    TaskScheduler::registerTask(timedRoutine2Test       , 10, "timedRoutine2Test");
    TaskScheduler::registerTask(slowTimedRoutineTest    , 10, "slowTimedRoutineTest");
    TaskScheduler::registerTask(slowTimedRoutine2Test   , 10, "slowTimedRoutine2Test");

    TaskScheduler::start();
    int SCHEDULER_CHECK_PERIOD = 5;
    static std::thread monitor_thread;
    monitor_thread = std::thread(&TaskScheduler::schedulerMonitorLoop, SCHEDULER_CHECK_PERIOD);
    monitor_thread.detach();
}

void timedRoutineTest()
{

    Logger::lprintf(DEBUG, "-------------------- Casos de teste 1 - 3 segundos - THREADED EXECUTION ----------------------------------\n");
    
    // Aguarda um tempo para verificar se a rotina foi executada
    std::this_thread::sleep_for(std::chrono::seconds(3));

    Logger::lprintf(DEBUG, "-------------------- Casos de teste 1 ENDED - THREADED EXECUTION ----------------------------------\n");

}

void timedRoutine2Test()
{
    Logger::lprintf(DEBUG, "-------------------- Casos de teste 2 - 8 segundos- THREADED EXECUTION ----------------------------------\n");
    
    // Aguarda um tempo para verificar se a rotina foi executada
    std::this_thread::sleep_for(std::chrono::seconds(8));

    Logger::lprintf(DEBUG, "-------------------- Casos de teste 2 ENDED - THREADED EXECUTION ----------------------------------\n");
}


void slowTimedRoutineTest()
{
    Logger::lprintf(DEBUG, "-------------------- Casos de teste 3 - 22 segundos- THREADED EXECUTION ----------------------------------\n");
    
    // Aguarda um tempo para verificar se a rotina foi executada
    std::this_thread::sleep_for(std::chrono::seconds(22));

    Logger::lprintf(DEBUG, "-------------------- Casos de teste 3 ENDED - THREADED EXECUTION ----------------------------------\n");
}


void slowTimedRoutine2Test()
{
    
    static int time = 1;
    time += 2;
    Logger::lprintf(DEBUG, "-------------------- Casos de teste 4 - %ld segundos - THREADED EXECUTION ----------------------------------\n", time);

    // Aguarda um tempo para verificar se a rotina foi executada
    std::this_thread::sleep_for(std::chrono::seconds(time));

    Logger::lprintf(DEBUG, "-------------------- Casos de teste 4 ENDED - THREADED EXECUTION ----------------------------------\n");
}

int main() 
{
    Logger::setSystemLevel(DEBUG);
    startCheckSystem();

    Logger::lprintf(DEBUG, "TaskScheduler started!\n\n");

    return 0;
}

