
//{----------------------------------------------------------------------------
//!                       CALCULATIONS CODEGEN
//}----------------------------------------------------------------------------

CMD_DEF(ADD, "+",
{
    curr_node->val.num = LEFT_NUM + RIGHT_NUM;
})

CMD_DEF(SUB, "-",
{
    curr_node->val.num = LEFT_NUM - RIGHT_NUM;
})

CMD_DEF(MUL, "*",
{
    curr_node->val.num = LEFT_NUM * RIGHT_NUM;
})

CMD_DEF(DIV, "\\over",
{
    curr_node->val.num = LEFT_NUM / RIGHT_NUM;
})

CMD_DEF(POW, "^",
{
    curr_node->val.num = pow (LEFT_NUM, RIGHT_NUM);
})

CMD_DEF(SIN, "sin",
{
    curr_node->val.num = sin (RIGHT_NUM);
})

CMD_DEF(COS, "cos",
{
    curr_node->val.num = cos (RIGHT_NUM);
})

CMD_DEF(LN, "ln",
{
    curr_node->val.num = log (RIGHT_NUM);
})

CMD_DEF(TG, "tg",
{
    curr_node->val.num = tan (RIGHT_NUM);
})

CMD_DEF(CTG, "ctg",
{
    curr_node->val.num = 1 / tan (RIGHT_NUM);
})

