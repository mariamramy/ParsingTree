#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cctype>
#include <memory>

using namespace std;

// ------------------ Expression Tree Node ------------------
struct ExprNode {
    string value;
    ExprNode* left;
    ExprNode* right;

    ExprNode(string val) : value(val), left(nullptr), right(nullptr) {}
};

// ------------------ Shunting Yard (Infix to Postfix) ------------------
class ShuntingYard {
private:
    unordered_map<string, int> precedence;
    unordered_map<string, bool> rightAssociative;

    void initializeOperators(bool isBoolean) {
        if (isBoolean) {
            precedence = {
                {"NOT", 3},
                {"AND", 2},
                {"OR", 1}
            };
            rightAssociative = {
                {"NOT", true},
                {"AND", false},
                {"OR", false}
            };
        }
        else {
            precedence = {
                {"^", 3},
                {"*", 2}, {"/", 2}, {"%", 2},
                {"+", 1}, {"-", 1}
            };
            rightAssociative = {
                {"^", true},
                {"*", false}, {"/", false}, {"%", false},
                {"+", false}, {"-", false}
            };
        }
    }

    bool isOperator(const string& token) {
        return precedence.find(token) != precedence.end();
    }

    vector<string> tokenize(const string& expr) {
        vector<string> tokens;
        stringstream ss(expr);
        string token;
        while (ss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    ShuntingYard(bool isBoolean = false) {
        initializeOperators(isBoolean);
    }

    vector<string> convertToPostfix(const string& infix) {
        vector<string> output;
        stack<string> operators;
        vector<string> tokens = tokenize(infix);

        for (const string& token : tokens) {
            if (!isOperator(token) && token != "(" && token != ")") {
                output.push_back(token);
            }
            else if (token == "(") {
                operators.push(token);
            }
            else if (token == ")") {
                while (!operators.empty() && operators.top() != "(") {
                    output.push_back(operators.top());
                    operators.pop();
                }
                if (!operators.empty() && operators.top() == "(") {
                    operators.pop();
                }
            }
            else if (isOperator(token)) {
                while (!operators.empty() && isOperator(operators.top())) {
                    string top = operators.top();
                    if ((rightAssociative[token] == false && precedence[token] <= precedence[top]) ||
                        (rightAssociative[token] == true && precedence[token] < precedence[top])) {
                        output.push_back(top);
                        operators.pop();
                    }
                    else {
                        break;
                    }
                }
                operators.push(token);
            }
        }

        while (!operators.empty()) {
            output.push_back(operators.top());
            operators.pop();
        }

        return output;
    }
};

// ------------------ Expression Tree Builder ------------------

bool isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" ||
        token == "^" || token == "AND" || token == "OR" || token == "NOT";
}

bool isUnary(const string& token) {
    return token == "NOT";
}

ExprNode* buildExpressionTree(const vector<string>& postfix) {
    stack<ExprNode*> st;

    for (const string& token : postfix) {
        if (!isOperator(token)) {
            st.push(new ExprNode(token));
        }
        else if (isUnary(token)) {
            ExprNode* node = new ExprNode(token);
            node->right = st.top(); st.pop();
            st.push(node);
        }
        else {
            ExprNode* node = new ExprNode(token);
            node->right = st.top(); st.pop();
            node->left = st.top(); st.pop();
            st.push(node);
        }
    }

    return st.top(); // Root of the expression tree
}

// ----------------- inorder traversal for Testing
void printInOrder(ExprNode* node) {
    if (!node) return;
    bool isOp = isOperator(node->value);

    if (isOp && node->left) cout << "(";
    printInOrder(node->left);
    cout << " " << node->value << " ";
    printInOrder(node->right);
    if (isOp && node->right) cout << ")";
}

int main() {
    cout << "Enter the expression (space between tokens): ";
    string expr;
    getline(cin, expr);

    cout << "Is this a boolean expression? (y/n): ";
    char isBool;
    cin >> isBool;

    ShuntingYard parser(isBool == 'y');
    vector<string> postfix = parser.convertToPostfix(expr);

    cout << "Postfix: ";
    for (const string& token : postfix) {
        cout << token << " ";
    }
    cout << endl;

    ExprNode* root = buildExpressionTree(postfix);

    cout << "Infix (from tree): ";
    printInOrder(root);
    cout << endl;

    return 0;
}
