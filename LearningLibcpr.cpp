// LearningLibcpr

// compile With:
// cmake -Bbuild-cmake -H. -GNinja

#include <iostream>
#include <cpr/cpr.h>

int main(int argc, char** argv) {
    cpr::Response r = cpr::Get(cpr::Url{"http://www.httpbin.org/get"});
    
    // r.status_code;                  // 200
    // r.header["content-type"];       // application/json; charset=utf-8
    // r.text;                         // JSON text string
    return 0;
}