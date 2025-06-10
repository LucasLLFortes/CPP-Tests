#include "TaskScheduler.h"
#include "Logger.h"
#include "misc.h"
#include <sstream>
#include <algorithm>
#include <numeric>

void TaskScheduler::start()
{

    stopExecution = false;

    // Barreira para indicar quando a thread iniciar
    std::promise<void> barrier;

    // Chama a task para ser executada
    scheduler_end = std::async(std::launch::async, &TaskScheduler::scheduler, std::ref(barrier));

    // Espera a confirmacao que a thread foi iniciada para terminar o metodo
    std::future<void> barrier_future = barrier.get_future();
    barrier_future.wait();

    // A scheduler foi inicializada
    was_started = true;
}

void TaskScheduler::stop()
{

    Logger::lprintf(NORMAL, "TaskScheduler::stop() - Signaling scheduler to end\n");
    stopExecution = true;

    cv.notify_all();

    // Espera a execucao de executionIntern terminar
    Logger::lprintf(NORMAL, "TaskScheduler::stop() - Waiting last scheduler end\n");
    scheduler_end.wait();

    // Remocao de todas as tasks registradas
    for(auto &v : execution_time) {
        
        Logger::lprintf(NORMAL, "TaskScheduler::stop() - Removing task %s (ID: %zu)\n", v.task_name.c_str(), v.id);
        removeTask(v.id);
        Logger::lprintf(NORMAL, "TaskScheduler::stop() - Removed task %s (ID: %zu)\n", v.task_name.c_str(), v.id);
        //if(v.execution_end.valid()) v.execution_end.wait();
    }

    // A scheduler foi finalizada
    was_started = false;
}

bool TaskScheduler::started()
{
    return was_started;
}

void TaskScheduler::restart() {
    Logger::lprintf(NORMAL, "TaskScheduler::restart() - Scheduler restart\n");
    
    // Store current configs if needed
    std::vector<TaskConfig> current_configs;
    {
        std::scoped_lock tlock(tmutex);
        for (const auto& task : execution_time) {
            if (!task.ignore) {
                if (task.type == ScheduledType::PERIOD) {
                    current_configs.emplace_back(task.f, task.period, task.task_name);
                } else {
                    current_configs.emplace_back(task.f, task.execute_timer, task.task_name);
                }
            }
        }
    }
    
    // Stop the scheduler
    stop();
    Logger::lprintf(NORMAL, "TaskScheduler::restart() - Scheduler stopped. Clearing configs\n");

    // Clear all existing tasks
    {
        std::scoped_lock tlock(tmutex);
        execution_time.clear();
        never_removed = true;
        taskUpdate = false;
        id = 0;
    }
    
    // Recreate tasks from saved configurations
    for (const auto& config : current_configs) {
        if (config.type == ScheduledType::PERIOD) {
            registerTask(config.task_function, config.period, config.name);
        } else {
            registerTask(config.task_function, config.timer_function, config.name);
        }
    }
    
    // Start the scheduler again
    Logger::lprintf(NORMAL, "TaskScheduler::restart() - Starting scheduler\n");
    start();
    
    Logger::lprintf(NORMAL, "TaskScheduler::restart() - Scheduler successfully restarted\n");
}

void TaskScheduler::endTask(std::vector<TaskScheduler::EXECUTION_DATA>::iterator it, std::unique_lock<std::mutex> &tlock)
{
    // Indica para ignorar qualquer nova execucao da task
    it->ignore = true;

    // Espera a execucao da task finalizar
    tlock.unlock();

    if(it->waiting_block) it->execution_end.wait();

    { std::scoped_lock tlock2(tlock);

        // Elemento encontrado
        swap_and_remove(execution_time, it);
        never_removed = false;

        // Embora o nome aqui fique ambiguo, precisamos registrar 
        // a task para avisar que uma foi retirada
        taskUpdate = true;
    }

    cv.notify_all();
}

bool TaskScheduler::removeTask(size_t taskid)
{
    LogMethod(DEBUG);

    std::unique_lock tlock(tmutex);

    Logger::lprintf(DEBUG, "TaskScheduler::removeTask() - Quantidade de tasks: %ld, getNewDataID sendo removida: %ld\n", execution_time.size(),id);

    // Procura o elemento e o remove
    auto predicate = [=](const EXECUTION_DATA &a) -> bool
    {
        return (a.id == taskid);
    };
  
    auto it = std::find_if(execution_time.begin(), execution_time.end(), predicate);
    if(it != execution_time.end())
    {

        endTask(it, tlock);

        // Elemento encontrado
        return true;
    }

    // Elemento não encontrado
    return false;
}

bool TaskScheduler::updateTaskTime(size_t taskid, size_t period)
{

    std::unique_lock tlock(tmutex);

    auto update = 
    [&](std::vector<EXECUTION_DATA>::iterator it)
    {

        it->period = period;
        it->type = ScheduledType::PERIOD;

        taskUpdate = true;

        tlock.unlock();

        cv.notify_all();
    };

    bool rc = updateTaskTime(update, taskid);

    return rc;
}

void TaskScheduler::scheduler(std::promise<void> &barrier)
{
    // Indica que a thread foi inicializada
    barrier.set_value();

    // Auxiliares
    EXECUTION_DATA *ed;

    while(!stopExecution)
    {
        std::unique_lock tlock(tmutex);
        cv.wait_for(tlock, seconds(1), [&]{ return taskUpdate || stopExecution; } );

        // Finaliza o metodo
        if(stopExecution) break;

        // Indica que nao ha nenhuma nova task atualizada
        taskUpdate = false;

        // Inicialmente coloca um tempo "infinito" para verificar a task novamente
        // next_execution = std::numeric_limits<seconds>::max();

        // Analisa os outros elementos do vetor
        for(size_t next = 0; next < execution_time.size(); next++)
        {
            ed = &execution_time[next];
            if(ed->ignore)
                continue;
            
            // Retorna a duracao da task e a executa se for o caso
            int64_t dur = duration_cast<seconds>(steady_clock::now()-ed->last_execution).count();

            // Se ja estiver rodando ou a duracao for menor que o periodo, nao executa a task 
            if(dur < ed->period || ed->is_running.exchange(true))
            {
                
                if(ed->is_running && dur >= ed->period)
                    Logger::lprintf(DEBUG, "TaskScheduler::scheduler() - Task %s was slow, another is already running (ID: %zu). Period: %ld Last Execution: %ld\n", ed->task_name.c_str(), ed->id, ed->period, dur);
                
                continue;
            }

            // Task deve ser executada
            try {

                Logger::lprintf(VERBOSE, "TaskScheduler::scheduler() - Executing task %s (ID: %zu)\n", ed->task_name.c_str(), ed->id);
                // Inicializa a thread de execucao

                ed->execution_end = std::async(std::launch::async, [ed]() {
                    try {
                        ed->f();
                    } catch(const std::exception& e) {
                        Logger::lprintf(NORMAL, "TaskScheduler: Task %s (ID: %zu) failed: %s\n",
                            ed->task_name.c_str(), ed->id, e.what());
                    }
                    ed->is_running.store(false);
                });

                Logger::lprintf(VERBOSE, "TaskScheduler::scheduler() - Launched - task %s (ID: %zu)\n", ed->task_name.c_str(), ed->id);

                // Reinicia o contador
                switch(ed->type)
                {
                    case ScheduledType::PERIOD:
                    {
                        // Reinicia o contador
                        dur = ed->period;
                        break;
                    }
                    case ScheduledType::METHOD:
                    {
                        // Reinicia o contador
                        time_t rawtime;
                        time(&rawtime);

                        dur = ed->execute_timer(rawtime);
                        ed->period = dur;
                        break;
                    }
                }

                ed->last_execution = steady_clock::now();
            }
            catch(...) {
                ed->is_running.store(false);
                throw;
            }
            // Verifica se e o proximo a executar, e se for, indica em quanto tempo
            // if(next_execution > seconds(dur)) next_execution = seconds(dur);
        }

        // pausa de execucao de thread por 4 segundos. Deve ser menor que metade do tempo da rotina mais frequente da taskScheduler:
        // No caso, atualmente a task "TaskScheduler::registerTask(WebSSS::CheckSSSAlive, CHECK_SYSTEM_INTERVAL)"
        // std::this_thread::sleep_for(seconds(4)); 
    }

}

std::string TaskScheduler::getStatus() {
    std::stringstream status;
    
    // Check if scheduler is running
    status << "Scheduler Status: " << (was_started ? "RUNNING" : "STOPPED") << "\n";
    status << "Total registered tasks: " << execution_time.size() << "\n";
    status << "\nActive Tasks:\n";
    
    for (const auto& task : execution_time) {
        if (!task.ignore) {
            auto now = steady_clock::now();
            auto duration = duration_cast<seconds>(now - task.last_execution).count();
            
            status << "  Task ID: "           << task.id << "\t"
                   << "  Period: "          << task.period << " seconds\t"
                   << "  Type: "            << (task.type == ScheduledType::PERIOD ? "PERIODIC" : "METHOD") << "\t"
                   << "  Last execution: "  << duration << " seconds ago\t"
                   << "-------------------\n";
        }
    }
    
    return status.str();
}

void TaskScheduler::schedulerMonitorLoop(int check_period_seconds) 
{
    bool is_monitor_running = true;
    long scheduler_fail_counter = 0;
    while (is_monitor_running) 
    {
        if(!checkSchedulerStatus(check_period_seconds))
        {
            scheduler_fail_counter++;
            if(scheduler_fail_counter > 3)
            {
                Logger::lprintf(NORMAL, "TaskScheduler::schedulerMonitorLoop() - Scheduler is not responding. Restarting the scheduler.\n");
                restart();
                scheduler_fail_counter = 0;
            }
        }
        else
        {
            scheduler_fail_counter = 0;
        }

        if(!started())
        {
            is_monitor_running = false;
        } 

        std::this_thread::sleep_for(std::chrono::seconds(check_period_seconds));
    }
}

bool TaskScheduler::checkSchedulerStatus(const int check_period_seconds) 
{
    bool scheduler_running = started();
    bool scheduler_healthy = true;
    
    Logger::lprintf(NORMAL, "TaskScheduler Monitor Status:\n");

    if (scheduler_running) 
    {        
        Logger::lprintf(NORMAL, "Total registered tasks: %zu\n", 
            TaskScheduler::execution_time.size());
    }
    else 
    {
        Logger::lprintf(NORMAL, "AVISO. TaskScheduler Travado!!\n");
    }

    for (const auto& task : TaskScheduler::execution_time) 
    {
        if (task.ignore) 
        {
            continue;
        }

        auto now = steady_clock::now();
        auto duration = duration_cast<seconds>(now - task.last_execution).count();
        
        Logger::lprintf(NORMAL,
            "Task ID: %zu\t"
            "  Name: %s\t"
            "  Period: %zu seconds\t"
            "  Type: %s\t"
            "  Last execution: %ld seconds ago\t"
            "-------------------\n",
            task.id,
            task.task_name.c_str(),
            task.period,
            (task.type == TaskScheduler::ScheduledType::PERIOD ? "PERIODIC" : "METHOD"),
            duration
        );

        // Consistency check        
        if (duration > task.period + 8 * check_period_seconds) 
        {
            Logger::lprintf(NORMAL, "TaskScheduler::checkSchedulerStatus() - Task %s (ID: %zu) has not executed for more than %ld seconds.\n", task.task_name.c_str(), task.id, 8 * check_period_seconds);
            scheduler_healthy = false;
        }
    }

    Logger::lprintf(NORMAL, "Scheduler Status: %s - %s\n", 
    scheduler_running ? "RUNNING" : "STOPPED",
    scheduler_healthy ? "HEALTHY" : "UNHEALTHY");
        
    return scheduler_healthy;
}

int64_t TaskScheduler::checkForTaskExecution(EXECUTION_DATA *ed)
{
    int64_t dur = duration_cast<seconds>(steady_clock::now()-ed->last_execution).count();

    // Se ja estiver rodando ou a duracao for menor que o periodo, nao executa a task 
    if((uint64_t)dur < ed->period || ed->is_running.exchange(true))
    {
        
        if(ed->is_running)
            Logger::lprintf(VERBOSE, "TaskScheduler::scheduler() - Task %s is already running (ID: %zu)\n", ed->task_name.c_str(), ed->id);
        
        return dur;
    }

    // Task deve ser executada
    try {

        Logger::lprintf(VERBOSE, "TaskScheduler::scheduler() - Executing task %s (ID: %zu)\n", ed->task_name.c_str(), ed->id);
        // Inicializa a thread de execucao

        ed->execution_end = std::async(std::launch::async, [ed]() {
            try {
                ed->f();
            } catch(const std::exception& e) {
                Logger::lprintf(NORMAL, "TaskScheduler: Task %s (ID: %zu) failed: %s\n",
                    ed->task_name.c_str(), ed->id, e.what());
            }
            ed->is_running.store(false);
        });

        Logger::lprintf(VERBOSE, "TaskScheduler::scheduler() - Launched - task %s (ID: %zu)\n", ed->task_name.c_str(), ed->id);

        // Reinicia o contador
        switch(ed->type)
        {
            case ScheduledType::PERIOD:
            {
                // Reinicia o contador
                dur = ed->period;
                break;
            }
            case ScheduledType::METHOD:
            {
                // Reinicia o contador
                time_t rawtime;
                time(&rawtime);

                dur = ed->execute_timer(rawtime);
                ed->period = dur;
                break;
            }
        }

        ed->last_execution = steady_clock::now();
    }
    catch(...) {
        ed->is_running.store(false);
        throw;
    }
    

    return dur;
};