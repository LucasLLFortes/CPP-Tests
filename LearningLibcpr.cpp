// LearningLibcpr
// compile With:
// g++ -Wall -pthread -std=c++17 -L ./cpr/build/lib -lcpr -lcurl -o LearningLibcpr LearningLibcpr.cpp

#include <iostream>
#include <cpr/cpr.h>

int main(int argc, char** argv) {
    cpr::Response r = cpr::Get(cpr::Url{"http://www.httpbin.org/get"});
    
    // r.status_code;                  // 200
    // r.header["content-type"];       // application/json; charset=utf-8
    // r.text;                         // JSON text string
    return 0;
}