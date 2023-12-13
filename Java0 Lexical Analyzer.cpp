//
//  main.cpp
//  Lexical Analyzer
//
//  Created by William Pano on 4/13/23.
//
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <cctype>
#include <stack>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

enum class ST1{//The states of the inital state table which is used to iterate character by character for the first wave of tokenization.
    S0,S1,S2,S3,S4,S5,S6,S7,S8,S9,S10,S11,S12,S13,S14,S15,S16,S17,S18,S19,S20,S21,S22,S23,S24,S25,S26,S27,STATES1COUNT
};

enum class CharInput{//character inputs to tokenize multicharacter tokens and other symbols.
    L,D,space,mult,div,add,subtract,lessthan,greaterthan,equal,LB,RB,LP,RP,comma,semi,period,charinputcount
};

enum class TokenType{
    variable,   //classification 0
    integer,    //classification 1
    mop,        //classification 2
    addop,      //classification 3
    relop,      //classification 4
    assign,     //classification 5
    LB,         //classification 6
    RB,         //classification 7
    LP,         //classification 8
    RP,         //classification 9
    comma,      //classification 10
    semi,       //classification 11
    period,     //classification 12
    unknown
};

//the first state transition table which is utilized to create a stack of tokens
const ST1 state_transition_table[static_cast<int>(ST1::STATES1COUNT)][static_cast<int>(CharInput::charinputcount)] = {
//  Letter    Digit    Space    Multi    Div      Add      Sub      <        >        =        LB       RB       LP       RP       comma    semi     period
    {ST1::S1 ,ST1::S3 ,ST1::S0 ,ST1::S5 ,ST1::S6 ,ST1::S10,ST1::S11,ST1::S12,ST1::S15,ST1::S18,ST1::S21,ST1::S22,ST1::S23,ST1::S24,ST1::S25,ST1::S26,ST1::S27},//State Transitions from State 0
    {ST1::S1 ,ST1::S1 ,ST1::S2 ,ST1::S2 ,ST1::S2 ,ST1::S2 ,ST1::S2 ,ST1::S2 ,ST1::S2 ,ST1::S2 ,ST1::S2 ,ST1::S2 ,ST1::S2 ,ST1::S2 ,ST1::S2 ,ST1::S2 ,ST1::S2 },//State Transitions from State 1
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State 2 is a success state it will be used in the success state function to tokenize
    {ST1::S4 ,ST1::S3 ,ST1::S4 ,ST1::S4 ,ST1::S4 ,ST1::S4 ,ST1::S4 ,ST1::S4 ,ST1::S4 ,ST1::S4 ,ST1::S4 ,ST1::S4 ,ST1::S4 ,ST1::S4 ,ST1::S4 ,ST1::S4 ,ST1::S4 },//State Transitions from State 3
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State 4 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State 5 is a success state it will be used in the success state function to tokenize
    {ST1::S9 ,ST1::S9 ,ST1::S9 ,ST1::S7 ,ST1::S9 ,ST1::S9 ,ST1::S9 ,ST1::S9 ,ST1::S9 ,ST1::S9 ,ST1::S9 ,ST1::S9 ,ST1::S9 ,ST1::S9 ,ST1::S9 ,ST1::S9 ,ST1::S9 },//State Transitions from State 6
    {ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S8 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 },//State Transitions from State 7
    {ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S0 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 ,ST1::S7 },//State Transitions from State 8
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State 9 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State10 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State11 is a success state it will be used in the success state function to tokenize
    {ST1::S13,ST1::S13,ST1::S13,ST1::S13,ST1::S13,ST1::S13,ST1::S13,ST1::S13,ST1::S13,ST1::S14,ST1::S13,ST1::S13,ST1::S13,ST1::S13,ST1::S13,ST1::S13,ST1::S13},//State Transitions from State 12
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State13 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State14 is a success state it will be used in the success state function to tokenize
    {ST1::S16,ST1::S16,ST1::S16,ST1::S16,ST1::S16,ST1::S16,ST1::S16,ST1::S16,ST1::S16,ST1::S17,ST1::S16,ST1::S16,ST1::S16,ST1::S16,ST1::S16,ST1::S16,ST1::S16},//State Transistions from State 15
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State16 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State17 is a success state it will be used in the success state function to tokenize
    {ST1::S19,ST1::S19,ST1::S19,ST1::S19,ST1::S19,ST1::S19,ST1::S19,ST1::S19,ST1::S19,ST1::S20,ST1::S19,ST1::S19,ST1::S19,ST1::S19,ST1::S19,ST1::S19,ST1::S19},//State Transitions from State 18
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State19 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State20 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State21 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State22 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State23 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State24 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State25 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 },//State26 is a success state it will be used in the success state function to tokenize
    {ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 ,ST1::S0 }//State27 is a success state it will be used in the success state function to tokenize
};

//function which when given the filepath will open the file contents as a string.
std::string ScanFile(const std::string &FilePath){
    std::ifstream file(FilePath);
    if (!file.is_open()){
        std::cerr << "Error: Unable to open file: " << FilePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
};

struct Token {
    std::string value;
    std::string type;
};

//function to turn string input into enum classes of CharInput.
CharInput charToCharInput(char c) {
    if (isalpha(c)) return CharInput::L;
    if (isdigit(c)) return CharInput::D;
    if (isspace(c)) return CharInput::space;
    switch (c) {
        case ' ': case '\t': case '\n': return CharInput::space;
        case '*': return CharInput::mult;
        case '/': return CharInput::div;
        case '+': return CharInput::add;
        case '-': return CharInput::subtract;
        case '<': return CharInput::lessthan;
        case '>': return CharInput::greaterthan;
        case '=': return CharInput::equal;
        case '{': return CharInput::LB;
        case '}': return CharInput::RB;
        case '(': return CharInput::LP;
        case ')': return CharInput::RP;
        case ',': return CharInput::comma;
        case ';': return CharInput::semi;
        case '.': return CharInput::period;
         default: throw std::runtime_error("Unexpected character");
    }
}

//function to output tokens from success states.
TokenType SuccessStateTokens(ST1 state){
    switch(state){
        case ST1::S2: return TokenType::variable;
        case ST1::S4: return TokenType::integer;
        case ST1::S5: return TokenType::mop; // * success
        case ST1::S9: return TokenType::mop; // / success
        case ST1::S10: return TokenType::addop; // + success
        case ST1::S11: return TokenType::addop; // - success
        case ST1::S13: return TokenType::relop; // < success
        case ST1::S14: return TokenType::relop; // <= success
        case ST1::S16: return TokenType::relop; // > success
        case ST1::S17: return TokenType::relop; // >= success
        case ST1::S19: return TokenType::assign; // = success
        case ST1::S20: return TokenType::relop; // == success
        case ST1::S21: return TokenType::LB; //LB success
        case ST1::S22: return TokenType::RB; //RB success
        case ST1::S23: return TokenType::LP; //LP success
        case ST1::S24: return TokenType::RP; //RP success
        case ST1::S25: return TokenType::comma; //comma success
        case ST1::S26: return TokenType::semi; //semi success
        case ST1::S27: return TokenType::period; //period success
        default: return TokenType::unknown;
    }
}

std::string TokenRename (int tokenType){
    std::string TokenName;
    switch (tokenType){
        case 0:
            TokenName = "<var>";
            break;
        case 1:
            TokenName = "<int>";
            break;
        case 2:
            TokenName = "<mop>";
            break;
        case 3:
            TokenName = "<addop>";
            break;
        case 4:
            TokenName = "<relop>";
            break;
        case 5:
            TokenName = "<assign>";
            break;
        case 6:
            TokenName = "$LB";
            break;
        case 7:
            TokenName = "$RB";
            break;
        case 8:
            TokenName = "$LP";
            break;
        case 9:
            TokenName = "$RP";
            break;
        case 10:
            TokenName = "<comma>";
            break;
        case 11:
            TokenName = "<semi>";
            break;
        case 12:
            TokenName = "<period>";
            break;
        case 13:
            TokenName = "<unknown>";
            break;
    }
    return TokenName;
}

std::vector<std::string> reservedwords ={"CONST", "IF", "VAR", "THEN", "PROCEDURE", "WHILE", "CALL", "DO", "ODD", "CLASS", "FOR"};

bool isreservedword(const std::string &word, const std::vector<std::string>& reservedwords){
    for(const auto& reservedWord : reservedwords){
        if(word == reservedWord){
            return true;
        }
    }
    return false;
};


// first stage of tokenization using the first state table. Additionally the reserved word lookup is completed here.
std::stack<Token> FirstTokenList(const std::string &input){
    ST1 NextState = ST1::S0;
    std::string buffer;
    std::stack<Token> tokenStack;
    
    for(int i = 0; i < input.length(); i++){
        char c = input[i];
        CharInput ci = charToCharInput(c);
        NextState = state_transition_table[static_cast<int>(NextState)][static_cast<int>(ci)];

        TokenType tokenType = SuccessStateTokens(NextState);
        if(c != ' ' && c != '\n' && c != '\r' && c != '\t'){
            buffer += c;
        }
        if(tokenType != TokenType::unknown){
            //stores the token value as the current string buffer and stores the TokenType as a string that is processed with the TokenRename function(switch state).
            Token token;
            //std::cout<< token.value << " " << token.type <<std::endl;
            if(isreservedword(buffer, reservedwords)){
                token.type = "$" + buffer;
            }else{
                token.type = TokenRename(int(tokenType));
            }
            token.value = buffer;
            if (!token.type.empty()){
                tokenStack.push(token);
            }
            buffer.clear();
            NextState = ST1::S0;
            }
        }
            return tokenStack;
        };

           
           
//function for debugging as it prints out whatever stack is used to call it.
void printTokenStack(std::stack<Token> tokenStack){
    std::stack<Token> tempStack;
    Token temps;
    
    
    
    while (!tokenStack.empty()){
        Token currentToken = tokenStack.top();
        //std::cout << "Token: " << currentToken.value << ", Classification: "<<currentToken.type <<std::endl;
        tempStack.push(currentToken);
        tokenStack.pop();
    }
    std::ofstream outFile;
    outFile.open("TokenList.txt", std::ios::out);
    while(!tempStack.empty()){
        temps = tempStack.top();
        outFile << temps.value << " " << temps.type << std::endl;
        std::cout << "Token: " << temps.value << ", Classification: "<<temps.type <<std::endl;
        //tokenStack.push(tempStack.top());
        tempStack.pop();
        
    }
    outFile.close();
}

//reversing the stack so that it may be used in the second state table above.
std::stack<Token> ReversedStack(const std::stack<Token> &tokenStack){
    std::stack<Token> tempStack;
    std::stack<Token> reversedStack = tokenStack;
    
    while(!reversedStack.empty()){
        tempStack.push(reversedStack.top());
        reversedStack.pop();
    }
    return tempStack;
};

//States for second state table to construct symbol table
enum class ST2 {
    S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, STATES2COUNT
};

//Token input classes for state table to construct symbol table
enum class tokenInput {
    CLASS, var, LB, CONST, assign, integer, comma, semi, VAR, FOR, IF, THEN, PROCEDURE, WHILE, CALL, DO, ODD, mop, addop, relop, LP, RP, RB, End, tokenInputCount, UNKNOWN
};

//second state table to create symbol table
const ST2 state_transition_table2[static_cast<int>(ST2::STATES2COUNT)][static_cast<int>(tokenInput::tokenInputCount)] = {
//  $CLASS    <var>    $LB      $CONST   <assign> <int>    <comma>  <semi>   $VAR     $FOR     $IF      $THEN    $PROCED  $WHILE   $CALL    $DO      $ODD     <mop>    <addop>  <relop>  $LP      $RP      $RB      EOF
    {ST2::S1 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 }, //transitions from state 0
    {ST2::S0 ,ST2::S2 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 }, //transitions from state 1
    {ST2::S0 ,ST2::S0 ,ST2::S3 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 }, //transitions from state 2
    {ST2::S0 ,ST2::S10,ST2::S0 ,ST2::S4 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S8 ,ST2::S10,ST2::S10,ST2::S10,ST2::S10,ST2::S10,ST2::S10,ST2::S10,ST2::S10,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 }, //transitions from state 3
    {ST2::S0 ,ST2::S5 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 }, //transitions from state 4
    {ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S6 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 }, //transitions from state 5
    {ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S7 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 }, //transitions from state 6
    {ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S4 ,ST2::S3 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 }, //transitions from state 7
    {ST2::S0 ,ST2::S9 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 }, //transitions from state 8
    {ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S8 ,ST2::S3 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 }, //transitions from state 9
    {ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S11,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S12}, //transitions from state 10
    {ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S10 ,ST2::S12}, //transitions from state 11
    {ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 ,ST2::S0 }  //transitions from state 12
};

tokenInput typetotokenInput (const std::string & type){
    if(type == "$CLASS"){
        return tokenInput::CLASS;
    }
    else if(type == "<var>"){
        return tokenInput::var;
    }
    else if(type == "$LB"){
        return tokenInput::LB;
    }
    else if(type == "$CONST"){
        return tokenInput::CONST;
    }
    else if(type == "<assign>"){
        return tokenInput::assign;
    }
    else if(type == "<int>"){
        return tokenInput::integer;
    }
    else if(type == "<comma>"){
        return tokenInput::comma;
    }
    else if(type == "<semi>"){
        return tokenInput::semi;
    }
    else if(type == "$VAR"){
        return tokenInput::VAR;
    }
    else if(type == "$FOR"){
        return tokenInput::FOR;
    }
    else if(type == "$IF"){
        return tokenInput::IF;
    }
    else if(type == "$THEN"){
        return tokenInput::THEN;
    }
    else if(type == "$PROCEDURE"){
        return tokenInput::PROCEDURE;
    }
    else if(type == "$WHILE"){
        return tokenInput::WHILE;
    }
    else if(type == "$CALL"){
        return tokenInput::CALL;
    }
    else if(type == "$DO"){
        return tokenInput::DO;
    }
    else if(type == "$ODD"){
        return tokenInput::ODD;
    }
    else if(type == "<mop>"){
        return tokenInput::mop;
    }
    else if(type == "<addop>"){
        return tokenInput::addop;
    }
    else if(type == "<relop>"){
        return tokenInput::relop;
    }
    else if(type == "$LP"){
        return tokenInput::LP;
    }
    else if(type == "$RP"){
        return tokenInput::RP;
    }
    else if(type == "$RB"){
        return tokenInput::RB;
    }
    else if(type == "$PROCEDURE"){
        return tokenInput::PROCEDURE;
    }
    else{
        return tokenInput::UNKNOWN;
    }
};

void incrementIndexAndSetTempAddress(int &index, int &tempAddress) {
    index += 2;
    tempAddress = index == 0 ? 0 : index;
}


struct symboltablerow {
    std::string Token;
    std::string Class;
    int Value;
    std::string Address;
    std::string Segment;
};

std::stack<symboltablerow> createSymbolTable(const std::stack<Token> &tokenStack){
    std::stack<Token> tokens(tokenStack);
    ST2 prevState = ST2::S0;
    ST2 currState = ST2::S0;
    int index = 0;
    
    std::string tempToken;
    std::string tempClass;
    int tempValue;
    int tempAddress = 0;
    std::string tempSegment;
    
    std::stack<symboltablerow> symbolTable;

    // Iterate through the tokens stack
    while(!tokens.empty()) {
        Token currentToken = tokens.top();
        tokens.pop();

        // Convert the token type to the corresponding input for the state table
        tokenInput input = typetotokenInput(currentToken.type);

        // Update the current and previous state
        prevState = currState;
        currState = state_transition_table2[static_cast<int>(prevState)][static_cast<int>(input)];

        symboltablerow newRow;
        
        // Perform actions based on prevState and currState transitions
        if (prevState == ST2::S1 && currState == ST2::S2) {

            newRow.Token = currentToken.value;
            newRow.Class = "<Program Name>";
            newRow.Value = 0;
            newRow.Address = "0";
            newRow.Segment = "CS";
            
            std::cout << "Token: " << newRow.Token << " Class: " << newRow.Class << " Value: "<< newRow.Value << " Address: "<< newRow.Address << " Segment: "<< newRow.Segment <<std::endl;
            symbolTable.push(newRow);
            
        }
        if (prevState == ST2::S3 && currState == ST2::S4){
            tempClass = currentToken.type;
        }
        if (prevState == ST2::S4 && currState == ST2::S5){
            tempToken = currentToken.value;
        }
        if (prevState == ST2::S5 && currState == ST2::S6){
            
        }
        if (prevState == ST2::S6 && currState == ST2::S7){
            tempValue = stoi(currentToken.value);
        }
        if (prevState == ST2::S7 && currState == ST2::S4){
            incrementIndexAndSetTempAddress(index, tempAddress);
            newRow.Token = tempToken;
            newRow.Class = tempClass;
            newRow.Value = tempValue;
            newRow.Address = tempAddress;
            newRow.Segment = "DS";
            
            std::cout << "Token: " << newRow.Token << " Class: " << newRow.Class << " Value: "<< newRow.Value << " Address: "<< newRow.Address << " Segment: "<< newRow.Segment <<std::endl;
            symbolTable.push(newRow);
            index = index + 2;
            }

        if (prevState == ST2::S7 && currState == ST2::S3){
            newRow.Token = tempToken;
            newRow.Class = tempClass;
            newRow.Value = tempValue;
            newRow.Address = tempAddress;
            newRow.Segment = "DS";
            
            std::cout << "Token: " << newRow.Token << " Class: " << newRow.Class << " Value: "<< newRow.Value << " Address: "<< newRow.Address << " Segment: "<< newRow.Segment <<std::endl;
            symbolTable.push(newRow);
            index = index + 2;
        }
        if (prevState == ST2::S3 && currState == ST2::S8){
            tempClass = currentToken.type;
        }
        if (prevState == ST2::S8 && currState == ST2::S9){
            tempToken = currentToken.value;
        }
        if (prevState == ST2::S9 && currState == ST2::S8){
            newRow.Token = tempToken;
            newRow.Class = tempClass;
            newRow.Value = 0;
            newRow.Address = tempAddress;
            newRow.Segment = "DS";
            
            std::cout << "Token: " << newRow.Token << " Class: " << newRow.Class << " Value: "<< newRow.Value << " Address: "<< newRow.Address << " Segment: "<< newRow.Segment <<std::endl;
            symbolTable.push(newRow);
            
            index = index + 2;
        }
        if (prevState == ST2::S3 && currState == ST2::S10){
        }
        if (prevState == ST2::S10 && currState == ST2::S10){
        }
        if (prevState == ST2::S10 && currState == ST2::S11){
            tempClass = "$NumLit";
            tempToken = currentToken.value;
            tempValue = stoi(currentToken.value);
                newRow.Token = tempToken;
                newRow.Class = tempClass;
                newRow.Value = tempValue;
                newRow.Address = tempAddress;
                newRow.Segment = "DS";
                
                std::cout << "Token: " << newRow.Token << " Class: " << newRow.Class << " Value: "<< newRow.Value << " Address: "<< newRow.Address << " Segment: "<< newRow.Segment <<std::endl;
                symbolTable.push(newRow);
                
            index = index + 2;
        }
        if (prevState == ST2::S11 && currState == ST2::S10){
            
        }
    }
    return symbolTable;
};

enum class Prec{
    LT , GT, EQ, NO, PRECCOUNT
};

enum class OpType{
    delimiter, assign, add, sub, LP, RP, mult,  div,  IF,  THEN,  ODD, equality, nonequality, greater, less, greaterequal, lessequal, LB, RB, CALL, CLASS, VAR, CONST, AsIs, OPCOUNT
};

Prec precedence_table[static_cast<int>(OpType::OPCOUNT)][static_cast<int>(OpType::OPCOUNT)] = {
// delimiter       assign    add       sub       LP        RP        mult      div       $IF       $THEN     $ODD      ==        !=        >         <         >=        <=        LB        RB        CALL      CLASS     VAR       CONST    AsIs(<var> or <num>
    {Prec::NO, Prec::LT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::LT, Prec::NO, Prec::NO, Prec::NO},//delimiter
    {Prec::GT, Prec::NO, Prec::LT, Prec::LT, Prec::LT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//assign
    {Prec::GT, Prec::NO, Prec::GT, Prec::GT, Prec::LT, Prec::GT, Prec::LT, Prec::LT, Prec::NO, Prec::GT, Prec::NO, Prec::GT, Prec::GT, Prec::GT, Prec::GT, Prec::GT, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//add
    {Prec::GT, Prec::NO, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::GT, Prec::NO, Prec::GT, Prec::GT, Prec::GT, Prec::GT, Prec::GT, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//sub
    {Prec::NO, Prec::NO, Prec::LT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//LP
    {Prec::GT, Prec::NO, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//RP
    {Prec::GT, Prec::NO, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::GT, Prec::NO, Prec::GT, Prec::GT, Prec::GT, Prec::GT, Prec::GT, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//multi
    {Prec::GT, Prec::NO, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::GT, Prec::NO, Prec::GT, Prec::GT, Prec::GT, Prec::GT, Prec::GT, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//div
    {Prec::NO, Prec::NO, Prec::LT, Prec::LT, Prec::LT, Prec::NO, Prec::LT, Prec::LT, Prec::NO, Prec::EQ, Prec::LT, Prec::LT, Prec::LT, Prec::LT, Prec::LT, Prec::LT, Prec::LT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//IF
    {Prec::NO, Prec::LT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::LT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::LT, Prec::NO, Prec::LT, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//THEN
    {Prec::NO, Prec::NO, Prec::LT, Prec::LT, Prec::LT, Prec::NO, Prec::LT, Prec::LT, Prec::NO, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//ODD
    {Prec::NO, Prec::NO, Prec::LT, Prec::LT, Prec::LT, Prec::NO, Prec::LT, Prec::LT, Prec::NO, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//==
    {Prec::NO, Prec::NO, Prec::LT, Prec::LT, Prec::LT, Prec::NO, Prec::LT, Prec::LT, Prec::NO, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//!=
    {Prec::NO, Prec::NO, Prec::LT, Prec::LT, Prec::LT, Prec::NO, Prec::LT, Prec::LT, Prec::NO, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//Greater
    {Prec::NO, Prec::NO, Prec::LT, Prec::LT, Prec::LT, Prec::NO, Prec::LT, Prec::LT, Prec::NO, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//Less
    {Prec::NO, Prec::NO, Prec::LT, Prec::LT, Prec::LT, Prec::NO, Prec::LT, Prec::LT, Prec::NO, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//GreaterEqual
    {Prec::NO, Prec::NO, Prec::LT, Prec::LT, Prec::LT, Prec::NO, Prec::LT, Prec::LT, Prec::NO, Prec::GT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//LessEqual
    {Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::LT, Prec::NO, Prec::LT, Prec::LT, Prec::NO},//{
    {Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//}
    {Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//CALL
    {Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::LT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//CLASS
    {Prec::GT, Prec::LT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO},//VAR
    {Prec::GT, Prec::LT, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO, Prec::NO}//CONST
};
OpType TokenToOpType (const std::string & type){
    if(type == "<semi>"){
       return OpType::delimiter;
    }
    else if(type == "<comma>"){
        return OpType::delimiter;
    }
    else if(type == "<assign>"){
        return OpType::assign;
    }
    else if(type == "+" ){
        return OpType::add;
    }
    else if(type == "-"){
        return OpType::sub;
    }
    else if(type == "$LP"){
        return OpType::LP;
    }
    else if(type == "$RP"){
        return OpType::RP;
    }
    else if(type == "*"){
        return OpType::mult;
    }
    else if(type == "/"){
        return OpType::div;
    }
    else if(type == "$IF"){
        return OpType::IF;
    }
    else if(type == "$THEN"){
        return OpType::THEN;
    }
    else if(type == "$ODD"){
        return OpType::ODD;
    }
    else if(type == "=="){
        return OpType::equality;
    }
    else if(type == "!="){
        return OpType::nonequality;
    }
    else if(type == ">"){
        return OpType::greater;
    }
    else if(type == "<"){
        return OpType::less;
    }
    else if(type == ">="){
        return OpType::greaterequal;
    }
    else if(type == "<="){
        return OpType::lessequal;
    }
    else if(type == "$LB"){
        return OpType::LB;
    }
    else if(type == "$RB"){
        return OpType::RB;
    }
    else if(type == "$CALL")
        return OpType::CALL;
    else if(type == "$CLASS"){
        return OpType::CLASS;
    }
    else if(type == "$VAR"){
        return OpType::VAR;
    }
    else if(type == "$CONST"){
        return OpType::CONST;
    }
    else{
        return OpType::AsIs;
    }
};
//a function to collect the approprioate input based on the token
std::string TokenToAppropriateOpType(const Token &token) {
    if(token.type == "<relop>" or token.type == "<addop>" or token.type == "<mop>"){
        return token.value;
    }
    else{
        return token.type;
    }
};


std::stack<std::string> SyntaxStack (std::stack <Token> & tokenStack){
    std::stack<Token> tokens(tokenStack);
    std::stack<std::string> SyntaxStack;
    
    std::string terminator{"<semi>"};
    SyntaxStack.push(terminator);
    
    OpType previousOpType;
    OpType currentOpType;
    
    previousOpType= TokenToOpType(terminator);
    while(!tokens.empty()) {
        Token currentToken = tokens.top();
        tokens.pop();
        
        currentOpType = TokenToOpType(TokenToAppropriateOpType(currentToken));
        
        Prec prec = precedence_table[static_cast<int>(previousOpType)][static_cast<int>(currentOpType)];
        if(prec == Prec::LT){
            SyntaxStack.push(TokenToAppropriateOpType(currentToken));
            std::cout<< "Current Token: " << SyntaxStack.top() << " Previous OpType: " <<
                static_cast<int>(previousOpType) << " Current OpType: " << static_cast<int>(currentOpType) << std::endl;
            
            previousOpType = currentOpType;

        }
        if(prec == Prec::NO){
            SyntaxStack.push(TokenToAppropriateOpType(currentToken));
            std::cout<< currentToken.value <<std::endl;
        }
        if(prec == Prec::GT){
            std::cout<< "Current Token: " << TokenToAppropriateOpType(currentToken) << " Previous OpType: " << static_cast<int>(previousOpType) << " Current OpType: " << static_cast<int>(currentOpType) << std::endl;
            //std::cout<< currentToken.value <<std::endl;
            //tempCounter++;
            if(static_cast<int>(previousOpType) == 1){
            };
            if(static_cast<int>(previousOpType) == 18){
            };
        }
    }
    return SyntaxStack;
};



int main() {
    std::string FilePath = "example.txt";
    std::string FileContent = ScanFile("/Users/williampano/Documents/Compilers Files/Lexical Analyzer/Lexical Analyzer/" + FilePath);
    
    std::stack<Token> tokenList = FirstTokenList(FileContent);
    createSymbolTable(ReversedStack(tokenList));
    printTokenStack(tokenList);
    //SyntaxStack(tokenList);
    return 0;
}