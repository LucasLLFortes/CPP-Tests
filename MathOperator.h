#pragma once

#include "MathExpression.h"
#include <string>

/////////////////////////////////////////////////////////////////////
// Operadores
/////////////////////////////////////////////////////////////////////

class MathOperator : public MathExpression<double, double, std::string>
{

public:

    // Informacao dos operadores
    typedef struct {

        std::function<double(const double&, const double&)> f;
        uint32_t precedence;
        bool binaryOp;

    } OperatorInfo;

    // Construtor/Destrutor
    MathOperator();
    virtual ~MathOperator();

    // Indica se encontra um operador no comeco da string
    bool isOperator(char* &c, size_t &len);

private:

    // Mapa contendo infomacoes dos operadores
    std::unordered_map<std::string, OperatorInfo> map_op;
    // Vetor indicando todos operadores (busca mais eficiente)
    std::vector<std::string> voperators;

};

////////////////////////////////////////////////////////////////////////////////
// OPERADORES ESPECIAIS DE APLICACAO
////////////////////////////////////////////////////////////////////////////////

#ifdef IS_SSIT_APPLICATION
// Reinterpreta um inteiro para float
double interpIntToFloat(const double &num);
#endif