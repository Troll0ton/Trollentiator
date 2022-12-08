#include "../include/middleend.h"

//-----------------------------------------------------------------------------

Node *calc_derivative (Node *curr_node, Tree_info *info)
{
    switch(curr_node->type)
    {
        case NUM:
        {
            curr_node->val.num = 0;

            return curr_node;
        }

        case VAR:
        {
            curr_node->type = NUM;
            curr_node->val.num = 1;

            return curr_node;
        }

        case OP:
        {
            switch(curr_node->val.op)
            {
                case '+':
                {
                    curr_node->left  = calc_derivative (curr_node->left,  info);
                    curr_node->right = calc_derivative (curr_node->right, info);

                    return curr_node;
                }

                case '-':
                {
                    curr_node->left  = calc_derivative (curr_node->left,  info);
                    curr_node->right = calc_derivative (curr_node->right, info);

                    return curr_node;
                }

                case '*':
                {
                    Node *left_node_c  = copy_tree (curr_node->left, info);
                    Node *right_node_c = copy_tree (curr_node->right, info);

                    Node *new_node_1   = create_node ();
                    new_node_1->type   = OP;
                    new_node_1->val.op = '*';
                    new_node_1->priority = 2;
                    new_node_1->parent = curr_node;
                    new_node_1->left   = calc_derivative (curr_node->left, info);
                    new_node_1->right  = right_node_c;
                    new_node_1->left->parent = new_node_1;
                    new_node_1->right->parent = new_node_1;

                    Node *new_node_2   = create_node ();
                    new_node_2->type   = OP;
                    new_node_2->val.op = '*';
                    new_node_2->priority = 2;
                    new_node_2->parent = curr_node;
                    new_node_2->left   = left_node_c;
                    new_node_2->right  = calc_derivative (curr_node->right, info);
                    new_node_2->left->parent = new_node_2;
                    new_node_2->right->parent = new_node_2;

                    curr_node->val.op = '+';
                    curr_node->priority = 1;
                    curr_node->left  = new_node_1;
                    curr_node->right = new_node_2;

                    return curr_node;
                }

                case '/':
                {
                    Node *new_node_sub = create_node ();
                    new_node_sub->parent = curr_node;

                    Node *left_node_c   = copy_tree (curr_node->left, info);
                    Node *right_node_c  = copy_tree (curr_node->right, info);
                    Node *right_node_c2 = copy_tree (curr_node->right, info);

                    Node *new_node_1   = create_node ();
                    new_node_1->type   = OP;
                    new_node_1->val.op = '*';
                    new_node_1->priority = 2;
                    new_node_1->parent = new_node_sub;
                    new_node_1->left   = calc_derivative (curr_node->left, info);
                    new_node_1->right  = right_node_c;
                    new_node_1->left->parent = new_node_1;
                    new_node_1->right->parent = new_node_1;

                    Node *new_node_2   = create_node ();
                    new_node_2->type   = OP;
                    new_node_2->val.op = '*';
                    new_node_2->priority = 2;
                    new_node_2->parent = new_node_sub;
                    new_node_2->left   = left_node_c;
                    new_node_2->right  = calc_derivative (curr_node->right, info);
                    new_node_2->left->parent = new_node_2;
                    new_node_2->right->parent = new_node_2;

                    new_node_sub->type = OP;
                    new_node_sub->val.op = '-';
                    new_node_sub->priority = 1;
                    new_node_sub->left  = new_node_1;
                    new_node_sub->right = new_node_2;

                    //-----------------------------------------------------------------------------

                    Node *new_node_deg   = create_node ();
                    new_node_deg->type   = OP;
                    new_node_deg->val.op = '*';
                    new_node_deg->priority = 2;
                    new_node_deg->parent = curr_node;
                    new_node_deg->left   = right_node_c2;
                    new_node_deg->right  = right_node_c2;
                    new_node_deg->left->parent  = new_node_deg;
                    new_node_deg->right->parent = new_node_deg;

                    //-----------------------------------------------------------------------------

                    curr_node->left  = new_node_sub;
                    curr_node->right = new_node_deg;

                    return curr_node;
                }

                case '^':
                {
                    Node *left_node_c  = copy_tree (curr_node->left, info);
                    Node *right_node_c = copy_tree (curr_node->right, info);

                    Node *new_node_1   = create_node ();
                    new_node_1->type   = OP;
                    new_node_1->val.op = '-';
                    new_node_1->priority = 1;
                    new_node_1->parent = curr_node;
                    new_node_1->left   = right_node_c;

                    Node *one_node     = create_node ();
                    one_node->type     = NUM;
                    one_node->val.num  = 1;
                    one_node->priority = 4;
                    one_node->parent   = new_node_1;

                    new_node_1->left   = right_node_c
                    new_node_1->right  = one_node;
                    new_node_1->left->parent  = new_node_1;
                    new_node_1->right->parent = new_node_1;

                    Node *new_node_2   = create_node ();
                    new_node_2->type   = OP;
                    new_node_2->val.op = '*';
                    new_node_2->priority = 2;
                    new_node_2->parent = curr_node;
                    new_node_2->left   = left_node_c;
                    new_node_2->right  = calc_derivative (curr_node->right, info);
                    new_node_2->left->parent = new_node_2;
                    new_node_2->right->parent = new_node_2;

                    curr_node->val.op = '*';
                    curr_node->priority = 2;
                    curr_node->left  = new_node_1;
                    curr_node->right = new_node_2;

                    return curr_node;
                    break;
                }

                default:
                {
                    printf ("E");

                    break;
                }
            }

            break;
        }

        default:
        {
            printf ("MIDDLEEND - UNKNOWN TYPE!: %d\n",
                    curr_node->type);
            break;
        }
    }
}

//-----------------------------------------------------------------------------


