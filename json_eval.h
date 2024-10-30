#ifndef JSON_EVAL
#define JSON_EVAL

#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <unordered_map>
#include <exception>
#include <memory>


class JsonBase {
    public:
        virtual std::string eval(std::string parameter) = 0;
        virtual int sizeOf(std::string parameter) = 0;
        virtual int min(std::string parameter) = 0;
        virtual int max(std::string parameter) = 0;
        virtual ~JsonBase();
};

class JsonObject : public JsonBase {
    private:
        std::string raw;
        std::unordered_map<std::string, std::unique_ptr<JsonBase>> properties;
        int size;
    public:
        explicit JsonObject(std::ifstream *file);
        std::string eval(std::string parameter) override;
        int sizeOf(std::string parameter) override;
        int min(std::string parameter) override;
        int max(std::string parameter) override;
        ~JsonObject() override;
};

class JsonArray : public JsonBase {
    private:
        std::string raw;
        std::unordered_map<int, std::unique_ptr<JsonBase>> properties;
        int size;
    public:
        explicit JsonArray(std::ifstream *file);
        std::string eval(std::string parameter) override;
        int sizeOf(std::string parameter) override;
        int min(std::string parameter) override;
        int max(std::string parameter) override;
        ~JsonArray() override;
};

class JsonString : public JsonBase {
    private:
        std::string raw;
        int size;
    public:
        explicit JsonString(std::ifstream *file);
        std::string eval(std::string parameter) override;
        int sizeOf(std::string parameter) override;
        int min(std::string parameter) override;
        int max(std::string parameter) override;
        ~JsonString() override;
};

class JsonNumber : public JsonBase {
    private:
        std::string raw;
        float number;
        int size = 1;
    public:
        explicit JsonNumber(std::ifstream *file);
        std::string eval(std::string parameter) override;
        int sizeOf(std::string parameter) override;
        int min(std::string parameter) override;
        int max(std::string parameter) override;
        ~JsonNumber() override;
};

class JsonFactory {
    public:
        std::unique_ptr<JsonBase> readJson(std::ifstream *file); 
};

class InvalidJson : public std::exception {
    private:
        const char * message;
    public:
        explicit InvalidJson(const char* msg) : message(msg) {}
        const char* what() const throw() {
            return message;
        }
};



#endif
