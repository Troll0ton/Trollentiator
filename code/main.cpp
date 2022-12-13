#include "../include/frontend.h"
#include "../include/middleend.h"

//-----------------------------------------------------------------------------

#define CURR_LINE    &(info.Text[line].begin_line)
#define NUM_OF_LINES info.File_input->num_of_lines

int main ()
{
    Tree_info info = { 0 };

    tree_info_ctor (&info);

    create_latex_file (&info);

    bool mode_derivo = choose_mode ();

    for(int line = 0; line < NUM_OF_LINES - 1; line++)
    {
        nullify_tree_pars (&info);

        info.root = get_grammar (CURR_LINE);

        print_expression (&info);

        if(mode_derivo)
        {
            print_derivative (&info);
        }

        print_simplified (&info);

        tree_dtor (info.root);
    }

    convert_to_pdf (&info);

    tree_info_dtor (&info);

    return 0;
}

#undef CURR_LINE
#undef NUM_OF_LINES

//-----------------------------------------------------------------------------
