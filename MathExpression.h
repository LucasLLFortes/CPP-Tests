/**
 *  MathExpression.h
 *
 *  Copyright(c) 2020 ATI - Felipe Magalhaes
 *  All rights reserved.
 *
 *****************************************************************************/

/*

    Aplica o algoritmo de Shunting-Yard para realizar um processamento de dados
    matematicos. O algoritmo le uma expressao matematica e a transforma em uma
    expressao RPN (Reverse Polish Notation), muito mais simples de ser calculada

*/

/*****************************************************************************/

// Printa na saida as operacoes
// #define PRINT_SY

#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>

// Indica que a aplicacao E A SSIT
#define IS_SSIT_APPLICATION

/**
 * @brief Classe de aplicacao do algoritmo Shunting-Yard
 * 
 * @tparam Data Dado armazenado na fila
 * @tparam RData Tipo de retorno de uma operacao na fila
 * @tparam OpType Tipo do operador
 */
template <typename Data, typename RData, typename OpType>
class MathExpression
{

    // Esta classe foi projetada de forma a ser HERDADA
    // As classes filhas devem registrar as operacoes que se adequam a elas

    /////////////////////////////////////////////////////////////////////
    // Estrutura de dadps
    /////////////////////////////////////////////////////////////////////

public:

    /***********************************/
    /* Estrutura de um operador em RPN */

    struct RPNOperationData_s1
    {

        // Armazena um vetor de funcoes que realizam operacoes entre dados empilhados
        std::function< Data(const Data&, const Data&) > fdd;
        std::function< Data(const RData&, const Data&) > frd;
        std::function< Data(const Data&, const RData&) > fdr;
        std::function< Data(const RData&, const RData&) > frr;
        // Indica se o operador e unario ou binario
        bool binaryOp;

    };

    struct RPNOperationData_s2
    {

        // Armazena um vetor de funcoes que realizam operacoes entre dados empilhados
        std::function< Data(const Data&, const Data&) > fdd;
        // Indica se o operador e unario ou binario
        bool binaryOp;

    };

    using RPNOperationData =
    typename std::conditional<std::is_same<Data, RData>::value, 
    struct RPNOperationData_s2, struct RPNOperationData_s1 >::type;

    /*********************************/
    /* Armazena os dados na fila RPN */

    // Cada celula da pilha pode armazenar um operador OU um dado de operacao, nao os dois ao mesmo
    // A estrutura varia se Data != RData e Data == RData
    
    // Data != RData
    struct RPNQueueData_s1
    {
        // Operador
        std::shared_ptr< RPNOperationData > operation;
        // Dados a serem armazenados
        std::shared_ptr< Data > data;
        // Dado de retorno
        std::shared_ptr< RData > rdata;

    };

    // Data == RData
    struct RPNQueueData_s2
    {
        // Operador
        std::shared_ptr< RPNOperationData > operation;
        // Dados a serem armazenados
        std::shared_ptr< Data > data;

    };

    using RPNQueueData =
    typename std::conditional<std::is_same<Data, RData>::value, 
    struct RPNQueueData_s2, struct RPNQueueData_s1 >::type;

    // Estrutura de um operador em uma pilha de operacoes
    typedef struct
    {
        // Operador
        std::shared_ptr< RPNOperationData > operation;
        // Armazena um quantificador de precedencia de uma operacao
        // Operadores de maior precedencia devem acontecer antes dos de menor
        uint32_t precedence;
        // Registra o operador
        OpType ch;
        // Indica se o operador realiza operacoes em valores na direita ou esquerda
        // Relevante para operacoes unitarias como a B & !A, onde "!" opera em "A" e nao em "B"
        bool left;

    } StackOperationData;

private:

    // Pilha de operadores
    std::stack<StackOperationData> op_stack;

    // Dados de um operador
    std::unordered_map<OpType, StackOperationData> opdata;

    // Fila RPN gerada
    std::queue<RPNQueueData> RPNqueue;

    /////////////////////////////////////////////////////////////////////
    // Construtor
    /////////////////////////////////////////////////////////////////////

public:

    MathExpression() = delete;

    // Construtor para char, char* e std::string
    template<typename U = OpType,
        typename std::enable_if<
            std::is_same<U, char>::value ||
            std::is_same<U, char*>::value ||
            std::is_same<U, std::string>::value>::type* = nullptr>
    MathExpression(const Data default_value);

    // Construtor para outros tipos
    // Aqui alem do valor padrao deve se registrar o parenteses esquerdo e direito
    template <typename U = OpType,
        typename std::enable_if<
            !std::is_same<U, char>::value &&
            !std::is_same<U, char*>::value &&
            !std::is_same<U, std::string>::value>::type* = nullptr>
    MathExpression(const Data default_value,
        const OpType leftPar, const OpType rightPar);

    virtual ~MathExpression() {};

    /////////////////////////////////////////////////////////////////////
    // Registros
    /////////////////////////////////////////////////////////////////////

protected:

    // Registra um novo operador enumerado
    template <typename D = Data, typename R = RData> 
    typename std::enable_if<!std::is_same<D, R>::value, void>::type
    registerOperatorInfo(
                          std::function< Data(const Data&, const Data&)> fdd,
                          std::function< Data(const RData&, const Data&)> frd,
                          std::function< Data(const Data&, const RData&)> fdr,
                          std::function< Data(const RData&, const RData&)> frr,
                          const OpType &operation,
                          const uint32_t &precedence,
                          const bool &binaryOp=true,
                          const bool &left=true);

    // Registra um novo operador enumerado
    template <typename D = Data, typename R = RData> 
    typename std::enable_if<std::is_same<D, R>::value, void>::type
    registerOperatorInfo(
                          std::function< Data(const Data&, const Data&)> f,
                          const OpType &operation,
                          const uint32_t &precedence,
                          const bool &binaryOp=true,
                          const bool &left=true);

public:

    // Registra um novo dado na fila
    // Como nao sabemos a forma do dado, passamos uma copia
    void registerData(Data data);

    // Registra uma nova posicao da variavel
    void registerVariable();
    // Registra uma nova posicao de variavel com ID
    void registerVariableID(const uint32_t &id);

    /********************************/
    /* Registro de operacoes */

public:
    // Operacao comum
    bool registerOperation(const OpType &operation);

    // Operacoes especiais

private:
    // Registro de parenteses para tipos genericos
    template <typename U = OpType> 
    typename std::enable_if<
    !std::is_same<U, char>::value && 
    !std::is_same<U, char*>::value && 
    !std::is_same<U, std::string>::value
    , U>::type
    leftParentheses();

    template <typename U = OpType> 
    typename std::enable_if<
    !std::is_same<U, char>::value && 
    !std::is_same<U, char*>::value && 
    !std::is_same<U, std::string>::value
    , U>::type
    rightParentheses();

    // Registra os simbolos em um tipo generico
    OpType leftPar, rightPar;

public:
    // OpType = char
    template <typename U = OpType> typename std::enable_if<std::is_same<U, char>::value, bool>::type
    registerOperation(const std::string &operation);
    template <typename U = OpType> typename std::enable_if<std::is_same<U, char>::value, bool>::type
    registerOperation(const char* &operation);

private:
    template <typename U = OpType> typename std::enable_if<std::is_same<U, char>::value, char>::type
    convertOperation(const std::string &operation);
    template <typename U = OpType> typename std::enable_if<std::is_same<U, char>::value, char>::type
    convertOperation(const char* &operation);

    template <typename U = OpType> typename std::enable_if<std::is_same<U, char>::value, char>::type
    leftParentheses();
    template <typename U = OpType> typename std::enable_if<std::is_same<U, char>::value, char>::type
    rightParentheses();

public:
    // OpType = char*
    template <typename U = OpType> typename std::enable_if<std::is_same<U, char*>::value, bool>::type
    registerOperation(const std::string &operation);
    template <typename U = OpType> typename std::enable_if<std::is_same<U, char*>::value, bool>::type
    registerOperation(const char &operation);

private:
    template <typename U = OpType> typename std::enable_if<std::is_same<U, char*>::value, char*>::type
    convertOperation(const std::string &operation);
    template <typename U = OpType> typename std::enable_if<std::is_same<U, char*>::value, char*>::type
    convertOperation(const char &operation);

    template <typename U = OpType> typename std::enable_if<std::is_same<U, char*>::value, char*>::type
    leftParentheses();
    template <typename U = OpType> typename std::enable_if<std::is_same<U, char*>::value, char*>::type
    rightParentheses();

public:
    // OpType = std::string
    template <typename U = OpType> typename std::enable_if<std::is_same<U, std::string>::value, bool>::type
    registerOperation(const char &operation);
    template <typename U = OpType> typename std::enable_if<std::is_same<U, std::string>::value, bool>::type
    registerOperation(const char* &operation);

private:
    template <typename U = OpType> typename std::enable_if<std::is_same<U, std::string>::value, std::string>::type
    convertOperation(const char &operation);
    template <typename U = OpType> typename std::enable_if<std::is_same<U, std::string>::value, std::string>::type
    convertOperation(const char* &operation);

    template <typename U = OpType> typename std::enable_if<std::is_same<U, std::string>::value, std::string>::type
    leftParentheses();
    template <typename U = OpType> typename std::enable_if<std::is_same<U, std::string>::value, std::string>::type
    rightParentheses();

    /********************************/

private:

    // Registra um parenteses esquerdo
    void registerLeftParentheses();

    // Registra um parenteses direito
    bool registerRightParentheses();

private:
    // Termina o registro
    bool finishRegistration();

    /////////////////////////////////////////////////////////////////////
    // Calculo
    /////////////////////////////////////////////////////////////////////


public:

    // Calcula a aritmetica em RPN
    bool calculate(RData &result);

    // Avalia se uma expressao e verdadeira ou falsa e retorna o resultado
    bool evaluate(const Data data, bool &result);

    // Valor padrao ao ser retornado quando a pilha RPN estiver vazia
    RData default_value;

private:

    // Calcula a aritmetica em RPN
    template <typename D = Data, typename R = RData> typename std::enable_if<std::is_same<D, R>::value, bool>::type
    calculate(std::queue<RPNQueueData> &RPNqueue, RData &result); // (DATA == RDATA)

    // Calcula a aritmetica em RPN
    template <typename D = Data, typename R = RData> typename std::enable_if<!std::is_same<D, R>::value, bool>::type
    calculate(std::queue<RPNQueueData> &RPNqueue, RData &result); // (DATA != RDATA)

    /********************************/
    /* Variaveis */

private:
    // Variavel sem um ID especifico
    std::shared_ptr<Data> var_pos;
    // Variaveis referentes a um ID
    std::unordered_map<uint32_t, std::shared_ptr<Data>> variable_id;

public:
    // Retorna um vetor com os IDs das variaveis requisitadas
    std::vector<uint32_t> getVarID();

    // Armazena o valor de uma variavel com id
    bool setVarIDValue(const uint32_t &id, const Data &data);

};

#include "MathExpression.cpp"