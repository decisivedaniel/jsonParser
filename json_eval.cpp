#include "json_eval.h"

int main(int argc, char** argv) {
    std::string filename(argv[0]);
    std::string argument(argv[1]);

    std::ifstream JsonFile(filename);

    JsonFactory factory;

    std::unique_ptr<JsonBase> json = factory.readJson(&JsonFile);

    std::cout << json->eval(argument);

    JsonFile.close();

    return 0;
}

char getNextChar(std::ifstream *file, std::stringstream *str) {
    char c;
    do {
        file->get(c);
        if (file->eof()) {
            throw std::out_of_range("End of file");
        }
        str->put(c);
    } while (c != ' ');
    return c;
}


 JsonObject::JsonObject(std::ifstream *file) {
    // read to }
    std::stringstream obj("{");
    char current;
    do {
        current = getNextChar(file, &obj);
        switch (current) {
            case '}':
                //end of json object
                break;
            case '"':
                //get property
                do {
                    current = getNextChar(file, &obj);
                    if (current == ':') {
                        throw InvalidJson("Invalid json missing found :");
                    }
                } while (current != '"');

                //json factory for next object
                break;
            default:
                throw InvalidJson("Invalid json missing \"");

        } 
    } while (current != '}');
    
    raw.assign(obj.str());
};
        

std::string JsonObject::eval(std::string parameter) {
    return "";
};
int JsonObject::sizeOf(std::string parameter) {
    return 0;
};
int JsonObject::min(std::string parameter) {
    return 0;
};
int JsonObject::max(std::string parameter) {
    return 0;
};

JsonArray::JsonArray(std::ifstream *file) {

};
std::string JsonArray::eval(std::string parameter) {
    return "";
};
int JsonArray::sizeOf(std::string parameter) {
    return 0;
};
int JsonArray::min(std::string parameter) {
    return 0;
};
int JsonArray::max(std::string parameter) {
    return 0;
};

JsonString::JsonString(std::ifstream *file) {

};
std::string JsonString::eval(std::string parameter) {
    return "";
};
int JsonString::sizeOf(std::string parameter) {
    return 0;
};
int JsonString::min(std::string parameter) {
    return 0;
};
int JsonString::max(std::string parameter) {
    return 0;
};

JsonNumber::JsonNumber(std::ifstream *file) {
    // read to whitespace
    return;
};
std::string JsonNumber::eval(std::string parameter) {
    return "";
};
int JsonNumber::sizeOf(std::string parameter) {
    return 0;
};
int JsonNumber::min(std::string parameter) {
    return 0;
};
int JsonNumber::max(std::string parameter) {
    return 0;
};


std::unique_ptr<JsonBase> JsonFactory::readJson(std::ifstream *file) {
    std::unique_ptr<JsonBase> base;
    char current;
    do {
        file->get(current);
    } while (current != ' ');
    switch (current) {
        case '[':
            base = std::make_unique<JsonArray>(file);
            break;
        case '{':
            base = std::make_unique<JsonObject>(file);
            break;
        case '"':
            base = std::make_unique<JsonString>(file);
            break;
        default:
            file->putback(current);
            base = std::make_unique<JsonNumber>(file);
    }
    return base; 
}; 
