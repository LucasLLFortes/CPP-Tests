// LearningEnvVariables
// compile With:
// g++ -Wall -pthread -std=c++11 -o LearningFileDataStream LearningFileDataStream.cpp


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

int main()
{
    std::cout << "Hello, lets try writing to a file!\n";

    // std::string path(std::getenv("PATH"));
    std::string home = std::getenv("HOME");

    //Exemplo variavel que nao existe
    std::string path = "";
    if(std::getenv("PATH") != NULL)  {path = std::getenv("PATH");}
        
    if(path.size() == 0){
        std::cout << "Not Found\n";
        exit(0);
    }

        // MethodInfo_YYYYMMDD
    std::string file_name = "WriteEnvVar.txt";
    std::cout << "File name: " << file_name << "\n\n";

    // Arquivo de leitura
    std::ofstream file(file_name.c_str(), std::ios_base::trunc | std::ios_base::out);

    std::cout << "Written to file: \n";
    std::cout << "Environment Variables:" << "\n";
    std::cout << "$HOME=" << home << "\n";
    std::cout << "$PATH=" << path << "\n";

    file << "Environment Variables:" << "\n";
    file << "$HOME=" << home << "\n";
    file << "$PATH=" << path << "\n";
}