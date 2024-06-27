#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <functional>
#include <stdexcept>
#include <any>

// class Environment : public std::enable_shared_from_this<Environment> {
// public:
//     using Ptr = std::shared_ptr<Environment>;

//     Environment(Ptr parent = nullptr)
//         : vars(parent ? std::make_shared<std::unordered_map<std::string, std::any>>(*parent->vars)
//                       : std::make_shared<std::unordered_map<std::string, std::any>>()), parent(parent) {}

//     Ptr extend() {
//         return std::make_shared<Environment>(shared_from_this());
//     }

//     Ptr lookup(const std::string& name) {
//         Ptr scope = shared_from_this();
//         while (scope) {
//             if (scope->vars->find(name) != scope->vars->end())
//                 return scope;
//             scope = scope->parent;
//         }
//         return nullptr;
//     }

//     std::any get(const std::string& name) {
//         Ptr scope = lookup(name);
//         if (scope) {
//             return (*scope->vars)[name];
//         }
//         throw std::runtime_error("Undefined variable " + name);
//     }

//     void set(const std::string& name, const std::any& value) {
//         Ptr scope = lookup(name);
//         if (!scope && parent) {
//             throw std::runtime_error("Undefined variable " + name);
//         }
//         (*scope->vars)[name] = value;
//     }

//     void def(const std::string& name, const std::any& value) {
//         (*vars)[name] = value;
//     }

// private:
//     std::shared_ptr<std::unordered_map<std::string, std::any>> vars;
//     Ptr parent;
// };


class Environment {
private:

    std::shared_ptr<Environment> parent;
    std::unordered_map<std::string, std::any> var;  // map of var name and type

public:

    Environment(Environment* parent = nullptr) : parent (parent) {

        var = std::unordered_map<std::string, std::any> ();

    }
    
    Environment* extend() {

        return new Environment(this);

    }

    std::shared_ptr<Environment> lookup (const std::string& name){

        std::shared_ptr<Environment> scope(this);

        while (scope) {

            if (scope->var.find(name) != scope->var.end()) {                
                return scope;
            }
            else 
                scope = scope->parent;
        }
        return nullptr;
    }

    std::get any 

};