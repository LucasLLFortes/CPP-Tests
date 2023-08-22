/**
 * Logger.cc
 *
 * Copyright(c) 2002 ATI - Janaina Senna
 * All rights reserved.
 */

#include <ctime>
#include <execinfo.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <mutex>
#include <thread>
#include <stdlib.h>

#include "Logger.h"

#ifdef USE_UT_CXXTEST
int Logger::systemLevel = CXXTEST;
#else
int Logger::systemLevel = DEBUG;
#endif

void Logger::setSystemLevel(int level) 
{
  if(level != SILENT)
    Logger::lprintf(SILENT, "Alterando nível de DEBUG para %d.\n", level);
  systemLevel = level;
}

void Logger::printBuffer(int level, const char *buffer, int bufferSize)
{

  if( level <= systemLevel )
  {
    int oldstate;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&oldstate);

    lprintf(level, "Buffer - %d: [", bufferSize);
    for (int i = 0; i < bufferSize; i++) 
    {
      if (buffer[i] < ' ' || buffer[i] > '~') 
      {
        printf("x%02X", (unsigned char)buffer[i]);
      }
      else 
      {
        printf("%c", buffer[i]);
      }
    }
    printf("]\n");
    lprintf(level, "\n");

    pthread_setcancelstate(oldstate,NULL);
  }
}

void Logger::lprintf(int level, const char *format, ...) 
{

  if(level <= systemLevel)
  {

      std::scoped_lock tlock(print_mutex);

      time_t now = time(NULL);
      struct tm tm_now;
      localtime_r(&now, &tm_now);
      strftime(date, 21, "%d/%m/%Y-%H:%M:%S", &tm_now);

      int size = snprintf(buffer, buffer_size, "%s-%ld: %s", 
          date, syscall(__NR_gettid), format);

      if(size >= buffer_size)
      {
          buffer_size = size+1;
          delete[] buffer;
          buffer = (char*)realloc(&buffer, buffer_size*sizeof(char));
          sprintf(buffer, "%s-%ld: %s", 
              date, syscall(__NR_gettid), format);
      }

      va_list ap;
      va_start(ap, format);

      vprintf(buffer, ap);

      fflush(stdout);
      va_end(ap);
  }
}

void Logger::lfprintf(int level, FILE *stream, const char *format, ...) 
{

  if (level <= systemLevel) 
  {
    char buf[1024];
    char date[50];
    struct tm result;
    time_t t;
    va_list ap;
    int oldstate;

    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&oldstate);

    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    //strcat(buf, "\n");
    fflush(stream);
    time(&t);
    localtime_r(&t, &result);
    strftime(date, 50, "%d/%m/%Y-%H:%M:%S", &result);
    fprintf(stream, "%s-%ld: %s", date, syscall(__NR_gettid), buf);
    fflush(stream);
    va_end(ap);

    pthread_setcancelstate(oldstate,NULL);
  }
}

/* Obtain a backtrace and print it to stdout. */
void Logger::print_trace( int level )
{
  if (level <= systemLevel) 
  {
    void *array[10];
    size_t size;
    char **strings;
    size_t i;
    int oldstate;

    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&oldstate);

    size = backtrace (array, 10);
    strings = backtrace_symbols (array, size);

    lprintf(level, "Stack: [");
    for (i = 0; i < size; i++)
      printf("%s\n", strings[i]);
    printf("]\n");
    lprintf(level, "\n");

    free (strings);

    pthread_setcancelstate(oldstate,NULL);
  }
}

char* Logger_methodName(const char* prettyfunction, const char* methodname) noexcept
{

    std::string prettyfunctionStr(prettyfunction);
    std::string methodnameStr(methodname);

    methodnameStr += '(';

    size_t symbols;

    // Retira o que estiver em ( )
    prettyfunctionStr.erase(prettyfunctionStr.find(methodnameStr)+methodnameStr.size());
    prettyfunctionStr += ')';

    // Retira o que estiver em < >
    do
    {
      size_t position_isymbol;

      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
      size_t position_lsymbol;
      #pragma GCC diagnostic pop

      symbols = 0;

      for(size_t pos = 0; pos < prettyfunctionStr.size(); pos++)
      {
        if(prettyfunctionStr[pos] == '<')
        {
          if(symbols++ == 0) position_isymbol = pos;
        }
        else if(prettyfunctionStr[pos] == '>') {
          if(symbols == 1)
          {
            position_lsymbol = pos;
            break;
          }
          symbols--;
        } 
      }

      // É impossivel position_lsymbol iniciar sem inicialização, porém não tem como o compilador saber isso
      // portando retiramos o warning
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
      if(symbols) {
        prettyfunctionStr.erase(position_isymbol, position_lsymbol-position_isymbol+1);
      }
      #pragma GCC diagnostic pop

    } while(symbols);

    // Retira o tipo do metodo
    prettyfunctionStr.erase(0, prettyfunctionStr.find_last_of(' ')+1);

    // Copia para a string
    size_t size = prettyfunctionStr.size();

    char* class_and_method = new char[size+1];
    prettyfunctionStr.copy(class_and_method, size);
    class_and_method[size] = '\0';

    // Olha a posicao onde esta o ultimo espaço

    // A ideia e que class_and_method seja uma variavel estatica
    // Portanto, ela nao precisa ter um delete pois e limpa na finalizacao do programa!

    return class_and_method;
}