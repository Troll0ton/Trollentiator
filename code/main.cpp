#include "../include/frontend.h"
#include "../include/middleend.h"

//-----------------------------------------------------------------------------

int main ()
{
    FILE *file_in = fopen ("../files/task1.txt", "rb");

    File *File_input = file_reader (file_in);
    Line *Text = lines_separator (File_input);

    fclose (file_in);

    Tree_info info = { 0 };

    create_latex_file (&info);

    for(int line = 0; line < File_input->num_of_lines; line++)
    {
        tree_info_ctor (&info, File_input, Text);

        info.root = get_g (&(info.Text[line].begin_line));

        print_tree_inorder (info.root, &info);

        tree_dump (&info);

        fprintf (info.file_tex, "\\\\\n");

        calc_derivative (info.root, &info);

        print_tree_inorder (info.root, &info);

        tree_dump (&info);

        Node *root = info.root;

        tree_dtor (root);

        tree_info_dtor (&info);

        fprintf (info.file_tex, "\\\\\n");
    }

    clear_mem (Text, File_input);

    convert_to_pdf (&info);

    return 0;
}

//-----------------------------------------------------------------------------
