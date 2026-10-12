#pragma once

#include "Parser.hpp"
#include <unordered_map>
#include <string>
#include <variant>
#include <sstream>
#include <stdexcept>

// VodScript'in çalışma anındaki dinamik değer tipleri
struct Value {
    using ValVariant = std::variant<std::monostate, double, std::string, bool>;
    ValVariant data;

    Value() : data(std::monostate{}) {}
    Value(double v) : data(v) {}
    Value(std::string v) : data(std::move(v)) {}
    Value(const char* v) : data(std::string(v)) {}
    Value(bool v) : data(v) {}

    bool is_number() const { return std::holds_alternative<double>(data); }
    bool is_string() const { return std::holds_alternative<std::string>(data); }
    bool is_bool() const { return std::holds_alternative<bool>(data); }

    double as_number() const { return std::get<double>(data); }
    const std::string& as_string() const { return std::get<std::string>(data); }
    bool as_bool() const { return std::get<bool>(data); }

    // Değeri doğrudan standart JSON metnine dönüştürür
    std::string to_json() const {
        if (is_number()) {
            std::ostringstream ss;
            ss << as_number();
            return ss.str();
        }
        if (is_string()) {
            return "\"" + as_string() + "\"";
        }
        if (is_bool()) {
            return as_bool() ? "true" : "false";
        }
        return "null";
    }
};

class Evaluator {
public:
    std::string evaluate(const ProgramNode* program) {
        std::string json_output = "null";

        for (const auto& stmt : program->statements) {
            if (stmt->type() == NodeType::LET_STMT) {
                auto let_stmt = static_cast<const LetStmtNode*>(stmt.get());
                environment_[let_stmt->name] = eval_expression(let_stmt->initializer.get());
            } else if (stmt->type() == NodeType::EXPORT_STMT) {
                auto export_stmt = static_cast<const ExportStmtNode*>(stmt.get());
                json_output = eval_to_json(export_stmt->expression.get());
            }
        }

        return json_output;
    }

private:
    std::unordered_map<std::string, Value> environment_;

    Value eval_expression(const ASTNode* node) {
        switch (node->type()) {
            case NodeType::NUMBER_LITERAL: {
                auto n = static_cast<const NumberNode*>(node);
                return Value(n->value);
            }
            case NodeType::STRING_LITERAL: {
                auto s = static_cast<const StringNode*>(node);
                return Value(s->value);
            }
            case NodeType::BOOLEAN_LITERAL: {
                auto b = static_cast<const BoolNode*>(node);
                return Value(b->value);
            }
            case NodeType::IDENTIFIER: {
                auto id = static_cast<const IdentifierNode*>(node);
                auto it = environment_.find(id->name);
                if (it == environment_.end()) {
                    throw std::runtime_error("Tanimlanmamis degisken: " + id->name);
                }
                return it->second;
            }
            case NodeType::BINARY_EXPR: {
                auto b = static_cast<const BinaryExprNode*>(node);
                Value left = eval_expression(b->left.get());
                Value right = eval_expression(b->right.get());

                if (left.is_number() && right.is_number()) {
                    double l = left.as_number();
                    double r = right.as_number();
                    switch (b->op) {
                        case TokenType::PLUS:  return Value(l + r);
                        case TokenType::MINUS: return Value(l - r);
                        case TokenType::STAR:  return Value(l * r);
                        case TokenType::SLASH: {
                            if (r == 0.0) throw std::runtime_error("Sifira bolme hatasi.");
                            return Value(l / r);
                        }
                        default: break;
                    }
                }
                throw std::runtime_error("Uyumsuz tipler uzerinde matematiksel islem yapilamaz.");
            }
            default:
                throw std::runtime_error("Desteklenmeyen ifade dugumu.");
        }
    }

    std::string eval_to_json(const ASTNode* node) {
        if (node->type() == NodeType::OBJECT_LITERAL) {
            auto obj = static_cast<const ObjectNode*>(node);
            std::string json = "{\n";
            for (size_t i = 0; i < obj->fields.size(); ++i) {
                const auto& [key, val_node] = obj->fields[i];
                json += "  \"" + key + "\": ";

                if (val_node->type() == NodeType::OBJECT_LITERAL) {
                    json += eval_to_json(val_node.get());
                } else {
                    json += eval_expression(val_node.get()).to_json();
                }

                if (i + 1 < obj->fields.size()) json += ",";
                json += "\n";
            }
            json += "}";
            return json;
        }

        return eval_expression(node).to_json();
    }
};