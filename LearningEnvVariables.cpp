// LearningEnvVariables
// compile With:
// g++ -Wall -pthread -std=c++11 -o LearningEnvVariables LearningEnvVariables.cpp

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>

int main()
{
    std::cout << "Hello, lets try reading and setting some environment variables!\n";
    std::cout << "Lets read some common ones:\n\n";

    // std::string path(std::getenv("PATH"));
    std::string home = std::getenv("HOME");

    //Exemplo variavel que nao existe
    std::string path = "";
    if(std::getenv("PAH") != NULL){
        path = std::getenv("PAH");
    }
        
    if(path.size() == 0){
        std::cout << "$PAH: Not Found\n";
    }

    if(std::getenv("PATH") != NULL){
        path = std::getenv("PATH");
    }
        
    if(path.size() == 0){
        std::cout << "Not Found\n";
        exit(0);
    }

    std::cout << "\n$PATH: "     << path.c_str()     << std::endl;
    std::cout << "$HOME: "     << home.c_str()     << "\n" << std::endl;
    
    std::cout << "Now lets write other new ones and try reading them:\n";

    // setenv("VARIAVEL_LUCAS","moon",1);
    system("export VARIAVEL_LUCAS=moon");
    setenv("VARIAVEL_LUCAS2","/home/lucasfortessss",1);

    if(std::getenv("VARIAVEL_LUCAS") != NULL){
        std::string var1(std::getenv("VARIAVEL_LUCAS"));
        std::cout << "\n$VARIAVEL_LUCAS >> "    << var1.c_str()   << std::endl;
    }

    if(std::getenv("VARIAVEL_LUCAS2") != NULL){   
        std::string var2(std::getenv("VARIAVEL_LUCAS2"));
        std::cout << "$VARIAVEL_LUCAS2 >> "     << var2.c_str() << std::endl;
    }
}

// int main ()
// {
//   char* pPath;
//   pPath = getenv ("PATH");
//   if (pPath!=NULL)
//     printf ("The current path is: %s",pPath);
//   return 0;
// }