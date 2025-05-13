#include "ExpressionEvaluator.hpp"
#include <iostream>
#include <sstream>
#include <stack>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <regex>

ExpressionEvaluator::ExpressionEvaluator() {
    // Initialize binary operators
    binaryOps["+"] = [](double a, double b) { return a + b; };
    binaryOps["-"] = [](double a, double b) { return a - b; };
    binaryOps["*"] = [](double a, double b) { return a * b; };
    binaryOps["/"] = [](double a, double b) {
        if (b == 0) throw ExpressionError("Error: Division by zero");
        return a / b;
    };
    binaryOps["%"] = [](double a, double b) {
        if (b == 0) throw ExpressionError("Error: Modulo by zero");
        return std::fmod(a, b);
    };
    // Add power operator
    binaryOps["^"] = [](double a, double b) { return std::pow(a, b); };
    
    binaryOps["=="] = [](double a, double b) { return a == b ? 1.0 : 0.0; };
    binaryOps["!="] = [](double a, double b) { return a != b ? 1.0 : 0.0; };
    binaryOps["<"] = [](double a, double b) { return a < b ? 1.0 : 0.0; };
    binaryOps[">"] = [](double a, double b) { return a > b ? 1.0 : 0.0; };
    binaryOps["<="] = [](double a, double b) { return a <= b ? 1.0 : 0.0; };
    binaryOps[">="] = [](double a, double b) { return a >= b ? 1.0 : 0.0; };
    binaryOps["&&"] = [](double a, double b) { return (a != 0 && b != 0) ? 1.0 : 0.0; };
    binaryOps["||"] = [](double a, double b) { return (a != 0 || b != 0) ? 1.0 : 0.0; };
    binaryOps["&"] = [](double a, double b) { return static_cast<double>(static_cast<int>(a) & static_cast<int>(b)); };
    binaryOps["|"] = [](double a, double b) { return static_cast<double>(static_cast<int>(a) | static_cast<int>(b)); };
    // Use "xor" for bitwise XOR to avoid conflict with power operator
    binaryOps["xor"] = [](double a, double b) { return static_cast<double>(static_cast<int>(a) ^ static_cast<int>(b)); };
    binaryOps["<<"] = [](double a, double b) { return static_cast<double>(static_cast<int>(a) << static_cast<int>(b)); };
    binaryOps[">>"] = [](double a, double b) { return static_cast<double>(static_cast<int>(a) >> static_cast<int>(b)); };
    
    // Keyword operators
    binaryOps["and"] = [](double a, double b) { return (a != 0 && b != 0) ? 1.0 : 0.0; };
    binaryOps["or"] = [](double a, double b) { return (a != 0 || b != 0) ? 1.0 : 0.0; };
    
    // Initialize unary operators
    unaryOps["-"] = [](double a) { return -a; };
    unaryOps["~"] = [](double a) { return static_cast<double>(~static_cast<int>(a)); };
    unaryOps["not"] = [](double a) { return (a == 0) ? 1.0 : 0.0; };
}

// Parse an expression and build the expression tree
ExpressionTree ExpressionEvaluator::buildExpressionTree(const std::string& expression) {
    std::vector<std::string> tokens = tokenize(expression);
    std::vector<std::string> postfix = infixToPostfix(tokens);
    NodePtr root = buildTreeFromPostfix(postfix);
    return ExpressionTree(root);
}

// Evaluate the expression tree and return the result
double ExpressionEvaluator::evaluate(const ExpressionTree& tree) {
    return evaluateNode(tree.getRoot());
}

// Direct evaluation from expression string
double ExpressionEvaluator::evaluate(const std::string& expression) {
    ExpressionTree tree = buildExpressionTree(expression);
    return evaluate(tree);
}

// Tokenize the input expression
std::vector<std::string> ExpressionEvaluator::tokenize(const std::string& expression) {
    std::vector<std::string> tokens;
    std::string expr = expression;
    
    // Replace keywords with operators for easier processing
    std::map<std::string, std::string> keywords = {
        {"and", "&&"},
        {"or", "||"}
        // Keep "not" as is since it's a unary operator
    };
    
    // List of multicharacter operators
    std::vector<std::string> multiCharOps = {"==", "!=", "<=", ">=", "&&", "||", "<<", ">>"};
    
    // Replace keywords with their operator equivalents
    for (const auto& keyword : keywords) {
        std::regex pattern("\\b" + keyword.first + "\\b");
        expr = std::regex_replace(expr, pattern, keyword.second);
    }
    
    std::string token;
    for (size_t i = 0; i < expr.length(); ++i) {
        char c = expr[i];
        
        // Skip whitespace
        if (std::isspace(c)) {
            continue;
        }
        
        // Handle numbers
        if (std::isdigit(c) || c == '.') {
            token = "";
            while (i < expr.length() && (std::isdigit(expr[i]) || expr[i] == '.')) {
                token += expr[i++];
            }
            tokens.push_back(token);
            --i; // Move back one step since the for loop will increment
            continue;
        }
        
        // Handle parentheses
        if (c == '(' || c == ')') {
            tokens.push_back(std::string(1, c));
            continue;
        }
        
        // Handle operators - check for multi-character operators first
        bool isMultiChar = false;
        for (const auto& op : multiCharOps) {
            if (i + op.length() <= expr.length() && expr.substr(i, op.length()) == op) {
                tokens.push_back(op);
                i += op.length() - 1; // Skip characters
                isMultiChar = true;
                break;
            }
        }
        
        if (isMultiChar) continue;
        
        // Handle single-character operators
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' ||
            c == '<' || c == '>' || c == '=' || c == '!' ||
            c == '&' || c == '|' || c == '~') {
            
            // Handle unary operators at start of expression or after another operator or opening parenthesis
            if ((c == '-' || c == '+' || c == '~' || c == '!') &&
                (tokens.empty() || tokens.back() == "(" || isOperator(tokens.back()))) {
                
                // Convert + to nothing (unary plus doesn't change value)
                if (c == '+') continue;
                
                // For unary operators, we treat them specially
                if (c == '-') {
                    tokens.push_back("u-"); // Mark as unary minus
                } else if (c == '~') {
                    tokens.push_back("~");
                } else if (c == '!') {
                    tokens.push_back("not");
                }
            } else {
                tokens.push_back(std::string(1, c));
            }
            continue;
        }
        
        // Handle alphabetic characters (for variables or function names)
        if (std::isalpha(c)) {
            token = "";
            while (i < expr.length() && (std::isalnum(expr[i]) || expr[i] == '_')) {
                token += expr[i++];
            }
            --i; // Move back one step since the for loop will increment
            
            // Handle 'not' keyword
            if (token == "not") {
                tokens.push_back("not");
            } else {
                tokens.push_back(token); // For future variable support
            }
            continue;
        }
    }
    
    return tokens;
}

// Convert infix expression to postfix using the Shunting Yard algorithm
std::vector<std::string> ExpressionEvaluator::infixToPostfix(const std::vector<std::string>& tokens) {
    std::vector<std::string> postfix;
    std::stack<std::string> operators;
    
    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::string& token = tokens[i];
        
        // If token is a number, add it to the output
        if (isNumber(token)) {
            postfix.push_back(token);
        }
        // If token is a left parenthesis, push it onto the stack
        else if (token == "(") {
            operators.push(token);
        }
        // If token is a right parenthesis, pop operators from stack and add to output until left parenthesis
        else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                postfix.push_back(operators.top());
                operators.pop();
            }
            
            if (!operators.empty() && operators.top() == "(") {
                operators.pop(); // Discard the left parenthesis
            } else {
                throw ExpressionError("Error: Mismatched parentheses");
            }
        }
        // If token is an operator
        else if (isOperator(token)) {
            // Handle special case for unary operators
            if (token == "u-" || token == "~" || token == "not") {
                operators.push(token);
            } else {
                while (!operators.empty() && operators.top() != "(" &&
                       ((isRightAssociative(token) && getPrecedence(token) < getPrecedence(operators.top())) ||
                        (!isRightAssociative(token) && getPrecedence(token) <= getPrecedence(operators.top())))) {
                    postfix.push_back(operators.top());
                    operators.pop();
                }
                operators.push(token);
            }
        }
        // Handle variable names or function calls (future enhancement)
        else {
            postfix.push_back(token);
        }
    }
    
    // Pop any remaining operators from the stack and add to output
    while (!operators.empty()) {
        if (operators.top() == "(") {
            throw ExpressionError("Error: Mismatched parentheses");
        }
        postfix.push_back(operators.top());
        operators.pop();
    }
    
    return postfix;
}

// Build the expression tree from postfix notation
NodePtr ExpressionEvaluator::buildTreeFromPostfix(const std::vector<std::string>& postfix) {
    std::stack<NodePtr> nodeStack;
    
    for (const std::string& token : postfix) {
        if (isNumber(token)) {
            // Convert the token to a double
            double value = std::stod(token);
            nodeStack.push(std::make_shared<Node>(value));
        }
        // Handle unary operators
        else if (isUnaryOperator(token)) {
            if (nodeStack.empty()) {
                throw ExpressionError("Error: Invalid expression syntax for unary operator");
            }
            
            NodePtr right = nodeStack.top();
            nodeStack.pop();
            
            // Create a unary operator node
            std::string opStr = (token == "u-") ? "-" : token; // Convert u- back to -
            nodeStack.push(std::make_shared<Node>(opStr, right));
        }
        // Handle binary operators
        else if (isOperator(token)) {
            if (nodeStack.size() < 2) {
                throw ExpressionError("Error: Invalid expression syntax for binary operator");
            }
            
            NodePtr right = nodeStack.top();
            nodeStack.pop();
            NodePtr left = nodeStack.top();
            nodeStack.pop();
            
            // Create a binary operator node
            nodeStack.push(std::make_shared<Node>(token, left, right));
        }
    }
    
    if (nodeStack.size() != 1) {
        throw ExpressionError("Error: Invalid expression");
    }
    
    return nodeStack.top();
}

// Evaluate a node in the expression tree
double ExpressionEvaluator::evaluateNode(NodePtr node) {
    if (!node) {
        throw ExpressionError("Error: Null node encountered during evaluation");
    }
    
    // If the node is an operand, return its value
    if (node->isOperand()) {
        return node->getValue();
    }
    
    // If the node is a unary operator
    if (node->isUnaryOp()) {
        std::string op = node->getOperator();
        double rightValue = evaluateNode(node->getRight());
        
        // Check if the operator exists in our unary operators map
        auto it = unaryOps.find(op);
        if (it != unaryOps.end()) {
            return it->second(rightValue);
        }
        
        throw ExpressionError("Error: Unknown unary operator '" + op + "'");
    }
    
    // If the node is a binary operator
    double leftValue = evaluateNode(node->getLeft());
    double rightValue = evaluateNode(node->getRight());
    std::string op = node->getOperator();
    
    // Check if the operator exists in our binary operators map
    auto it = binaryOps.find(op);
    if (it != binaryOps.end()) {
        return it->second(leftValue, rightValue);
    }
    
    throw ExpressionError("Error: Unknown binary operator '" + op + "'");
}

// Return the precedence of an operator
int ExpressionEvaluator::getPrecedence(const std::string& op) {
    if (op == "u-" || op == "~" || op == "not") {
        return 8; // Unary operators have highest precedence
    } else if (op == "^") {
        return 7; // Power operator has next highest precedence
    } else if (op == "*" || op == "/" || op == "%") {
        return 6;
    } else if (op == "+" || op == "-") {
        return 5;
    } else if (op == "<<" || op == ">>") {
        return 4;
    } else if (op == "<" || op == ">" || op == "<=" || op == ">=") {
        return 3;
    } else if (op == "==" || op == "!=") {
        return 2;
    } else if (op == "&" || op == "xor" || op == "|" || op == "&&" || op == "||" || op == "and" || op == "or") {
        return 1;
    }
    return 0; // Default precedence for unknown operators
}

// Check if a string is a valid operator
bool ExpressionEvaluator::isOperator(const std::string& token) {
    static const std::vector<std::string> operators = {
        "+", "-", "*", "/", "%", "^",
        "==", "!=", "<", ">", "<=", ">=",
        "&&", "||", "&", "|", "xor", "<<", ">>", "~",
        "u-", "not", "and", "or"
    };
    
    return std::find(operators.begin(), operators.end(), token) != operators.end();
}

// Check if an operator is unary
bool ExpressionEvaluator::isUnaryOperator(const std::string& token) {
    static const std::vector<std::string> unaryOps = {"u-", "~", "not"};
    return std::find(unaryOps.begin(), unaryOps.end(), token) != unaryOps.end();
}

// Check if an operator is right-associative
bool ExpressionEvaluator::isRightAssociative(const std::string& op) {
    // Most operators in C++ are left-associative
    // Exponentiation, unary operators, and assignment operators are right-associative
    static const std::vector<std::string> rightAssociative = {"^", "u-", "~", "not"};
    return std::find(rightAssociative.begin(), rightAssociative.end(), op) != rightAssociative.end();
}

// Check if a token is a number
bool ExpressionEvaluator::isNumber(const std::string& token) {
    if (token.empty()) return false;
    
    // Check if the token is a valid floating-point number
    try {
        std::size_t pos = 0;
        std::stod(token, &pos);
        return pos == token.length();
    } catch (const std::exception&) {
        return false;
    }
}