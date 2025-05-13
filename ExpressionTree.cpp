#include "ExpressionTree.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

// Constructor
ExpressionTree::ExpressionTree(NodePtr root) : root(root) {}

// Destructor
ExpressionTree::~ExpressionTree() {
    // Shared pointers will handle memory cleanup automatically
}

// In-order traversal (Left -> Root -> Right)
std::string ExpressionTree::inOrderTraversal() const {
    std::string result;
    inOrderHelper(root, result);
    return result;
}

// Pre-order traversal (Root -> Left -> Right)
std::string ExpressionTree::preOrderTraversal() const {
    std::string result;
    preOrderHelper(root, result);
    return result;
}

// Post-order traversal (Left -> Right -> Root)
std::string ExpressionTree::postOrderTraversal() const {
    std::string result;
    postOrderHelper(root, result);
    return result;
}

// Helper method for in-order traversal
void ExpressionTree::inOrderHelper(NodePtr node, std::string& result) const {
    if (!node) return;
    
    // Add parentheses for binary operators to maintain precedence visibility
    bool needParentheses = node->isOperator() && (node->getLeft() || node->getRight());
    
    if (needParentheses) result += "(";
    
    // Handle left child
    if (node->getLeft()) {
        inOrderHelper(node->getLeft(), result);
    }
    
    // Handle current node
    if (node->isOperand()) {
        // Convert double to string with precision handling
        std::ostringstream ss;
        ss << node->getValue();
        result += ss.str();
    } else {
        result += " " + node->getOperator() + " ";
    }
    
    // Handle right child
    if (node->getRight()) {
        inOrderHelper(node->getRight(), result);
    }
    
    if (needParentheses) result += ")";
}

// Helper method for pre-order traversal
void ExpressionTree::preOrderHelper(NodePtr node, std::string& result) const {
    if (!node) return;
    
    // Handle current node
    if (node->isOperand()) {
        std::ostringstream ss;
        ss << node->getValue();
        result += ss.str() + " ";
    } else {
        result += node->getOperator() + " ";
    }
    
    // Handle left child
    if (node->getLeft()) {
        preOrderHelper(node->getLeft(), result);
    }
    
    // Handle right child
    if (node->getRight()) {
        preOrderHelper(node->getRight(), result);
    }
}

// Helper method for post-order traversal
void ExpressionTree::postOrderHelper(NodePtr node, std::string& result) const {
    if (!node) return;
    
    // Handle left child
    if (node->getLeft()) {
        postOrderHelper(node->getLeft(), result);
    }
    
    // Handle right child
    if (node->getRight()) {
        postOrderHelper(node->getRight(), result);
    }
    
    // Handle current node
    if (node->isOperand()) {
        std::ostringstream ss;
        ss << node->getValue();
        result += ss.str() + " ";
    } else {
        result += node->getOperator() + " ";
    }
}

// Display the tree structure (useful for debugging)
void ExpressionTree::displayTree() const {
    std::cout << "Expression Tree Structure:\n";
    displayTreeHelper(root, 0);
    std::cout << std::endl;
}

// Helper method for displaying the tree
void ExpressionTree::displayTreeHelper(NodePtr node, int level) const {
    if (!node) return;
    
    // Display right subtree
    displayTreeHelper(node->getRight(), level + 1);
    
    // Display current node
    std::cout << std::setw(level * 4) << "";
    if (node->isOperand()) {
        std::cout << node->getValue() << std::endl;
    } else {
        std::cout << node->getOperator() << std::endl;
    }
    
    // Display left subtree
    displayTreeHelper(node->getLeft(), level + 1);
}
