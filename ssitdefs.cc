#include <iostream>
#include "ssitdefs.h"

using namespace SSIT;

TIMESTAMP::TIMESTAMP(int day, int month, int year, int hour, int minute, int seconds) :
    day(day),
    month(month),
    year(year),
    hour(hour),
    minute(minute),
    seconds(seconds)
{}

TIMESTAMP::TIMESTAMP(const struct tm &tms)
{
    *this = tms;
}

TIMESTAMP& TIMESTAMP::operator=(TIMESTAMP const& B)
{
    
    if (this != &B) { // self-assignment check expected

        year = B.year;
        month = B.month;
        day = B.day;
        hour = B.hour;
        minute = B.minute;
        seconds = B.seconds;
    }

    return *this;
}

TIMESTAMP& TIMESTAMP::operator=(const struct tm &tms)
{
    year = tms.tm_year+1900; // year since 1900
    month = tms.tm_mon+1; // month of year from 0 to 11
    day = tms.tm_mday; // day of month from 1 to 31
    hour = tms.tm_hour; // hours of day from 0 to 24
    minute = tms.tm_min; // minutes of hour from 0 to 59
    seconds = tms.tm_sec; // seconds of minutes from 0 to 61

    return *this;
}

bool TIMESTAMP::operator==(TIMESTAMP const& B) const
{
    return (
    year == B.year &&
    month == B.month &&
    day == B.day &&
    hour == B.hour &&
    minute == B.minute &&
    seconds == B.seconds
    );
}

bool TIMESTAMP::operator!=(TIMESTAMP const& B) const
{
    return (
    year != B.year ||
    month != B.month ||
    day != B.day ||
    hour != B.hour ||
    minute != B.minute ||
    seconds != B.seconds
    );
}

bool TIMESTAMP::operator>(struct TIMESTAMP const& B) const
{
    return
    (year > B.year ||
    (year == B.year && 
        (month > B.month || 
        (month == B.month &&
            (day > B.day ||
            (day == B.day &&
                (hour > B.hour ||
                (hour == B.hour &&
                    (minute > B.minute ||
                    (minute == B.minute &&
                        (seconds > B.seconds)
                    )
                    )
                )
                )
            )
            )
        )
        )
    )
    );
}

bool TIMESTAMP::operator>=(TIMESTAMP const& B) const
{
    return ((*this == B) || (*this > B));
}

bool TIMESTAMP::operator<(struct TIMESTAMP const& B) const
{
    return
    (year < B.year ||
    (year == B.year && 
        (month < B.month || 
        (month == B.month &&
            (day < B.day ||
            (day == B.day &&
                (hour < B.hour ||
                (hour == B.hour &&
                    (minute < B.minute ||
                    (minute == B.minute &&
                        (seconds < B.seconds)
                    )
                    )
                )
                )
            )
            )
        )
        )
    )
    );
}

bool TIMESTAMP::operator<=(struct TIMESTAMP const& B) const
{
    return ((*this == B) || (*this < B));
}

// Soma uma duracao em segundos
TIMESTAMP& TIMESTAMP::operator+=(const size_t duration)
{ 
    *this = *this + duration;
    return *this;
};
// Subtrai uma duracao em segundos
TIMESTAMP& TIMESTAMP::operator-=(const size_t duration)
{
    *this = *this - duration;
    return *this;
}

// Verifica se os dados são todos iguais a 0
bool TIMESTAMP::operator!() const
{
    return (day == 0 && month == 0 && year == 0 && hour == 0 && minute == 0 && seconds == 0);
}

// Converte de TIMESTAMP para struct tm
struct tm TIMESTAMP::getTM() const
{

    struct tm tms;
    
    tms.tm_year = year-1900; // year since 1900
    tms.tm_mon = month-1; // month of year from 0 to 11
    tms.tm_mday = day; // day of month from 1 to 31
    tms.tm_hour = hour; // hours of day from 0 to 24
    tms.tm_min = minute; // minutes of hour from 0 to 59
    tms.tm_sec = seconds; // seconds of minutes from 0 to 61
    tms.tm_isdst = 0;
    
    return tms;
}

// Soma uma duracao em segundos
TIMESTAMP TIMESTAMP::operator+(const size_t duration) const
{
    
    // Obtem o tm relacionado ao timestamp
    struct tm tms = getTM();
    time_t rawtime = mktime(&tms);

    // O time_t é uma estrutura que retorna o numero de segundos desde uma certa data
    // Entao, basta somar duration
    rawtime += duration;

    // Converte a nova data para struct tm
    localtime_r(&rawtime, &tms);

    // Retorna nossa copia
    return TIMESTAMP(tms);

}

// Retorna a diferenca em segundos entre dois timestamps
double TIMESTAMP::operator-(struct TIMESTAMP const& B) const
{
    struct tm tm1, tm2;
    tm1 = B.getTM();
    tm2 = getTM();

    return difftime(mktime(&tm2),mktime(&tm1));
}

void TIMESTAMP::setZero()
{
    year = 0;
    month = 0;
    day = 0;
    hour = 0;
    minute = 0;
    seconds = 0;
}

bool TIMESTAMP::isSameDay(struct TIMESTAMP const& other)
{
    if (year == other.year && month == other.month && day == other.day)
        return true;
    else
        return false;
}

// Converte um TIMESTAMP para string
std::string TIMESTAMP::toString() const
{
    std::string yearzeros, monthzeros, dayzeros, hourzeros, minutezeros, seczeros;
    if(year < 1000) {
    yearzeros = "0";
    if(year < 100) {
        yearzeros.append("0");

        if(year < 10) yearzeros.append("0");
    }
    }

    if(month < 10) monthzeros = "0";
    if(day < 10) dayzeros = "0";
    if(hour < 10) hourzeros = "0";
    if(minute < 10) minutezeros = "0";
    if(seconds < 10) seczeros = "0";

    std::string str;
    str = dayzeros + std::to_string(day) + "/" +
    monthzeros + std::to_string(month) + "/" +
    yearzeros + std::to_string(year) + "-" +
    hourzeros + std::to_string(hour) + ":" +
    minutezeros + std::to_string(minute) + ":" +
    seczeros + std::to_string(seconds);

    return str;
}

// Converte um TIMESTAMP para string no padrao Javascript (Json compatible)
std::string TIMESTAMP::toJsonString() const
{
    std::string yearzeros, monthzeros, dayzeros, hourzeros, minutezeros, seczeros;
    if(year < 1000) {
    yearzeros = "0";
    if(year < 100) {
        yearzeros.append("0");

        if(year < 10) yearzeros.append("0");
        }
    }

    if(month < 10) monthzeros = "0";
    if(day < 10) dayzeros = "0";
    if(hour < 10) hourzeros = "0";
    if(minute < 10) minutezeros = "0";
    if(seconds < 10) seczeros = "0";

    std::string str;
    str =   
        yearzeros   + std::to_string(year)      + "-" +
        monthzeros  + std::to_string(month)     + "-" +
        dayzeros    + std::to_string(day)       + "T" +
        hourzeros   + std::to_string(hour)      + ":" +
        minutezeros + std::to_string(minute)    + ":" +
        seczeros    + std::to_string(seconds)   ;

    return str;
}

/**
 * @brief Retorna qual o ultimo dia do mes atual, considerando tambem anos bissextos
 * 
 * @param month mes atual
 * @param year ano atual
 * @return char 
 */
short TIMESTAMP::lastDay(const int month, const int year)
{
  static const int days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

  /* Calculo do ano bissexto, definicao:
     "Ano bissexto em calendário Gregoriano, é um ano com 366 dias. Um ano bissexto é aquele cujo número 
     é divisível por quatro e, sendo múltiplo de 100, será aquele cujo número é divisível por 400."*/
  if (month==2) // Caso seja fevereiro
  {
    if (year%4==0)
    {
      if (year%100==0)
      {
        if (year%400==0)
          return(29);
      }
      else
        return(29);
    }
  }
  return(days[month-1]);
}

/**
 * @brief Soma ou subtrai uma quantidade de dias a um Timestamp (Maximo 28)
 * 
 * @param days Dias a se somar/subtrair
 */
void TIMESTAMP::addDays(const int days_to_add)
{    
// subtrair dias    
    if(days_to_add < 0){    
        // Caso nao sejam os primeiros dias do mes
        if(day + days_to_add >= 1){
            day += days_to_add;
            return;
        }

        // Caso tenha que subtrair um mes        
        if(month != 1){
            //Caso nao seja o primeiro mes do ano
            day = day + days_to_add + lastDay(month-1, year);
            month--;
            return;
        }

        // Caso primeiros dias de primeiro mes do ano
        day = day + days_to_add + 31; //31 Dias em Dezembro
        month = 12;
        year--;
        return;
    }

    
// somar dias
    // Caso dia final nao seja o ultimo dia do mes
    if(day + days_to_add <= lastDay(month, year)){
        
        day += days_to_add;
        return;
    }
    
    // Caso ultimos dias do mes, dia sera o primeiro e somar um mes
    day = day + days_to_add - lastDay(month, year);
    if(month != 12){
        //Caso nao seja o ultimo mes do ano
        month++;
        return;
    }
    
    // Caso ultimos dias de ultimo mes do ano
    month = 1;
    year++;
    return;
}



void TIMESTAMP::StrtoTimestamp(const std::string &timestr)
{

    day = std::stoi( timestr.substr(0, 2) );
    month = std::stoi( timestr.substr(3, 2) );
    year = std::stoi( timestr.substr(6, 4) );
    hour = std::stoi( timestr.substr(11, 2) );
    minute = std::stoi( timestr.substr(14, 2) );
    seconds = std::stoi( timestr.substr(17, 2) );
}

void TIMESTAMP::JSONStrtoTimestamp(const std::string &timestr)
{
    // Example: 2000-01-01T01:00:00.000Z ou outros formatos. Mas nao pega GMT
    
    year =      std::stoi (timestr.substr(0, 4) );
    month =     std::stoi (timestr.substr(5, 2) );
    day =       std::stoi (timestr.substr(8, 2) );
    hour =      std::stoi (timestr.substr(11, 2) );
    minute =    std::stoi (timestr.substr(14, 2) );
    seconds =   std::stoi (timestr.substr(17, 2) );
}

void TIMESTAMP::getLocalTime(struct tm &tm, time_t &rawtime)
{

    time( &rawtime );
    localtime_r( &rawtime, &tm );

    *this = tm;
}

void TIMESTAMP::getLocalTime()
{

    time_t rawtime;
    struct tm tm;

    getLocalTime(tm, rawtime);
}
/*
// Versao thread safe
void TIMESTAMP::getLocalTime_safe(struct tm &tm, time_t &rawtime)
{

    time( &rawtime );
    localtime_r( &rawtime, &tm );

    tmutex.lock();
    *this = tm;
    tmutex.unlock();
}

// Versao thread safe
void TIMESTAMP::getLocalTime_safe()
{

    time_t rawtime;
    struct tm tm;

    getLocalTime_safe(tm, rawtime);
}*/
