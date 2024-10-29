#include "json_eval.h"
#include <iostream>


int main(int argc, char** argv) {
    std::string filename = argv[0];
    std::string argument = argv[1];

    std::ifstream JsonFile(filename);

    JsonBase json = JsonFactory().readJson(&JsonFile);

    std::cout << json.eval(&argument);

    JsonFile.close();

    return 0;
}