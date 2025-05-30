#include "TaskScheduler.h"
#include "Logger.h"
#include "misc.h"

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

    stopExecution = true;

    cv.notify_all();

    // Espera a execucao de executionIntern terminar
    scheduler_end.wait();

    // Espera a finalizacao de todas as tasks registradas
    for(auto &v : execution_time) {
        if(v.execution_end.valid()) v.execution_end.wait();
    }

    // A scheduler foi finalizada
    was_started = false;
}

bool TaskScheduler::started()
{
    return was_started;
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

    Logger::lprintf(DEBUG, "TaskScheduler::removeTask() - Task Mais Recente: %ld, getNewDataID sendo removida: %ld\n", execution_time.size(),id);

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

    // Em quantos segundos será executado a thread de execucao
    seconds next_execution = seconds(0);

    // Auxiliares
    EXECUTION_DATA *ed;

    while(!stopExecution)
    {
        std::unique_lock tlock(tmutex);
        cv.wait_for(tlock, next_execution, [&]{ return taskUpdate || stopExecution; } );

        // Finaliza o metodo
        if(stopExecution) break;

        // Indica que nao ha nenhuma nova task atualizada
        taskUpdate = false;

        // Inicialmente coloca um tempo "infinito" para verificar a task novamente
        next_execution = std::numeric_limits<seconds>::max();

        // Analisa os outros elementos do vetor
        for(size_t next = 0; next < execution_time.size(); next++)
        {
            ed = &execution_time[next];
            if(!ed->ignore)
            {
                // Retorna a duracao da task e a executa se for o caso
                int64_t dur = next_period_calculator(ed);

                // Verifica se e o proximo a executar, e se for, indica em quanto tempo
                if(next_execution > seconds(dur)) next_execution = seconds(dur);
            }
        }

        // pausa de execucao de thread por 4 segundos. Deve ser menor que metade do tempo da rotina mais frequente da taskScheduler:
        // No caso, atualmente a task "TaskScheduler::registerTask(WebSSS::CheckSSSAlive, CHECK_SYSTEM_INTERVAL)"
        std::this_thread::sleep_for(seconds(4)); 
    }
}

int64_t TaskScheduler::next_period_calculator(EXECUTION_DATA *ed)
{
    int64_t dur = duration_cast<seconds>(steady_clock::now()-ed->last_execution).count();

    // Se o tempo for maior do que o periodo para ser executado, executa
    if((uint64_t)dur >= ed->period)
    {
        // Inicializa a thread de execucao
        ed->execution_end = std::async(std::launch::async, ed->f);

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

    return dur;
};