/*
*****************************************************************************
** Copyright: (C) ATITEL
**
** Classification:  ATITEL  Internal Use Only
**
** Original Author and Date:
**                 Faria    18/06/2002
** Revision:
**
*/
/** Comments:
**     Definicao de Tipos para serem usados na aplicacao.
**
*/
/****************************************************************************/
#pragma once

#ifndef _ssitdefs_h_
#define _ssitdefs_h_

#include <vector>
#include <string>
#include <time.h>

using namespace std;

namespace SSIT {

  namespace DB {

    enum SSS_STATUS { SSS_UP = 0, SSS_DOWN = 1, SSS_NOCONFIGURED = 2, SSS_CONFIGURED = 3, SSS_DISABLED = 4, SSS_UNKNOWN = 5, SSS_LOCAL = 6 };
    enum ETS_STATUS {    } ;

    const unsigned int ETS_NO_CONFIGURED = 0;
    const unsigned int ETS_CONFIGURED = 1;
    const unsigned int ETS_DOWN = 0;
    const unsigned int ETS_UP = 1;
    const unsigned int ETS_NOALARMED = 0;
    const unsigned int ETS_ALARMED = 1;
    const unsigned int ETS_DISABLED = 0;
    const unsigned int ETS_ENABLED = 1;
    const unsigned int ETS_NOMONITOR = 0;
    const unsigned int ETS_WITHMONITOR = 1;

    const unsigned int TO_ENABLED = 1 ;   // POS 0
    const unsigned int TO_DISABLED = 0;   // POS 0
    const unsigned int TO_ALARMED = 1;    // POS 1
    const unsigned int TO_NO_ALARMED = 0;  // POS 1

    const unsigned int OPR_ENABLED=1;
    const unsigned int OPR_DISABLE=0;

    const unsigned int TOTYPE=0;
    const unsigned int TMTYPE=2;
    const unsigned int TCTYPE=1;
    const unsigned int TSTYPE=3;
    const unsigned int FILETYPE=4;

    enum MONITOR_STATE { M_INACTIVE = 0, M_ACTIVE = 1 };
    enum ETS_UPDATE_TYPE { ETS_CONFIGURATION = 1, ETS_ALIVE = 2, ETS_NORMAL = 3, ETS_ENABLE = 4, ETS_MONITOR = 5, ETS_NOTALARM = 6, ETS_ALARMURGENT = 7 };
    enum TO_UPDATE_TYPE { TO_ALIVE = 0, TO_STATE = 1 };
  };

  const unsigned int MAX_CLIENT_CONN = 100;

  enum APP_TYPE: int {
    CONEXAO_SOCKET = 0,
    CONEXAO_MQTT = 1
  };

  typedef long WEBCLIENT_ID;
  //typedef unsigned long CLIENT_ID;
  typedef long SSS_CODE;
  typedef long ETS_CODE;
  typedef long STA_CODE;

  typedef vector<ETS_CODE>  ETS_LIST;
  struct SSS_TYPE {
    SSS_CODE ssscode;
    DB::SSS_STATUS sssstatus;
    APP_TYPE appType;
  };

  typedef vector<SSS_TYPE> SSS_LIST;
  typedef long TO_CODE;
  typedef unsigned int TOSTATE_TYPE;
  typedef unsigned int TOTYPE_TYPE;

class TIMESTAMP {
public:
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int seconds;

    // Construtores
    TIMESTAMP() {};
    TIMESTAMP(int day, int month, int year, int hour, int minute, int seconds);
    explicit TIMESTAMP(const struct tm &tms);

    // Operador de igualdade
    TIMESTAMP& operator=(TIMESTAMP const& B);
    TIMESTAMP& operator=(const struct tm &tms);

    // Operadores de comparação
    bool operator==(TIMESTAMP const& B) const;
    bool operator!=(TIMESTAMP const& B) const;
    bool operator>(struct TIMESTAMP const& B) const;
    bool operator>=(TIMESTAMP const& B) const;
    bool operator<(struct TIMESTAMP const& B) const;
    bool operator<=(struct TIMESTAMP const& B) const;
    
    // Soma uma duracao em segundos
    TIMESTAMP operator+(const size_t duration) const;
    // Subtrai uma duracao em segundos
    inline TIMESTAMP operator-(const size_t duration) const { return (*this + (-duration)); };
    // Retorna a diferenca em segundos entre dois timestamps
    double operator-(struct TIMESTAMP const& B) const;

    // Soma uma duracao em segundos
    TIMESTAMP& operator+=(const size_t duration);
    // Subtrai uma duracao em segundos
    TIMESTAMP& operator-=(const size_t duration);

    // Verifica se os dados são todos iguais a 0
    bool operator!() const;

    // Converte de TIMESTAMP para struct tm
    struct tm getTM() const;

    // Coloca como 0 todos os dados
    void setZero();

    // Retorna true se as datas (DD/MM/YYYY) sao iguais
    bool isSameDay(struct TIMESTAMP const& other);

    // Converte um TIMESTAMP para string
    std::string toString() const;

    // Converte um TIMESTAMP para string em formato Json
    std::string toJsonString() const;

    // Checa se e o ultimo dia de um mes
    short lastDay(const int month, const int year); 

    // Soma ou subtrai dias a um Timestamp
    void addDays(const int days_to_add); 

    // Converte uma string para o tipo TIMESTAMP
    void StrtoTimestamp(const std::string &timestr);

    // Converte uma string do tipo JSON para o tipo TIMESTAMP
    void JSONStrtoTimestamp(const std::string &timestr);

    // Obtem o horario local
    void getLocalTime(struct tm &tm, time_t &rawtime);
    void getLocalTime();

};

  typedef struct {
    long         topos;
    unsigned int totype;
    unsigned int tostate;
    double       tovalue;
    unsigned int tospectype;
    TIMESTAMP    totime;
  } TOOBJ;

  typedef struct {
    unsigned int  configured;
    unsigned int  up;
    unsigned int  alarmed;
    unsigned int  enabled;
    unsigned int  monitored;
    unsigned int alarmcritic;
  } ETS_INFO;

  typedef vector<TOOBJ> TOLIST;

  typedef  struct  {
    SSS_CODE sss;
    ETS_CODE ets;
  } MO;     // managed object type

  struct TO_ALARM {
    ETS_CODE etscode;
    TO_CODE topos;
    TOTYPE_TYPE tospectype;
    int value;
    TIMESTAMP ts;
  };

  typedef vector<TO_ALARM> TO_ALARM_LIST;


  enum RESULT_CODE { RESULT_UNKNOWN = -4, UNKNOWN_ERROR = -3, DENIED = -2, DISABLED = -1, SUCCESS = 0, ERROR = 1 };
  enum ACTION { SSS_CONFIG = 0, GET_TO = 1, SEND_TO = 2, SET_MONITOR = 3, GET_VARTO = 4, DISABLE_TO = 5, ENABLE_TO = 6, GET_TIME = 7, SET_TIME = 8, ENABLE_ETS = 9, DISABLE_ETS = 10, INVALID = -1 };
  enum ALARM_TYPE { SYSTEM_ALARM,  NETWORK_ALARM };
  enum SYSTEM_ALARM_CODES { DATABASE_ALRM };
  enum AUDIT_TYPE { DATABASE_AUDIT , SYSTEM_AUDIT, MANAGEMENT_AUDIT } ;
  enum AUDIT_CODE {
    A_LOGIN = 0,
    A_LOGOUT = 1,
    A_SENDASRCONF = 2,
    A_SENDTO = 3,
    A_DISABLETO = 4,
    A_ENABLETO = 5,
    A_SETMONITOR = 6,
    A_SETTIME = 7,
    A_ENABLEETS = 8,
    A_DISABLEETS = 9,
    A_GETTIME = 10,
    A_GETTO  = 11,
    A_GETALARM = 12,
    A_ACKALARM = 13,
    A_ACKSYSALARM = 14,
    A_ENDALARM = 15
  };

  //string Float2Str( const float value );
  //string Time2Str( const TIMESTAMP ts );

  class Request;

  // Codigos de Severidade para alarmes de sistema

  const unsigned short ETS_DOWN_SEV_CODE = 1;
  const unsigned short SSS_DOWN_SEV_CODE = 1;
  const unsigned short ETS_UP_SEV_CODE = 5;
  const unsigned short SSS_UP_SEV_CODE = 5;
  const unsigned short ETS_ENDMONIT_SEV_CODE = 5;
  const unsigned short ETS_NOTCONFIG_SEV_CODE = 3;
  const unsigned short ETS_FAILURE_SEV_CODE = 1;
  const unsigned short SSS_FAILURE_SEV_CODE = 1;
  const unsigned short DB_ALARM_SEV_CODE = 5;
  const unsigned short REQ_LIST_SEV_CODE = 3;
};

namespace std
{
    template<>
    class hash<SSIT::TIMESTAMP>
    {
    public:

        size_t operator()(SSIT::TIMESTAMP const& c) const
        {
            return c.day + c.month + c.year; 
        }

    };

    template<>
    class equal_to<SSIT::TIMESTAMP>
    {
    public:

        bool operator()(const SSIT::TIMESTAMP &lhs, const SSIT::TIMESTAMP &rhs) const 
        {
            return (lhs == rhs);
        }

    };
}

#endif