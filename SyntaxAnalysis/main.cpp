/*
begin if((a+b))>=7 {a:=8;}  else {t:=45;} end$
begin int a:=2; bool d; d:=false; end$
begin a:=9 x:=2*3; b:=a+x; end$
begin a:=7; end$
x:=a+b*c end$
*/

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<stack>
#include<cstring>
using namespace std;
// ps:如果要增加或删除关键字，记得修改函数int FindKeyWord();中的循环终止条件,以及查找界符种别码函数中的22+i
static char KeyWord[24][20]={
  "begin","if","then","while","do","end","int","long","break","case",
  "char","continue","default","double","else","float","goto","return",
  "void","switch","static","true","false","bool"
};
//目前只有16个运算符和界符  ps:如果要增加或删除运算符和界符，记得修改函数int中的循环终止条件
int FindOperator_Boundary(char sub[]);
static char Operator_Boundary[26][10]={
    "+","-","*","/",":",":=","<","<>","<=",">",">=","=",";","(",")","#","!=",">>","<<","&&","&","||","]","[","{","}"};
//判断当前字符是否为数字
bool isDigit(char c);
//判断是否为字母
bool isLetter(char c);
//查找是否为关键字，若是则返回对应的种别码，否则返回-1，表示不是关键字，是标识符
int FindKeyWord(char sub[]);
//查找是否为运算符或界符
int FindOperator_Boundary(char sub[]);
//用于提取有用代码
void extractCode(char Source[],int n);
void Lexical_Analysis(int &syn,int &p,char SourceFile[],char token[],int &q,char TargetFile[]);
char LL1[100][100][100]={{"->bTe","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","bull","null","null","null","null","null","null"},
                       {"null","null","null","->XY","null","null","null","null","null","null","null","->AY","null","null","null","null","null","null","null","->WY","->QY","->QY","->QY","null","null","->AY","->AY"},
                       {"null","null","null","->i:S;","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null"},
                       {"null","->#","null","->XY","null","null","null","null","null","null","null","->AY","null","null","null","null","null","->#","null","->WY","->QY","->QY","->QY","null","null","->AY","->AY"},
                       {"null","null","null","null","null","null","null","null","null","null","null","->fBDH","null","null","null","null","null","null","null","null","null","null","null","null","null","->wBD","->dDwB"},
                       {"null","null","null","->IL","null","null","null","null","null","->IL","null","null","null","null","null","null","null","null","->IL","null","null","null","null","null","null","null","null"},
                       {"null","null","null","->MN","null","null","null","null","null","->MN","null","null","null","null","null","null","null","null","->MN","null","null","null","null","null","null","null","null"},
                       {"null","null","->#","null","null","->+IL","->-IL","null","null","null","->#","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null"},
                       {"null","null","null","->i","null","null","null","null","null","->(Z)","null","null","null","null","null","null","null","null","->n","null","null","null","null","null","null","null","null"},
                       {"null","null","->#","null","null","->#","->#","->*MN","->/MN","null","->#","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null"},
                       {"null","null","null","null","null","null","null","null","null","null","null","null","->>","-><","->=","null","null","null","null","null","null","null","null","null","null","null","null"},
                       {"null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","->{T}","null","null","null","null","null","null","null","null","null","null"},
                       {"null","->#","null","->#","null","null","null","null","null","null","null","->#","null","null","null","->sD","null","->#","null","->#","->#","->#","->#","","","->#","->#"},
                       {"null","null","null","null","null","null","null","null","null","->(G)","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null"},
                       {"null","null","null","->MFM","null","null","null","null","null","->MFM","null","null","null","null","null","null","null","null","->MFM","null","null","null","null","null","null","null","null"},
                       {"null","null","null","->iJ","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null"},
                       {"null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","->lV","null","null","null","null","null","null","null"},
                       {"null","null","->;","","->:K;","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null"},
                       {"null","null","null","->iC","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null"},
                       {"null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","->r","->m","null","null"},
                       {"null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","->tR","->oR","->uR","null","null","null","null"},
                       {"null","null","->;","null","->:Z;","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null","null"},
                       {"null","null","null","->Z","null","null","null","null","null","->Z","null","null","null","null","null","null","null","null","->Z","null","null","null","null","->K","->K","null","null"}
                        };
char NonTerminal[200]="ETXYAZILMNFDHBGVWJRKQCS";
char Terminator[200]="be;i:+-*/()f><=s{}nltourmwd";
stack<char>temp;
int error(int i,int cnt, int len, char p[],char str[]);
int search_NonTerminal(char x);
int search_Terminator(char x);
void Analysis(char str[],int len);

int main()
{
    char SourceFile[2000];
    char TargetFile[2000];
    char ch;
    int q=0; //相当于TargetFile数组的下标
    char token[20];//存储单词记号
    int syn=-1;//种别码
    int p=0;//用于访问文件的指针,相当于SourceFile[2000]数组的下标
    do{
        scanf("%c",&ch);
        SourceFile[p++]=ch;
    }while(ch!='$');
    SourceFile[p]='\0';
    //提取出真正的代码（即去除回车，注释等无效字符）
    extractCode(SourceFile,p);
    p=0;
    TargetFile[q]='\0';
    while(syn!=0)
    {
        //依次分析
        Lexical_Analysis(syn,p,SourceFile,token,q,TargetFile);
        if(syn==0)
            continue;
    }
    TargetFile[q++]='$';
    TargetFile[q]='\0';

    int len = strlen(TargetFile);
    temp.push('$');
    temp.push('E'); //开始符号入栈
    Analysis(TargetFile,len+1);
    return 0;
}

int search_NonTerminal(char x)
{
    for(int i = 0; i < 23; i++)//找到对应行,返回行号
    {
        if(x==NonTerminal[i])
        {
            return i;
        }
    }
    return -1;
}
int search_Terminator(char x)
{
    for(int i = 0; i < 27; i++)//找到对应列，返回列号
    {
        if(x==Terminator[i])
        {
            return i;
        }
    }
    return -1;
}
int error(int i,int cnt, int len, char p[],char str[])
{
    printf("%d\t%s\t",cnt,p);
    for(int q = i; q<len;q++)
    {
        cout<<str[q];
    }
    printf("\t报错\n");
    return len;
}
void Analysis(char str[],int len)
{
    int cnt = 1;//输出Step专用
    int i = 0;
    char p[200] = "$E";//模拟栈，已达到输出栈中当前内容的功能，初始状态只有开始符号，$表示栈空，字符串分析完毕
    int index = 2;  //模拟栈顶指针，指向当前有效字符存放的下一个位置（从0开始）
    printf("Step\tStack\tString\tRule\n");
    while(i<len)
    {
        int x,y;
        char ch = temp.top();
        if(ch>='A'&&ch<='Z')  //说明栈顶为非终结符,需要按照产生式规约
        {
            temp.pop();   //弹出非终结符
            x = search_NonTerminal(ch);   //找到预测分析表中对应非终结符所属的行
            y = search_Terminator(str[i]);  //找到预测分析表中当前看到的终结符的那一列
            if(x!=-1&&y!=-1)   //找到对应表项
            {
                int len2 = strlen(LL1[x][y]);
                if(strcmp(LL1[x][y],"null")==0)  //出错，分析表中对应的项为空
                {
                    i = error(i,cnt,len,p,str);
                    continue;
                }
                printf("%d\t%s\t",cnt,p);  //输出当前操作的步骤序号以及当前栈中的内容
                if(p[index-1] != '$')  //栈未空
                {
                    p[index] = '\0';  //将刚刚弹出栈的符号覆盖
                    index--;          //栈顶指针下移
                }
                if(LL1[x][y][2]!='#')  //产生式不推出空(即不为"->#")时
                {
                    for(int q = len2-1; q>1; q--)  //逆向将产生式入栈
                    {
                        p[index++] = LL1[x][y][q];
                        temp.push(LL1[x][y][q]);
                    }
                }
                else
                {
                    p[index] = '\0';  //产生式为"->#"时，不入栈,直接将当前栈顶覆盖即可
                    //index--;
                }
                for(int q = i; q<len;q++)  //输出剩余的还未分析的串
                {
                    cout<<str[q];
                }
                printf("\t%c%s\n",ch,LL1[x][y]);  //输出应使用的产生式
            }
            else   //未在分析表中找到对应的符号
            {
                i = error(i,cnt,len,p,str);
                continue;
            }

        }
        else  //如果当前栈顶为终结符
        {
            if(ch==str[i])  //如果栈顶元素与当前看到的非终结符匹配
            {
                temp.pop();    //非终结符出栈
                printf("%d\t%s\t",cnt,p);
                if(ch=='$'&&str[i]=='$')  //全部分析完毕
                {
                    printf("$\t接受\n");
                    return ;
                }
                for(int q = i; q<len;q++)  //输出剩余还未分析的串
                {
                    cout<<str[q];
                }
                printf("\t%c匹配\n",ch);
                index--;
                p[index] = '\0';
                i++;
            }
            else   //不匹配，出错
            {
                i = error(i,cnt,len,p,str);
                continue;
            }
        }
        cnt++;
    }
}
bool isDigit(char c)
{
    if(c>='0' && c<='9')
        return true;
    else
        return false;
}
bool isLetter(char c)
{
    if((c>='A' && c<='Z')||(c>='a' && c<='z')||c=='_')  //在C语言中"_"也可以出现在标识符的任意位置，因此加在此处
        return true;
    else
        return false;
}
int FindKeyWord(char sub[])
{
    for(int i=0;i<24;i++)
    {
        if(strcmp(KeyWord[i],sub)==0)
        {
            return i+1;  //成功找到，返回对应关键字的种别码
        }
    }
    return -1;//没有找到，返回-1；
}
int FindOperator_Boundary(char sub[])
{
    for(int i=0;i<26;i++)
    {
        if(strcmp(Operator_Boundary[i],sub)==0)
        {
            return i+25;
        }
    }
    return -1;
}
void extractCode(char Source[],int n)
{
    char temp[2000];
    int con=0;
    for(int i=0;i<=n;i++)
    {
        if(Source[i]=='/' && Source[i+1]=='/')   //遇到单行注释，直接找到回车即可
        {
            while(Source[i]!='\n')
            {
                i++;
            }
        }
        if(Source[i]=='/' && Source[i+1]=='*')  //遇到多行注释，要找到对应的另一半*/
        {
            i=i+2;
            while(Source[i]!='*' && Source[i+1]!='/')
            {
                i++;
                if(Source[i]=='$')  //说明已经读到文件末尾，但仍未找到另一半注释符号，说明出错
                {
                    printf("Error!没有找到 */ !");
                    exit(0);
                }
            }
            i=i+2;
        }
        if(Source[i]!='\n' && Source[i]!='\t' && Source[i]!='\r' && Source[i]!='\v')
        {
            temp[con++]=Source[i];
        }
    }
    temp[con++]='\0';
    strcpy(Source,temp);
}
void Lexical_Analysis(int &syn,int &p,char SourceFile[],char token[],int &q,char TargetFile[])  //缺少对界符查找返回-1的判断
{
    int con=0;
    char ch;
    ch=SourceFile[p];
    while(ch==' ')  //去掉开头的空格，避免影响
    {
        p++;
        ch=SourceFile[p];
    }
    for(int i=0;i<20;i++)  //token数组初始化
    {
        token[i]='\0';
    }
    if(isLetter(SourceFile[p]))  //首字符为 字母，表示为标识符或关键字
    {

        token[con++]=SourceFile[p++];
        while(isLetter(SourceFile[p]) || isDigit(SourceFile[p]))  //第二个字符为数字或字母
        {
            token[con++]=SourceFile[p++];   //将字符放入token数组，同时移动指针
        }
        token[con]='\0';


        syn=FindKeyWord(token);  //查找对应种别码
        if(syn==-1) //关键字中没有对应的种别码，说明不是关键字，是标识符
        {
            syn=101; //标识符的种别码为101
            strcat(TargetFile,"i");
            q=q+1;
            TargetFile[q]='\0';
        }
        else if(!strcmp(token,"else"))
        {
            strcat(TargetFile,"s");
            q=q+1;
            TargetFile[q]='\0';
        }
        else if(!strcmp(token,"begin"))
        {
            strcat(TargetFile,"b");
            q=q+1;
            TargetFile[q]='\0';
        }
        else if(!strcmp(token,"end"))
        {
            strcat(TargetFile,"e");
            q=q+1;
            TargetFile[q]='\0';
        }
        else if(!strcmp(token,"if"))
        {
            strcat(TargetFile,"f");
            q=q+1;
            TargetFile[q]='\0';
        }
        else if(!strcmp(token,"while"))
        {
            strcat(TargetFile,"w");
            q=q+1;
            TargetFile[q]='\0';
        }
        else if(!strcmp(token,"bool"))
        {
            strcat(TargetFile,"l");
            q=q+1;
            TargetFile[q]='\0';
        }
        else if(!strcmp(token,"int"))
        {
            strcat(TargetFile,"t");
            q=q+1;
            TargetFile[q]='\0';
        }
        else if(!strcmp(token,"float"))
        {
            strcat(TargetFile,"o");
            q=q+1;
            TargetFile[q]='\0';
        }
        else if(!strcmp(token,"double"))
        {
            strcat(TargetFile,"u");
            q=q+1;
            TargetFile[q]='\0';
        }
        else if(!strcmp(token,"true"))
        {
            strcat(TargetFile,"r");
            q=q+1;
            TargetFile[q]='\0';
        }
        else if(!strcmp(token,"false"))
        {
            strcat(TargetFile,"m");
            q=q+1;
            TargetFile[q]='\0';
        }
        else if(!strcmp(token,"do"))
        {
            strcat(TargetFile,"d");
            q=q+1;
            TargetFile[q]='\0';
        }
        return;
    }
    else if(isDigit(SourceFile[p]))  //以数字开头，为常数
    {
        //token[con++]=SourceFile[p++];
        while(isDigit(SourceFile[p]))
            token[con++]=SourceFile[p++];
        token[con]='\0';

        strcat(TargetFile,"n");
        q=q+1;
        TargetFile[q]='\0';

        syn=100; //常数的种别码为100
        return;
    }
    else if(ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch==';' || ch=='(' || ch==')'
    || ch=='#' || ch=='=' || ch=='[' || ch==']' || ch=='{' || ch=='}')   //为运算符和界符，且不需要向前搜索
    {
        token[0]=ch;
        token[1]='\0';

        strcat(TargetFile,token);
        q=q+1;
        TargetFile[q]='\0';

        syn=FindOperator_Boundary(token);  //查找对应的种别码
        p++;  //指针后移
        return ;
    }
    else if(ch=='<')
    {
        token[con++]=SourceFile[p++];  //<
        if(SourceFile[p]=='=' || SourceFile[p]=='>' || SourceFile[p]=='<')  //<=,<>,<<
        {
            token[con++]=SourceFile[p++];
        }
        token[con]='\0';

        strcat(TargetFile,"<");
        q=q+1;
        TargetFile[q]='\0';

        syn=FindOperator_Boundary(token);
        //p++;
        return;
    }
    else if(ch=='>')
    {
        token[con++]=SourceFile[p++];  //>
        if(SourceFile[p]=='=' || SourceFile[p]=='>')  //>=,>>
        {
            token[con++]=SourceFile[p++];
        }
        token[con]='\0';

        strcat(TargetFile,">");
        q=q+1;
        TargetFile[q]='\0';


        syn=FindOperator_Boundary(token);
        //p++;
        return;
    }
    else if(ch==':')
    {
        token[con++]=SourceFile[p++];  //:
        if(SourceFile[p]=='=')  //:=
        {
            token[con++]=SourceFile[p++];
        }
        token[con]='\0';
        if(!strcmp(token,":="))
        {
            strcat(TargetFile,":");
            q=q+1;
            TargetFile[q]='\0';
        }
        syn=FindOperator_Boundary(token);
       // p++;
        return;
    }
    else if(ch=='!')
    {
        token[con++]=SourceFile[p++];  //!
        if(SourceFile[p]=='=')  //!=
        {
            token[con++]=SourceFile[p++];
        }
        token[con]='\0';

        strcat(TargetFile,token);
        q=q+con;
        TargetFile[q]='\0';

        syn=FindOperator_Boundary(token);
       // p++;
        return;
    }
    else if(ch=='&')
    {
        token[con++]=SourceFile[p++];  //&
        if(SourceFile[p]=='&')  //&&
        {
            token[con++]=SourceFile[p++];
        }
        token[con]='\0';

        strcat(TargetFile,token);
        q=q+con;
        TargetFile[q]='\0';

        syn=FindOperator_Boundary(token);
       // p++;
        return;
    }
    else if(ch=='|')
    {
        token[con++]=SourceFile[p++];  //|
        if(SourceFile[p]=='|')  //||
        {
            token[con++]=SourceFile[p++];
        }
        else
        {
            p--;
            printf("Error!未找到'|'!\n");  //出错退出
            exit(0);
        }
        token[con]='\0';

        strcat(TargetFile,token);
        q=q+con;
        TargetFile[q]='\0';

        syn=FindOperator_Boundary(token);
       // p++;
        return;
    }
    else if(ch=='$')
    {
        syn=0;
        return ;
    }
    else  //无法分析，退出
    {
        printf("Error! %c 不存在!\n",ch);
        exit(0);
    }
}

