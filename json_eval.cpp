#include "json.h"


int main(int argc, char** argv) {
    std::string filename(argv[1]);
    std::string argument (argv[2]);

    std::cout << filename << std::endl;

    std::shared_ptr<Json::Node> json = Json::readJson(filename);

    std::cout << eval(json, argument);

    //JsonFile.close();

    return 0;
}