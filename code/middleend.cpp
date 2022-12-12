#include "../include/middleend.h"

//-----------------------------------------------------------------------------

#define COPY_LEFT copy_tree (curr_node->left, info)
#define COPY_RIGHT copy_tree (curr_node->right, info)
#define LEFT_NODE curr_node->left
#define RIGHT_NODE curr_node->right
#define CALC_DRV(node) calc_derivative (node, info)

Node *calc_derivative (Node *curr_node, Tree_info *info)
{
    value val = { 0 };

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
                case ADD:
                {
                    LEFT_NODE  = CALC_DRV (LEFT_NODE);
                    RIGHT_NODE = CALC_DRV (RIGHT_NODE);

                    return curr_node;
                }

                case SUB:
                {
                    LEFT_NODE  = CALC_DRV (LEFT_NODE);
                    RIGHT_NODE = CALC_DRV (RIGHT_NODE);

                    return curr_node;
                }

                case MUL:
                {
                    Node *left_node_c  = COPY_LEFT;
                    Node *right_node_c = COPY_RIGHT;

                    val.op = MUL;

                    INIT_NODE (new_node_1, CALC_DRV (LEFT_NODE), right_node_c, curr_node, OP, val, 2);

                    val.op = MUL;

                    INIT_NODE (new_node_2, left_node_c, CALC_DRV (RIGHT_NODE), curr_node, OP, val, 2);

                    val.op = ADD;

                    ASSIGN_NODE (curr_node, new_node_1, new_node_2, curr_node->parent, OP, val, 1);

                    return curr_node;
                }

                case DIV:
                {
                    Node *left_node_c   = COPY_LEFT;
                    Node *right_node_c  = COPY_RIGHT;
                    Node *right_node_c1 = COPY_RIGHT;
                    Node *right_node_c2 = COPY_RIGHT;

                    val.op = MUL;
                    INIT_NODE (new_node_1, CALC_DRV (LEFT_NODE), right_node_c, NULL, OP, val, 2);

                    val.op = MUL;
                    INIT_NODE (new_node_2, left_node_c, CALC_DRV (RIGHT_NODE), NULL, OP, val, 2);

                    val.op = SUB;
                    INIT_NODE (new_node_sub, new_node_1, new_node_2, curr_node, OP, val, 1);

                    val.op = MUL;
                    INIT_NODE (new_node_deg, right_node_c1, right_node_c2, curr_node, OP, val, 2);

                    val.op = DIV;
                    ASSIGN_NODE (curr_node, new_node_sub, new_node_deg, curr_node->parent, OP, val, 2);

                    return curr_node;
                }

                case SIN:
                {
                    Node *right_node_c = COPY_RIGHT;

                    val.op = COS;

                    INIT_NODE (new_node_1, NULL, right_node_c, NULL, OP, val, 4);

                    val.op = MUL;

                    ASSIGN_NODE (curr_node, new_node_1, CALC_DRV (RIGHT_NODE), curr_node->parent, OP, val, 2);

                    return curr_node;
                }

                case COS:
                {
                    Node *right_node_c = COPY_RIGHT;

                    val.op = SIN;

                    INIT_NODE (new_node_sin, NULL, right_node_c, NULL, OP, val, 4);

                    val.num = -1;

                    INIT_NODE (node_one, NULL, NULL, NULL, NUM, val, 4);

                    val.op = MUL;

                    INIT_NODE (new_node_1, node_one, new_node_sin, NULL, OP, val, 2);

                    val.op = MUL;

                    ASSIGN_NODE (curr_node, new_node_1, CALC_DRV (RIGHT_NODE), curr_node->parent, OP, val, 2);

                    return curr_node;
                }

                default:
                {
                    printf ("UNKNOWN FUNCTION!\n");

                    return NULL;

                    break;
                }
            }

            break;
        }

        default:
        {
            printf ("MIDDLEEND - UNKNOWN TYPE!: %d\n", curr_node->type);

            return NULL;

            break;
        }
    }
}

//-----------------------------------------------------------------------------

#define LEFT_NUM  curr_node->left->val.num
#define RIGHT_NUM curr_node->right->val.num

void simplify_tree (Node *curr_node, Tree_info *info)
{
    if(curr_node->left)
    {
        simplify_tree (curr_node->left, info);
    }

    if(curr_node->right)
    {
        simplify_tree (curr_node->right, info);
    }

    if(curr_node->type == OP    &&
       curr_node->priority == 4 &&
       curr_node->right->type == NUM)
    {
        switch(curr_node->val.op)
        {
            case SIN:
            {
                curr_node->type = NUM;
                curr_node->val.op = sin (RIGHT_NUM);

                break;
            }

            case COS:
            {
                curr_node->type = NUM;
                curr_node->val.op = cos (RIGHT_NUM);

                break;
            }
        }

        tree_dtor (curr_node->right);
    }

    else if(curr_node->type == OP         &&
            curr_node->right->type == NUM &&
            curr_node->left->type  == NUM   )
    {
        curr_node->type = NUM;
        curr_node->priority = 4;

        switch(curr_node->val.op)
        {
            case ADD:
            {
                curr_node->val.num = LEFT_NUM + RIGHT_NUM;

                break;
            }

            case SUB:
            {
                curr_node->val.num = LEFT_NUM - RIGHT_NUM;

                break;
            }

            case MUL:
            {
                curr_node->val.num = LEFT_NUM * RIGHT_NUM;

                break;
            }

            case DIV:
            {
                curr_node->val.num = LEFT_NUM / RIGHT_NUM;

                break;
            }
        }

        tree_dtor (LEFT_NODE);
        tree_dtor (RIGHT_NODE);
    }

    else if(curr_node->type == OP    &&
       curr_node->val.op == MUL &&
     ((LEFT_NODE->type == NUM   &&
       is_equal (LEFT_NUM, 0))  ||
      (RIGHT_NODE->type == NUM  &&
       is_equal (RIGHT_NUM, 0))))
    {
        curr_node->type = NUM;
        curr_node->priority = 4;
        curr_node->val.num = 0;
        tree_dtor (LEFT_NODE);
        tree_dtor (RIGHT_NODE);
    }

    else if(curr_node->type == OP    &&
       curr_node->val.op == DIV &&
      (LEFT_NODE->type == NUM   &&
       is_equal (LEFT_NUM, 0)))
    {
        curr_node->type = NUM;
        curr_node->priority = 4;
        curr_node->val.num = 0;
        tree_dtor (LEFT_NODE);
        tree_dtor (RIGHT_NODE);
    }

    else if(curr_node->type == OP    &&
      (curr_node->val.op == ADD ||
       curr_node->val.op == SUB)&&
      (LEFT_NODE->type == NUM   &&
       is_equal (LEFT_NUM, 0)))
    {
        Node *old_right = RIGHT_NODE;
        Node *old_left  = LEFT_NODE;

        ASSIGN_NODE (curr_node, old_right->left, old_right->right, curr_node->parent, old_right->type, old_right->val, old_right->priority);

        free (old_right);
        free (old_left);
    }

    else if(curr_node->type == OP    &&
      (curr_node->val.op == ADD ||
       curr_node->val.op == SUB)&&
      (RIGHT_NODE->type == NUM  &&
       is_equal (RIGHT_NUM, 0)))
    {
        Node *old_right = RIGHT_NODE;
        Node *old_left  = LEFT_NODE;

        ASSIGN_NODE (curr_node, old_left->left, old_left->right, curr_node->parent, old_left->type, old_left->val, old_left->priority);

        free (old_right);
        free (old_left);
    }

    else if(curr_node->type == OP    &&
      (curr_node->val.op == MUL ||
       curr_node->val.op == DIV)&&
      (RIGHT_NODE->type == NUM  &&
       is_equal (RIGHT_NUM, 1)))
    {
        Node *old_right = RIGHT_NODE;
        Node *old_left  = LEFT_NODE;

        ASSIGN_NODE (curr_node, old_left->left, old_left->right, curr_node->parent, old_left->type, old_left->val, old_left->priority);

        free (old_right);
        free (old_left);
    }

    else if(curr_node->type == OP    &&
       curr_node->val.op == MUL &&
      (LEFT_NODE->type == NUM   &&
       is_equal (LEFT_NUM, 1)))
    {
        Node *old_right = RIGHT_NODE;
        Node *old_left  = LEFT_NODE;

        ASSIGN_NODE (curr_node, old_right->left, old_right->right, curr_node->parent, old_right->type, old_right->val, old_right->priority);

        free (old_right);
        free (old_left);
    }
}

#undef LEFT_NUM
#undef RIGHT_NUM
#undef LEFT_NODE
#undef RIGHT_NODE
#undef COPY_LEFT
#undef COPY_RIGHT

//-----------------------------------------------------------------------------

void print_expression (Tree_info *info)
{
    fprintf (info->file_tex, "---------------------------------------------------------------------------------------------------------------------------\\\\\n");

    fprintf (info->file_tex, "Current expression:\\\\\n");

    print_tree_inorder (info->root, info);

    tree_dump (info);

    fprintf (info->file_tex, "\\\\\n");
}

//-----------------------------------------------------------------------------

void print_derivative (Tree_info *info)
{
    fprintf (info->file_tex, "Devirative:\\\\\n");

    calc_derivative (info->root, info);

    print_tree_inorder (info->root, info);

    tree_dump (info);

    fprintf (info->file_tex, "\\\\\n");
}

//-----------------------------------------------------------------------------

void print_simplified (Tree_info *info)
{
    fprintf (info->file_tex, "Simplify something:\\\\\n");

    simplify_tree (info->root, info);

    print_tree_inorder (info->root, info);

    tree_dump (info);

    fprintf (info->file_tex, "\\\\\n");

    fprintf (info->file_tex, "---------------------------------------------------------------------------------------------------------------------------\\\\\n");

    fprintf (info->file_tex, "\\\\\n");
}

//-----------------------------------------------------------------------------


