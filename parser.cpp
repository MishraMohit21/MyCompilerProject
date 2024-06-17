#include "parser.h"

void print_Tree(Node *node){

    if(node == NULL){
        return;
    }

    std::cout << node->value << std::endl;

    print_Tree(node->left);
    print_Tree(node->right);
}

void init_node(Node *node, std::string value, NodeTypes type)
{
    node = new Node;
    node->value = value;
    node->type = type;

}

void parser(std::vector<Token> tokens){

    Token *current_token = &tokens[0];
    Node *root = new Node;
    Node *left = new Node;
    Node *right = new Node;
    init_node(root, "PROGRAM", BEGINNING);


    print_Tree(root);

    Node *current = root;

    while(current_token->type != END_OF_TOKEN){

        if (current == NULL){
            break;
        }
        if (current == root){

        }

        if (current_token->type == KEYWORD && current_token->value == "exit"){

            Node *exit_node = new Node;
    std::cout << "Made it here\n";
            init_node(exit_node, current_token->value, STATEMENT);
            root->right = exit_node;
            current = exit_node;
            current_token++;
            if (current_token->type != SEPERATOR)
            {
                std::cerr << "Error Encountered: Missing Seperator\n";
                exit(1);
            }

            Node* open_paren_node = new Node;
            init_node(open_paren_node, current_token->value, EXTRA);
            current->left = open_paren_node;
            current_token++;

            Node *expr_node = new Node;
            init_node(expr_node, current_token->value, INT_LIT);
            current->left->left = expr_node;
            current_token++;

            Node *semi_node = new Node;
            init_node(semi_node, current_token->value, EXTRA);
            current->right = semi_node;

            std::cout << "EXIT\n";
            break;
        }
        current_token++;
    }
    print_Tree(root);

    delete root;
    delete left;
    delete right;
    //return current_token;
}