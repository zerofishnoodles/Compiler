#include "def.h"

struct node *mknode(int kind, struct node *first, struct node *second, struct node *third, int pos)
{
    struct node *T = (struct node *)malloc(sizeof(struct node));
    T->kind = kind;
    T->ptr[0] = first;
    T->ptr[1] = second;
    T->ptr[2] = third;
    T->pos = pos;
    return T;
}

void display(struct node *T, int indent)
{ //对抽象语法树的先根遍历
    int i = 1;
    struct node *T0;
    if (T)
    {
        switch (T->kind)
        {
        case EXT_DEF_LIST:
            display(T->ptr[0], indent); //显示该外部定义列表中的第一个
            display(T->ptr[1], indent); //显示该外部定义列表中的其它外部定义
            break;
        case EXT_VAR_DEF:
            printf("%*c外部变量定义:\n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示外部变量类型
            printf("%*c定义的变量:\n", indent + 3, ' ');
            display(T->ptr[1], indent + 6); //显示变量列表
            break;
        case EXT_CONST_VAR_DEF:
            printf("%*c外部常量定义:\n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示外部常量类型
            printf("%*c定义的常量:\n", indent + 3, ' ');
            display(T->ptr[1], indent + 6); //显示常量列表
            break;
        case TERM:
            if (T->ptr[0])
            {
                printf("%*c数组: ", indent, ' ');
            }
            else
            {
                printf("%*cID: ", indent, ' ');
            }
            printf("%s\n", T->type_id);
            display(T->ptr[0], indent + 3);
            break;
        case ARRAYS:
            T0 = T;
            i = 1;
            while (T0 != NULL)
            {
                printf("%*c第%d维: \n", indent, ' ', i++);
                display(T0->ptr[0], indent + 3);
                T0 = T0->ptr[1];
            }
            break;
        case TYPE:
        case VOID:
            printf("%*c类型: %s\n", indent, ' ', T->type_id);
            break;
        case EXT_DEC_LIST:
            display(T->ptr[0], indent); //依次显示外部变量名，
            display(T->ptr[1], indent); //后续还有相同的，仅显示语法树此处理代码可以和类似代码合并
            break;
        case VAR_DEC:
            printf("%*c变量名: %s\n", indent, ' ', T->type_id);
            if (T->ptr[0])
            {
                printf("%*c它的初值:\n", indent + 3, ' ');
                display(T->ptr[0], indent + 6); //表达式初值
            }
            break;
        case FUNC_DEF:
            printf("%*c函数定义: \n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示函数返回类型
            display(T->ptr[1], indent + 3); //显示函数名和参数
            display(T->ptr[2], indent + 3); //显示函数体
            break;
        case FUNC_DEC:
            printf("%*c函数名: %s\n", indent, ' ', T->type_id);
            if (T->ptr[0])
            {
                printf("%*c函数形参: \n", indent, ' ');
                display(T->ptr[0], indent + 3); //显示函数参数列表
            }
            else
                printf("%*c无参函数\n", indent + 3, ' ');
            break;
        case PARAM_LIST:
            display(T->ptr[0], indent); //依次显示全部参数类型和名称，
            display(T->ptr[1], indent);
            break;
        case PARAM_DEC:
            printf("%*c类型: %s, 参数名: %s\n", indent, ' ', T->ptr[0]->type == INT ? "int" : "float", T->ptr[1]->type_id);
            break;
        case EXP_STMT:
            printf("%*c表达式语句: \n", indent, ' ');
            display(T->ptr[0], indent + 3);
            break;
        case RETURN:
            if (T->ptr[0])
            {
                printf("%*c返回语句: \n", indent, ' ');
                display(T->ptr[0], indent + 3);
            }
            else
            {
                printf("%*c空返回语句\n", indent, ' ');
            }
            break;
        case CONTINUE_STMT:
            printf("%*c继续语句\n", indent, ' ');
            break;
        case BREAK_STMT:
            printf("%*c打断语句\n", indent, ' ');
            break;
        case COMP_STM:
            if (T->ptr[0])
            {
                printf("%*c复合语句: \n", indent, ' ');
                display(T->ptr[0], indent + 3);
            }
            else
            {
                printf("%*c空复合语句\n", indent, ' ');
            }
            break;
        case STM_DEF_LIST:
            display(T->ptr[0], indent); //显示第一条语句
            display(T->ptr[1], indent); //显示剩下语句
            break;
        case WHILE:
            printf("%*c循环语句: \n", indent, ' ');
            printf("%*c循环条件: \n", indent + 3, ' ');
            display(T->ptr[0], indent + 6); //显示循环条件
            printf("%*c循环体: \n", indent + 3, ' ');
            display(T->ptr[1], indent + 6); //显示循环体
            break;
        case FOR_STMT:
            printf("%*cfor 循环语句: \n", indent, ' ');
            display(T->ptr[0], indent + 3);
            printf("%*c循环体: \n", indent + 3, ' ');
            display(T->ptr[1], indent + 6);
            break;
        case FOR_ARGS:
            printf("%*cfor 循环起始表达式: \n", indent, ' ');
            if (T->ptr[0])
            {
                display(T->ptr[0], indent + 3);
            }
            else
            {
                printf("%*c无\n", indent + 3, ' ');
            }
            printf("%*cfor 循环条件表达式: \n", indent, ' ');
            if (T->ptr[1])
            {
                display(T->ptr[1], indent + 3);
            }
            else
            {
                printf("%*c无\n", indent + 3, ' ');
            }
            printf("%*cfor 循环第三表达式: \n", indent, ' ');
            if (T->ptr[2])
            {
                display(T->ptr[2], indent + 3);
            }
            else
            {
                printf("%*c无\n", indent + 3, ' ');
            }
            break;
        case IF_THEN:
            printf("%*c条件语句(IF_THEN): \n", indent, ' ');
            printf("%*c条件: \n", indent + 3, ' ');
            display(T->ptr[0], indent + 6); //显示条件
            printf("%*cIF子句: \n", indent + 3, ' ');
            display(T->ptr[1], indent + 6); //显示if子句
            break;
        case IF_THEN_ELSE:
            printf("%*c条件语句(IF_THEN_ELSE): \n", indent, ' ');
            printf("%*c条件: \n", indent + 3, ' ');
            display(T->ptr[0], indent + 6); //显示条件
            printf("%*cIF子句: \n", indent + 3, ' ');
            display(T->ptr[1], indent + 6); //显示if子句
            printf("%*cELSE子句: \n", indent + 3, ' ');
            display(T->ptr[2], indent + 6); //显示else子句
            break;
        case VAR_DEF:
            printf("%*c局部变量定义: \n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示变量类型
            printf("%*c定义的变量:\n", indent + 3, ' ');
            display(T->ptr[1], indent + 6); //显示该定义的全部变量名
            break;
        case DEC_LIST:
            display(T->ptr[0], indent);
            display(T->ptr[1], indent);
            break;
        case NODE_ID:
            printf("%*cID:  %s\n", indent, ' ', T->type_id);
            break;
        case INT:
            printf("%*cINT: %d\n", indent, ' ', T->type_int);
            break;
        case FLOAT:
            printf("%*cFLAOT: %f\n", indent, ' ', T->type_float);
            break;
        case ASSIGN:
        case AND:
        case OR:
        case RELOP:
        case ADD:
        case MINUS:
        case MUL:
        case DIV:
        case MOD:
            printf("%*c%s\n", indent, ' ', T->type_id);
            display(T->ptr[0], indent + 3);
            display(T->ptr[1], indent + 3);
            break;
        case NOT:
        case UMINUS:
            printf("%*c%s\n", indent, ' ', T->type_id);
            display(T->ptr[0], indent + 3);
            break;
        case FUNC_CALL:
            printf("%*c函数调用: \n", indent, ' ');
            printf("%*c函数名: %s\n", indent + 3, ' ', T->type_id);
            display(T->ptr[0], indent + 3);
            break;
        case ARGS:
            i = 1;
            while (T)
            { // ARGS表示实际参数表达式序列结点，其第一棵子树为其一个实际参数表达式，第二棵子树为剩下的。
                struct node *T0 = T->ptr[0];
                printf("%*c第%d个实际参数表达式: \n", indent, ' ', i++);
                display(T0, indent + 3);
                T = T->ptr[1];
            }
            //                  printf("%*c第%d个实际参数表达式: \n",indent,' ',i);
            //                  display(T,indent+3);
            // printf("\n");
            break;
        case SELF_ADD_EXP:
            printf("%*c后置自增: \n", indent, ' ');
            printf("%*c变量: %s\n", indent + 3, ' ', T->type_id);
            break;
        case SELF_MINUS_EXP:
            printf("%*c后置自减: \n", indent, ' ');
            printf("%*c变量: %s\n", indent + 3, ' ', T->type_id);
            break;
        case INIT_LIST:
            display(T->ptr[0], indent + 3);
            display(T->ptr[0], indent + 3);
            break;
        }
    }
}

int semantic_analysis(struct node *T)
{
    int i = 1;
    int indent = 0;
    int paramnum;
    int top;
    int temp;
    struct node *T0;
    if (T)
    {
        switch (T->kind)
        {
        case EXT_DEF_LIST:
            semantic_analysis(T->ptr[0]); //显示该外部定义列表中的第一个
            semantic_analysis(T->ptr[1]); //显示该外部定义列表中的其它外部定义
            break;
        case EXT_VAR_DEF:
            tmp_symbols[0].cmd = EXT_VAR_DEF;
            // printf("%*c外部变量定义:\n", indent, ' ');
            semantic_analysis(T->ptr[0]); //显示外部变量类型
            // printf("%*c定义的变量:\n", indent + 3, ' ');
            // printf("%d\n", T->ptr[1]->kind);
            semantic_analysis(T->ptr[1]); //显示变量列表
            if (T->ptr[1]->kind == TERM || T->ptr[1]->kind == VAR_DEC || T->ptr[1]->kind == ARRAY_DEC)
            {
                insert_symtable(&tmp_symbols[0]);
                // DisplaySymbolTable();
                // memset(&tmp_symbols[0], 0, sizeof(struct symbol));
            }
            break;
        case EXT_CONST_VAR_DEF:
            // printf("%*c外部常量定义:\n", indent, ' ');
            semantic_analysis(T->ptr[0]); //显示外部常量类型
            // printf("%*c定义的常量:\n", indent + 3, ' ');
            tmp_symbols[0].cmd = EXT_CONST_VAR_DEF;
            semantic_analysis(T->ptr[1]); //显示常量列表
            if (T->ptr[1]->kind == TERM)
            {
                tmp_symbols[0].flag = 'C';
                insert_symtable(&tmp_symbols[0]);
                // DisplaySymbolTable();
                // memset(&tmp_symbols[0], 0, sizeof(struct symbol));
            }
            break;
        case TERM:
            // if(T->ptr[0] && tmp_symbols[0].cmd == ASSIGN){
            //     tmp_symbols[0].cmd = ARRAYS;
            // }
            // printf("cmd: %d\n", tmp_symbols[0].cmd);
            term_help(T, tmp_symbols[0].cmd);
            // if (T->ptr[0])
            // {
            //   // printf("%*c数组: ", indent, ' ');
            // }
            // else
            // {
            //   // printf("%*cID: ", indent, ' ');
            // }
            // if (tmp_symbols[0].cmd == VAR_DEC)
            // {
            //   i = search_symtable(T->type_id);
            //   if(i == -1){
            //     semantic_error(T->pos, T->type_id, "undeclared");  // 不考虑类型转换
            //   }
            // }
            // else
            // {
            //   tmp_symbols[0].flag = 'V';
            //   strcpy(tmp_symbols[0].name, T->type_id);
            //   tmp_symbols[0].pos = T->pos;
            //   semantic_analysis(T->ptr[0]);
            // }
            semantic_analysis(T->ptr[0]);
            break;
        case ARRAYS: // 对维度处理
            if (T != NULL && tmp_symbols[0].cmd == ARRAY_DEC)
            {
                tmp_symbols[0].flag = 'A';
            }
            if (tmp_symbols[0].cmd == ARRAY_DEC)
            {
                T0 = T;
                i = 0;
                while (T0 != NULL)
                {
                    // printf("%*c第%d维: \n", indent, ' ', i++);
                    // semantic_analysis(T0->ptr[0]);
                    if (T0->ptr[0]->type != INT)
                    {
                        semantic_error(T0->pos, " array subscript is not an integer", "", 2);
                    }
                    else
                    {
                        tmp_symbols[0].param_type[i] = T0->ptr[0]->type_int;
                    }
                    T0 = T0->ptr[1];
                    i++;
                }
                tmp_symbols[0].paramnum = i;
            }
            else if (tmp_symbols[0].cmd == ASSIGN)
            {
                T0 = T;
                while (T0 != NULL)
                {
                    // printf("%*c第%d维: \n", indent, ' ', i++);
                    // semantic_analysis(T0->ptr[0]);
                    if (T0->ptr[0]->type != INT)
                    {
                        semantic_error(T0->pos, " array subscript is not an integer", "", 2);
                    }
                    T0 = T0->ptr[1];
                }
            }
            break;
        case TYPE:
        case VOID:
            // printf("%*c类型: %s\n", indent, ' ', T->type_id);
            tmp_symbols[0].type = T->type;
            break;
        case EXT_DEC_LIST:
            tmp_symbols[0].flag = 'V';
            if (tmp_symbols[0].cmd == EXT_CONST_VAR_DEF)
                tmp_symbols[0].flag = 'C';
            semantic_analysis(T->ptr[0]); //依次显示外部变量名，
            insert_symtable(&tmp_symbols[0]);
            // memset(&tmp_symbols[0], 0, sizeof(struct symbol));
            semantic_analysis(T->ptr[1]); //后续还有相同的，仅显示语法树此处理代码可以和类似代码合并
            // DisplaySymbolTable();
            break;
        case VAR_DEC:
            // printf("%*c变量名: %s\n", indent, ' ', T->type_id);
            tmp_symbols[0].pos = T->pos;
            strcpy(tmp_symbols[0].name, T->type_id);
            tmp_symbols[0].flag = 'V';
            if (tmp_symbols[0].cmd == EXT_CONST_VAR_DEF)
                tmp_symbols[0].flag = 'C';
            if (T->ptr[0])
            {
                // printf("%*c它的初值:\n", indent + 3, ' ');
                tmp_symbols[0].cmd = VAR_DEC;
                semantic_analysis(T->ptr[0]); //表达式初值
            }
            break;
        case ARRAY_DEC:
            tmp_symbols[0].pos = T->pos;
            strcpy(tmp_symbols[0].name, T->type_id);
            if (T->ptr[1])
            {
                tmp_symbols[0].cmd = ARRAY_DEC;
                semantic_analysis(T->ptr[0]); // 维度
                semantic_analysis(T->ptr[1]); // 初值
            }
            break;
        case FUNC_DEF:
            // printf("%*c函数定义: \n", indent, ' ');
            tmp_symbols[0].flag = 'F';
            semantic_analysis(T->ptr[0]); //显示函数返回类型
            semantic_analysis(T->ptr[1]); //显示函数名和参数
            paramnum = tmp_symbols[0].paramnum;
            insert_symtable(&tmp_symbols[0]);
            // memset(&tmp_symbols[0], 0, sizeof(struct symbol));
            // DisplaySymbolTable();
            for (int j = 1; j <= paramnum; j++)
            {
                insert_symtable(&tmp_symbols[j]);
                // memset(&tmp_symbols[j], 0, sizeof(struct symbol));
                // DisplaySymbolTable();
            }
            semantic_analysis(T->ptr[2]); //显示函数体
            break;
        case FUNC_DEC:
            // printf("%*c函数名: %s\n", indent, ' ', T->type_id);
            tmp_symbols[0].pos = T->pos;
            strcpy(tmp_symbols[0].name, T->type_id);
            tmp_symbols[0].paramnum = 0;
            if (T->ptr[0])
            {
                // printf("%*c函数形参: \n", indent, ' ');
                semantic_analysis(T->ptr[0]); //显示函数参数列表
            }
            // else
            // printf("%*c无参函数\n", indent + 3, ' ');
            break;
        case PARAM_LIST:
            tmp_symbols[0].paramnum++;
            semantic_analysis(T->ptr[0]); //依次显示全部参数类型和名称，
            semantic_analysis(T->ptr[1]);
            break;
        case PARAM_DEC:
            // 目前不支持有缺省值
            // printf("%*c类型: %s, 参数名: %s\n", indent, ' ', T->ptr[0]->type == INT ? "int" : "float", T->ptr[1]->type_id);
            paramnum = tmp_symbols[0].paramnum;
            tmp_symbols[0].param_type[paramnum] = T->ptr[0]->type; // paramnum从1开始给
            strcpy(tmp_symbols[paramnum].name, T->ptr[1]->type_id);
            tmp_symbols[paramnum].type = T->ptr[0]->type;
            tmp_symbols[paramnum].flag = 'P';
            break;
        case EXP_STMT:
            // printf("%*c表达式语句: \n", indent, ' ');
            tmp_symbols[0].cmd = EXP_STMT;
            semantic_analysis(T->ptr[0]);
            break;
        case RETURN:
            // if (T->ptr[0])
            // {
            //   printf("%*c返回语句: \n", indent, ' ');
            //   display(T->ptr[0], indent + 3);
            // }
            // else
            // {
            //   printf("%*c空返回语句\n", indent, ' ');
            // }
            for (i = symbolTable.index - 1; i >= 0; i--)
            {
                if (symbolTable.symbols[i].flag == 'F')
                    break;
            }
            if (symbolTable.symbols[i].type == VOID)
            {
                semantic_error(T->pos, " ‘return’ with a value, in function returning void", "", 2);
            }
            break;
        case CONTINUE_STMT:
            // printf("%*c继续语句\n", indent, ' ');
            if (islooping == 0)
            {
                semantic_error(T->pos, " continue statement not within a loop", "", 2);
            }
            break;
        case BREAK_STMT:
            // printf("%*c打断语句\n", indent, ' ');
            if (islooping == 0)
            {
                semantic_error(T->pos, " break statement not within a loop", "", 2);
            }
            break;
        case COMP_STM:
            lev++;
            top = symbol_scope_TX.top;
            paramnum = 0;
            for (i = symbolTable.index - 1; i >= 0; i--)
            {
                if (symbolTable.symbols[i].flag == 'F' && symbolTable.symbols[i].level == lev - 1)
                {
                    paramnum = symbolTable.symbols[i].paramnum;
                    break;
                }
            }
            symbol_scope_TX.TX[top] = symbolTable.index - paramnum;
            symbol_scope_TX.top++;
            if (T->ptr[0])
            {
                // printf("%*c复合语句: \n", indent, ' ');
                semantic_analysis(T->ptr[0]);
            }
            // else
            // {
            //   // printf("%*c空复合语句\n", indent, ' ');
            // }
            top = symbol_scope_TX.top;
            temp = symbolTable.index;
            symbolTable.index = symbol_scope_TX.TX[top - 1];
            symbol_scope_TX.top--;
            lev--;
            if (temp != symbolTable.index)
                //   DisplaySymbolTable();
                break;
        case STM_DEF_LIST:
            semantic_analysis(T->ptr[0]); //显示第一条语句
            semantic_analysis(T->ptr[1]); //显示剩下语句
            break;
        case WHILE:
            // printf("%*c循环语句: \n", indent, ' ');
            // printf("%*c循环条件: \n", indent + 3, ' ');
            // display(T->ptr[0], indent + 6); //显示循环条件
            // printf("%*c循环体: \n", indent + 3, ' ');
            islooping = 1;
            semantic_analysis(T->ptr[1]); //显示循环体
            islooping = 0;
            break;
        case FOR_STMT:
            // printf("%*cfor 循环语句: \n", indent, ' ');
            // semantic_analysis(T->ptr[0]);
            // printf("%*c循环体: \n", indent + 3, ' ');
            islooping = 1;
            semantic_analysis(T->ptr[1]);
            islooping = 0;
            break;
        case FOR_ARGS:
            // printf("%*cfor 循环起始表达式: \n", indent, ' ');
            // if (T->ptr[0])
            // {
            //   display(T->ptr[0], indent + 3);
            // }
            // else
            // {
            //   printf("%*c无\n", indent + 3, ' ');
            // }
            // printf("%*cfor 循环条件表达式: \n", indent, ' ');
            // if (T->ptr[1])
            // {
            //   display(T->ptr[1], indent + 3);
            // }
            // else
            // {
            //   printf("%*c无\n", indent + 3, ' ');
            // }
            // printf("%*cfor 循环第三表达式: \n", indent, ' ');
            // if (T->ptr[2])
            // {
            //   display(T->ptr[2], indent + 3);
            // }
            // else
            // {
            //   printf("%*c无\n", indent + 3, ' ');
            // }
            break;
        case IF_THEN:
            // printf("%*c条件语句(IF_THEN): \n", indent, ' ');
            // printf("%*c条件: \n", indent + 3, ' ');
            // display(T->ptr[0], indent + 6); //显示条件
            // printf("%*cIF子句: \n", indent + 3, ' ');
            semantic_analysis(T->ptr[1]); //显示if子句
            break;
        case IF_THEN_ELSE:
            // printf("%*c条件语句(IF_THEN_ELSE): \n", indent, ' ');
            // printf("%*c条件: \n", indent + 3, ' ');
            // display(T->ptr[0], indent + 6); //显示条件
            // printf("%*cIF子句: \n", indent + 3, ' ');
            semantic_analysis(T->ptr[1]); //显示if子句
            // printf("%*cELSE子句: \n", indent + 3, ' ');
            semantic_analysis(T->ptr[2]); //显示else子句
            break;
        case VAR_DEF:
            // printf("%*c局部变量定义: \n", indent, ' ');
            semantic_analysis(T->ptr[0]); //显示变量类型
            // printf("%*c定义的变量:\n", indent + 3, ' ');
            semantic_analysis(T->ptr[1]); //显示该定义的全部变量名
            // DisplaySymbolTable();
            break;
        case DEC_LIST:
            tmp_symbols[0].cmd = DEC_LIST;
            tmp_symbols[0].flag = 'V';
            semantic_analysis(T->ptr[0]);
            insert_symtable(&tmp_symbols[0]);
            // temp = tmp_symbols[0].type;
            // memset(&tmp_symbols[0], 0, sizeof(struct symbol));
            // tmp_symbols[0].type = temp;
            semantic_analysis(T->ptr[1]);
            break;
        case NODE_ID:
            // printf("%*cID:  %s\n", indent, ' ', T->type_id);
            break;
        case INT:
            // printf("%*cINT: %d\n", indent, ' ', T->type_int);
            break;
        case FLOAT:
            // printf("%*cFLAOT: %f\n", indent, ' ', T->type_float);
            break;
        case ASSIGN:
            tmp_symbols[0].cmd = ASSIGN;
            semantic_analysis(T->ptr[0]);
            semantic_analysis(T->ptr[1]);
            break;
        case AND:
            break;
        case OR:
            break;
        case RELOP:
            break;
        case ADD:
            tmp_symbols[0].cmd = ADD;
            semantic_analysis(T->ptr[0]);
            semantic_analysis(T->ptr[1]);
            break;
        case MINUS:
            tmp_symbols[0].cmd = MINUS;
            semantic_analysis(T->ptr[0]);
            semantic_analysis(T->ptr[1]);
            break;
        case MUL:
            tmp_symbols[0].cmd = MUL;
            semantic_analysis(T->ptr[0]);
            semantic_analysis(T->ptr[1]);
            break;
        case DIV:
            tmp_symbols[0].cmd = DIV;
            semantic_analysis(T->ptr[0]);
            semantic_analysis(T->ptr[1]);
            break;
        case MOD:
            tmp_symbols[0].cmd = MOD;
            semantic_analysis(T->ptr[0]);
            semantic_analysis(T->ptr[1]);
            break;
            // printf("%*c%s\n", indent, ' ', T->type_id);
        case NOT:
        case UMINUS:
            // printf("%*c%s\n", indent, ' ', T->type_id);
            // display(T->ptr[0], indent + 3);
            break;
        case FUNC_CALL:
            // printf("%*c函数调用: \n", indent, ' ');
            // printf("%*c函数名: %s\n", indent + 3, ' ', T->type_id);
            i = search_symtable(T->type_id);
            if (i == -1)
            {
                semantic_error(T->pos, T->type_id, " undeclared", 0);
            }
            else if (symbolTable.symbols[i].flag != 'F')
            {
                semantic_error(T->pos, T->type_id, " is not a function", 0);
            }
            else
            {
                temp = semantic_analysis(T->ptr[0]);
                if (temp < symbolTable.symbols[i].paramnum)
                {
                    semantic_error(T->pos, "too few arguments to function", T->type_id, 1);
                }
                else if (temp > symbolTable.symbols[i].paramnum)
                {
                    semantic_error(T->pos, "too many arguments to function", T->type_id, 1);
                }
            }

            break;
        case ARGS:
            i = 0;
            while (T)
            { // ARGS表示实际参数表达式序列结点，其第一棵子树为其一个实际参数表达式，第二棵子树为剩下的。
                i++;
                struct node *T0 = T->ptr[0];
                // printf("%*c第%d个实际参数表达式: \n", indent, ' ', i++);
                // display(T0, indent + 3);
                T = T->ptr[1];
            }
            return i;
            break;
        case SELF_ADD_EXP:
            // printf("%*c后置自增: \n", indent, ' ');
            // printf("%*c变量: %s\n", indent + 3, ' ', T->type_id);
            break;
        case SELF_MINUS_EXP:
            // printf("%*c后置自减: \n", indent, ' ');
            // printf("%*c变量: %s\n", indent + 3, ' ', T->type_id);
            break;
        case INIT_LIST:
            i = 0;
            struct node *T0 = T->ptr[0];
            while (T0)
            {
                if (T0->kind == INIT_LIST && T0->type != INT && T0->type != FLOAT)
                {
                    i++;
                }
                else if ((T0->type ^ tmp_symbols[0].type) != 0)
                {
                    semantic_error(T0->pos, "conflicting init types for", tmp_symbols[0].name, 1);
                }
                T0 = T0->ptr[0];
            }
            // 没有处理维度error
        }
    }
}

void semantic_error(int pos, char *msg1, char *msg2, int flag)
{
    char buf[256];
    if (flag == 0)
    { //前面打‘’
        sprintf(buf, "%s:%d ‘%s’%s\n", filename, pos, msg1, msg2);
    }
    else if (flag == 1)
    { //后面打‘’
        sprintf(buf, "%s:%d %s ‘%s’\n", filename, pos, msg1, msg2);
    }
    else
    { // 不打
        sprintf(buf, "%s:%d%s%s\n", filename, pos, msg1, msg2);
    }
    if (strcmp(buf, last_error_line) == 0)
        return;
    else if (strcmp("putint", msg1) == 0)
        return;
    else
    {
        printf("%s", buf);
        strcpy(last_error_line, buf);
    }
}

void term_help(struct node *T, int cmd)
{
    int i;
    i = search_symtable(T->type_id);
    switch (cmd)
    {
    case ASSIGN:
        if (i == -1)
        {
            semantic_error(T->pos, T->type_id, " undeclared", 0); // 不考虑类型转换
        }
        else if (symbolTable.symbols[i].flag == 'F')
        {
            semantic_error(T->pos, " lvalue required as left operand of assignment", "", 2);
        }
        else if (symbolTable.symbols[i].flag == 'A' && T->ptr[0] == NULL)
        {
            semantic_error(T->pos, " assignment to expression with array type", "", 2);
        }
        break;
    case VAR_DEC:
        if (i == -1)
        {
            semantic_error(T->pos, T->type_id, " undeclared", 0); // 不考虑类型转换
        }
        else if (T->ptr[0] != NULL && symbolTable.symbols[i].flag != 'A')
        {
            semantic_error(T->pos, " subscripted value is not an array", "", 2);
        }
        break;
    case ADD:
        if (i == -1)
        {
            semantic_error(T->pos, T->type_id, " undeclared", 0); // 不考虑类型转换
        }
        else if (symbolTable.symbols[i].flag == 'F')
        {
            semantic_error(T->pos, " invalid operands to binary operator ", "+", 2);
        }
        break;
    case MINUS:
        if (i == -1)
        {
            semantic_error(T->pos, T->type_id, " undeclared", 0); // 不考虑类型转换
        }
        else if (symbolTable.symbols[i].flag == 'F')
        {
            semantic_error(T->pos, " invalid operands to binary operator ", "-", 2);
        }
        break;
    case MUL:
        if (i == -1)
        {
            semantic_error(T->pos, T->type_id, " undeclared", 0); // 不考虑类型转换
        }
        else if (symbolTable.symbols[i].flag == 'F')
        {
            semantic_error(T->pos, " invalid operands to binary operator ", "*", 2);
        }
        break;
    case DIV:
        if (i == -1)
        {
            semantic_error(T->pos, T->type_id, " undeclared", 0); // 不考虑类型转换
        }
        else if (symbolTable.symbols[i].flag == 'F')
        {
            semantic_error(T->pos, " invalid operands to binary operator ", "/", 2);
        }
        break;
    case MOD:
        if (i == -1)
        {
            semantic_error(T->pos, T->type_id, " undeclared", 0); // 不考虑类型转换
        }
        else if (symbolTable.symbols[i].flag == 'F')
        {
            semantic_error(T->pos, " invalid operands to binary operator ", "%", 2);
        }
        break;
    // case ARRAYS: // 对数组赋值时维度的判断 只判断了整形，没有判断整形表达式
    //   if(i == -1){
    //     semantic_error(T->pos, T->type_id, "undeclared");  // 不考虑类型转换
    //   }else if(symbolTable.symbols[i].flag == 'V' )
    default:
        tmp_symbols[0].flag = 'V';
        strcpy(tmp_symbols[0].name, T->type_id);
        tmp_symbols[0].pos = T->pos;
        tmp_symbols[0].cmd = ARRAY_DEC;
        break;
    }
}

void prn_symbol()
{ //显示符号表
    int i = 0;
    printf("%6s %6s %6s  %6s %4s %6s\n", "变量名", "别 名", "层 号", "类  型", "标记", "偏移量");
    for (i = 0; i < symbolTable.index; i++)
        printf("%6s %6s %6d  %6s %4c %6d\n", symbolTable.symbols[i].name,
               symbolTable.symbols[i].alias, symbolTable.symbols[i].level,
               symbolTable.symbols[i].type == INT ? "int" : "float",
               symbolTable.symbols[i].flag, symbolTable.symbols[i].offset);
}

int searchSymbolTable(char *name)
{
    int i, flag = 0;
    for (i = symbolTable.index - 1; i >= 0; i--)
    {
        if (symbolTable.symbols[i].level == 0)
            flag = 1;
        if (flag && symbolTable.symbols[i].level == 1)
            continue; //跳过前面函数的形式参数表项
        if (!strcmp(symbolTable.symbols[i].name, name))
            return i;
    }
    return -1;
}

//填写临时变量到符号表，返回临时变量在符号表中的位置
int fill_Temp(char *name, int level, int type, char flag, int offset)
{
    strcpy(symbolTable.symbols[symbolTable.index].name, "");
    strcpy(symbolTable.symbols[symbolTable.index].alias, name);
    symbolTable.symbols[symbolTable.index].level = level;
    symbolTable.symbols[symbolTable.index].type = type;
    symbolTable.symbols[symbolTable.index].flag = flag;
    symbolTable.symbols[symbolTable.index].offset = offset;
    return symbolTable.index++; //返回的是临时变量在符号表中的位置序号
}

int fillSymbolTable(char *name, char *alias, int level, int type, char flag, int offset)
{
    //首先根据name查符号表，不能重复定义 重复定义返回-1
    int i;
    /*符号查重，考虑外部变量声明前有函数定义，
    其形参名还在符号表中，这时的外部变量与前函数的形参重名是允许的*/
    for (i = symbolTable.index - 1; i >= 0 && (symbolTable.symbols[i].level == level || level == 0); i--)
    {
        if (level == 0 && symbolTable.symbols[i].level == 1)
            continue; //外部变量和形参不必比较重名
        if (!strcmp(symbolTable.symbols[i].name, name))
            return -1;
    }
    //填写符号表内容
    strcpy(symbolTable.symbols[symbolTable.index].name, name);
    strcpy(symbolTable.symbols[symbolTable.index].alias, alias);
    symbolTable.symbols[symbolTable.index].level = level;
    symbolTable.symbols[symbolTable.index].type = type;
    symbolTable.symbols[symbolTable.index].flag = flag;
    symbolTable.symbols[symbolTable.index].offset = offset;
    return symbolTable.index++; //返回的是符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
}

// 显示当前符号表中的内容,请补充代码
void DisplaySymbolTable()
{
    int i;
    printf("----------------symbol table-----------------------\n");
    printf("%s\t%s\t%s\t%s\t%s\t%s\n", "Index", "Name", "Level", "Type", "Flag", "Param_num");
    printf("---------------------------------------------------\n");
    for (i = 0; i < symbolTable.index; i++)
    {
        if (symbolTable.symbols[i].flag == 'F')
        {
            printf("%d\t%s\t%d\t%s\t%c\t%d\n", i, symbolTable.symbols[i].name,
                   symbolTable.symbols[i].level, symbolTable.symbols[i].fun_type,
                   symbolTable.symbols[i].flag, symbolTable.symbols[i].paramnum);
        }

        else
            printf("%d\t%s\t%d\t%s\t%c\n", i, symbolTable.symbols[i].name,
                   symbolTable.symbols[i].level, symbolTable.symbols[i].type == INT ? "int" : "float",
                   symbolTable.symbols[i].flag);
    }
    printf("---------------------------------------------------");
    printf("\n");
}

int insert_symtable(struct symbol *tmp_symbol)
{
    // search_symtable
    int i = search_symtable(tmp_symbol->name);
    int cur_lev = lev; // 给形参的特判 因为形参还没到复合语句内，但是处于下一层 我知道这代码写的像shi。。。别骂了
    if (tmp_symbol->flag == 'P')
        cur_lev++;
    if (i != -1 && symbolTable.symbols[i].level == cur_lev)
    {
        if (symbolTable.symbols[i].flag != tmp_symbol->flag)
        {
            semantic_error(tmp_symbol->pos, tmp_symbol->name, " redeclared as different kind of symbol", 0);
        }
        else if (symbolTable.symbols[i].type != tmp_symbol->type)
        {
            semantic_error(tmp_symbol->pos, "conflicting types for", tmp_symbol->name, 1);
        }
        else
        {
            semantic_error(tmp_symbol->pos, "redefinition of", tmp_symbol->name, 1);
        }

        return 0;
    }

    // insert
    int index = symbolTable.index;
    symbolTable.symbols[index].level = lev;
    if (tmp_symbol->flag == 'P')
        symbolTable.symbols[index].level++;
    strcpy(symbolTable.symbols[index].name, tmp_symbol->name);
    strcpy(symbolTable.symbols[index].alias, tmp_symbol->alias);
    symbolTable.symbols[index].flag = tmp_symbol->flag;
    symbolTable.symbols[index].offset = tmp_symbol->offset;
    symbolTable.symbols[index].paramnum = tmp_symbol->paramnum;
    symbolTable.symbols[index].type = tmp_symbol->type;
    memcpy(symbolTable.symbols[index].param_type, tmp_symbol->param_type, sizeof(int) * MAXPARAMNUM);
    if (tmp_symbol->flag == 'F')
    {
        char type[256];
        strcpy(type, tmp_symbol->type == INT ? "int(" : (tmp_symbol->type == FLOAT ? "float(" : "void("));
        if (tmp_symbol->paramnum > 0)
        {
            for (int j = 1; j <= tmp_symbol->paramnum; j++)
            {
                strcat(type, tmp_symbol->param_type[j] == INT ? "int," : "float,");
            }
        }
        else
        {
            strcat(type, "void,");
        }
        int length = strlen(type);
        type[length - 1] = ')';
        strcpy(symbolTable.symbols[index].fun_type, type);
    }
    symbolTable.index++;
    //   DisplaySymbolTable();
    return 1;
}

int search_symtable(char *name)
{
    int i, flag = 0;
    for (i = symbolTable.index - 1; i >= 0; i--)
    {
        // if (symbolTable.symbols[i].level == 0)
        //   flag = 1;
        // if (flag && symbolTable.symbols[i].level == 1)
        //   continue; //跳过前面函数的形式参数表项
        if (!strcmp(symbolTable.symbols[i].name, name))
            return i;
    }
    return -1;
}

char *strcat0(char *s1, char *s2)
{
    static char result[10];
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
char *newAlias()
{
    static int no = 1;
    char s[10];
    snprintf(s, 10, "%d", no++);
    return strcat0("v", s);
}
char *newLabel()
{
    static int no = 1;
    char s[10];
    snprintf(s, 10, "%d", no++);
    return strcat0("label", s);
}
char *newTemp()
{
    static int no = 1;
    char s[10];
    snprintf(s, 10, "%d", no++);
    return strcat0("temp", s);
}
//生成一条TAC代码的结点组成的双向循环链表，返回头指针
struct codenode *genIR(int op, struct opn opn1, struct opn opn2, struct opn result)
{
    struct codenode *h = (struct codenode *)malloc(sizeof(struct codenode));
    h->op = op;
    h->opn1 = opn1;
    h->opn2 = opn2;
    h->result = result;
    h->next = h->prior = h;
    return h;
}
//生成一条标号语句，返回头指针
struct codenode *genLabel(char *label)
{
    struct codenode *h = (struct codenode *)malloc(sizeof(struct codenode));
    h->op = LABEL;
    strcpy(h->result.id, label);
    h->next = h->prior = h;
    return h;
}
//生成GOTO语句，返回头指针
struct codenode *genGoto(char *label)
{
    struct codenode *h = (struct codenode *)malloc(sizeof(struct codenode));
    h->op = GOTO;
    strcpy(h->result.id, label);
    h->next = h->prior = h;
    return h;
}
//合并多个中间代码的双向循环链表，首尾相连
struct codenode *merge(int num, ...)
{
    struct codenode *h1, *h2, *p, *t1, *t2;
    va_list ap;
    va_start(ap, num);
    h1 = va_arg(ap, struct codenode *);
    while (--num > 0)
    {
        h2 = va_arg(ap, struct codenode *);
        if (h1 == NULL)
            h1 = h2;
        else if (h2)
        {
            t1 = h1->prior;
            t2 = h2->prior;
            t1->next = h2;
            t2->next = h1;
            h1->prior = t2;
            h2->prior = t1;
        }
    }
    va_end(ap);
    return h1;
}

void printIR(struct codenode *head)
{
    char opnstr1[32], opnstr2[32], resultstr[32];
    struct codenode *h = head;
    do
    {
        if (h->opn1.kind == INT)
            sprintf(opnstr1, "%d", h->opn1.const_int);
        if (h->opn1.kind == FLOAT)
            sprintf(opnstr1, "%f", h->opn1.const_float);
        if (h->opn1.kind == ID)
            sprintf(opnstr1, "%s", h->opn1.id);
        if (h->opn2.kind == INT)
            sprintf(opnstr2, "%d", h->opn2.const_int);
        if (h->opn2.kind == FLOAT)
            sprintf(opnstr2, "%f", h->opn2.const_float);
        if (h->opn2.kind == ID)
            sprintf(opnstr2, "%s", h->opn2.id);
        sprintf(resultstr, "%s", h->result.id);
        switch (h->op)
        {
        case ASSIGN:
            printf("\t%s = %s\n", resultstr, opnstr1);
            break;
        case ADD:
        case MINUS:
        case MUL:
        case DIV:
        case MOD:
            printf("\t%s = %s %c %s\n", resultstr, opnstr1,
                   h->op == ADD ? '+' : h->op == MINUS ? '-'
                                    : h->op == MUL     ? '*'
                                    : h->op == DIV     ? '/'
                                                       : '%',
                   opnstr2);
            break;
        case FUNCTION:
            printf("FUNCTION %s :\n", h->result.id);
            break;
        case PARAM:
            printf("\tPARAM %s\n", h->result.id);
            break;
        case LABEL:
            printf("LABEL %s :\n", h->result.id);
            break;
        case GOTO:
            printf("\tGOTO %s\n", h->result.id);
            break;
        case JLE:
            printf("\tIF %s <= %s GOTO %s\n", opnstr1, opnstr2, resultstr);
            break;
        case JLT:
            printf("\tIF %s < %s GOTO %s\n", opnstr1, opnstr2, resultstr);
            break;
        case JGE:
            printf("\tIF %s >= %s GOTO %s\n", opnstr1, opnstr2, resultstr);
            break;
        case JGT:
            printf("\tIF %s > %s GOTO %s\n", opnstr1, opnstr2, resultstr);
            break;
        case EQ:
            printf("\tIF %s == %s GOTO %s\n", opnstr1, opnstr2, resultstr);
            break;
        case NEQ:
            printf("\tIF %s != %s GOTO %s\n", opnstr1, opnstr2, resultstr);
            break;
        case ARG:
            printf("\tARG %s\n", h->result.id);
            break;
        case CALL:
            if (!strcmp(opnstr1, "putint"))
                printf("\tCALL %s\n", opnstr1);
            else
                printf("\t%s = CALL %s\n", resultstr, opnstr1);
            break;
        case RETURN:
            if (h->result.kind)
                printf("\tRETURN %s\n", resultstr);
            else
                printf("\tRETURN\n");
            break;
        }
        h = h->next;
    } while (h != head);
}

void boolExp(struct node *T)
{ //布尔表达式，参考文献[2]p84的思想
    struct opn opn1, opn2, result;
    int op;
    int rtn;
    if (T)
    {
        switch (T->kind)
        {
        case INT:
            break;
        case FLOAT:
            break;
        case TERM:
            break;
        case RELOP: //处理关系运算表达式,2个操作数都按基本表达式处理
            T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
            Exp(T->ptr[0]);
            T->width = T->ptr[0]->width;
            Exp(T->ptr[1]);
            if (T->width < T->ptr[1]->width)
                T->width = T->ptr[1]->width;
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
            opn1.offset = symbolTable.symbols[T->ptr[0]->place].offset;
            opn2.kind = ID;
            strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
            opn2.offset = symbolTable.symbols[T->ptr[1]->place].offset;
            result.kind = ID;
            strcpy(result.id, T->Etrue);
            if (strcmp(T->type_id, "<") == 0)
                op = JLT;
            else if (strcmp(T->type_id, "<=") == 0)
                op = JLE;
            else if (strcmp(T->type_id, ">") == 0)
                op = JGT;
            else if (strcmp(T->type_id, ">=") == 0)
                op = JGE;
            else if (strcmp(T->type_id, "==") == 0)
                op = EQ;
            else if (strcmp(T->type_id, "!=") == 0)
                op = NEQ;
            T->code = genIR(op, opn1, opn2, result);
            T->code = merge(4, T->ptr[0]->code, T->ptr[1]->code, T->code, genGoto(T->Efalse));
            break;
        case AND:
        case OR:
            if (T->kind == AND)
            {
                strcpy(T->ptr[0]->Etrue, newLabel());
                strcpy(T->ptr[0]->Efalse, T->Efalse);
            }
            else
            {
                strcpy(T->ptr[0]->Etrue, T->Etrue);
                strcpy(T->ptr[0]->Efalse, newLabel());
            }
            strcpy(T->ptr[1]->Etrue, T->Etrue);
            strcpy(T->ptr[1]->Efalse, T->Efalse);
            T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
            boolExp(T->ptr[0]);
            T->width = T->ptr[0]->width;
            boolExp(T->ptr[1]);
            if (T->width < T->ptr[1]->width)
                T->width = T->ptr[1]->width;
            if (T->kind == AND)
                T->code = merge(3, T->ptr[0]->code, genLabel(T->ptr[0]->Etrue), T->ptr[1]->code);
            else
                T->code = merge(3, T->ptr[0]->code, genLabel(T->ptr[0]->Efalse), T->ptr[1]->code);
            break;
        case NOT:
            strcpy(T->ptr[0]->Etrue, T->Efalse);
            strcpy(T->ptr[0]->Efalse, T->Etrue);
            boolExp(T->ptr[0]);
            T->code = T->ptr[0]->code;
            break;
        }
    }
}

int match_param(int i, struct node *T)
{
    int j, num = symbolTable.symbols[i].paramnum;
    int type1, type2, pos = T->pos;
    T = T->ptr[0];
    if (num == 0 && T == NULL)
        return 1;
    for (j = 1; j <= num; j++)
    {
        if (!T)
        {
            // semantic_error(pos,"", "函数调用参数太少!");
            return 0;
        }
        type1 = symbolTable.symbols[i + j].type; //形参类型
        type2 = T->ptr[0]->type;
        if (type1 != type2)
        {
            // semantic_error(pos,"", "参数类型不匹配");
            return 0;
        }
        T = T->ptr[1];
    }
    if (T)
    { // num个参数已经匹配完，还有实参表达式
        // semantic_error(pos,"", "函数调用参数太多!");
        return 0;
    }
    return 1;
}

void Exp(struct node *T)
{ //处理基本表达式，参考文献[2]p82的思想
    int rtn, num, width;
    struct node *T0;
    struct opn opn1, opn2, result;
    if (T)
    {
        switch (T->kind)
        {
        case TERM: //查符号表，获得符号表中的位置，类型送type
            rtn = searchSymbolTable(T->type_id);
            // if (rtn == -1)
            // semantic_error(T->pos, T->type_id, "变量未定义");
            //     if (symbolTable.symbols[rtn].flag == 'F')
            //     {
            //     }
            // semantic_error(T->pos, T->type_id, "是函数名，类型不匹配");
            //     else
            //     {
            if (symbolTable.symbols[rtn].flag == 'A')
            {
                T->place = rtn;
                T->type = symbolTable.symbols[rtn].type;
                T->offset = symbolTable.symbols[rtn].offset;
                T->width = 0;
                T0 = T->ptr[0];
                int i = 0;
                width = T->type == INT ? 4 : 8;
                int off =  1;  // 计算数组寻址
                while(i < symbolTable.symbols[rtn].paramnum) {  
                    off *= symbolTable.symbols[rtn].param_type[i];
                    i++;
                }

                

                i = 0;
                char temp_name[33];
                while (T0)
                {
                    // 这里的变量没有放进符号表中
                    off /= symbolTable.symbols[rtn].param_type[i];
                    opn1.kind = INT;
                    opn1.const_int = width * off;
                    result.kind = ID;
                    strcpy(result.id, newTemp());
                    T0->code = genIR(ASSIGN, opn1, opn2, result);

                    opn1.kind = ID;
                    strcpy(opn1.id ,T0->ptr[0]->type_id );
                    opn2.kind = ID;
                    strcpy(opn2.id, result.id);
                    result.kind = ID;
                    strcpy(result.id, newTemp());
                    T0->code = merge(2, T0->code, genIR(MUL, opn1, opn2, result));

                    if(i >= 1){
                        opn1.kind = ID;
                        strcpy(opn1.id, temp_name);
                        opn2.kind = ID;
                        strcpy(opn2.id, result.id);
                        result.kind = ID;
                        strcpy(result.id, newTemp());
                        T0->code = merge(2, T0->code, genIR(ADD, opn1, opn2, result));
                    }
                    strcpy(temp_name, result.id);

                    T->code = merge(2, T->code, T0->code);


                    T0 = T0->ptr[0];
                    i++;
                }
            }
            else
            {
                T->place = rtn; //结点保存变量在符号表中的位置
                T->code = NULL; //标识符不需要生成TAC
                T->type = symbolTable.symbols[rtn].type;
                T->offset = symbolTable.symbols[rtn].offset;
                T->width = 0; //未再使用新单元
            }

            //     }
            break;
        case INT:
            T->place = fill_Temp(newTemp(), lev, T->type, 'T', T->offset); //为整常量生成一个临时变量
            T->type = INT;
            opn1.kind = INT;
            opn1.const_int = T->type_int;
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->place].alias);
            result.offset = symbolTable.symbols[T->place].offset;
            T->code = genIR(ASSIGN, opn1, opn2, result);
            T->width = 4;
            break;
        case FLOAT:
            T->place = fill_Temp(newTemp(), lev, T->type, 'T', T->offset); //为浮点常量生成一个临时变量
            T->type = FLOAT;
            opn1.kind = FLOAT;
            opn1.const_float = T->type_float;
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->place].alias);
            result.offset = symbolTable.symbols[T->place].offset;
            T->code = genIR(ASSIGN, opn1, opn2, result);
            T->width = 4;
            break;
        case ASSIGN:
            if (T->ptr[0]->kind != TERM)
            {
                // semantic_error(T->pos, "", "赋值语句需要左值");
            }
            else
            {
                Exp(T->ptr[0]); //处理左值，例中仅为变量
                T->ptr[1]->offset = T->offset;
                Exp(T->ptr[1]);
                T->type = T->ptr[0]->type;
                T->width = T->ptr[1]->width;
                T->code = merge(2, T->ptr[0]->code, T->ptr[1]->code);
                opn1.kind = ID;
                strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].alias); //右值一定是个变量或临时变量
                opn1.offset = symbolTable.symbols[T->ptr[1]->place].offset;
                result.kind = ID;
                strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
                result.offset = symbolTable.symbols[T->ptr[0]->place].offset;
                T->code = merge(2, T->code, genIR(ASSIGN, opn1, opn2, result));
            }
            break;
        case AND:   //按算术表达式方式计算布尔值，未写完
        case OR:    //按算术表达式方式计算布尔值，未写完
        case RELOP: //按算术表达式方式计算布尔值，未写完
            T->type = INT;
            T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
            Exp(T->ptr[0]);
            Exp(T->ptr[1]);
            break;
        case ADD:
        case MINUS:
        case MUL:
        case DIV:
        case MOD:
            T->ptr[0]->offset = T->offset;
            Exp(T->ptr[0]);
            T->ptr[1]->offset = T->offset + T->ptr[0]->width;
            Exp(T->ptr[1]);
            //判断T->ptr[0]，T->ptr[1]类型是否正确，可能根据运算符生成不同形式的代码，给T的type赋值
            //下面的类型属性计算，没有考虑错误处理情况
            if (T->ptr[0]->type == FLOAT || T->ptr[1]->type == FLOAT)
                T->type = FLOAT, T->width = T->ptr[0]->width + T->ptr[1]->width + 4;
            else
                T->type = INT, T->width = T->ptr[0]->width + T->ptr[1]->width + 2;
            T->place = fill_Temp(newTemp(), lev, T->type, 'T', T->offset + T->ptr[0]->width + T->ptr[1]->width);
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
            opn1.type = T->ptr[0]->type;
            opn1.offset = symbolTable.symbols[T->ptr[0]->place].offset;
            opn2.kind = ID;
            strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
            opn2.type = T->ptr[1]->type;
            opn2.offset = symbolTable.symbols[T->ptr[1]->place].offset;
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->place].alias);
            result.type = T->type;
            result.offset = symbolTable.symbols[T->place].offset;
            T->code = merge(3, T->ptr[0]->code, T->ptr[1]->code, genIR(T->kind, opn1, opn2, result));
            T->width = T->ptr[0]->width + T->ptr[1]->width + (T->type == INT ? 4 : 8);
            break;
        case NOT: //未写完整
            break;
        case UMINUS: //未写完整
            break;
        case FUNC_CALL: //根据T->type_id查出函数的定义，如果语言中增加了实验教材的read，write需要单独处理一下
            rtn = searchSymbolTable(T->type_id);
            if (rtn == -1)
            {
                // semantic_error(T->pos, T->type_id, "函数未定义");
                break;
            }
            if (symbolTable.symbols[rtn].flag != 'F')
            {
                // semantic_error(T->pos, T->type_id, "不是一个函数");
                break;
            }
            T->type = symbolTable.symbols[rtn].type;
            width = T->type == INT ? 4 : 8; //存放函数返回值的单数字节数
            if (T->ptr[0])
            {
                T->ptr[0]->offset = T->offset;
                Exp(T->ptr[0]);                      //处理所有实参表达式求值，及类型
                T->width = T->ptr[0]->width + width; //累加上计算实参使用临时变量的单元数
                T->code = T->ptr[0]->code;
            }
            else
            {
                T->width = width;
                T->code = NULL;
            }
            match_param(rtn, T); //处理所有参数的匹配
                                 //处理参数列表的中间代码
            T0 = T->ptr[0];
            while (T0)
            {
                result.kind = ID;
                strcpy(result.id, symbolTable.symbols[T0->ptr[0]->place].alias);
                result.offset = symbolTable.symbols[T0->ptr[0]->place].offset;
                T->code = merge(2, T->code, genIR(ARG, opn1, opn2, result));
                T0 = T0->ptr[1];
            }
            T->place = fill_Temp(newTemp(), lev, T->type, 'T', T->offset + T->width - width);
            opn1.kind = ID;
            strcpy(opn1.id, T->type_id); //保存函数名
            opn1.offset = rtn;           //这里offset用以保存函数定义入口,在目标代码生成时，能获取相应信息
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->place].alias);
            result.offset = symbolTable.symbols[T->place].offset;
            T->code = merge(2, T->code, genIR(CALL, opn1, opn2, result)); //生成函数调用中间代码
            break;
        case ARGS: //此处仅处理各实参表达式的求值的代码序列，不生成ARG的实参系列
            T->ptr[0]->offset = T->offset;
            Exp(T->ptr[0]);
            T->width = T->ptr[0]->width;
            T->code = T->ptr[0]->code;
            if (T->ptr[1])
            {
                T->ptr[1]->offset = T->offset + T->ptr[0]->width;
                Exp(T->ptr[1]);
                T->width += T->ptr[1]->width;
                T->code = merge(2, T->code, T->ptr[1]->code);
            }
            break;
        }
    }
}

void ext_var_list(struct node *T)
{ //处理变量列表
    int rtn, num = 1;
    switch (T->kind)
    {
    case EXT_DEC_LIST:
        T->ptr[0]->type = T->type;                //将类型属性向下传递变量结点
        T->ptr[0]->offset = T->offset;            //外部变量的偏移量向下传递
        T->ptr[1]->type = T->type;                //将类型属性向下传递变量结点
        T->ptr[1]->offset = T->offset + T->width; //外部变量的偏移量向下传递
        T->ptr[1]->width = T->width;
        ext_var_list(T->ptr[0]);
        ext_var_list(T->ptr[1]);
        T->num = T->ptr[1]->num + 1;
        break;
    case TERM:
        rtn = fillSymbolTable(T->type_id, newAlias(), lev, T->type, 'V', T->offset); //最后一个变量名
        if (rtn == -1)
        {
        }
        // semantic_error(T->pos, T->type_id, "变量重复定义");
        else
            T->place = rtn;
        T->num = 1;
        break;
    }
}

void generate_IR(struct node *T)
{ //对抽象语法树的先根遍历,按display的控制结构修改完成符号表管理和语义检查和TAC生成（语句部分）
    int rtn, num, width;
    struct node *T0;
    struct opn opn1, opn2, result;
    if (T)
    {
        switch (T->kind)
        {
        case EXT_DEF_LIST:
            if (!T->ptr[0])
                break;
            T->ptr[0]->offset = T->offset;
            generate_IR(T->ptr[0]); //访问外部定义列表中的第一个
            T->code = T->ptr[0]->code;
            if (T->ptr[1])
            {
                T->ptr[1]->offset = T->ptr[0]->offset + T->ptr[0]->width;
                generate_IR(T->ptr[1]); //访问该外部定义列表中的其它外部定义
                T->code = merge(2, T->code, T->ptr[1]->code);
            }
            break;
        case EXT_VAR_DEF: //处理外部说明,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
            T->type = T->ptr[1]->type = !strcmp(T->ptr[0]->type_id, "int") ? INT : FLOAT;
            T->ptr[1]->offset = T->offset;                        //这个外部变量的偏移量向下传递
            T->ptr[1]->width = T->type == INT ? 4 : 8;            //将一个变量的宽度向下传递
            ext_var_list(T->ptr[1]);                              //处理外部变量说明中的标识符序列
            T->width = (T->type == INT ? 4 : 8) * T->ptr[1]->num; //计算这个外部变量说明的宽度
            T->code = NULL;                                       //这里假定外部变量不支持初始化
            break;
        case FUNC_DEF:                                                          //填写函数定义信息到符号表
            T->ptr[1]->type = !strcmp(T->ptr[0]->type_id, "int") ? INT : FLOAT; //获取函数返回类型送到含函数名、参数的结点
            T->width = 0;                                                       //函数的宽度设置为0，不会对外部变量的地址分配产生影响
            T->offset = DX;                                                     //设置局部变量在活动记录中的偏移量初值
            generate_IR(T->ptr[1]);                                             //处理函数名和参数结点部分，这里不考虑用寄存器传递参数
            T->offset += T->ptr[1]->width;                                      //用形参单元宽度修改函数局部变量的起始偏移量
            T->ptr[2]->offset = T->offset;
            strcpy(T->ptr[2]->Snext, newLabel()); //函数体语句执行结束后的位置属性
            generate_IR(T->ptr[2]);               //处理函数体结点
            //计算活动记录大小,这里offset属性存放的是活动记录大小，不是偏移
            symbolTable.symbols[T->ptr[1]->place].offset = T->offset + T->ptr[2]->width;
            T->code = merge(3, T->ptr[1]->code, T->ptr[2]->code, genLabel(T->ptr[2]->Snext)); //函数体的代码作为函数的代码
            break;
        case FUNC_DEC:                                                           //根据返回类型，函数名填写符号表
            rtn = fillSymbolTable(T->type_id, newAlias(), lev, T->type, 'F', 0); //函数不在数据区中分配单元，偏移量为0
            if (rtn == -1)
            {
                // semantic_error(T->pos,T->type_id, "函数重复定义");
                break;
            }
            else
                T->place = rtn;
            result.kind = ID;
            strcpy(result.id, T->type_id);
            result.offset = rtn;
            T->code = genIR(FUNCTION, opn1, opn2, result); //生成中间代码：FUNCTION 函数名
            T->offset = DX;                                //设置形式参数在活动记录中的偏移量初值
            if (T->ptr[0])
            { //判断是否有参数
                T->ptr[0]->offset = T->offset;
                generate_IR(T->ptr[0]); //处理函数参数列表
                T->width = T->ptr[0]->width;
                symbolTable.symbols[rtn].paramnum = T->ptr[0]->num;
                T->code = merge(2, T->code, T->ptr[0]->code); //连接函数名和参数代码序列
            }
            else
                symbolTable.symbols[rtn].paramnum = 0, T->width = 0;
            break;
        case PARAM_LIST: //处理函数形式参数列表
            T->ptr[0]->offset = T->offset;
            generate_IR(T->ptr[0]);
            if (T->ptr[1])
            {
                T->ptr[1]->offset = T->offset + T->ptr[0]->width;
                generate_IR(T->ptr[1]);
                T->num = T->ptr[0]->num + T->ptr[1]->num;             //统计参数个数
                T->width = T->ptr[0]->width + T->ptr[1]->width;       //累加参数单元宽度
                T->code = merge(2, T->ptr[0]->code, T->ptr[1]->code); //连接参数代码
            }
            else
            {
                T->num = T->ptr[0]->num;
                T->width = T->ptr[0]->width;
                T->code = T->ptr[0]->code;
            }
            break;
        case PARAM_DEC:
            rtn = fillSymbolTable(T->ptr[1]->type_id, newAlias(), 1, T->ptr[0]->type, 'P', T->offset); // 默认函数不嵌套
            if (rtn == -1)
            {
            }
            // semantic_error(T->ptr[1]->pos,T->ptr[1]->type_id, "参数名重复定义");
            else
                T->ptr[1]->place = rtn;
            T->num = 1;                                //参数个数计算的初始值
            T->width = T->ptr[0]->type == INT ? 4 : 8; //参数宽度
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[rtn].alias);
            result.offset = T->offset;
            T->code = genIR(PARAM, opn1, opn2, result); //生成：FUNCTION 函数名
            break;
        case COMP_STM:
            lev++;
            //设置层号加1，并且保存该层局部变量在符号表中的起始位置在symbol_scope_TX
            symbol_scope_TX.TX[symbol_scope_TX.top++] = symbolTable.index;
            T->width = 0;
            T->code = NULL;
            // if (T->ptr[0])
            // {
            //     T->ptr[0]->offset = T->offset;
            //     generate_IR(T->ptr[0]); //处理该层的局部变量DEF_LIST
            //     T->width += T->ptr[0]->width;
            //     T->code = T->ptr[0]->code;
            // }
            // if (T->ptr[1])
            // {
            //     T->ptr[1]->offset = T->offset + T->width;
            //     strcpy(T->ptr[1]->Snext, T->Snext); // S.next属性向下传递
            //     generate_IR(T->ptr[1]);             //处理复合语句的语句序列
            //     T->width += T->ptr[1]->width;
            //     T->code = merge(2, T->code, T->ptr[1]->code);
            // }
            if (T->ptr[0])
            { // def/stmt
                T->ptr[0]->offset = T->offset;
                strcpy(T->ptr[0]->Snext, T->Snext);
                strcpy(T->ptr[0]->Etrue, T->Etrue);
                strcpy(T->ptr[0]->Efalse, T->Efalse);
                generate_IR(T->ptr[0]);
                T->width += T->ptr[0]->width;
                T->code = T->ptr[0]->code;
            }
#if (DEBUG)
            prn_symbol(); // c在退出一个符合语句前显示的符号表
            system("pause");
#endif
            lev--;                                                         //出复合语句，层号减1
            symbolTable.index = symbol_scope_TX.TX[--symbol_scope_TX.top]; //删除该作用域中的符号
            break;

        case DEC_LIST:
            T->code = NULL;
            if (T->ptr[0])
            {
                T->ptr[0]->offset = T->offset;
                generate_IR(T->ptr[0]); //处理一个局部变量定义
                T->code = T->ptr[0]->code;
                T->width = T->ptr[0]->width;
            }
            if (T->ptr[1])
            {
                T->ptr[1]->offset = T->offset + T->ptr[0]->width;
                generate_IR(T->ptr[1]); //处理剩下的局部变量定义
                T->code = merge(2, T->code, T->ptr[1]->code);
                T->width += T->ptr[1]->width;
            }
            break;
        case VAR_DEF: //处理一个局部变量定义,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
                      //类似于上面的外部变量EXT_VAR_DEF，换了一种处理方法
            T->code = NULL;
            T->ptr[1]->type = !strcmp(T->ptr[0]->type_id, "int") ? INT : FLOAT; //确定变量序列各变量类型
            T0 = T->ptr[1];                                                     // T0为变量名列表子树根指针，对ID、ASSIGNOP类结点在登记到符号表，作为局部变量
            num = 0;
            T0->offset = T->offset;
            T->width = 0;
            width = T->ptr[1]->type == INT ? 4 : 8; //一个变量宽度

            while (T0)
            { //处理所以DEC_LIST结点
                num++;
                T0->ptr[0]->type = T0->type; //类型属性向下传递
                if (T0->ptr[1])
                    T0->ptr[1]->type = T0->type;
                T0->ptr[0]->offset = T0->offset; //类型属性向下传递
                if (T0->ptr[1])
                    T0->ptr[1]->offset = T0->offset + width;
                if (T0->ptr[0]->kind == TERM)
                {
                    if (T0->ptr[0]->ptr[0]) //数组
                    {
                        rtn = fillSymbolTable(T0->ptr[0]->type_id, newAlias(), lev, T0->ptr[0]->type, 'A', T->offset + T->width); //此处偏移量未计算，暂时为0
                    }
                    else
                    {
                        rtn = fillSymbolTable(T0->ptr[0]->type_id, newAlias(), lev, T0->ptr[0]->type, 'V', T->offset + T->width); //此处偏移量未计算，暂时为0
                    }
                    if (rtn == -1)
                    {
                    }
                    // semantic_error(T0->ptr[0]->pos,T0->ptr[0]->type_id, "变量重复定义");
                    else
                        T0->ptr[0]->place = rtn;
                    if (T0->ptr[0]->ptr[0]) //数组
                    {
                        struct node *temp = T0->ptr[0]->ptr[0];
                        int total_num = 1;
                        while (temp)
                        {
                            total_num *= temp->ptr[0]->type_int;
                            int i = symbolTable.symbols[rtn].paramnum;
                            symbolTable.symbols[rtn].param_type[i] = temp->ptr[0]->type_int;
                            symbolTable.symbols[rtn].paramnum++;
                            temp = temp->ptr[1];
                        }
                        T->width += width * total_num;
                    }
                    else
                    {
                        T->width += width;
                    }
                }
                else if (T0->ptr[0]->kind == ASSIGN)
                {
                    rtn = fillSymbolTable(T0->ptr[0]->ptr[0]->type_id, newAlias(), lev, T0->ptr[0]->type, 'V', T->offset + T->width); //此处偏移量未计算，暂时为0
                    if (rtn == -1)
                    {
                    }
                    // semantic_error(T0->ptr[0]->ptr[0]->pos,T0->ptr[0]->ptr[0]->type_id, "变量重复定义");
                    else
                    {
                        T0->ptr[0]->place = rtn;
                        T0->ptr[0]->ptr[1]->offset = T->offset + T->width + width;
                        Exp(T0->ptr[0]->ptr[1]);
                        opn1.kind = ID;
                        strcpy(opn1.id, symbolTable.symbols[T0->ptr[0]->ptr[1]->place].alias);
                        result.kind = ID;
                        strcpy(result.id, symbolTable.symbols[T0->ptr[0]->place].alias);
                        T->code = merge(3, T->code, T0->ptr[0]->ptr[1]->code, genIR(ASSIGN, opn1, opn2, result));
                    }
                    T->width += width + T0->ptr[0]->ptr[1]->width;
                }
                T0 = T0->ptr[1];
            }
            break;
        case STM_DEF_LIST:
            if (!T->ptr[0])
            {
                T->code = NULL;
                T->width = 0;
                break;
            }              //空语句序列
            if (T->ptr[1]) // 2条以上语句连接，生成新标号作为第一条语句结束后到达的位置
                strcpy(T->ptr[0]->Snext, newLabel());
            else //语句序列仅有一条语句，S.next属性向下传递
            {
                strcpy(T->ptr[0]->Snext, T->Snext);
                strcpy(T->ptr[0]->Etrue, T->Etrue);
                strcpy(T->ptr[0]->Efalse, T->Efalse);
            }

            T->ptr[0]->offset = T->offset;
            generate_IR(T->ptr[0]);
            T->code = T->ptr[0]->code;
            T->width = T->ptr[0]->width;

            if (T->ptr[1])
            { // 2条以上语句连接,S.next属性向下传递
                strcpy(T->ptr[1]->Snext, T->Snext);
                strcpy(T->ptr[1]->Etrue, T->Etrue);
                strcpy(T->ptr[1]->Efalse, T->Efalse);
                T->ptr[1]->offset = T->offset; //顺序结构共享单元方式
                                               //                  T->ptr[1]->offset=T->offset+T->ptr[0]->width; //顺序结构顺序分配单元方式
                generate_IR(T->ptr[1]);
                //序列中第1条为表达式语句，返回语句，复合语句时，第2条前不需要标号???
                if (T->ptr[0]->kind == RETURN || T->ptr[0]->kind == EXP_STMT || T->ptr[0]->kind == COMP_STM)
                    T->code = merge(2, T->code, T->ptr[1]->code);
                else
                    //   T->code = merge(3, T->code, genLabel(T->ptr[0]->Snext), T->ptr[1]->code);
                    T->code = merge(2, T->code, T->ptr[1]->code);
                if (T->ptr[1]->width > T->width)
                    T->width = T->ptr[1]->width; //顺序结构共享单元方式
                                                 //                        T->width+=T->ptr[1]->width;//顺序结构顺序分配单元方式
            }
            break;
        case IF_THEN:
            strcpy(T->ptr[0]->Etrue, newLabel()); //设置条件语句真假转移位置
            strcpy(T->ptr[0]->Efalse, T->Snext);
            T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
            boolExp(T->ptr[0]);
            T->width = T->ptr[0]->width;
            strcpy(T->ptr[1]->Snext, T->Snext);
            strcpy(T->ptr[1]->Etrue, T->Etrue);
            strcpy(T->ptr[1]->Efalse, T->Efalse);
            generate_IR(T->ptr[1]); // if子句
            if (T->width < T->ptr[1]->width)
                T->width = T->ptr[1]->width;
            T->code = merge(3, T->ptr[0]->code, genLabel(T->ptr[0]->Etrue), T->ptr[1]->code);
            break; //控制语句都还没有处理offset和width属性
        case IF_THEN_ELSE:
            strcpy(T->ptr[0]->Etrue, newLabel()); //设置条件语句真假转移位置
            strcpy(T->ptr[0]->Efalse, newLabel());
            T->ptr[0]->offset = T->ptr[1]->offset = T->ptr[2]->offset = T->offset;
            boolExp(T->ptr[0]); //条件，要单独按短路代码处理
            T->width = T->ptr[0]->width;
            strcpy(T->ptr[1]->Snext, T->Snext);
            generate_IR(T->ptr[1]); // if子句
            if (T->width < T->ptr[1]->width)
                T->width = T->ptr[1]->width;
            strcpy(T->ptr[2]->Snext, T->Snext);
            generate_IR(T->ptr[2]); // else子句
            if (T->width < T->ptr[2]->width)
                T->width = T->ptr[2]->width;
            T->code = merge(6, T->ptr[0]->code, genLabel(T->ptr[0]->Etrue), T->ptr[1]->code,
                            genGoto(T->Snext), genLabel(T->ptr[0]->Efalse), T->ptr[2]->code);
            break;
        case WHILE:
            islooping = 1;
            strcpy(T->ptr[0]->Etrue, newLabel()); //子结点继承属性的计算
            strcpy(T->ptr[0]->Efalse, T->Snext);
            T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
            boolExp(T->ptr[0]); //循环条件，要单独按短路代码处理
            T->width = T->ptr[0]->width;
            strcpy(T->ptr[1]->Snext, newLabel());
            strcpy(T->ptr[1]->Etrue, T->ptr[0]->Etrue);
            strcpy(T->ptr[1]->Efalse, T->ptr[0]->Efalse);
            generate_IR(T->ptr[1]); //循环体
            if (T->width < T->ptr[1]->width)
                T->width = T->ptr[1]->width;
            T->code = merge(6, genLabel(T->ptr[1]->Snext), T->ptr[0]->code,
                            genLabel(T->ptr[0]->Etrue), T->ptr[1]->code, genGoto(T->ptr[1]->Snext), genLabel(T->ptr[0]->Efalse));
            islooping = 0;
            break;
        case EXP_STMT:
            T->ptr[0]->offset = T->offset;
            generate_IR(T->ptr[0]);
            T->code = T->ptr[0]->code;
            T->width = T->ptr[0]->width;
            break;
        case RETURN:
            if (T->ptr[0])
            {
                T->ptr[0]->offset = T->offset;
                Exp(T->ptr[0]);

                /*需要判断返回值类型是否匹配*/

                T->width = T->ptr[0]->width;
                result.kind = ID;
                strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
                result.offset = symbolTable.symbols[T->ptr[0]->place].offset;
                T->code = merge(2, T->ptr[0]->code, genIR(RETURN, opn1, opn2, result));
            }
            else
            {
                T->width = 0;
                result.kind = 0;
                T->code = genIR(RETURN, opn1, opn2, result);
            }
            break;
        case TERM:
        case INT:
        case FLOAT:
        case ASSIGN:
        case AND:
        case OR:
        case RELOP:
        case ADD:
        case MINUS:
        case MUL:
        case DIV:
        case MOD:
        case NOT:
        case UMINUS:
        case FUNC_CALL:
            Exp(T); //处理基本表达式
            break;
        case CONTINUE_STMT:
            T->code = genGoto(T->Etrue);
            break;
        case BREAK_STMT:
            T->code = genGoto(T->Efalse);
            break;
        }
    }
}

void generate_IR0(struct node *T)
{
    symbolTable.index = 0;
    // fillSymbolTable("read", "", 0, INT, 'F', 4);
    // symbolTable.symbols[0].paramnum = 0; // read的形参个数
    // fillSymbolTable("write", "", 0, INT, 'F', 4);
    // symbolTable.symbols[1].paramnum = 1;
    // fillSymbolTable("x", "", 1, INT, 'P', 12);
    fillSymbolTable("putint", "", 0, INT, 'F', 4);
    symbolTable.symbols[0].paramnum = 1;
    // fillSymbolTable("x", "", 1, INT, 'P', 12);
    symbol_scope_TX.TX[0] = 0; //外部变量在符号表中的起始序号为0
    symbol_scope_TX.top = 1;
    T->offset = 0; //外部变量在数据区的偏移量
    generate_IR(T);
    printIR(T->code);
    // objectCode(T->code);
}
