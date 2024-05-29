#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_set>
#include <unordered_map>

using namespace std;

enum token_type {
    number,
    float_literal,
    hex_literal,
    string_literal,
    char_literal,
    preprocessor,
    comment,
    reserved_word,
    operator_literal,
    delimiter,
    identifier,
    unknown
};

struct token {
    string value;
    token_type type;
};

const unordered_set<string> reserved_words = {
    "If", "Then", "Else", "End", "While", "For", "Next",
    "Function", "Sub", "Dim", "As", "Integer", "String", "Double",
    "Boolean", "True", "False"
};

const unordered_set<string> operators = {
    "+", "-", "*", "/", "=", "<>", "<", ">", "<=",
    ">=", "&", "And", "Or", "Not"
};

const unordered_set<char> delimiters = {
    '(', ')', '[', ']', '{', '}', ',', ';', ':'
};

bool is_hex(const string& str) {
    if (str.size() < 3 || str[0] != '&' || (str[1] != 'H' && str[1] != 'h')) {
        return false;
    }
    for (size_t i = 2; i < str.size(); ++i) {
        if (!isxdigit(str[i])) {
            return false;
        }
    }
    return true;
}

vector<token> lexical_analysis(const string& code) {
    vector<token> tokens;
    string current_token;
    size_t i = 0;

    while (i < code.size()) {
        char c = code[i];

        if (isspace(c)) {
            ++i;
            continue;
        }

        if (isdigit(c)) {
            current_token += c;
            while (isdigit(code[i + 1])) {
                current_token += code[++i];
            }
            if (code[i + 1] == '.') {
                current_token += code[++i];
                while (isdigit(code[i + 1])) {
                    current_token += code[++i];
                }
                tokens.push_back({ current_token, float_literal });
            }
            else {
                tokens.push_back({ current_token, number });
            }
        }
        else if (c == '&' && (code[i + 1] == 'H' || code[i + 1] == 'h')) {
            current_token += c;
            current_token += code[++i];
            while (isxdigit(code[i + 1])) {
                current_token += code[++i];
            }
            tokens.push_back({ current_token, hex_literal });
        }
        else if (c == '"') {
            current_token += c;
            while (++i < code.size() && code[i] != '"') {
                current_token += code[i];
            }
            current_token += '"';
            tokens.push_back({ current_token, string_literal });
        }
        else if (c == '\'') {
            current_token += c;
            while (++i < code.size() && code[i] != '\n') {
                current_token += code[i];
            }
            tokens.push_back({ current_token, comment });
        }
        else if (delimiters.count(c)) {
            current_token += c;
            tokens.push_back({ current_token, delimiter });
        }
        else if (isalpha(c) || c == '_') {
            current_token += c;
            while (isalnum(code[i + 1]) || code[i + 1] == '_') {
                current_token += code[++i];
            }
            if (reserved_words.count(current_token)) {
                tokens.push_back({ current_token, reserved_word });
            }
            else {
                tokens.push_back({ current_token, identifier });
            }
        }
        else {
            current_token += c;
            if (operators.count(current_token)) {
                tokens.push_back({ current_token, operator_literal });
            }
            else {
                tokens.push_back({ current_token, unknown });
            }
        }

        current_token.clear();
        ++i;
    }

    return tokens;
}

void print_tokens(const vector<token>& tokens) {
    for (const auto& token : tokens) {
        cout << token.value << ", ";
        switch (token.type) {
        case number: cout << "NUMBER"; break;
        case float_literal: cout << "FLOAT"; break;
        case hex_literal: cout << "HEX"; break;
        case string_literal: cout << "STRING_LITERAL"; break;
        case char_literal: cout << "CHAR_LITERAL"; break;
        case preprocessor: cout << "PREPROCESSOR"; break;
        case comment: cout << "COMMENT"; break;
        case reserved_word: cout << "RESERVED_WORD"; break;
        case operator_literal: cout << "operator_literal"; break;
        case delimiter: cout << "DELIMITER"; break;
        case identifier: cout << "IDENTIFIER"; break;
        default: cout << "UNKNOWN"; break;
        }
        cout << endl;
    }
}

int main()
{
    const string code = R"(Dim x As Float
                        x = 10.6
                        Dim y As String
                        y = "Hello"
                        Dim z As Hex
                        z = &HFF
                        If x > 0 Then
                            x = x + 1
                        End If 
                        #define MAX 100
                        ' comment)";

    const vector<token> tokens = lexical_analysis(code);
    print_tokens(tokens);

    return 0;
}