/**
 *  ExpressionParserExtras.h
 *
 *  Copyright(c) 2020 ATI - Felipe Magalhaes
 *  All rights reserved.
 *
 ****************************************************************************
 
 *****************************************************************************/

#pragma once

#include <stack>
#include <string>
#include <vector>
#include <memory>

#include "MathOperator.h"

/*

    Le uma expressao matematica e indica se esta dentro dos limites dados

Exemplos: 
    $v<30 => numero menor que 30
    $v>200 && $v<500 => numero entre 200 e 500
    $v=1000 => numero igual a 1000
    ($v>100 && $v<357) || ($v < 800 && $v > 679) => 
        numero maior que 100 e menor que 357 OU menor que 800 e maior que 679
    $v>10 || $v < -2 => numero maior que 10 ou menor que -2
*/

class ExpressionParser : public MathOperator {


    /////////////////////////////////////////////////////////////////////
    // Construtor/Destrutor
    /////////////////////////////////////////////////////////////////////
public:
    ExpressionParser() = delete;
    // Construtor da classe
    ExpressionParser(char *c);
    ExpressionParser(std::string expression);
    virtual ~ExpressionParser() {};

    /////////////////////////////////////////////////////////////////////
    // METODOS PUBLICOS
    /////////////////////////////////////////////////////////////////////
public:

    // Indica se a expressao dada e valida
    bool validExpression();

    // Calcula um valor e o retorna
    bool calculateValue(double &result); 
    bool calculateValue(const double data, double &result); 

    // Avalia se uma expressao e verdadeira ou falsa e retorna o resultado
    bool evaluateValue(const double data, bool &result); 

    // Armazena onde foi encontrado um erro
    std::string error;

    /////////////////////////////////////////////////////////////////////
    // METODOS PRIVADOS
    /////////////////////////////////////////////////////////////////////
private:

    // Realiza o parsing
    void doParsing(std::string expression);
    void doParsing(char *c);
    
    //////////////////////////
    // Verificadores de caracteres
    //////////////////////////

    // Pula espacos e avanca a posicao
    void skipSpaces(char* &c);

    // Variavel
    bool isVariable(char* &c);

    // Variavel com ID
    bool isVariableID(char* &c, uint32_t &id);

    #ifdef IS_SSIT_APPLICATION
    bool isVariableIntToFloat(char* &c);

    bool isVariableIDIntToFloat(char* &c, uint32_t &id);
    #endif

    // Digito
    bool isNumber(char* &c, double &num, const char &lastChar='(');

    // Digito sem sinal
    bool isUnsNaturalNumber(char* &c, uint32_t &num);

    // Hexadecimal
    bool isHexadecimal(char* &c, double &num);

    // Verifica se e um digito valido de um numero hexadecimal
    bool isHexDigit(const char &c);

    // Binary
    bool isBinary(char* &c, double &num);

    // Verifica se e um digito valido de um numero binario
    bool isBinDigit(const char &c);

    //////////////////////////
    // Operacoes
    //////////////////////////

    // Verifica um operador ternario e retorna se foi possivel operar
    bool verifyTernaryOperator(char* &c);

    /////////////////////////////////////////////////////////////////////
    // VARIAVEIS PRIVADAS
    /////////////////////////////////////////////////////////////////////
private:

    //////////////////////////
    // Geral
    //////////////////////////

    // Indica se a expressao dada e valida
    bool expressionIsValid;
    
    // Resultado da expressao
    double result;

    // Condicoes e operacoes para operadores ternarios
    std::shared_ptr<ExpressionParser> cond, op1, op2;

};