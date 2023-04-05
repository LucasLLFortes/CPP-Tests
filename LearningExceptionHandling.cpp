// LearningExceptionHandling
// compile With:
// g++ -Wall -pthread -std=c++11 -o LearningExceptionHandling LearningExceptionHandling.cpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <string>
 
// using namespace std::string_literals;

int     layer0(bool forceExcept0, bool forceExcept1, bool forceExcept2);
int     layer1(int a, int b, bool forceExcept0, bool forceExcept1, bool forceExcept2);
void    layer2(int c, int d, bool forceExcept0, bool forceExcept1, bool forceExcept2);

int main() {
    
    // Normal
    std::cout << "-- No exception\n";    
    int val = -1;
    try{
        val = layer0(false, false, false);
        std::cout << "main() - Final - a + b = " << val << " \n";
    }catch(const std::exception& e){
        std::cout << "main() - Exception: " << e.what();
    }catch(...){ 
        std::cout << "main() - Unknown Exception";        
    }

    // Exception level 2
    std::cout << "\n\n-- Exception level 2\n";

    val = -1;
    try{
        val = layer0(false, false, true);
        std::cout << "main() - Final - a + b = " << val << " \n";
    }catch(const std::exception& e){
        std::cout << "main() - Exception: " << e.what();
    }catch(...){ 
        std::cout << "main() - Unknown Exception";        
    }

    // Exception Level 1
    std::cout << "\n\n-- Exception level 1\n";
    val = -1;
    try{
        val = layer0(false, true, false);
        std::cout << "main() - Final - a + b = " << val << " \n";
    }catch(const std::exception& e){
        std::cout << "main() - Exception: " << e.what();
    }catch(...){ 
        std::cout << "main() - Unknown Exception";        
    }

    // Exception Level 0
    std::cout << "\n\n-- Exception level 0\n";
    val = -1;
    try{
        val = layer0(true, false, false);
        std::cout << "main() - Final - a + b = " << val << " \n";
    }catch(const std::exception& e){
        std::cout << "main() - Exception: " << e.what();
    }catch(...){ 
        std::cout << "main() - Unknown Exception";        
    }

    exit(0);
}

int layer0(bool forceExcept0, bool forceExcept1, bool forceExcept2){

    int resp, val = -1;
    try{
        if(forceExcept0){
            throw 0;
        }

        val = layer1(1,1,forceExcept0,forceExcept1,forceExcept2);
        std::cout << "layer0() - a + b = " << val << " \n";

    
    }catch(const std::exception& e){
        std::cout << "layer0() - Exception: " << e.what();
        throw;

    }catch(...){ 
        std::cout << "layer0() - Exception caught\n";
    }
    
    resp = 10 * val;

    return resp;
}

int layer1(int a, int b, bool forceExcept0, bool forceExcept1, bool forceExcept2){

    int resp;
    try{

        if(forceExcept1){
            throw 0;
        }

        resp = a + b;
        std::cout << "layer1() - a + b = " << resp << " \n";
        layer2(4,5,forceExcept0,forceExcept1,forceExcept2);

    }catch(const std::exception& e){
        std::cout << "layer1() - Exception: " << e.what();
        throw std::runtime_error( "Erro durante execucao 1 (repassada por layer2() )\n");
        resp = -99; // Nao e executado (obviamente)

    }catch(...){ 

        std::cout << "layer1() - Exception: unknown\n";
        throw std::runtime_error( "Erro durante execucao 2\n");
        resp = -999; // Nao e executado (obviamente)
    }

    return resp;
}

void layer2(int c, int d, bool forceExcept0, bool forceExcept1, bool forceExcept2){

    try{

        if(forceExcept2){
            // throw 0;
            throw std::runtime_error("Erro durante execucao 1.1.1");
        }

        int vallayer2 = 12300 + 10*c + d;
        std::cout << "layer2() - 12300 + 10*c + d =" << vallayer2 << " \n";

    }catch(const std::exception& e){
        std::cout << "layer2() - Exception: " << e.what() << "\n";

        throw std::runtime_error( "Erro durante execucao 1.1\n");
        // throw std::invalid_argument( "Argumento invalido 1.1\n"); // Tambem poderia ser
    }
    catch(...){ 

        std::cout << "layer2() - Exception: unknown\n";
        
        // throw std::invalid_argument( "Argumento invalido 2.2.2\n");
        throw;
    }
}
