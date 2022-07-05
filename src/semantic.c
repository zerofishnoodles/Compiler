#include "def.h"

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
    case ARRAYS:  // 对维度处理
      if(T != NULL && tmp_symbols[0].cmd == ARRAY_DEC ){
        tmp_symbols[0].flag = 'A';
      }
      if(tmp_symbols[0].cmd == ARRAY_DEC) {
        T0 = T;
        i = 0;
        while (T0 != NULL)
        {
          // printf("%*c第%d维: \n", indent, ' ', i++);
          // semantic_analysis(T0->ptr[0]);
          if(T0->ptr[0]->type != INT){
            semantic_error(T0->pos, " array subscript is not an integer", "",2);
          }else{
            tmp_symbols[0].param_type[i] = T0->ptr[0]->type_int;
          }
          T0 = T0->ptr[1];
          i++;
        }
        tmp_symbols[0].paramnum = i;
      }else if(tmp_symbols[0].cmd == ASSIGN) {
        T0 = T;
        while (T0 != NULL)
        {
          // printf("%*c第%d维: \n", indent, ' ', i++);
          // semantic_analysis(T0->ptr[0]);
          if(T0->ptr[0]->type != INT){  
            semantic_error(T0->pos, " array subscript is not an integer", "",2);
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
      if(tmp_symbols[0].cmd == EXT_CONST_VAR_DEF) tmp_symbols[0].flag = 'C';
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
      if(tmp_symbols[0].cmd == EXT_CONST_VAR_DEF) tmp_symbols[0].flag = 'C';
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
      if (T->ptr[1]){
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
      for(i = symbolTable.index-1; i>=0; i--){
        if(symbolTable.symbols[i].flag == 'F') break;
      }
      if(symbolTable.symbols[i].type == VOID){
        semantic_error(T->pos, " ‘return’ with a value, in function returning void", "",2);
      }
      break;
    case CONTINUE_STMT:
      // printf("%*c继续语句\n", indent, ' ');
	if(islooping == 0){
		semantic_error(T->pos, " continue statement not within a loop", "",2);
	}
      break;
    case BREAK_STMT:
      // printf("%*c打断语句\n", indent, ' ');
	if(islooping == 0){
		semantic_error(T->pos, " break statement not within a loop", "",2);
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
      if(i == -1){  // 没找到
        semantic_error(T->pos, T->type_id, " undeclared",0);
      }else if(symbolTable.symbols[i].flag != 'F'){  // 不是函数
        semantic_error(T->pos, T->type_id, " is not a function",0);
      }else{
        temp = semantic_analysis(T->ptr[0]);
        if(temp < symbolTable.symbols[i].paramnum){  // 参数不对
          semantic_error(T->pos, "too few arguments to function", T->type_id,1);
        }else if(temp > symbolTable.symbols[i].paramnum){   
          semantic_error(T->pos, "too many arguments to function", T->type_id,1);
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
      while(T0){
        if (T0->kind == INIT_LIST && T0->type != INT && T0->type != FLOAT){
          i++;
        }else if((T0->type^tmp_symbols[0].type) != 0){
          semantic_error(T0->pos, "conflicting init types for", tmp_symbols[0].name,1);  
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
	if(flag == 0){  //前面打‘’
		sprintf(buf, "%s:%d ‘%s’%s\n", filename, pos, msg1, msg2);
	}else if(flag==1){ //后面打‘’
		sprintf(buf, "%s:%d %s ‘%s’\n", filename, pos, msg1, msg2);
	}else{  // 不打
		sprintf(buf, "%s:%d%s%s\n", filename, pos, msg1, msg2);
	}
	if(strcmp(buf, last_error_line) == 0) return;
	else if(strcmp("putint", msg1) == 0) return;
	else{
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
    if(i == -1){
      semantic_error(T->pos, T->type_id, " undeclared", 0);  // 不考虑类型转换
    }else if(symbolTable.symbols[i].flag == 'F'){
      semantic_error(T->pos, " lvalue required as left operand of assignment", "", 2);
    }else if(symbolTable.symbols[i].flag == 'A' && T->ptr[0] == NULL){
      semantic_error(T->pos, " assignment to expression with array type","",2);
    }
    break;
  case VAR_DEC:
    if(i == -1){
      semantic_error(T->pos, T->type_id, " undeclared", 0);  // 不考虑类型转换
    }else if(T->ptr[0]!=NULL && symbolTable.symbols[i].flag != 'A'){
	semantic_error(T->pos, " subscripted value is not an array", "", 2);
    }
    break;
  case ADD:
    if(i == -1){
      semantic_error(T->pos, T->type_id, " undeclared",0);  // 不考虑类型转换
    }else if(symbolTable.symbols[i].flag == 'F'){
      semantic_error(T->pos, " invalid operands to binary operator ", "+", 2);
    }
    break;
  case MINUS:
    if(i == -1){
      semantic_error(T->pos, T->type_id, " undeclared", 0);  // 不考虑类型转换
    }else if(symbolTable.symbols[i].flag == 'F'){
      semantic_error(T->pos, " invalid operands to binary operator ", "-",2);
    }
    break;
  case MUL:
    if(i == -1){
      semantic_error(T->pos, T->type_id, " undeclared", 0);  // 不考虑类型转换
    }else if(symbolTable.symbols[i].flag == 'F'){
      semantic_error(T->pos, " invalid operands to binary operator ", "*", 2);
    }
    break;
  case DIV:
    if(i == -1){
      semantic_error(T->pos, T->type_id, " undeclared", 0);  // 不考虑类型转换
    }else if(symbolTable.symbols[i].flag == 'F'){
      semantic_error(T->pos, " invalid operands to binary operator ", "/",2);
    }
    break;
  case MOD:
    if(i == -1){
      semantic_error(T->pos, T->type_id, " undeclared", 0);  // 不考虑类型转换
    }else if(symbolTable.symbols[i].flag == 'F'){
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