#include "ExpressionParser.h"
#include "MathOperator.h"

#include <cmath>
#include <cstring>
#include <iostream>

/**
 * @brief Construct a new Expression Parser:: Expression Parser object
 * 
 * @param c Expressao a ser lida
 */
ExpressionParser::ExpressionParser(char *c)
{
    doParsing(c);
}

/**
 * @brief Construct a new Expression Parser:: Expression Parser object
 * 
 * @param expression Expressao a ser lida
 */
ExpressionParser::ExpressionParser(std::string expression)
{
    doParsing(expression);
}

/**
 * @brief Realiza o parsing
 * 
 * @param expression String a ser lida
 */
void ExpressionParser::doParsing(std::string expression)
{
    // Copia a string para um vetor de caracteres
    char c[expression.size()+1];
    std::strcpy(c, expression.c_str());

    doParsing(c);
}

/**
 * @brief Realiza o parsing
 * 
 * @param c String a ser lida
 */
void ExpressionParser::doParsing(char *c)
{

    cond = nullptr;
    op1 = nullptr;
    op2 = nullptr;

    // Auxiliar de leitura
    char *caux = c;

    // Verifica uma operacao ternaria
    if(verifyTernaryOperator(caux)) {
        // Indica que a expressao e valida
        expressionIsValid = true;
        return;
    } 

    // Tamanho da leitura
    size_t len;
    double num;
    
    // Verifica se o ultimo caractere lido era um numero
    char lastChar = '(';

    // ID de uma variavel
    uint32_t id;

    expressionIsValid = true;

    while(caux[0] != '\0') {

        // Pula espacos
        skipSpaces(caux);

        #ifdef IS_SSIT_APPLICATION
        // Simbolo usado por operadores especiais. Nao pode ser usado por um usuario
        if(caux[0] == '\a') goto ERRO;
        #endif

        if(caux[0] == '\0') break;

        if(isVariable(caux)) {
            #ifdef PRINT_SY
            std::cout << "Variavel" << std::endl;
            #endif
            // O numero utilizado aqui nao importa, somente que e um numero
            lastChar = '0';
            registerVariable();
        }
        else if(isVariableID(caux, id)) {
            #ifdef PRINT_SY
            std::cout << "Variavel(ID" << id << ")" << std::endl;
            #endif
            // O numero utilizado realmente nao importa, somente que e um numero
            lastChar = '0';
            registerVariableID(id);
        }
        #ifdef IS_SSIT_APPLICATION
        else if(isVariableIntToFloat(caux)) {
            #ifdef PRINT_SY
            std::cout << "Variavel ItF" << std::endl;
            #endif
            // Registra uma operacao de converter reinterpretar Int para Float
            char copstr[3];
            copstr[0] = '\a';
            copstr[1] = '0';
            copstr[2] = '\0';
            registerOperation(std::string(copstr));
            // O numero utilizado aqui nao importa, somente que e um numero
            lastChar = '0';
            registerVariable();
        }
        else if(isVariableIDIntToFloat(caux, id)) {
            #ifdef PRINT_SY
            std::cout << "Variavel ItF(ID" << id << ")" << std::endl;
            #endif
            // Registra uma operacao de converter reinterpretar Int para Float
            char copstr[3];
            copstr[0] = '\a';
            copstr[1] = '0';
            copstr[2] = '\0';
            registerOperation(std::string(copstr));
            // O numero utilizado realmente nao importa, somente que e um numero
            lastChar = '0';
            registerVariableID(id);
        }
        #endif
        else if(isNumber(caux, num, lastChar)) {
            #ifdef PRINT_SY
            std::cout << "Numero: " << num << std::endl;
            #endif
            // O numero utilizado realmente nao importa, somente que e um numero
            lastChar = '0';
            registerData(num);
        }
        else {

            lastChar = caux[0];
        
            if(isOperator(caux, len)) {
                std::string s;
                s.assign(caux, len);
                #ifdef PRINT_SY
                std::cout << "Operador: " << s << std::endl;
                #endif
                if(!registerOperation(s)) goto ERRO;
                caux += len; 
            }
            else {
                ERRO:
                // ERRO DE LEITURA
                expressionIsValid = false;

                // Armazena o erro
                for(size_t i = 0; c+i != caux; i++) {
                    error+=c[i];
                }
                error+=caux[0];

                break;
            }
        }

    }

    if(expressionIsValid) {

        // Calcula a expressao para o valor padrao, para verificar se a expressao
        // e valida
        double result;
        if(!calculateValue(result)) expressionIsValid = false;
    }

}

/**
 * @brief Pula os espacos e atualiza a posicao da string
 * 
 * @param c String lida
 */
void ExpressionParser::skipSpaces(char* &c)
{

    // Pula os espacos e atualiza a posicao
    while(c[0] == ' ') c+=1;

}

/**
 * @brief Indica se o caractere e uma variavel
 * 
 * @param c String avaliada
 * @return true Variavel
 * @return false Nao e uma variavel
 */
bool ExpressionParser::isVariable(char* &c)
{
    if(c[0] != '$' || c[1] != 'v') return false;

    // Variavel do tipo $v

    // Atualiza a posicao
    c+=2;

    return true;
}

/**
 * @brief Verifica se e uma variavel do tipo ID
 * 
 * @param c String avaliada
 * @param id 
 * @return true 
 * @return false 
 */
bool ExpressionParser::isVariableID(char* &c, uint32_t &id)
{
    if(c[0] != 'I' || c[1] != 'D') return false;

    // Variavel do tipo ID123456789

    char *caux = c+2;

    // Nao tem numero apos ID, retorna falso
    if(!isUnsNaturalNumber(caux, id)) return false;

    // Atualiza a posicao da string
    c = caux;

    return true;
}


/**
 * @brief Verifica se e um numero e atualiza a posicao da string
 * 
 * @param c String lida
 * @param num Numero retornado
 * @param lastCharNum Indica se o ultimo caractere foi um numero
 * @return true Numero
 * @return false Nao e um numero
 */
bool ExpressionParser::isNumber(char* &c, double &num, const char &lastChar)
{

    // Caractere auxiliar de leitura
    char caux = c[0];

    // O caractere nao e um digito, entao retorna falso
    if(!isdigit(caux) && caux != '+' && caux != '-') return false;

    int len = 0;

    // Multiplicador 
    bool negative = false;

    // Verifica se e um sinal/operador
    if(caux == '+' || caux == '-') {

        // Temos um operador "+ " ou "- ", e nao um sinal
        if(lastChar == '0' || lastChar == ')' ) return false;

        // Indica que se deve inverter o sinal do numero apos ler o mesmo
        if((caux == '-')) negative = true;

        caux = c[++len];

    }

    // Checa se o numero e hexadecimal
    double numh;
    char *chex = c+len;
    if(isHexadecimal(chex, numh) || isBinary(chex, numh)) {
        if(negative) numh *= (-1);
        num = numh;

        c = chex;
        return true;
    }

    // Indica se o numero e decimal ou nao
    bool isDecimal = false;
    double div = 1;

    // O numero comeca como 0
    num = 0;

    do {

        // Registra a parte inteira do numero
        if(!isDecimal) {
            // O numero anterior registrado e multiplicado por 10 e soma o novo numero
            num *= 10;
            num += caux-'0';
        }
        // Registra a parte fracionaria
        else {
            // O divisor e aumentado por 10 e soma o novo numero dividido pelo divisor
            div *= 10;
            num += (caux-'0')/div;
        }

        caux = c[++len];

        // Verifica se e um ponto e se o mesmo ja foi lido antes
        if(caux == '.') {
            if(!isDecimal) {
                isDecimal = true;
                // Proximo digito
                caux = c[++len];

                // Se o proximo caractere nao for um ponto, tem erro
                if(!isdigit(caux)) return false;
            }
            // Ponto ja lido, erro
            else return false;
        }

    } while(isdigit(caux));

    // Numero negativo
    if(negative) num *= (-1);

    // Atualiza a posicao
    c += len;

    // Leitura do numero executado com sucesso
    return true;
}

/**
 * @brief Verifica se e um numero natural sem sinal e atualiza a posicao da string
 * 
 * @param c String lida
 * @param num Numero retornado
 * @return true Numero
 * @return false Nao e um numero
 */
bool ExpressionParser::isUnsNaturalNumber(char* &c, uint32_t &num)
{

    // Caractere auxiliar de leitura
    char caux = c[0];

    // O caractere nao e um digito, entao retorna falso
    if(!isdigit(caux)) return false;

    int len = 0;

    // O numero comeca como 0
    num = 0;

    do {

        // Registra a parte inteira do numero
        // O numero anterior registrado e multiplicado por 10 e soma o novo numero
        num *= 10;
        num += caux-'0';

        caux = c[++len];

    } while(isdigit(caux));

    // Atualiza a posicao
    c += len;

    // Leitura do numero executado com sucesso
    return true;
}

/**
 * @brief Verifica se a string e um hexadecimal
 * 
 * @param c String lida
 * @param num Numero retornado
 * @return true Hexadecimal
 * @return false Nao e hexadecimal
 */
bool ExpressionParser::isHexadecimal(char* &c, double &num) 
{

    // Nao e hexadecimal
    if(c[0] != '0' || c[1] != 'x' || !isHexDigit(c[2])) return false;

    // Pula o '0x'
    char caux = c[2];

    int len = 2;

    // O numero comeca como 0
    num = 0;

    do {

        // O numero anterior registrado e multiplicado por 16 e soma o novo numero
        num *= 16;
        // Faz a subtracao correta referente a tabela ASCII
        if(caux >= 'A' && caux <= 'F') num += caux-'A'+10;
        else if(caux >= 'a' && caux >= 'f') num += caux-'a'+10;
        else num += caux-'0';

        caux = c[++len];

    } while(isHexDigit(caux));

    // Atualiza a posicao
    c += len;

    // Numero hexadecimal
    return true;
}

/**
 * @brief Verifica se e um digito valido de um numero hexadecimal
 * 
 * @param c String lida
 * @return true E um digito hexadecimal
 * @return false Nao e um digito hexadecimal
 */
bool ExpressionParser::isHexDigit(const char &c) 
{
    if(isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) return true;
    return false;
}

/**
 * @brief Verifica se a string e um hexadecimal
 * 
 * @param c String lida
 * @param num Numero retornado
 * @return true Hexadecimal
 * @return false Nao e hexadecimal
 */
bool ExpressionParser::isBinary(char* &c, double &num) 
{

    // Nao e hexadecimal
    if(c[0] != '0' || c[1] != 'b' || !isBinDigit(c[2])) return false;

    // Pula o '0b'
    char caux = c[2];

    int len = 2;

    // O numero comeca como 0
    num = 0;

    do {

        // O numero anterior registrado e multiplicado por 2 e soma o novo numero
        num *= 2;
        num += caux;

        caux = c[++len];

    } while(isBinDigit(caux));

    // Atualiza a posicao
    c += len;

    // Numero hexadecimal
    return true;
}

/**
 * @brief Verifica se e um digito valido de um numero hexadecimal
 * 
 * @param c String lida
 * @return true E um digito hexadecimal
 * @return false Nao e um digito hexadecimal
 */
bool ExpressionParser::isBinDigit(const char &c) 
{
    if(c == 0 || c == 1) return true;
    return false;
}

/**
 * @brief Verifica um operador ternario
 * 
 * @param c String lida
 * @return true Operador ternario
 * @return false Nao e um operador ternario
 */
bool ExpressionParser::verifyTernaryOperator(char* &c)
{
    
    for(size_t i = 0; c[i] != '\0'; i++) {

        // Encontrou a operacao
        if(c[i] == '?') {

            // Numero de parenteses abertos
            long open_brackets = 0;
            char caux;

            for(size_t j = 1; c[i+j] != '\0'; j++) {

                caux = c[i+j];
                if(caux == '(') open_brackets++;
                else if(caux == ')') open_brackets--;
                // Encontrou o delimitador
                else if(caux == ':') {
                    // Todos parenteses tem que estar fechados para estar certo
                    if(open_brackets != 0) return false;

                    // Verifica se tem algo apos o delimitador
                    size_t k;
                    for(k = i+j+1; c[k] != '\0' && c[k] != ' '; k++);

                    // Nao encontrou nada apos o delimitador
                    if(c[k] == '\0') return false;

                    // Obtem o valor da expressao antes de '?'
                    cond = std::make_shared<ExpressionParser>(std::string(c, i));

                    // Expressao invalida
                    if(!cond->validExpression()) return false;

                    // Obtem o valor da primeira expressao
                    op1 = std::make_shared<ExpressionParser>(std::string(c+i+1, j-1));

                    // Expressao invalida
                    if(!op1->validExpression()) return false;

                    // Obtem o valor da segunda expressao
                    op2 = std::make_shared<ExpressionParser>(std::string(c+i+j+1));

                    // Expressao invalida
                    if(!op2->validExpression()) return false;

                    // Operacao encontrada
                    return true;
                }
            }

            break;
        }
    }

    // Nao encontrou a operacao
    return false;
}


/**
 * @brief Indica se a expressao dada e valida
 * 
 * @return true Valida
 * @return false Invalida
 */
bool ExpressionParser::validExpression()
{
    return expressionIsValid;
}

/**
 * @brief Calculo um valor na expressao
 * 
 * @param result Resultado do calculo
 * @return true Expressao valida
 * @return false Expressao invalida
 */
bool ExpressionParser::calculateValue(double &result)
{

    // Se a expressao nao for valida, retorna falso
    if(!validExpression()) return false;

    // Utiliza o valor padrao no calculo
    double data = default_value;

    bool r;

    // Se cond existir, indica uma expressao ternaria
    if(cond != nullptr) {
        cond->evaluateValue(data, r);

        // Avalia a resposta da condicao e calcula com base na resposta
        if(r) op1->calculateValue(data, result);
        else op2->calculateValue(data, result);
    }
    // Nao e uma expressao ternaria
    else {
        // Avalia para mudar o valor da variavel
        evaluate(data, r);
        // Calcula o resultado
        calculate(result);
    }

    return true;
}

/**
 * @brief Calculo um valor na expressao
 * 
 * @param data Valor a ser armazenado
 * @param result Resultado do calculo
 * @return true Expressao valida
 * @return false Expressao invalida
 */
bool ExpressionParser::calculateValue(const double data, double &result)
{

    // Se a expressao nao for valida, retorna falso
    if(!validExpression()) return false;

    bool r;

    // Se cond existir, indica uma expressao ternaria
    if(cond != nullptr) {
        cond->evaluateValue(data, r);

        // Avalia a resposta da condicao e calcula com base na resposta
        if(r) op1->calculateValue(data, result);
        else op2->calculateValue(data, result);
    }
    // Nao e uma expressao ternaria
    else {
        // Avalia para mudar o valor da variavel
        evaluate(data, r);
        // Calcula o resultado
        calculate(result);
    }

    return true;
}

/**
 * @brief Avalia se uma expressao e verdadeira ou falsa e retorna o resultado
 * 
 * @param data Valor a ser armazenado
 * @param result Resultado da expressao booleana
 * @return true Expressao valida
 * @return false Expressao invalida
 */
bool ExpressionParser::evaluateValue(const double data, bool &result)
{

    // Se a expressao nao for valida, retorna falso
    if(!validExpression()) return false;

    // Se cond existir, indica uma expressao ternaria
    if(cond != nullptr) {
        cond->evaluateValue(data, result);

        // Avalia a resposta da condicao e calcula com base na resposta
        if(result) op1->evaluateValue(data, result);
        else op2->evaluateValue(data, result);
    }
    // Nao e uma expressao ternaria
    else {
        evaluate(data, result);
    }

    return true;
}

#ifdef IS_SSIT_APPLICATION

/**
 * @brief Indica se o caractere e uma variavel com conversao de IntToFloat
 * 
 * @param c String avaliada
 * @return true Variavel
 * @return false Nao e uma variavel
 */
bool ExpressionParser::isVariableIntToFloat(char* &c)
{
    if(c[0] != '$' || c[1] != 'f' || c[2] != 'v') return false;

    // Variavel do tipo $fv

    // Atualiza a posicao
    c+=3;

    return true;
}

/**
 * @brief Verifica se e uma variavel do tipo ID  com conversao de IntToFloat
 * 
 * @param c String avaliada
 * @param id 
 * @return true 
 * @return false 
 */
bool ExpressionParser::isVariableIDIntToFloat(char* &c, uint32_t &id)
{
    if(c[0] != 'f' || c[1] != 'I' || c[2] != 'D') return false;

    // Variavel do tipo fID123456789

    char *caux = c+3;

    // Nao tem numero apos ID, retorna falso
    if(!isUnsNaturalNumber(caux, id)) return false;

    // Atualiza a posicao da string
    c = caux;

    return true;
}

#endif