#pragma once

#include <string_view>
#include <vector>
#include <cctype>
#include <iostream>

// ==========================================
// 1. TOKEN TANIMLARI
// ==========================================

enum class TokenType {
    END_OF_FILE,
    INVALID,

    // Değişmezler & Tanımlayıcılar
    IDENTIFIER,   // x, toplam, oran
    NUMBER,       // 42, 3.14
    STRING,       // "metin"

    // Anahtar Kelimeler
    LET,          // let
    FN,           // fn
    RETURN,       // return
    EXPORT,       // export
    IMPORT,       // import
    TRUE,         // true
    FALSE,        // false
    NULL_TOKEN,   // null

    // Ayraçlar & Semboller
    LBRACE,       // {
    RBRACE,       // }
    LBRACKET,     // [
    RBRACKET,     // ]
    LPAREN,       // (
    RPAREN,       // )
    COMMA,        // ,
    COLON,        // :
    SEMICOLON,    // ;
    DOT,          // .

    // Operatörler
    ASSIGN,       // =
    PLUS,         // +
    MINUS,        // -
    STAR,         // *
    SLASH,        // /
    ARROW         // =>
};

struct Token {
    TokenType type;
    std::string_view lexeme; // Orijinal metne sıfır-kopyalama referans
    size_t line;
};

// ==========================================
// 2. LEXER SINIFI
// ==========================================

class Lexer {
public:
    explicit Lexer(std::string_view source) 
        : source_(source), cursor_(0), line_(1) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        
        while (!is_at_end()) {
            skip_whitespace_and_comments();
            if (is_at_end()) break;

            tokens.push_back(scan_token());
        }

        tokens.push_back(Token{TokenType::END_OF_FILE, "", line_});
        return tokens;
    }

private:
    std::string_view source_;
    size_t cursor_;
    size_t line_;

    bool is_at_end() const {
        return cursor_ >= source_.size();
    }

    char peek() const {
        if (is_at_end()) return '\0';
        return source_[cursor_];
    }

    char peek_next() const {
        if (cursor_ + 1 >= source_.size()) return '\0';
        return source_[cursor_ + 1];
    }

    char advance() {
        return source_[cursor_++];
    }

    void skip_whitespace_and_comments() {
        while (!is_at_end()) {
            char c = peek();
            if (c == ' ' || c == '\t' || c == '\r') {
                advance();
            } else if (c == '\n') {
                line_++;
                advance();
            } else if (c == '/' && peek_next() == '/') {
                // Tek satırlık yorum satırlarını geç: //
                while (peek() != '\n' && !is_at_end()) advance();
            } else {
                break;
            }
        }
    }

    Token scan_token() {
        size_t start = cursor_;
        char c = advance();

        // 1. Sayılar (Tam sayı ve Ondalıklı)
        if (std::isdigit(static_cast<unsigned char>(c))) {
            while (std::isdigit(static_cast<unsigned char>(peek()))) advance();
            if (peek() == '.' && std::isdigit(static_cast<unsigned char>(peek_next()))) {
                advance(); // '.' karakterini tüket
                while (std::isdigit(static_cast<unsigned char>(peek()))) advance();
            }
            return make_token(TokenType::NUMBER, start);
        }

        // 2. Tanımlayıcılar & Anahtar Kelimeler
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_') advance();
            std::string_view text = source_.substr(start, cursor_ - start);
            return Token{resolve_keyword(text), text, line_};
        }

        // 3. String Değişmezleri ("...")
        if (c == '"') {
            while (peek() != '"' && !is_at_end()) {
                if (peek() == '\n') line_++;
                advance();
            }
            if (is_at_end()) return Token{TokenType::INVALID, "Kapatilmamis string", line_};
            advance(); // Kapanış çift tırnağını tüket
            return make_token(TokenType::STRING, start);
        }

        // 4. Semboller ve Operatörler
        switch (c) {
            case '{': return make_token(TokenType::LBRACE, start);
            case '}': return make_token(TokenType::RBRACE, start);
            case '[': return make_token(TokenType::LBRACKET, start);
            case ']': return make_token(TokenType::RBRACKET, start);
            case '(': return make_token(TokenType::LPAREN, start);
            case ')': return make_token(TokenType::RPAREN, start);
            case ',': return make_token(TokenType::COMMA, start);
            case ':': return make_token(TokenType::COLON, start);
            case ';': return make_token(TokenType::SEMICOLON, start);
            case '.': return make_token(TokenType::DOT, start);
            case '+': return make_token(TokenType::PLUS, start);
            case '-': return make_token(TokenType::MINUS, start);
            case '*': return make_token(TokenType::STAR, start);
            case '/': return make_token(TokenType::SLASH, start);
            case '=':
                if (peek() == '>') {
                    advance();
                    return make_token(TokenType::ARROW, start); // =>
                }
                return make_token(TokenType::ASSIGN, start);
        }

        return make_token(TokenType::INVALID, start);
    }

    Token make_token(TokenType type, size_t start) {
        std::string_view lexeme = source_.substr(start, cursor_ - start);
        return Token{type, lexeme, line_};
    }

    TokenType resolve_keyword(std::string_view text) {
        if (text == "let") return TokenType::LET;
        if (text == "fn") return TokenType::FN;
        if (text == "return") return TokenType::RETURN;
        if (text == "export") return TokenType::EXPORT;
        if (text == "import") return TokenType::IMPORT;
        if (text == "true") return TokenType::TRUE;
        if (text == "false") return TokenType::FALSE;
        if (text == "null") return TokenType::NULL_TOKEN;
        return TokenType::IDENTIFIER;
    }
};

// ==========================================
// 3. YARDIMCI YAZDIRMA FONKSİYONU
// ==========================================

inline const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::LET:         return "LET";
        case TokenType::FN:          return "FN";
        case TokenType::RETURN:      return "RETURN";
        case TokenType::EXPORT:      return "EXPORT";
        case TokenType::IMPORT:      return "IMPORT";
        case TokenType::TRUE:        return "TRUE";
        case TokenType::FALSE:       return "FALSE";
        case TokenType::NULL_TOKEN:  return "NULL";
        case TokenType::IDENTIFIER:  return "IDENTIFIER";
        case TokenType::NUMBER:      return "NUMBER";
        case TokenType::STRING:      return "STRING";
        case TokenType::LBRACE:      return "LBRACE ({)";
        case TokenType::RBRACE:      return "RBRACE (})";
        case TokenType::LBRACKET:    return "LBRACKET ([)";
        case TokenType::RBRACKET:    return "RBRACKET (])";
        case TokenType::LPAREN:      return "LPAREN (()";
        case TokenType::RPAREN:      return "RPAREN ())";
        case TokenType::COMMA:       return "COMMA (,)";
        case TokenType::COLON:       return "COLON (:)";
        case TokenType::SEMICOLON:   return "SEMICOLON (;)";
        case TokenType::DOT:         return "DOT (.)";
        case TokenType::ASSIGN:      return "ASSIGN (=)";
        case TokenType::PLUS:        return "PLUS (+)";
        case TokenType::MINUS:       return "MINUS (-)";
        case TokenType::STAR:        return "STAR (*)";
        case TokenType::SLASH:       return "SLASH (/)";
        case TokenType::ARROW:       return "ARROW (=>)";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::INVALID:     return "INVALID";
    }
    return "UNKNOWN";
}