#include "json_eval.h"


int main(int argc, char** argv) {
    std::string filename(argv[1]);
    std::string argument (argv[2]);

    std::cout << filename << std::endl;

    std::shared_ptr<Json::Node> json = Json::readJson(filename);

    //auto value = std::dynamic_pointer_cast<Json::Value>(json);

    std::cout << eval(json, argument);

    //JsonFile.close();

    return 0;
}

std::shared_ptr<Json::Node> Json::readJson(const std::string &filename) {
    std::ifstream jsonFile;
    jsonFile.open(filename);
    std::string line;
    //std::cout << filename << " is open: " << jsonFile.is_open() << std::endl;
    if (jsonFile.is_open()) {
        while (std::getline(jsonFile, line)) {
            std::cout << " - " << line << std::endl;
        }
    } 
    jsonFile.close();
    return std::make_shared<Json::Value>("test", "b");
}