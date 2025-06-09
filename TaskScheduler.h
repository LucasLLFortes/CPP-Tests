#pragma once

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <type_traits>
#include <vector>
#include "Logger.h"

using namespace std::chrono;

class TaskScheduler
{

private:

    enum class ScheduledType : size_t
    {
        PERIOD,
        METHOD
    };

    class EXECUTION_DATA {

    public:

        // ID do elemento
        size_t id;

        // Periodo para executar
        size_t period;

        // No caso de horarios estabelecidos para executar, estabelece quando
        size_t month, day, hour, min, sec;

        // Tempo de ultima execucao
        time_point<steady_clock> last_execution = steady_clock::now();

        // Metodo terminou execucao
        std::future<void> execution_end;

        // Metodo terminou execucao
        std::future<void> execution_wait;

        // Thread de espera ainda nao iniciou
        bool waiting_block = false;

        // No caso
        struct tm tm;

        // Metodo a ser executado
        std::function<void()> f;

        // Metodo indicando quando executar
        std::function<int64_t(time_t)> execute_timer;

        // Nome da task
        std::string task_name;

        // Indica se foi configurado por horario ou periodo
        ScheduledType type;

        // Indica se a estrutura deve ser ignorada
        bool ignore = false;

        // Construtores
        template<typename Function, typename Period, 
            typename std::enable_if<!std::is_invocable_v<Period, time_t>>::type* = nullptr>
        EXECUTION_DATA(Function&& f, size_t id, Period period, std::string name = "unnamed_task") :
            id(id), period(period), 
            f(std::forward<Function>(f)), 
            type(ScheduledType::PERIOD), 
            task_name(std::move(name)) 
            {};

        template<typename Function, typename PeriodFunction,
            typename std::enable_if<std::is_invocable_v<PeriodFunction, time_t>>::type* = nullptr>
        EXECUTION_DATA(Function&& f, size_t id, PeriodFunction&& execute_timer, std::string name = "unnamed_task") :
        id(id), 
        f(std::forward<Function>(f)), 
        execute_timer(std::forward<PeriodFunction>(execute_timer)), 
        type(ScheduledType::METHOD), 
        task_name(std::move(name))
        {
            time_t rawtime;
            time(&rawtime);

            period = this->execute_timer(rawtime);
        }
    };

    ///////////////////////////////////////////////////////////////////
    // Auxiliares
    //////////////////////////////////////////////////////////////////

private:
    static void endTask(std::vector<TaskScheduler::EXECUTION_DATA>::iterator it, std::unique_lock<std::mutex> &tlock);
    static int64_t next_period_calculator(EXECUTION_DATA *ed);

    ///////////////////////////////////////////////////////////////////
    // registerTask
    //////////////////////////////////////////////////////////////////
public:

    template<typename Function, typename PeriodFunction>
    typename std::enable_if_t<std::is_invocable_v<PeriodFunction, time_t>, size_t>
    static registerTask(Function&& f, PeriodFunction&& period, std::string task_name = "unnamed_task")
    {
        size_t retid;
        Logger::lprintf(DEBUG, "TaskScheduler::registerTask() - Iniciando register task\n");

        {   std::scoped_lock tlock(tmutex);

            retid = ++id;

            Logger::lprintf(DEBUG, "TaskScheduler::registerTask() - retid: %ld\n",retid);

            EXECUTION_DATA data(std::forward<Function>(f), 
                                retid,
                                std::forward<PeriodFunction>(period),
                                std::move(task_name));
            execution_time.push_back( std::move(data) );

            taskUpdate = true;
        }

        cv.notify_all();

        Logger::lprintf(DEBUG, "TaskScheduler::registerTask() - Terminando register task\n");
        return retid;
    }

    template<typename Function, typename Period>
    typename std::enable_if_t<!std::is_invocable_v<Period, time_t>, size_t>
    static registerTask(Function&& f, Period period, std::string task_name = "unnamed_task")
    {
        size_t retid;

        Logger::lprintf(DEBUG, "TaskScheduler::registerTaskB() - Iniciando register task\n");

        {   std::scoped_lock tlock(tmutex);

            retid = ++id;

            Logger::lprintf(DEBUG, "TaskScheduler::registerTaskB() - retid: %ld\n",retid);

            EXECUTION_DATA data(std::forward<Function>(f), 
                                retid, 
                                period, 
                                std::move(task_name));
            execution_time.push_back( std::move(data) );

            taskUpdate = true;
        }

        cv.notify_all();

        Logger::lprintf(DEBUG, "TaskScheduler::registerTaskB() - Terminando register task\n");

        return retid;
    }

    ///////////////////////////////////////////////////////////////////
    // updateTaskTime
    //////////////////////////////////////////////////////////////////
public:

    // Pega o status do modulo
    static std::string getStatus();

    // Atualiza o tempo de uma task
    static bool updateTaskTime(size_t taskid, size_t period);

    template<typename T>
    static bool updateTaskTime(size_t taskid, T execute_timer)
    {
        time_t rawtime;
        time(&rawtime);

        std::unique_lock tlock(tmutex);

        auto update = 
        [&](std::vector<EXECUTION_DATA>::iterator it)
        {

            it->period = std::invoke(execute_timer, rawtime);;
            it->type = ScheduledType::METHOD;

            taskUpdate = true;

            tlock.unlock();

            cv.notify_all();
        };

        bool rc = updateTaskTime(update, taskid);

        return rc;
    }

private:

    template <typename T>
    static bool updateTaskTime(T &update, size_t taskid)
    {
        if(never_removed)
        {
            // Elemento não encontrado
            if(execution_time.size() <= id) return false;

            // Elemento encontrado
            update(execution_time.begin()+id);
            return true;
        }

        // Procura o elemento e o remove
        auto predicate = [=](const EXECUTION_DATA &a) -> bool
        {
            return (a.id == taskid);
        };
    
        auto it = std::find_if(execution_time.begin(), execution_time.end(), predicate);
        if(it != execution_time.end())
        {
            // Elemento encontrado
            update(it);
            return true;
        }

        // Elemento não encontrado
        return false;
    }

    ///////////////////////////////////////////////////////////////////
    // removeTask
    //////////////////////////////////////////////////////////////////
public:

    static bool removeTask(size_t taskid);

    ///////////////////////////////////////////////////////////////////
    // Task Control
    //////////////////////////////////////////////////////////////////
public:

    // Start/Stop tasks
    static void start();
    static void stop();
    
    // Checks if scheduler started
    static bool started();

    // Monitors the scheduler status
    static void schedulerMonitorLoop(int check_period_seconds);
    static void checkSchedulerStatus();
private:

    static void scheduler(std::promise<void> &barrier);

    ///////////////////////////////////////////////////////////////////
    // Variables
    //////////////////////////////////////////////////////////////////
private:

    // Tempo de cada execução registrada rodar
    inline static std::vector<EXECUTION_DATA> execution_time;

    // Mutex da classe
    inline static std::mutex tmutex;
    inline static std::condition_variable cv;
    inline static std::future<void> scheduler_end;

    // Nunca uma task foi removida
    inline static bool never_removed = true;

    // Uma nova task foi registrada
    inline static bool taskUpdate = false;

    // Proximo id a ser registrado
    inline static size_t id = 0;

    // Iniciou execucao
    inline static bool was_started = false;

    // Parar execucao da thread
    inline static bool stopExecution;

};