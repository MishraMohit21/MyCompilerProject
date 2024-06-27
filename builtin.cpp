#include "builtin.h"

void defineBuiltins(Environment::Ptr globalEnv) {
    globalEnv->def("time", [](const std::function<void()>& func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
    });

    globalEnv->def("println", [](const std::string& val) {
        std::cout << val << std::endl;
    });

    globalEnv->def("print", [](const std::string& val) {
        std::cout << val;
    });
}
