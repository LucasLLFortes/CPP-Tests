#include <iostream>
#include <cpr/cpr.h>

int main(int argc, char** argv) {
    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/whoshuu/cpr/contributors"},
                      cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
                      cpr::Parameters{{"anon", "true"}, {"key", "value"}});
    
    std::cout << "Status code:\n" << r.status_code << std::endl;                  // 200
    std::cout << "\nHeader:\n" << r.header["content-type"] << std::endl;     // application/json; charset=utf-8
    std::cout << "Content:\n"<< r.text << std::endl;                         // JSON text string

    return 0;
}