#include "def.h"


void prn_symbol(){ //显示符号表
    int i=0;
    printf("%6s %6s %6s  %6s %4s %6s\n","变量名","别 名","层 号","类  型","标记","偏移量");
    for(i=0;i<symbolTable.index;i++)
        printf("%6s %6s %6d  %6s %4c %6d\n",symbolTable.symbols[i].name,\
                symbolTable.symbols[i].alias,symbolTable.symbols[i].level,\
                symbolTable.symbols[i].type==INT?"int":"float",\
                symbolTable.symbols[i].flag,symbolTable.symbols[i].offset);
}


int searchSymbolTable(char *name) {
    int i,flag=0;
    for(i=symbolTable.index-1;i>=0;i--){
        if (symbolTable.symbols[i].level==0)
            flag=1;
        if (flag && symbolTable.symbols[i].level==1)
            continue;   //跳过前面函数的形式参数表项
        if (!strcmp(symbolTable.symbols[i].name, name))  return i;
    }
    return -1;
}


//填写临时变量到符号表，返回临时变量在符号表中的位置
int fill_Temp(char *name,int level,int type,char flag,int offset) {
    strcpy(symbolTable.symbols[symbolTable.index].name,"");
    strcpy(symbolTable.symbols[symbolTable.index].alias,name);
    symbolTable.symbols[symbolTable.index].level=level;
    symbolTable.symbols[symbolTable.index].type=type;
    symbolTable.symbols[symbolTable.index].flag=flag;
    symbolTable.symbols[symbolTable.index].offset=offset;
    return symbolTable.index++; //返回的是临时变量在符号表中的位置序号
}


int fillSymbolTable(char *name,char *alias,int level,int type,char flag,int offset) {
    //首先根据name查符号表，不能重复定义 重复定义返回-1
    int i;
    /*符号查重，考虑外部变量声明前有函数定义，
    其形参名还在符号表中，这时的外部变量与前函数的形参重名是允许的*/
    for(i=symbolTable.index-1; i>=0 && (symbolTable.symbols[i].level==level||level==0); i--) {
        if (level==0 && symbolTable.symbols[i].level==1) continue;  //外部变量和形参不必比较重名
        if (!strcmp(symbolTable.symbols[i].name, name))  return -1;
        }
    //填写符号表内容
    strcpy(symbolTable.symbols[symbolTable.index].name,name);
    strcpy(symbolTable.symbols[symbolTable.index].alias,alias);
    symbolTable.symbols[symbolTable.index].level=level;
    symbolTable.symbols[symbolTable.index].type=type;
    symbolTable.symbols[symbolTable.index].flag=flag;
    symbolTable.symbols[symbolTable.index].offset=offset;
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