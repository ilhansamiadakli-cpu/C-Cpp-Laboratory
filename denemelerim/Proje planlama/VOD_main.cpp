#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "colors.h"
using namespace std;
namespace VOID {
    enum tokenType{
        LBRACE,    // {
        RBRACE,    // }
        LBRACKET,  // [
        RBRACKET,  // ]
        QUOTE,     // "
        AT,        // @
        RPAREN,    // )
        LPAREN,    // (
        COMMAD,    // commad
        TEXT,      // uzun metin yada ASCII art
        EROR_LOG,  // hata loglama, debug için 
        EOF_TOKEN  // dosya bitişini temsil eden (parser için)
    };
    enum readType{
        READ_COMMAD,    // @ den sonra gelen komutları okumak için
        READ_COMMAD_TİP,// @ den sonra gelen komutun () içindeki tipini okumak için
        READ_TEXT,      // { ve } arasındaki metinleri okumak için
        READ_IDLE       // herhangi bir şey okumuyoruz, normal durum
    };

    struct Token{
        tokenType Type;
        string text;    // uzun metin yada ASII art
        string commad;  // renk yada durum komutları
        int line;       // satır sayısı 
    };
    struct printOrder{
        string color;
        string text;
    };

class VodLexer {
    private:
        string filePath;
        int nowLine =1; // debug için 
        vector<Token> createdToken;
    
        void adToken(tokenType t,string d = "",string c =""){
            Token newtoken;
            newtoken.Type = t;
            newtoken.text = d;
            newtoken.commad = c;
            newtoken.line = nowLine;
            createdToken.push_back(newtoken);
        }
        bool isDelimiter(char harf) {
            return (harf == ' ' || harf == '\t' || harf == '[' || harf == ']' || 
            harf == '{' || harf == '}' || harf == '<' || harf == '>');
        }

    public:
        // constructor
        VodLexer(string path){
            filePath = path;
        }
        const vector<Token>& getTokens() {
            return createdToken;
        }
        void createTokens(){
            ifstream file(filePath);
            string line;
            readType read = READ_IDLE;
            string devText = "";
            while(getline(file,line)){
                for (int i=0;i<line.length();i++) {
                    char letter = line[i];
                    if(letter == '/' && i+1<line.length() && line[i+1] == '/'){
                        break;
                    }
                    if(read == READ_TEXT){
                        if(letter == '}'){
                            adToken(TEXT,devText);
                            devText = "";
                        }
                        else{
                            devText += letter;
                            continue;
                        }
                    }
                    switch (letter) {
                        case '{':
                            adToken(LBRACE);
                            read = READ_TEXT;
                        break;
                        case '}':
                            adToken(RBRACE);
                            read = READ_IDLE;
                        break;
                        case '[':
                            adToken(LBRACKET);
                            read = READ_COMMAD;
                        break;
                        case ']':
                            adToken(RBRACKET);
                            read = READ_IDLE;
                        break;
                        // yeni tokenler buraya eklenebilir, örneğin: commad, text, eror_log gibi
                        case ' ':
                        case '\t':
                            continue;
                        break;
                        default:
                            Read(line, i,read);
                        break;                                                
                    }
                
                }
                if (read == READ_TEXT) {
                    devText += '\n';
                }
                nowLine++;
            }
            adToken(EOF_TOKEN);
            file.close();
        }// createTokens sonu
        private:
            void Read(string line,int& i,readType read){
                if(read == READ_COMMAD){
                    string commad = "";
                    while(i< line.length() && !isDelimiter(line[i])){
                        commad += line[i];
                        i++;
                    }
                    adToken(COMMAD,"",commad);
                    i--;
                }
            }
}; // VodLexer class sonu
class VodParser {
    private:
        vector<Token> Tokens;
        string activeColor = "BEYAZ";
        string activeText = "";
        vector<printOrder> getprint;
        int cursor = 0;
        bool syntaxError = false;
        Token currentToken(){
            if(cursor >= Tokens.size() || cursor < 0) return {EOF_TOKEN,"","",-1};
            return Tokens[cursor];
        }
        void cursorBack(){
            cursor --;
        }
        void cursorForward(){
            cursor++;
        }
        void Compare(tokenType expected,string erorLog){
            if(currentToken().Type == expected)
                cursorForward();
            else{
                cerr<< KIRMIZI<<"[ VOD SYNTAX EROR]:"<<BEYAZ<< "satır "<< currentToken().line <<": "<< erorLog<< endl;
                syntaxError = true;
            }
            
        }

    public:
        // constructor
        VodParser(vector<Token> lexerOutput ){
            Tokens = lexerOutput;
        }
        vector<printOrder> getPrint(){
            return getprint;
        }
        bool CompareParse(){
            while (cursor < Tokens.size()) {
                Token t = currentToken();
                if(t.Type == LBRACKET){ // [
                    getParseCommad();
                }
                else if(t.Type == LBRACE){ // {
                    getParseText();
                }
                else{
                    cursorForward();
                }
            }
            return !syntaxError;
        }
        void pushParse(){
            cursor =0;
            while(cursor < Tokens.size()){
                Token t = currentToken();
                if(t.Type == COMMAD){
                    activeColor = t.commad;
                } else if(t.Type == TEXT){
                    activeText = t.text;
                    adOrder(activeColor,activeText);
                }
                cursorForward();
            }
        }
    private:
        void getParseCommad(){
            Compare(LBRACKET,"[ bekeniyordu!" );
            Compare(COMMAD, "komut bulunamadı");
            Compare(RBRACKET, "] bekleniyordu!");
        }
        void getParseText(){
            Compare(LBRACE, "{ bekleniyordu!");
            Compare(TEXT, "bir metin girilmesi gerekiyordu");
            Compare(RBRACE, "} bekleniyordu!");
        }
        void adOrder(string activeColor,string activetext = ""){
            printOrder t;
            t.color = activeColor;
            t.text = activetext;
            getprint.push_back(t);
        }

}; // VOID parser'in sonu
} // VOID namespace sonu

// Yardımcı fonksiyonlar
string Tokendönüştür( VOID::tokenType t){
    switch (t) {
        case VOID::LBRACE:
            return "LBRACE";
        case VOID::RBRACE:
            return "RBRACE";
        case VOID::LBRACKET:
            return "LBRACKET";
        case VOID::RBRACKET:
            return "RBRACKET";
        case VOID::COMMAD:
            return "COMMAD";
        case VOID::TEXT:
            return "TEXT";
        case VOID::EROR_LOG:
            return "EROR_LOG";
        case VOID::EOF_TOKEN:
            return "EOF_TOKEN";
        default:
            return "UNKNOWN_TOKEN";
    }
}
string Metinbas(VOID::Token text){
    if(text.Type == VOID::TEXT){
        string metin = "\tmetin: " +text.text;
        return metin;
    }
    else if (text.Type == VOID::COMMAD) {
        string commad = "\tcomut: " + text.commad;
        return commad;
    }
    return "";
}
string Renkdönüştür(string renk){
    if(renk == "BEYAZ" || renk == "beyaz") return BEYAZ;
    else if(renk == "SARI" || renk == "sarı") return SARI;
    else if(renk == "KIRMIZI" || renk == "kırmızı") return KIRMIZI;
    else if(renk == "MAVI" || renk == "mavi") return MAVI;
    else if(renk == "YESIL" || renk == "yesil") return YESIL;
    else if(renk == "SIYAH" || renk == "siyah") return SIYAH;
    else return "";
}
int main(){
    cout <<"\n\033[H\033[J" << endl;
    //cout << "-------VodLexer Testi-------" << endl;
    VOID::VodLexer lexer("deneme.txt");
    lexer.createTokens();
    vector<VOID::Token> tokens = lexer.getTokens();

    for (const auto& token : tokens) {
       // cout << "Token Type: " << Tokendönüştür(token.Type) << Metinbas(token) << "\tsatır: " << token.line << endl;
    }

    VOID::VodParser parser(tokens);
    if(parser.CompareParse()){
        parser.pushParse();
        vector<VOID::printOrder> Prints = parser.getPrint();
        cout << "\n-------VodParser Testi-------" << endl;
        for (const auto& print : Prints) {
            cout<< Renkdönüştür(print.color) << "\n"<< print.text << endl;
        }
    }
    return 0;
}