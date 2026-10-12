#pragma once

#include "Lexer.hpp"
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <charconv>

// ==========================================
// 1. AST DÜĞÜM (NODE) YAPILARI
// ==========================================

enum class NodeType {
    NUMBER_LITERAL,
    STRING_LITERAL,
    BOOLEAN_LITERAL,
    IDENTIFIER,
    BINARY_EXPR,
    OBJECT_LITERAL,
    EXPORT_STMT,
    LET_STMT,
    PROGRAM
};

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual NodeType type() const = 0;
};

using NodePtr = std::unique_ptr<ASTNode>;

// Sayı Düğümü: 42, 3.14
struct NumberNode : public ASTNode {
    double value;
    explicit NumberNode(double val) : value(val) {}
    NodeType type() const override { return NodeType::NUMBER_LITERAL; }
};

// String Düğümü: "merhaba"
struct StringNode : public ASTNode {
    std::string value;
    explicit StringNode(std::string val) : value(std::move(val)) {}
    NodeType type() const override { return NodeType::STRING_LITERAL; }
};

// Boolean Düğümü: true, false
struct BoolNode : public ASTNode {
    bool value;
    explicit BoolNode(bool val) : value(val) {}
    NodeType type() const override { return NodeType::BOOLEAN_LITERAL; }
};

// Değişken Adı Düğümü: fiyat, oran
struct IdentifierNode : public ASTNode {
    std::string name;
    explicit IdentifierNode(std::string_view n) : name(n) {}
    NodeType type() const override { return NodeType::IDENTIFIER; }
};

// İkili İşlem Düğümü: sol + sağ, sol * sağ
struct BinaryExprNode : public ASTNode {
    NodePtr left;
    TokenType op;
    NodePtr right;

    BinaryExprNode(NodePtr l, TokenType o, NodePtr r)
        : left(std::move(l)), op(o), right(std::move(r)) {}

    NodeType type() const override { return NodeType::BINARY_EXPR; }
};

// JSON Benzeri Obje Düğümü: { anahtar: deger }
struct ObjectNode : public ASTNode {
    std::vector<std::pair<std::string, NodePtr>> fields;
    NodeType type() const override { return NodeType::OBJECT_LITERAL; }
};

// Değişken Tanımı: let x = 10;
struct LetStmtNode : public ASTNode {
    std::string name;
    NodePtr initializer;

    LetStmtNode(std::string_view n, NodePtr init)
        : name(n), initializer(std::move(init)) {}

    NodeType type() const override { return NodeType::LET_STMT; }
};

// Dışa Aktarım İfadesi: export { ... };
struct ExportStmtNode : public ASTNode {
    NodePtr expression;
    explicit ExportStmtNode(NodePtr expr) : expression(std::move(expr)) {}
    NodeType type() const override { return NodeType::EXPORT_STMT; }
};

// Kök Program Düğümü
struct ProgramNode : public ASTNode {
    std::vector<NodePtr> statements;
    NodeType type() const override { return NodeType::PROGRAM; }
};

// ==========================================
// 2. PARSER SINIFI
// ==========================================

class Parser {
public:
    explicit Parser(std::vector<Token> tokens)
        : tokens_(std::move(tokens)), cursor_(0) {}

    std::unique_ptr<ProgramNode> parse_program() {
        auto program = std::make_unique<ProgramNode>();

        while (!is_at_end()) {
            if (match(TokenType::LET)) {
                program->statements.push_back(parse_let_statement());
            } else if (match(TokenType::EXPORT)) {
                program->statements.push_back(parse_export_statement());
            } else {
                throw std::runtime_error("Beklenmeyen ifade: " + std::string(peek().lexeme));
            }
        }

        return program;
    }

private:
    std::vector<Token> tokens_;
    size_t cursor_;

    bool is_at_end() const {
        return peek().type == TokenType::END_OF_FILE;
    }

    const Token& peek() const {
        return tokens_[cursor_];
    }

    const Token& previous() const {
        return tokens_[cursor_ - 1];
    }

    const Token& advance() {
        if (!is_at_end()) cursor_++;
        return previous();
    }

    bool check(TokenType type) const {
        if (is_at_end()) return false;
        return peek().type == type;
    }

    bool match(TokenType type) {
        if (check(type)) {
            advance();
            return true;
        }
        return false;
    }

    const Token& consume(TokenType type, const std::string& err_msg) {
        if (check(type)) return advance();
        throw std::runtime_error("Satir " + std::to_string(peek().line) + ": " + err_msg + " (Gelen: '" + std::string(peek().lexeme) + "')");
    }

    // --- İfade Çözümleme (Operatör Önceliği Mantığı) ---

    // let degisken = ifade;
    NodePtr parse_let_statement() {
        Token id_token = consume(TokenType::IDENTIFIER, "Degisken ismi bekleniyordu.");
        consume(TokenType::ASSIGN, "'=' bekleniyordu.");
        NodePtr init = parse_expression();
        consume(TokenType::SEMICOLON, "Ifade sonunda ';' bekleniyordu.");
        return std::make_unique<LetStmtNode>(id_token.lexeme, std::move(init));
    }

    // export { ... };
    NodePtr parse_export_statement() {
        NodePtr expr = parse_expression();
        match(TokenType::SEMICOLON); // Noktalı virgül isteğe bağlı olsun
        return std::make_unique<ExportStmtNode>(std::move(expr));
    }

    // En geniş ifade: Toplama & Çıkarma (En düşük ikili öncelik)
    NodePtr parse_expression() {
        return parse_additive();
    }

    NodePtr parse_additive() {
        NodePtr left = parse_multiplicative();

        while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
            TokenType op = previous().type;
            NodePtr right = parse_multiplicative();
            left = std::make_unique<BinaryExprNode>(std::move(left), op, std::move(right));
        }

        return left;
    }

    // Çarpma & Bölme (Toplama/Çıkarmadan önce işlenir)
    NodePtr parse_multiplicative() {
        NodePtr left = parse_primary();

        while (match(TokenType::STAR) || match(TokenType::SLASH)) {
            TokenType op = previous().type;
            NodePtr right = parse_primary();
            left = std::make_unique<BinaryExprNode>(std::move(left), op, std::move(right));
        }

        return left;
    }

    // Temel Yapılar: Değişmezler, İsimler, Nesneler, Parantez içi
    NodePtr parse_primary() {
        if (match(TokenType::NUMBER)) {
            double val = 0.0;
            std::string text(previous().lexeme);
            val = std::stod(text);
            return std::make_unique<NumberNode>(val);
        }

        if (match(TokenType::STRING)) {
            // Tırnakları kırp: "abc" -> abc
            std::string_view raw = previous().lexeme;
            std::string unquoted(raw.substr(1, raw.size() - 2));
            return std::make_unique<StringNode>(unquoted);
        }

        if (match(TokenType::TRUE))  return std::make_unique<BoolNode>(true);
        if (match(TokenType::FALSE)) return std::make_unique<BoolNode>(false);

        if (match(TokenType::IDENTIFIER)) {
            return std::make_unique<IdentifierNode>(previous().lexeme);
        }

        if (match(TokenType::LPAREN)) {
            NodePtr expr = parse_expression();
            consume(TokenType::RPAREN, "Kapanis parantezi ')' bekleniyordu.");
            return expr;
        }

        if (match(TokenType::LBRACE)) {
            return parse_object();
        }

        throw std::runtime_error("Satir " + std::to_string(peek().line) + ": Gecersiz sozcük/ifade: " + std::string(peek().lexeme));
    }

    // { anahtar: deger, ... }
    NodePtr parse_object() {
        auto obj = std::make_unique<ObjectNode>();

        if (!check(TokenType::RBRACE)) {
            do {
                Token key = consume(TokenType::IDENTIFIER, "Obje anahtari (key) bekleniyordu.");
                consume(TokenType::COLON, "Anahtardan sonra ':' bekleniyordu.");
                NodePtr value = parse_expression();
                obj->fields.emplace_back(std::string(key.lexeme), std::move(value));
            } while (match(TokenType::COMMA));
        }

        consume(TokenType::RBRACE, "Obje sonunda '}' bekleniyordu.");
        return obj;
    }
};