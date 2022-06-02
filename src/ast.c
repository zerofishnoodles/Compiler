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
    }
  }
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
  // search symtable

  // insert
  int index = symbolTable.index;
  symbolTable.symbols[index].level = lev;
  if(tmp_symbol->flag == 'P') symbolTable.symbols[index].level++;
  strcpy(symbolTable.symbols[index].name, tmp_symbol->name);
  strcpy(symbolTable.symbols[index].alias, tmp_symbol->alias);
  symbolTable.symbols[index].flag = tmp_symbol->flag;
  symbolTable.symbols[index].offset = tmp_symbol->offset;
  symbolTable.symbols[index].paramnum = tmp_symbol->paramnum;
  symbolTable.symbols[index].type = tmp_symbol->type;
  memcpy(symbolTable.symbols[index].param_type, tmp_symbol->param_type, sizeof(int)*MAXPARAMNUM);
  if(tmp_symbol->flag == 'F'){
    char type[256];
    strcpy(type, tmp_symbol->type == INT ? "int(" : "float(");
    if(tmp_symbol->paramnum > 0){
      for(int j = 1;j<=tmp_symbol->paramnum;j++) {
        strcat(type, tmp_symbol->param_type[j] == INT ? "int," : "float,");
      }
    }else{
      strcat(type, "void,");
    }
    int length = strlen(type);
    type[length-1] = ')';
    strcpy(symbolTable.symbols[index].fun_type, type);
  }
  symbolTable.index++;
  return 1;
}

void build_symtable(struct node *T)
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
      build_symtable(T->ptr[0]); //显示该外部定义列表中的第一个
      build_symtable(T->ptr[1]); //显示该外部定义列表中的其它外部定义
      break;
    case EXT_VAR_DEF:
      // printf("%*c外部变量定义:\n", indent, ' ');
      build_symtable(T->ptr[0]); //显示外部变量类型
      // printf("%*c定义的变量:\n", indent + 3, ' ');
      build_symtable(T->ptr[1]); //显示变量列表
      if(T->ptr[1]->kind == TERM) {
        insert_symtable(&tmp_symbols[0]);
        DisplaySymbolTable();
        memset(&tmp_symbols[0], 0, sizeof(struct symbol));
      }
      break;
    case EXT_CONST_VAR_DEF:
        // printf("%*c外部常量定义:\n", indent, ' ');
        // display(T->ptr[0], indent + 3); //显示外部常量类型
        // printf("%*c定义的常量:\n", indent + 3, ' ');
        // display(T->ptr[1], indent + 6); //显示常量列表
      break;
    case TERM:
      // if (T->ptr[0])
      // {
      //   // printf("%*c数组: ", indent, ' ');
      // }
      // else
      // {
      //   // printf("%*cID: ", indent, ' ');
      // }
      tmp_symbols[0].flag = 'V';
      strcpy(tmp_symbols[0].name, T->type_id);
      build_symtable(T->ptr[0]);
      break;
    case ARRAYS:
      // T0 = T;
      // i = 1;
      // while (T0 != NULL)
      // {
      //   printf("%*c第%d维: \n", indent, ' ', i++);
      //   display(T0->ptr[0], indent + 3);
      //   T0 = T0->ptr[1];
      // }
      break;
    case TYPE:
    case VOID:
      // printf("%*c类型: %s\n", indent, ' ', T->type_id);
      tmp_symbols[0].type = T->type;
      break;
    case EXT_DEC_LIST:
      tmp_symbols[0].flag = 'V';
      build_symtable(T->ptr[0]); //依次显示外部变量名，
      insert_symtable(&tmp_symbols[0]);
      memset(&tmp_symbols[0], 0, sizeof(struct symbol));
      build_symtable(T->ptr[1]); //后续还有相同的，仅显示语法树此处理代码可以和类似代码合并
      DisplaySymbolTable();
      break;
    case VAR_DEC:
      // printf("%*c变量名: %s\n", indent, ' ', T->type_id);
      strcpy(tmp_symbols[0].name, T->type_id);
      // if (T->ptr[0])
      // {
      //   printf("%*c它的初值:\n", indent + 3, ' ');
      //   display(T->ptr[0], indent + 6); //表达式初值
      // }
      break;
    case FUNC_DEF:
      // printf("%*c函数定义: \n", indent, ' ');
      tmp_symbols[0].flag = 'F';
      build_symtable(T->ptr[0]); //显示函数返回类型
      build_symtable(T->ptr[1]); //显示函数名和参数
      paramnum = tmp_symbols[0].paramnum;
      insert_symtable(&tmp_symbols[0]);
      memset(&tmp_symbols[0], 0, sizeof(struct symbol));
      DisplaySymbolTable();
      for(int j=1;j<=paramnum;j++) {
        insert_symtable(&tmp_symbols[j]);
        memset(&tmp_symbols[j], 0, sizeof(struct symbol));
        DisplaySymbolTable();
      }
      build_symtable(T->ptr[2]); //显示函数体
      break;
    case FUNC_DEC:
      // printf("%*c函数名: %s\n", indent, ' ', T->type_id);
      strcpy(tmp_symbols[0].name, T->type_id);
      if (T->ptr[0])
      {
        // printf("%*c函数形参: \n", indent, ' ');
        build_symtable(T->ptr[0]); //显示函数参数列表
      }
      // else
        // printf("%*c无参函数\n", indent + 3, ' ');
        break;
    case PARAM_LIST:
      tmp_symbols[0].paramnum++;
      build_symtable(T->ptr[0]); //依次显示全部参数类型和名称，
      build_symtable(T->ptr[1]);
      break;
    case PARAM_DEC:
      // 目前不支持有缺省值
      // printf("%*c类型: %s, 参数名: %s\n", indent, ' ', T->ptr[0]->type == INT ? "int" : "float", T->ptr[1]->type_id);
      paramnum = tmp_symbols[0].paramnum;
      tmp_symbols[0].param_type[paramnum] = T->ptr[0]->type;  // 从1开始给
      strcpy(tmp_symbols[paramnum].name, T->ptr[1]->type_id);
      tmp_symbols[paramnum].type = T->ptr[0]->type;
      tmp_symbols[paramnum].flag = 'P';
      break;
    case EXP_STMT:
      // printf("%*c表达式语句: \n", indent, ' ');
      // display(T->ptr[0], indent + 3);
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
      break;
    case CONTINUE_STMT:
      // printf("%*c继续语句\n", indent, ' ');
      break;
    case BREAK_STMT:
      // printf("%*c打断语句\n", indent, ' ');
      break;
    case COMP_STM:
      lev++;
      top = symbol_scope_TX.top;
      paramnum = 0;
      for(i=symbolTable.index-1;i>=0;i--){
        if(symbolTable.symbols[i].flag == 'F' && symbolTable.symbols[i].level == lev-1) {
          paramnum = symbolTable.symbols[i].paramnum;
          break;
        }
      }
      symbol_scope_TX.TX[top] = symbolTable.index-paramnum;
      symbol_scope_TX.top++;
      if (T->ptr[0])
      {
        // printf("%*c复合语句: \n", indent, ' ');
        build_symtable(T->ptr[0]);
      }
      // else
      // {
      //   // printf("%*c空复合语句\n", indent, ' ');
      // }
      top = symbol_scope_TX.top;
      temp = symbolTable.index;
      symbolTable.index = symbol_scope_TX.TX[top-1];
      symbol_scope_TX.top--;
      lev--;
      if(temp != symbolTable.index) DisplaySymbolTable();
      break;
    case STM_DEF_LIST:
      build_symtable(T->ptr[0]); //显示第一条语句
      build_symtable(T->ptr[1]); //显示剩下语句
      break;
    case WHILE:
        // printf("%*c循环语句: \n", indent, ' ');
        // printf("%*c循环条件: \n", indent + 3, ' ');
        // display(T->ptr[0], indent + 6); //显示循环条件
        // printf("%*c循环体: \n", indent + 3, ' ');
        build_symtable(T->ptr[1]); //显示循环体
      break;
    case FOR_STMT:
      // printf("%*cfor 循环语句: \n", indent, ' ');
      // build_symtable(T->ptr[0]);
      // printf("%*c循环体: \n", indent + 3, ' ');
      build_symtable(T->ptr[1]);
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
      build_symtable(T->ptr[1]); //显示if子句
      break;
    case IF_THEN_ELSE:
      // printf("%*c条件语句(IF_THEN_ELSE): \n", indent, ' ');
      // printf("%*c条件: \n", indent + 3, ' ');
      // display(T->ptr[0], indent + 6); //显示条件
      // printf("%*cIF子句: \n", indent + 3, ' ');
      build_symtable(T->ptr[1]); //显示if子句
      // printf("%*cELSE子句: \n", indent + 3, ' ');
      build_symtable(T->ptr[2]); //显示else子句
      break;
    case VAR_DEF:
      // printf("%*c局部变量定义: \n", indent, ' ');
      build_symtable(T->ptr[0]); //显示变量类型
      // printf("%*c定义的变量:\n", indent + 3, ' ');
      build_symtable(T->ptr[1]); //显示该定义的全部变量名
      DisplaySymbolTable();
      break;
    case DEC_LIST:
      tmp_symbols[0].flag = 'V';
      build_symtable(T->ptr[0]);
      insert_symtable(&tmp_symbols[0]);
      temp = tmp_symbols[0].type;
      memset(&tmp_symbols[0], 0, sizeof(struct symbol));
      tmp_symbols[0].type = temp;
      build_symtable(T->ptr[1]);
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
    case AND:
    case OR:
    case RELOP:
    case ADD:
    case MINUS:
    case MUL:
    case DIV:
    case MOD:
      // printf("%*c%s\n", indent, ' ', T->type_id);
      // display(T->ptr[0], indent + 3);
      // display(T->ptr[1], indent + 3);
      break;
    case NOT:
    case UMINUS:
      // printf("%*c%s\n", indent, ' ', T->type_id);
      // display(T->ptr[0], indent + 3);
      break;
    case FUNC_CALL:
      // printf("%*c函数调用: \n", indent, ' ');
      // printf("%*c函数名: %s\n", indent + 3, ' ', T->type_id);
      // display(T->ptr[0], indent + 3);
      break;
    case ARGS:
      // i = 1;
      // while (T)
      // { // ARGS表示实际参数表达式序列结点，其第一棵子树为其一个实际参数表达式，第二棵子树为剩下的。
      //   struct node *T0 = T->ptr[0];
      //   printf("%*c第%d个实际参数表达式: \n", indent, ' ', i++);
      //   display(T0, indent + 3);
      //   T = T->ptr[1];
      // }
      // //                  printf("%*c第%d个实际参数表达式: \n",indent,' ',i);
      // //                  display(T,indent+3);
      // // printf("\n");
      break;
    case SELF_ADD_EXP:
      // printf("%*c后置自增: \n", indent, ' ');
      // printf("%*c变量: %s\n", indent + 3, ' ', T->type_id);
      break;
    case SELF_MINUS_EXP:
      // printf("%*c后置自减: \n", indent, ' ');
      // printf("%*c变量: %s\n", indent + 3, ' ', T->type_id);
      break;
    }
  }
}
