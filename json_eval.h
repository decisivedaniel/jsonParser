#ifndef JSON_EVAL
#define JSON_EVAL

#include <iostream>
#include <fstream>
#include <string.h>
#include <unordered_map>

class JsonBase {
    public:
        virtual std::string eval(std::string *parameter);
        virtual int size(std::string *parameter);
        virtual int min(std::string *parameter);
        virtual int max(std::string *parameter);
        virtual ~JsonBase();
};

class JsonObject : public JsonBase {

};

class JsonArray : public JsonBase {

};

class JsonString : public JsonBase {

};

class JsonNumber : public JsonBase {

};

class JsonFactory {
    public:
        JsonFactory();
        JsonBase readJson(std::ifstream *file); 
};



#endif
