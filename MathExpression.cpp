#pragma once
#include "MathExpression.h"

#include <type_traits>
#include <iostream>

/////////////////////////////////////////////////////////////////////
// Construtor
/////////////////////////////////////////////////////////////////////

// Construtor para char, char* e std::string
template <typename Data, typename RData, typename OpType>
template <typename U,
    typename std::enable_if<
        std::is_same<U, char>::value ||
        std::is_same<U, char*>::value ||
        std::is_same<U, std::string>::value>::type*>
MathExpression<Data, RData, OpType>::MathExpression(const Data default_value)
{
    // Armazena o valor padrao de retorno da operacao
    this->default_value = default_value;

    // Aloca um espaco para variavel
    var_pos = std::make_shared<Data>();
}

// Construtor para outros tipos
// Aqui alem do valor padrao deve se registrar o parenteses esquerdo e direito
template <typename Data, typename RData, typename OpType>
template <typename U,
        typename std::enable_if<
            !std::is_same<U, char>::value &&
            !std::is_same<U, char*>::value &&
            !std::is_same<U, std::string>::value>::type*>
MathExpression<Data, RData, OpType>::MathExpression(const Data default_value, 
    const OpType leftPar, const OpType rightPar)
{
    // Armazena o valor padrao de retorno da operacao
    this->default_value = default_value;

    // Aloca um espaco para variavel
    var_pos = std::make_shared<Data>();

    // Armazena os simbolos de parenteses esquerdo e direito
    this->leftPar = leftPar;
    this->rightPar = rightPar;
}

/////////////////////////////////////////////////////////////////////
// Registros
/////////////////////////////////////////////////////////////////////


template <typename Data, typename RData, typename OpType>
template <typename D, typename R>   
typename std::enable_if<!std::is_same<D, R>::value, void>::type
inline MathExpression<Data, RData, OpType>::registerOperatorInfo(
                        std::function< Data(const Data&, const Data&)> fdd,
                        std::function< Data(const RData&, const Data&)> frd,
                        std::function< Data(const Data&, const RData&)> fdr,
                        std::function< Data(const RData&, const RData&)> frr,
                        const OpType &operation,
                        const uint32_t &precedence,
                        const bool &binaryOp,
                        const bool &left)
{
    // Armazena o dados da operacao no mapa de operadores
    StackOperationData od;
    od.operation = std::make_shared<RPNOperationData>();
    od.operation->fdd = fdd;
    od.operation->frd = frd;
    od.operation->fdr = fdr;
    od.operation->frr = frr;
    od.operation->binaryOp = binaryOp;
    od.left = left;
    od.precedence = precedence;
    od.ch = operation;

    opdata[operation] = od;
}

template <typename Data, typename RData, typename OpType>
template <typename D, typename R>   
typename std::enable_if<std::is_same<D, R>::value, void>::type
inline MathExpression<Data, RData, OpType>::registerOperatorInfo(
                          std::function< Data(const Data&, const Data&)> fdd,
                          const OpType &operation,
                          const uint32_t &precedence,
                          const bool &binaryOp,
                          const bool &left)
{

    // Armazena o dados da operacao no mapa de operadores
    StackOperationData od;
    od.operation = std::make_shared<RPNOperationData>();
    od.operation->fdd = fdd;
    od.operation->binaryOp = binaryOp;
    od.precedence = precedence;
    od.left = left;
    od.ch = operation;

    opdata[operation] = od;

}

// Registra um novo dado na fila
// Como nao sabemos a forma do dado, passamos uma copia
template <typename Data, typename RData, typename OpType>
void MathExpression<Data, RData, OpType>::registerData(Data data)
{

    // Registra os operadores de precedencia direita
    while(!op_stack.empty() && !op_stack.top().left && op_stack.top().ch != leftParentheses()) {
        // Coloca o ultimo operador dado na fila de RPN
        RPNQueueData qdata;
        qdata.operation = op_stack.top().operation;
        qdata.data = nullptr;
        RPNqueue.push(qdata);
        // Retira o ultimo dado da pilha de operadores
        op_stack.pop();
    }

    RPNQueueData qdata;
    qdata.operation = nullptr;
    qdata.data = std::make_shared<Data>(data);
    RPNqueue.push(qdata);
}

// Registra uma nova posicao da variavel
template <typename Data, typename RData, typename OpType>
void MathExpression<Data, RData, OpType>::registerVariable()
{
    // Adiciona a variavel na fila
    RPNQueueData qdata;
    qdata.operation = nullptr;
    qdata.data = var_pos;
    RPNqueue.push(qdata);
}

// Registra uma nova posicao da variavel com ID
template <typename Data, typename RData, typename OpType>
void MathExpression<Data, RData, OpType>::registerVariableID(const uint32_t &id)
{
    // Se nao esta alocado a posicao, o alova
    if(variable_id.find(id) == variable_id.end()) variable_id[id] = std::make_shared<Data>();

    // Adiciona a variavel na fila
    RPNQueueData qdata;
    qdata.operation = nullptr;
    qdata.data = variable_id[id];
    RPNqueue.push(qdata);
}

// Registra um parenteses esquerdo
template <typename Data, typename RData, typename OpType>
void MathExpression<Data, RData, OpType>::registerLeftParentheses()
{
    // Empilha o parenteses
    StackOperationData od;
    od.ch = leftParentheses();
    
    // Verifica se o operador anterior ao parenteses e do tipo precedencia direita
    // Se for, indica no parenteses
    if(!op_stack.empty() && !op_stack.top().left) {
        od.left = false;
    }
    else {
        od.left = true;
    }

    op_stack.push(od);
}

// Registra um parenteses direito
template <typename Data, typename RData, typename OpType>
bool MathExpression<Data, RData, OpType>::registerRightParentheses()
{

    while(!op_stack.empty()) {

        // Se encontrou um parenteses esquerdo, termina a execucao
        if(op_stack.top().ch == leftParentheses()) {

            // Verifica se foi registrado que se tem um operador direito antes
            // do parenteses. Se tiver, devemos tambem o acumular
            if(!op_stack.top().left) {
                // Retira o parenteses
                op_stack.pop();
                
                RPNQueueData qdata;
                qdata.operation = op_stack.top().operation;
                qdata.data = nullptr;

                // Coloca o operador direito na fila de operadores
                RPNqueue.push(qdata);
                // Retira o operador
                op_stack.pop();

            }
            else {
                // Retira o parenteses
                op_stack.pop();
            }
            return true;
        }

        RPNQueueData qdata;
        qdata.operation = op_stack.top().operation;
        qdata.data = nullptr;

        // Coloca o ultimo operador dado na fila de RPN
        RPNqueue.push(qdata);
        // Retira o ultimo dado da fila de operadores
        op_stack.pop();
    }

    return false;
}

// Termina o registro
template <typename Data, typename RData, typename OpType>
bool MathExpression<Data, RData, OpType>::finishRegistration()
{
    while(!op_stack.empty()) {

        // Parenteses esquerdo sobrando, retorna erro
        if(op_stack.top().ch == leftParentheses()) return false;

        RPNQueueData qdata;
        qdata.operation = op_stack.top().operation;
        qdata.data = nullptr;

        // Coloca o ultimo operador dado na pilha de RPN
        RPNqueue.push(qdata);
        // Retira o ultimo dado da pilha de operadores
        op_stack.pop();

    }

    // Registro finalizado com sucesso
    return true;
}

// Registra uma operacao
template <typename Data, typename RData, typename OpType>
bool MathExpression<Data, RData, OpType>::registerOperation(const OpType &operation)
{

    if(operation == leftParentheses()) {
        registerLeftParentheses();
        return true;
    }
    if(operation == rightParentheses()) return registerRightParentheses();

    // Nao encontrou a operacao
    if(opdata.find(operation) == opdata.end()) return false;

    auto &op = opdata[operation];

    // Retira os operadores de maior precedencia e os de associacao direita
    while(!op_stack.empty() && op_stack.top().ch != leftParentheses() && (
    (op_stack.top().left && op_stack.top().precedence < op.precedence)
    || (!op_stack.top().left)
    ))
    {
        // Coloca o ultimo operador dado na fila de RPN
        RPNQueueData qdata;
        qdata.operation = op_stack.top().operation;
        qdata.data = nullptr;
        RPNqueue.push(qdata);
        // Retira o ultimo dado da pilha de operadores
        op_stack.pop();
    }

    // Poe o dado na lista de operadores
    op_stack.push(op);

    // Operacao realizada com sucesso
    return true;
}

// Parenteses esquerdo
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<
    !std::is_same<U, char>::value && 
    !std::is_same<U, char*>::value && 
    !std::is_same<U, std::string>::value
    , U>::type
inline MathExpression<Data, RData, OpType>::leftParentheses()
{
    return leftPar;
}

// Parenteses direito
template <typename Data, typename RData, typename OpType>
template <typename U> 
    typename std::enable_if<
    !std::is_same<U, char>::value && 
    !std::is_same<U, char*>::value && 
    !std::is_same<U, std::string>::value
    , U>::type
inline MathExpression<Data, RData, OpType>::rightParentheses()
{
    return rightPar;
}

/********************************/
/* CHAR */

template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, char>::value, bool>::type
inline MathExpression<Data, RData, OpType>::registerOperation(const std::string &operation)
{
    registerOperation(convertOperation(operation));
}

template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, char>::value, bool>::type
inline MathExpression<Data, RData, OpType>::registerOperation(const char* &operation)
{
    registerOperation(convertOperation(operation));
}

// Conversao std::string -> char
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, char>::value, char>::type
inline MathExpression<Data, RData, OpType>::convertOperation(const std::string &operation)
{
    return operation.c_str()[0];
}

// Conversao char* -> char
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, char>::value, char>::type
inline MathExpression<Data, RData, OpType>::convertOperation(const char* &operation)
{
    return operation[0];
}

// Parenteses esquerdo para um char
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, char>::value, char>::type
inline MathExpression<Data, RData, OpType>::leftParentheses()
{
    return '(';
}

// Parenteses direito para um char
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, char>::value, char>::type
inline MathExpression<Data, RData, OpType>::rightParentheses()
{
    return ')';
}

/********************************/
/* CHAR* */

template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, char*>::value, bool>::type
inline MathExpression<Data, RData, OpType>::registerOperation(const char &operation)
{
    registerOperation(convertOperation(operation));
}

template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, char*>::value, bool>::type
inline MathExpression<Data, RData, OpType>::registerOperation(const std::string &operation)
{
    registerOperation(convertOperation(operation));
}

// Conversao char -> char*
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, char*>::value, char*>::type
inline MathExpression<Data, RData, OpType>::convertOperation(const char &operation)
{
    char* c[2];
    c[0] = operation;
    c[1] = '\0';
    return c;
}

// Conversao std::string -> char*
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, char*>::value, char*>::type
inline MathExpression<Data, RData, OpType>::convertOperation(const std::string &operation)
{
    return operation.c_str();
}

// Parenteses esquerdo para um char*
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, char*>::value, char*>::type
inline MathExpression<Data, RData, OpType>::leftParentheses()
{
    return "(";
}

// Parenteses direito para um char
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, char*>::value, char*>::type
inline MathExpression<Data, RData, OpType>::rightParentheses()
{
    return ")";
}

/********************************/
/* STD::STRING */

template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, std::string>::value, bool>::type
inline MathExpression<Data, RData, OpType>::registerOperation(const char &operation)
{
    registerOperation(convertOperation(operation));
}

template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, std::string>::value, bool>::type
inline MathExpression<Data, RData, OpType>::registerOperation(const char* &operation)
{
    registerOperation(convertOperation(operation));
}

// Conversao char -> std::string
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, std::string>::value, std::string>::type
inline MathExpression<Data, RData, OpType>::convertOperation(const char &operation)
{
    char* c[2];
    c[0] = operation;
    c[1] = '\0';
    return std::string(c);
}

// Conversao char* -> std::string
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, std::string>::value, std::string>::type
inline MathExpression<Data, RData, OpType>::convertOperation(const char* &operation)
{
    return std::string(operation);
}

// Parenteses esquerdo para um std::string
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, std::string>::value, std::string>::type
inline MathExpression<Data, RData, OpType>::leftParentheses()
{
    return std::string("(");
}

// Parenteses direito para um std::string
template <typename Data, typename RData, typename OpType>
template <typename U> typename std::enable_if<std::is_same<U, std::string>::value, std::string>::type
inline MathExpression<Data, RData, OpType>::rightParentheses()
{
    return std::string(")");
}

/////////////////////////////////////////////////////////////////////
// Calculo
/////////////////////////////////////////////////////////////////////

// Calcula a aritmetica em RPN
template <typename Data, typename RData, typename OpType>
bool MathExpression<Data, RData, OpType>::calculate(RData &result)
{

    // Termina o registro de operacoes e retorna falso caso de algum problema
    if(!finishRegistration()) return false;

    // Cria uma variavel auxiliar para que nao se perca a informacao posteriomente
    auto auxqueue = RPNqueue; 
    return calculate(auxqueue, result);
}

// Calcula a aritmetica em RPN
template <typename Data, typename RData, typename OpType>
template <typename D, typename R> typename std::enable_if<std::is_same<D, R>::value, bool>::type
inline MathExpression<Data, RData, OpType>::calculate(std::queue<RPNQueueData> &RPNqueue, RData &result)
{

    // Armazena os operandos
    Data first, second;

    // Pilha vazia, retorna valor padrao
    if(RPNqueue.empty()) {
        result = default_value;
        return true;
    }

    // Pilha de saida
    std::stack<Data> out;

    while(!RPNqueue.empty()) {

        // Armazena o primeiro valor e o retira da fila
        auto& front = RPNqueue.front();
        RPNqueue.pop();

        // Dado
        if(front.data != nullptr)
        {
            // Armazena o dado na fila de saida
            out.push(*(front.data));

            #ifdef PRINT_SY
            std::cout << *(front.data) << " ";
            #endif
        }
        // Operador
        else
        {

            // A fila esta vazia
            if(!out.size()) {
                #ifdef PRINT_SY
                std::cout << std::endl;
                #endif
                return false;
            }

            // Armazena os operandos
            second = out.top();
            out.pop();

            // Operador binario
            if(front.operation->binaryOp) {

                // Nao foi dado um segundo dado
                if(out.size() < 1) {
                    #ifdef PRINT_SY
                    std::cout << std::endl;
                    #endif
                    return false;
                }

                // Calcula a operacao e coloca o dado na fila de saida
                first = out.top();
                out.pop();
                out.push(front.operation->fdd(first, second));

            }
            else {
                // Nesse caso, nao faz diferenca um segundo parametro
                out.push(front.operation->fdd(second, second));
            }

        }
    }

    // Operacao realizada com sucesso. Armazena o resultado
    result = out.top();
    out.pop();

    #ifdef PRINT_SY
    std::cout << std::endl;
    #endif

    return true;
}

// Calcula a aritmetica em RPN
template <typename Data, typename RData, typename OpType>
template <typename D, typename R> typename std::enable_if<!std::is_same<D, R>::value, bool>::type
inline MathExpression<Data, RData, OpType>::calculate(std::queue<RPNQueueData> &RPNqueue, RData &result)
{

    // Armazena os operandos
    RPNQueueData first, second;

    // Pilha vazia, retorna valor padrao
    if(RPNqueue.empty()) {
        result = default_value;
        return true;
    }

    // Pilha de saida
    std::stack<RPNQueueData> out;

    while(!RPNqueue.empty()) {

        // Armazena o primeiro valor e o retira da fila
        auto& front = RPNqueue.front();
        RPNqueue.pop();

        // Dado
        if(front.data != nullptr)
        {
            // Armazena o dado na fila de saida
            out.push(front);

            #ifdef PRINT_SY
            std::cout << *(front.data) << " ";
            #endif
        }
        // Operador
        else
        {

            // A fila esta vazia
            if(!out.size())  {
                #ifdef PRINT_SY
                std::cout << std::endl;
                #endif
                return false;
            }

            // Armazena os operandos
            second = out.top();
            out.pop();

            // Operador binario
            if(front.operation->binaryOp) {

                // Nao foi dado um segundo dado
                if(out.size() < 1)  {
                    #ifdef PRINT_SY
                    std::cout << std::endl;
                    #endif
                    return false;
                }

                first = out.top();
                out.pop();

                RPNQueueData rpnoutdata;

                rpnoutdata.data = nullptr;
                rpnoutdata.operation = nullptr;

                // Checa o tipo de dado armazenado e retorna o valor correto
                if(first.data != nullptr) {
                    if(second.data != nullptr) {
                        rpnoutdata.rdata = std::make_shared<front.operation->fdd(first.data, second.data)>;
                    }
                    else {
                        rpnoutdata.rdata = std::make_shared<front.operation->fdr(first.data, second.rdata)>;
                    }
                }
                else {
                    if(second.data != nullptr) {
                        rpnoutdata.rdata = std::make_shared<front.operation->frd(first.rdata, second.data)>;
                    }
                    else {
                        rpnoutdata.rdata = std::make_shared<front.operation->frr(first.rdata, second.rdata)>;
                    }
                }

                // Calcula a operacao e coloca o dado na fila de saida
                out.push(rpnoutdata);

            }
            else {

                // Nesse caso, nao faz diferenca um segundo parametro
                if(second.data != nullptr) {
                    // Nesse caso, nao faz diferenca um segundo parametro
                    out.push(front.operation->fdd(second.data, second.data));
                }
                else if(second.data != nullptr) {
                    out.push(front.operation->frr(second.rdata, second.rdata));
                }
            }

        }
    }

    // Operacao realizada com sucesso. Armazena o resultado
    result = out.top();
    out.pop();

    #ifdef PRINT_SY
    std::cout << std::endl;
    #endif

    return true;
}

// Avalia se uma expressao e verdadeira ou falsa
template <typename Data, typename RData, typename OpType>
inline bool MathExpression<Data, RData, OpType>::evaluate(const Data data, bool &result) {

    Data value;

    // Modifica a variavel armazenada na fila
    *var_pos = data;

    // Erro de leitura
    if(!calculate(value)) return false;

    // Se a expressao for diferente de 0, retorna true
    result = value ? true : false;

    return true;
}

// Retorna um vetor com os IDs das variaveis requisitadas
template <typename Data, typename RData, typename OpType>
std::vector<uint32_t> MathExpression<Data, RData, OpType>::getVarID()
{
    std::vector<uint32_t> v;
    v.resize(variable_id.size());
    size_t i = 0;

    #ifdef PRINT_SY
    std::cout << "IDS EXISTENTES: ";
    #endif
    // Armazena todos IDs existentes no mapa
    for(auto &m : variable_id) {
        v[i++] = m.first;
        #ifdef PRINT_SY
        std::cout << m.first << " ";
        #endif
    }
    #ifdef PRINT_SY
    std::cout << std::endl;
    #endif

    return v;
}

// Armazena o valor de uma variavel com id
template <typename Data, typename RData, typename OpType>
inline bool MathExpression<Data, RData, OpType>::setVarIDValue(const uint32_t &id, const Data &data)
{
    // Nao encontrou o ID
    if(variable_id.find(id) == variable_id.end()) return false;

    // Armazena o dado na posicao
    *(variable_id[id]) = data;

    return true;
}