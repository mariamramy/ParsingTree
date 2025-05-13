#include "Node.hpp"
#include <iostream>

// Constructor for operands (numeric values)
Node::Node(double value) : type(OPERAND), value(value), op(""), left(nullptr), right(nullptr) {}

// Constructor for binary operators (char version)
Node::Node(char op, NodePtr left, NodePtr right)
    : type(OPERATOR), value(0), op(1, op), left(left), right(right) {}

// Constructor for unary operators (char version)
Node::Node(char op, NodePtr right)
    : type(UNARY_OP), value(0), op(1, op), left(nullptr), right(right) {}

// Constructor for binary operators (string version)
Node::Node(const std::string& op, NodePtr left, NodePtr right)
    : type(OPERATOR), value(0), op(op), left(left), right(right) {}

// Constructor for unary operators (string version)
Node::Node(const std::string& op, NodePtr right)
    : type(UNARY_OP), value(0), op(op), left(nullptr), right(right) {}

// Display node information (useful for debugging)
void Node::displayNode() const {
    if (type == OPERAND) {
        std::cout << "Operand: " << value;
    } else if (type == OPERATOR) {
        std::cout << "Operator: " << op;
    } else if (type == UNARY_OP) {
        std::cout << "Unary Operator: " << op;
    }
    
    std::cout << std::endl;
}
