// compile With:
// g++ -Wall -pthread -std=c++17 -o LearningExpressionParser LearningExpressionParser.cpp ExpressionParser.cpp MathExpression.cpp MathOperator.cpp


#include "ExpressionParser.h"
#include <iostream>

int main() {

    ExpressionParser e(std::string("$v > 100 && $v < 200"));

    double result, result2;
    int count = 0; 
    if(e.calculateValue(result)) std::cout << "Resultado " << ++count << ": " << result << "\n";

    if(e.calculateValue(150, result)) std::cout << "Resultado " << ++count << ": " << result << "\n";

    e = ExpressionParser(std::string("65+ID1 > 100-30 && ID2-54 < 200+78"));
    e.setVarIDValue(1, 150);
    e.setVarIDValue(2, 500);

    if(e.calculateValue(result)) std::cout << "Resultado " << ++count << ": " << result << "\n";

    e.setVarIDValue(2, 199);


    // e = ExpressionParser(std::string("(ID4 == 150) ? 150 : 0"));
    e = ExpressionParser(std::string("(ID4 == 0x000F)*ID4"));
    e.setVarIDValue(4, 15);

    if(e.calculateValue(0,result)) std::cout << "Resultado ESP-" << ++count << ": " << result << "\n";

    e.setVarIDValue(4, 1);

    if(e.calculateValue(0,result2)) std::cout << "Resultado ESP-" << ++count << ": " << result2 << "\n";

    // e = ExpressionParser(std::string("(65+ID1 > 100-30"));
    // e.setVarIDValue(1, 150);

    // if(e.calculateValue(result)) std::cout << "Resultado ESP-" << ++count << ": " << result << "\n";

    e = ExpressionParser(std::string("49@SAGA 3 afsafa "));

    if(e.calculateValue(result)) {
        std::cout << "Resultado " << ++count << ": " << result << "\n";
    }
    else {
        std::cout << "ERRO:" << e.error << "\n";
    }

    e = ExpressionParser(std::string("$v ? 1 : 0"));

    if(e.calculateValue(10, result)) std::cout << "Resultado " << ++count << ": " << result << "\n";

    if(e.calculateValue(0, result)) std::cout << "Resultado " << ++count << ": " << result << "\n";

    if(e.calculateValue(-1, result)) std::cout << "Resultado " << ++count << ": " << result << "\n";

    return 0;
}

