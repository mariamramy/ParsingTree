#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <memory>

// Forward declaration for the shared pointer definition
class Node;
using NodePtr = std::shared_ptr<Node>;

/**
 * Node class for the Expression Tree
 * Represents either an operand (value) or an operator in the expression
 */
class Node {
public:
    enum NodeType {
        OPERAND,    // Numeric value
        OPERATOR,   // Binary operator (+, -, *, /, etc.)
        UNARY_OP    // Unary operator (-, ~, not)
    };

    // Constructors
    Node(double value);                        // For operands
    Node(char op, NodePtr left, NodePtr right); // For binary operators
    Node(char op, NodePtr right);              // For unary operators
    Node(const std::string& op, NodePtr left, NodePtr right); // For multi-char operators
    Node(const std::string& op, NodePtr right); // For multi-char unary operators

    // Getters
    NodeType getType() const { return type; }
    double getValue() const { return value; }
    std::string getOperator() const { return op; }
    NodePtr getLeft() const { return left; }
    NodePtr getRight() const { return right; }
    
    // Check if node is a specific type
    bool isOperand() const { return type == OPERAND; }
    bool isOperator() const { return type == OPERATOR; }
    bool isUnaryOp() const { return type == UNARY_OP; }

    // Display methods (for debugging)
    void displayNode() const;
    
private:
    NodeType type;          // Type of the node
    double value;           // Value if the node is an operand
    std::string op;         // Operator string if the node is an operator
    NodePtr left;           // Left child
    NodePtr right;          // Right child
};

#endif // NODE_HPP
