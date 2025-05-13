#ifndef EXPRESSION_EVALUATOR_HPP
#define EXPRESSION_EVALUATOR_HPP

#include "ExpressionTree.hpp"
#include <string>
#include <vector>
#include <map>
#include <functional>

/**
 * Class to handle expression evaluation and parsing
 * This class includes both Member 1 and Member 3 responsibilities
 * to provide a complete implementation for testing
 */
class ExpressionEvaluator {
public:
    ExpressionEvaluator();
    
    // Parse an expression and build the expression tree
    ExpressionTree buildExpressionTree(const std::string& expression);
    
    // Evaluate the expression tree and return the result
    double evaluate(const ExpressionTree& tree);
    
    // Direct evaluation from expression string
    double evaluate(const std::string& expression);
    
private:
    // Tokenizes the input expression into tokens
    std::vector<std::string> tokenize(const std::string& expression);
    
    // Converts infix expression to postfix notation using Shunting Yard algorithm
    std::vector<std::string> infixToPostfix(const std::vector<std::string>& tokens);
    
    // Builds the expression tree from postfix notation
    NodePtr buildTreeFromPostfix(const std::vector<std::string>& postfix);
    
    // Evaluates a node in the expression tree
    double evaluateNode(NodePtr node);
    
    // Returns the precedence of an operator
    int getPrecedence(const std::string& op);
    
    // Checks if a string is a valid operator
    bool isOperator(const std::string& token);
    
    // Checks if an operator is unary
    bool isUnaryOperator(const std::string& token);
    
    // Checks if an operator is right-associative
    bool isRightAssociative(const std::string& op);
    
    // Checks if a token is a number
    bool isNumber(const std::string& token);
    
    // Maps for operators and their implementations
    std::map<std::string, std::function<double(double, double)>> binaryOps;
    std::map<std::string, std::function<double(double)>> unaryOps;
};

#endif // EXPRESSION_EVALUATOR_HPP
