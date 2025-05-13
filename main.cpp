#include "ExpressionEvaluator.hpp"
#include <iostream>
#include <iomanip>
#include <string>

/**
 * Main application entry point
 * Handles user input, expression evaluation, and output
 */
int main() {
    ExpressionEvaluator evaluator;
    std::string expression;
    
    std::cout << "Expression Tree Calculator" << std::endl;
    std::cout << "Type an expression to evaluate, or 'exit' to quit." << std::endl;
    std::cout << "Examples: '5+3', '(5+3)*2', '10-4+7', etc." << std::endl;
    std::cout << std::endl;
    
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, expression);
        
        // Exit if requested
        if (expression == "exit" || expression == "quit") {
            break;
        }
        
        // Skip empty inputs
        if (expression.empty()) {
            continue;
        }
        
        try {
            // Build expression tree
            ExpressionTree tree = evaluator.buildExpressionTree(expression);
            
            // Option to display tree structure (for debugging)
            // tree.displayTree();
            
            // Evaluate expression and display result
            double result = evaluator.evaluate(tree);
            
            // Show result with high precision for floating point values
            std::cout << "Result: ";
            
            // Check if result is effectively an integer
            if (std::abs(result - std::round(result)) < 1e-10) {
                std::cout << static_cast<long long>(result) << std::endl;
            } else {
                std::cout << std::fixed << std::setprecision(6) << result << std::endl;
            }
            
            // Optional: Display the expression in different traversal forms
            // std::cout << "Inorder: " << tree.inOrderTraversal() << std::endl;
            // std::cout << "Preorder: " << tree.preOrderTraversal() << std::endl;
            // std::cout << "Postorder: " << tree.postOrderTraversal() << std::endl;
            
        } catch (const ExpressionError& e) {
            std::cerr << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        
        std::cout << std::endl;
    }
    
    std::cout << "Goodbye!" << std::endl;
    return 0;
}
