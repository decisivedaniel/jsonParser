#include "json.h"


int main(int argc, char** argv) {
    std::string filename(argv[1]);
    std::string argument (argv[2]);

    std::shared_ptr<Json::Node> base = Json::readJson(filename);

    std::cout << Json::eval(base, argument) << std::endl;

    return 0;
}