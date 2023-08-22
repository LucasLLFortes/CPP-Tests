/**
 * Logger.h
 *
 * Copyright(c) 2002 ATI - Janaina Senna
 * All rights reserved.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <atomic>
#include <chrono>
#include <cstring>
#include <ctime>
#include <mutex>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/syscall.h>
#include <thread>
#include <unistd.h>


using namespace std::chrono;


#ifdef USE_UT_CXXTEST
#define CXXTEST    -1
#endif
#define SILENT      0
#define QUIET       1
#define NORMAL      2
#define VERBOSE     3
#define LOCAL_DEBUG 4

#ifdef DEBUG
#undef DEBUG       
#endif
#define DEBUG       5

#define DONT_LOG    100

// Warning de tempo de execucao maior ou igual que o especificado
#define warnLoggerExecTime 5000

class Logger {

    public:
        static void setSystemLevel(int);
        static void printBuffer(int level, const char *buffer, int bufferSize);
        static void lprintf(int, const char *, ...);
        static void lfprintf(int, FILE *, const char *, ...);
        static void print_trace( int level );

        static int systemLevel;

    inline static std::mutex print_mutex;
    inline static int buffer_size = 1024;
    inline static char* buffer = (char*)malloc(1024*sizeof(char));
    inline static char date[22];
};

/*
    Classes auxiliares para imprimir begin e end no inicio e fim das chamadas dos metodos

    Para utilizar, basta colocar LogMethod(LOCAL_DEBUG); ou LogMethodD(duration_warning);
    ou LogMethodSimple(LOCAL_DEBUG); ou LogMethodCT(LOCAL_DEBUG); ou LogMethodDC()
    no INICIO da definição do metodo, para que ele seja construido primeiro e destruido por ultimo

    O destrutor garante que "end" sempre será impresso, mesmo quando lançado uma exceção no metodo
*/

// Metodo interno para retornar uma string na forma Classe::Metodo()
// TODO - Tentar criar uma versao desse metodo em tempo de compilacao (constexpr)
// A dificuldade esta que é utilizado C++11, o que limita muito a capacidade de trabalhar com constexpr e strings
// Na forma atual, o programa processa sempre a mesma coisa na inicializacao, o que e desnecessario se o proprio
// compilador indicasse a formatação da string
char* Logger_methodName(const char* prettyfunction, const char* methodname) noexcept;

/* 
    A classe abaixo calcula o tempo de execucao e apresenta um contador da chamada do metodo
    Quando extrapolado um limite de tempo dado por duration_warning, mostra um aviso
*/
class LoggerMethod {

public:
    LoggerMethod(const char* method, const size_t log_print_level, const int64_t duration_warning) :
        method(method),
        log_print_level(log_print_level),
        init_time(steady_clock::now()),
        duration_warning(duration_warning)
    {
        Logger::lprintf(log_print_level, "%s - begin\n", method);
    }
    ~LoggerMethod()
    {
        int64_t duration = duration_cast<milliseconds>(steady_clock::now()-init_time).count();
        if(duration >= duration_warning) {
            Logger::lprintf(NORMAL, "%s - execution duration too high! (%ld ms)\n", method, duration);
        }
        
        if(Logger::systemLevel == DEBUG) {

            Logger::lprintf(DEBUG, "%s - end (%ld ms)\n", method, duration);
        }
        else {
            Logger::lprintf(log_print_level, "%s - end\n", method);
        }
    }

private:
    const char* method; // Nome do metodo
    const size_t log_print_level;   // Nível de print da log
    const std::chrono::time_point<std::chrono::steady_clock> init_time; // Inicio do tempo de execucao
    const int64_t duration_warning;   // Tempo do qual vai ser alertado que a duracao foi muito grande    

};

/* 
    A classe abaixo calcula o tempo de execucao, apresenta um contador da chamada do metodo
    e identifica a chamada do metodo por um contador
    Quando extrapolado um limite de tempo dado por duration_warning, mostra um aviso
*/
class LoggerMethodAndCount {

public:
    LoggerMethodAndCount(const char* method, const size_t log_print_level, 
    const int64_t duration_warning, const size_t call_count) :
        method(method),
        log_print_level(log_print_level),
        init_time(steady_clock::now()),
        call_count(call_count),
        duration_warning(duration_warning)
    {
        Logger::lprintf(log_print_level, "%s - begin (Method ID: %llu)\n", method, call_count);
    }
    LoggerMethodAndCount(const char* method, const size_t log_print_level, 
    const int64_t duration_warning, const size_t call_count,
    const std::chrono::time_point<std::chrono::steady_clock> init_time) :
        method(method),
        log_print_level(log_print_level),
        init_time(std::move(init_time)),
        call_count(call_count),
        duration_warning(duration_warning)
    {
        Logger::lprintf(log_print_level, "%s - begin (Method ID: %llu)\n", method, call_count);
    }
    ~LoggerMethodAndCount()
    {
        int64_t duration = duration_cast<milliseconds>(steady_clock::now()-init_time).count();
        if(duration >= duration_warning) {
            Logger::lprintf(NORMAL, "%s - execution duration too high! (Method ID: %llu) (%ld ms)\n", method, call_count, duration);
        }
        
        if(Logger::systemLevel == DEBUG) {

            Logger::lprintf(DEBUG, "%s - end (Method ID: %llu) (%ld ms)\n", method, call_count, duration);
        }
        else {
            Logger::lprintf(log_print_level, "%s - end (Method ID: %llu)\n", method, call_count);
        }
    }

private:
    const char* method; // Nome do metodo
    const size_t log_print_level;  // Nível de print da log
    const std::chrono::time_point<std::chrono::steady_clock> init_time; // Inicio do tempo de execucao
    const size_t call_count;        // Numero de execucoes do metodo
    const int64_t duration_warning;   // Tempo do qual vai ser alertado que a duracao foi muito grande

};

/* 
    A classe abaixo não calcula o tempo de execucao nem apresenta um contador
    Util para metodos simples que tem tempo de execucao irrelevante
*/

class LoggerMethodSimple {

public:
    LoggerMethodSimple(const char* method, const size_t log_print_level) :
        method(method),
        log_print_level(log_print_level)
    {
        Logger::lprintf(log_print_level, "%s - begin\n", method);
    }
    inline ~LoggerMethodSimple()
    {
        Logger::lprintf(log_print_level, "%s - end\n", method);
    }

private:
    const char* method; // Nome do metodo
    const size_t log_print_level;  // Nível de print da log
};

class LoggerMethodThread {

public:
    LoggerMethodThread(const char* method, const size_t log_print_level, const long thread_identifier) :
        method(method),
        log_print_level(log_print_level),
        thread_identifier(thread_identifier)
    {
        Logger::lprintf(log_print_level, "%s - thread(%ld) begin\n", method, thread_identifier);
    }
    inline ~LoggerMethodThread()
    {
        Logger::lprintf(log_print_level, "%s - thread(%ld) end\n", method, thread_identifier);
    }

private:
    const char* method; // Nome do metodo
    const size_t log_print_level;  // Nível de print da log
    const long thread_identifier; // Identificador da thread
};

/* MACROS para serem utilizadas no inicio das chamadas dos metodos
    As variaveis sao estaticas para que precisem ser processadas apenas uma vez
    method_call_count e uma variavel local e pode ser aproveitada em outras logs
*/

// Avisa se excedeu um limite de tempo de acordo com warnLoggerExecTime
#define LogMethod(print_log_level) static const char* class_and_methodname = Logger_methodName(__PRETTY_FUNCTION__, __FUNCTION__); \
                    LoggerMethod log_methodvariable(class_and_methodname, print_log_level, warnLoggerExecTime);

// Avisa se excedeu um limite de tempo de acordo com warnLoggerExecTime
// Mostra um contabilizador de chamada do metodo
#define LogMethodC(print_log_level) static const char* class_and_methodname = Logger_methodName(__PRETTY_FUNCTION__, __FUNCTION__); \
                    static std::atomic<size_t> meThod_call_Count(0); \
                    size_t method_call_count = meThod_call_Count.fetch_add(1, std::memory_order_relaxed); \
                    LoggerMethodAndCount log_methodvariable(class_and_methodname, print_log_level,  warnLoggerExecTime, method_call_count);

// Avisa se excedeu um limite de tempo de acordo com warnLoggerExecTime
// Mostra um contabilizador de chamada do metodo
// Apresenta uma variavel de inicializacao de tempo que pode ser aproveitada em outras partes do código (method_init_time)
#define LogMethodCT(print_log_level) static const char* class_and_methodname = Logger_methodName(__PRETTY_FUNCTION__, __FUNCTION__); \
                    static std::atomic<size_t> meThod_call_Count(0); \
                    const std::chrono::time_point<std::chrono::steady_clock> method_init_time = steady_clock::now(); \
                    size_t method_call_count = meThod_call_Count.fetch_add(1, std::memory_order_relaxed); \
                    LoggerMethodAndCount log_methodvariable(class_and_methodname, print_log_level, warnLoggerExecTime, method_call_count, method_init_time)

// Avisa se excedeu um limite de tempo de acordo com duration_warning
#define LogMethodD(print_log_level, duration_warning) static const char* class_and_methodname = Logger_methodName(__PRETTY_FUNCTION__, __FUNCTION__); \
                                        LoggerMethod log_methodvariable(class_and_methodname, print_log_level, duration_warning);

// Avisa se excedeu um limite de tempo de acordo com duration_warning
// Mostra um contabilizador de chamada do metodo
#define LogMethodDC(print_log_level, duration_warning) static const char* class_and_methodname = Logger_methodName(__PRETTY_FUNCTION__, __FUNCTION__); \
                                        static std::atomic<size_t> meThod_call_Count(0); \
                                        size_t method_call_count = meThod_call_Count.fetch_add(1, std::memory_order_relaxed); \
                                        LoggerMethodAndCount log_methodvariable(class_and_methodname, log_level, duration_warning, method_call_count);

// Apenas mostra begin e end
#define LogMethodSimple(print_log_level) static const char* class_and_methodname = Logger_methodName(__PRETTY_FUNCTION__, __FUNCTION__); \
                            LoggerMethodSimple log_methodvariable(class_and_methodname, print_log_level);

// Apenas mostra begin e end
#define LogMethodThread(print_log_level) static const char* class_and_methodname = Logger_methodName(__PRETTY_FUNCTION__, __FUNCTION__); \
            LoggerMethodThread log_methodvariable(class_and_methodname, print_log_level, syscall(__NR_gettid));
#endif
