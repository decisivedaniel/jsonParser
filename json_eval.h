#ifndef JSON_EVAL
#define JSON_EVAL

#include <iostream>
#include <fstream>
#include <string.h>
#include <unordered_map>
#include <memory>

namespace Json {

enum class node_type : char {
    value,
    array,
    object
};

class Node {
    private:
        std::string key;
        const node_type type;
    public:
        Node(const std::string &k, node_type t) : key(k), type(t) {}
        node_type GetType() const {return type;}
};

class Value : public Node {
    private:
        std::string value;
    public:
        Value(const std::string &k, const std::string &v) : Node(k, node_type::value), value(v) {}
        std::string asString() const {return value;}

};

class Array : public Node {
    private:
        std::unordered_map<int, Node> values;
    public:
        Array(const std::string &k, const std::string &v) :
            Node(k, node_type::array) {}
        const Array &operator[](int index);
};

class Object : public Node {
    private:
        std::unordered_map<std::string, Node> values;
    public:
        Object(const std::string &k, const std::string &v) : 
            Node(k, node_type::object) {}
        const Object &operator[](const std::string &key);
};

Node readJson(const std::string &filename);

std::string eval(Node n, std::string arg);


}

#endif
