#include "def.h"

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
  int cur_lev = lev;   // 给形参的特判 因为形参还没到复合语句内，但是处于下一层 我知道这代码写的像shi。。。别骂了
  if(tmp_symbol->flag == 'P') cur_lev++;
  if(i != -1 && symbolTable.symbols[i].level == cur_lev) {
    if(symbolTable.symbols[i].flag != tmp_symbol->flag){
      semantic_error(tmp_symbol->pos, tmp_symbol->name, " redeclared as different kind of symbol",0);
    }
    else if(symbolTable.symbols[i].type != tmp_symbol->type){
      semantic_error(tmp_symbol->pos, "conflicting types for", tmp_symbol->name,1);
    }else{
      semantic_error(tmp_symbol->pos, "redefinition of", tmp_symbol->name,1);
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