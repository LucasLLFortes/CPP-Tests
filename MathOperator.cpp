#include "MathOperator.h"

#include <algorithm>
#include <cmath>

/////////////////////////////////////////////////////////////////////
// Operadores
/////////////////////////////////////////////////////////////////////

/**
 * @brief Construtor da classe
 * 
 */
MathOperator::MathOperator() : MathExpression(0.0)
{

    OperatorInfo op;
    std::unordered_map<std::string, bool> left;

    // Precedencia baseada em:
    // https://en.cppreference.com/w/cpp/language/operator_precedence

    // LOGICAL NOT
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "! "; return !(a) ? 1 : 0; };
    #else
    op.f = [](const double& a, const double& b) -> double{ return !(a) ? 1 : 0; };
    #endif
    op.precedence = 3;
    op.binaryOp = false;
    map_op[std::string("!")] = op;
    left[std::string("!")] = false;

    // POW
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "^ "; return (std::pow(a, b)); };
    #else
    op.f = [](const double& a, const double& b) -> double{return (std::pow(a, b)); };
    #endif
    op.f = [](const double& a, const double& b) -> double{ return (std::pow(a, b)); };
    op.precedence = 4;
    op.binaryOp = true;
    map_op[std::string("**")] = op;
    left[std::string("**")] = true;

    // MULT
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "* "; return (a * b); };
    #else
    op.f = [](const double& a, const double& b) -> double{return (a * b); };
    #endif
    op.precedence = 5;
    op.binaryOp = true;
    map_op[std::string("*")] = op;
    left[std::string("*")] = true;

    // DIV
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "/ "; return (a / b); };
    #else
    op.f = [](const double& a, const double& b) -> double{return (a / b); };
    #endif
    op.precedence = 5;
    op.binaryOp = true;
    map_op[std::string("/")] = op;
    left[std::string("/")] = true;

    // REMAINDER
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "% "; return (double)((uint64_t)a % (uint64_t)b); };
    #else
    op.f = [](const double& a, const double& b) -> double{return ((uint64_t)a % (uint64_t)b); };
    #endif
    op.precedence = 5;
    op.binaryOp = true;
    map_op[std::string("%")] = op;
    left[std::string("%")] = true;


    // SUM
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "+ "; return (a + b); };
    #else
    op.f = [](const double& a, const double& b) -> double{return (a + b); };
    #endif
    op.precedence = 6;
    op.binaryOp = true;
    map_op[std::string("+")] = op;
    left[std::string("+")] = true;

    // SUB
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "- "; return (a - b); };
    #else
    op.f = [](const double& a, const double& b) -> double{return (a - b); };
    #endif
    op.precedence = 6;
    op.binaryOp = true;
    map_op[std::string("-")] = op;
    left[std::string("-")] = true;

    // LEFT SHIFT
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "<< "; return (double)((uint64_t)a << (uint64_t)b); };
    #else
    op.f = [](const double& a, const double& b) -> double{ return (double)((uint64_t)a << (uint64_t)b); };
    #endif
    op.precedence = 7;
    op.binaryOp = true;
    map_op[std::string("<<")] = op;
    left[std::string("<<")] = true;

    // RIGHT SHIFT
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << ">> "; return (double)((uint64_t)a >> (uint64_t)b); };
    #else
    op.f = [](const double& a, const double& b) -> double{ return (double)((uint64_t)a >> (uint64_t)b); };
    #endif
    op.precedence = 7;
    op.binaryOp = true;
    map_op[std::string(">>")] = op;
    left[std::string(">>")] = true;

    // HIGHER
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "> "; return (a > b) ? 1 : 0; };
    #else
    op.f = [](const double& a, const double& b) -> double{ return (a > b) ? 1 : 0; };
    #endif
    op.precedence = 9;
    op.binaryOp = true;
    map_op[std::string(">")] = op;
    left[std::string(">")] = true;

    // HIGHER OR EQUAL
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << ">= "; return (a >= b) ? 1 : 0; };
    #else
    op.f = [](const double& a, const double& b) -> double{ return (a >= b) ? 1 : 0; };
    #endif
    op.precedence = 9;
    op.binaryOp = true;
    map_op[std::string(">=")] = op;
    left[std::string(">=")] = true;

    // LOWER
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "< "; return (a < b) ? 1 : 0; };
    #else
    op.f = [](const double& a, const double& b) -> double{ return (a < b) ? 1 : 0; };
    #endif
    op.precedence = 9;
    op.binaryOp = true;
    map_op[std::string("<")] = op;
    left[std::string("<")] = true;

    // LOWER OR EQUAL
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "<= "; return (a <= b) ? 1 : 0; };
    #else
    op.f = [](const double& a, const double& b) -> double{ return (a <= b) ? 1 : 0; };
    #endif
    op.precedence = 9;
    op.binaryOp = true;
    map_op[std::string("<=")] = op;
    left[std::string("<=")] = true;

    // EQUAL
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "== "; return (a == b) ? 1 : 0; };
    #else
    op.f = [](const double& a, const double& b) -> double{ return (a == b) ? 1 : 0; };
    #endif
    op.precedence = 10;
    op.binaryOp = true;
    map_op[std::string("==")] = op;
    left[std::string("==")] = true;

    // NOT EQUAL
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "!= "; return (a != b) ? 1 : 0; };
    #else
    op.f = [](const double& a, const double& b) -> double{ return (a != b) ? 1 : 0; };
    #endif
    op.precedence = 10;
    op.binaryOp = true;
    map_op[std::string("!=")] = op;
    left[std::string("!=")] = true;

    // AND
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "& "; return ((uint64_t)a & (uint64_t)b); };
    #else
    op.f = [](const double& a, const double& b) -> double{ return ((uint64_t)a & (uint64_t)b); };
    #endif
    op.precedence = 11;
    op.binaryOp = true;
    map_op[std::string("&")] = op;
    left[std::string("&")] = true;

    // XOR
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "^ "; return ((uint64_t)a ^ (uint64_t)b); };
    #else
    op.f = [](const double& a, const double& b) -> double{ return ((uint64_t)a ^ (uint64_t)b); };
    #endif
    op.precedence = 12;
    op.binaryOp = true;
    map_op[std::string("^")] = op;
    left[std::string("^")] = true;

    // OR
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "| "; return ((uint64_t)a | (uint64_t)b); };
    #else
    op.f = [](const double& a, const double& b) -> double{ return ((uint64_t)a | (uint64_t)b); };
    #endif
    op.precedence = 13;
    op.binaryOp = true;
    map_op[std::string("|")] = op;
    left[std::string("|")] = true;

    // LOGICAL AND
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "&& "; return (a && b) ? 1 : 0; };
    #else
    op.f = [](const double& a, const double& b) -> double{ return (a && b) ? 1 : 0; };
    #endif
    op.precedence = 14;
    op.binaryOp = true;
    map_op[std::string("&&")] = op;
    left[std::string("&&")] = true;

    // LOGICAL_OR
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "|| "; return (a || b) ? 1 : 0; };
    #else
    op.f = [](const double& a, const double& b) -> double{ return (a || b) ? 1 : 0; };
    #endif
    op.precedence = 15;
    op.binaryOp = true;
    map_op[std::string("||")] = op;
    left[std::string("||")] = true;


    // OPERADORES INTERNOS
    // Os operadores aqui sao usados internamente e nao devem ser dados pelo usuario.
    // O ExpressionParser verifica casos especiais que geram esses operadores
    // Os operadores sempre estao na forma §Numero

    // Reinterpretacao de Int para Float
    #ifdef IS_SSIT_APPLICATION
    #ifdef PRINT_SY
    op.f = [](const double& a, const double& b) -> double{ std::cout << "ReinterpItF "; return interpIntToFloat(a); };
    #else
    op.f = [](const double& a, const double& b) -> double{ return interpIntToFloat(a); };
    #endif
    op.precedence = 1;
    op.binaryOp = false;
    map_op[std::string("\a0")] = op;
    left[std::string("\a0")] = false;
    #endif

    // Registra os operadores
    for(auto &m : map_op) {
        auto& op = m.second;
        registerOperatorInfo(op.f, m.first, op.precedence, op.binaryOp, left[m.first]);
    }

    for(auto& key : map_op) voperators.push_back(key.first);

    // Ordena o vetor de forma descrescente em ordem de tamanho
    std::sort(voperators.rbegin(), voperators.rend(),
        [](const std::string& first, const std::string& second) {
            return first.size() < second.size();
        }
    );

    // Apos realizar o sort, o vetor pode alocar mais espaco
    // Isso impede que seja lido caracteres inexistentes
    voperators.resize(map_op.size());

}

MathOperator::~MathOperator(){}

/**
 * @brief Indica se encontra um operador no comeco da string
 * 
 * @param c String lida
 * @param len Tamanho do operador lido
 * @return true Encontrou
 * @return false Nao encontrou
 */
bool MathOperator::isOperator(char* &c, size_t &len)
{

    // Parenteses
    if(c[0] == '(' || c[0] == ')') {
        len = 1;
        return true;
    }
    // Nao existe operador registrado
    if(!voperators.size()) {
        len = 0;
        return false;
    }

    // String igual a um operador
    bool equal;

    // Maximo tamanho de leitura da string
    size_t maxlen;

    // O primeiro operador e o de maior tamanho (ordem decrescente),
    // verificamos se ele nao estrapola o tamanho da string
    auto& first = voperators[0];
    for(maxlen = 0; c[maxlen] != '\0' && maxlen <= first.size(); maxlen++);

    // O vetor esta armazenado de forma decrescente, dessa forma,
    // Os operadores com mais caracteres como <= sao analisados antes de <
    // O que garante a leitura correta
    for(auto &v : voperators) {

        // Operador maior que a string lida
        if(maxlen <= v.size()) continue;

        equal = true;

        // Verifica cada caractere da string
        for(len = 0; len < v.size(); len++) {
            if(c[len] != v[len]) {
                equal = false;
                break;
            }
        }
        
        // Todos caracteres sao iguais, retorna verdadeiro
        if(equal) {
            return true;
        }
    }

    // Nenhum operador equivale ao inicio da string dada
    len = 0;
    return false;
}

#ifdef IS_SSIT_APPLICATION
/**
 * @brief Conversao de Int para Float em caso especial do SSIT
 * 
 * @param num Numero a ser convertido
 * @return double Resultado da conversao
 */
inline double interpIntToFloat(const double &num)
{

    /*

        Uma conversao no servidor foi requisitada para a seguinte situação:

        Um caso especial de dados recebidos diferentes de OBJ_TYPE_ANALOG_INPUT e OBJ_TYPE_ANALOG_OUTPUT 
        devem ser interpretados como FLOAT, porém de forma generica o servidor esta interpretando como INT
        O servidor sempre armazena no banco como DOUBLE

        Na hora de ler no ExpressionParser, quando ler o simbolo $fv, o servidor vai entender que a variavel
        esta nessa situação. O que deverá ser feito são os seguintes passos:

        1° CONVERTER de double para INT64. Note que não existe nenhum erro de conversão nessa situação.
        Um double só tem problemas de precisão com inteiros a partir de 2^53+1. O dado original tem
        32 bits

        2º CONVERTER de INT64 para INT32

        3° O valor obtido deve ser REINTERPRETADO como float

        4° CONVERTER de float para double

    */

   int64_t double_to_int64 = num;

   int32_t int64_to_int32 = static_cast<int32_t>(double_to_int64);

   float* int32_to_float = reinterpret_cast<float*>(&int64_to_int32);

   double result = *int32_to_float;

   return result;

}
#endif