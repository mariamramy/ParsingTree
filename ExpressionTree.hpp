#ifndef EXPRESSION_TREE_HPP
#define EXPRESSION_TREE_HPP

#include "Node.hpp"
#include <string>
#include <vector>
#include <stdexcept>

/**
 * Custom exception class for expression errors
 */
class ExpressionError : public std::runtime_error {
public:
    explicit ExpressionError(const std::string& message) : std::runtime_error(message) {}
};

/**
 * Expression Tree class
 * Handles the construction and traversal of expression trees
 */
class ExpressionTree {
public:
    // Constructor with root node
    ExpressionTree(NodePtr root = nullptr);
    
    // Destructor - cleans up any resources
    ~ExpressionTree();
    
    // Getter for root node
    NodePtr getRoot() const { return root; }
    
    // Set root node
    void setRoot(NodePtr newRoot) { root = newRoot; }
    
    // Tree traversal methods
    std::string inOrderTraversal() const;
    std::string preOrderTraversal() const;
    std::string postOrderTraversal() const;
    
    // Display the tree structure (for debugging)
    void displayTree() const;

private:
    NodePtr root;
    
    // Helper methods for traversals
    void inOrderHelper(NodePtr node, std::string& result) const;
    void preOrderHelper(NodePtr node, std::string& result) const;
    void postOrderHelper(NodePtr node, std::string& result) const;
    
    // Helper method for displaying the tree
    void displayTreeHelper(NodePtr node, int level) const;
};

#endif // EXPRESSION_TREE_HPP
