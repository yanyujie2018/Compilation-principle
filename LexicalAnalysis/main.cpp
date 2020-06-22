#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
using namespace std;
// ps:如果要增加或删除关键字，记得修改函数int FindKeyWord();中的循环终止条件,以及查找界符种别码函数中的22+i
static char KeyWord[24][20]={
  "begin","if","then","while","do","end","int","long","break","case",
  "char","continue","default","double","else","float","goto","return",
  "void","switch","static","true","false","bool"
};
//目前只有16个运算符和界符  ps:如果要增加或删除运算符和界符，记得修改函数int FindOperator_Boundary(char sub[]);中的循环终止条件
static char Operator_Boundary[26][10]={
    "+","-","*","/",":",":=","<","<>","<=",">",">=","=",";","(",")","#","!=",">>","<<","&&","&","||","]","[","{","}"
};
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
void Lexical_Analysis(int &syn,int &p,char SourceFile[],char token[],int &q,char TargetFile[200][200]);
int index=0;
int main()
{
    char SourceFile[2000]; //存放读出的文件信息
    char TargetFile[200][200]; //存放替换标识符后的字符串
    int q=0; //相当于TargetFile数组的下标
    char token[20];//存储单词记号
    int syn=-1;//种别码
    int p=0;//用于访问文件的指针,相当于SourceFile[2000]数组的下标
    char ch;
    do{
        scanf("%c",&ch);
        SourceFile[p++]=ch;
    }while(ch!='$');
    SourceFile[p]='\0';
    //提取出真正的代码（即去除回车，注释等无效字符）
    extractCode(SourceFile,p);
    p=0;
    for(int i=0;i<200;i++)
    {
        TargetFile[i][q]='\0';
    }

    while(syn!=0)
    {
        //依次分析
        Lexical_Analysis(syn,p,SourceFile,token,q,TargetFile);
        index++;
        if(syn==0)
            continue;
        printf("(%d,%s)\n",syn,token);//输出
    }
    return 0;
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
void Lexical_Analysis(int &syn,int &p,char SourceFile[],char token[],int &q,char TargetFile[200][200])  //缺少对界符查找返回-1的判断
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

            strcat(TargetFile[index],"ID");
            q=q+2;
            TargetFile[index][q]='\0';
            q=0;

        }

        else
        {
            strcat(TargetFile[index],token);
            q=q+con;
            TargetFile[index][q]='\0';
            q=0;
        }

        return;
    }
    else if(isDigit(SourceFile[p]))  //以数字开头，为常数
    {
        while(isDigit(SourceFile[p]))
            token[con++]=SourceFile[p++];
        token[con]='\0';

        strcat(TargetFile[index],"NUM");
        q=q+3;
        TargetFile[index][q]='\0';
        q=0;

        syn=100; //常数的种别码为100
        return;
    }
    else if(ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch==';' || ch=='(' || ch==')'
    || ch=='#' || ch=='=' || ch=='[' || ch==']' || ch=='{' || ch=='}')   //为运算符和界符，且不需要向前搜索
    {
        token[0]=ch;
        token[1]='\0';

        strcat(TargetFile[index],token);
        q=q+1;
        TargetFile[index][q]='\0';
        q=0;


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

        strcat(TargetFile[index],token);
        q=q+con;
        TargetFile[index][q]='\0';
        q=0;

        syn=FindOperator_Boundary(token);
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

        strcat(TargetFile[index],token);
        q=q+con;
        TargetFile[index][q]='\0';
        q=0;


        syn=FindOperator_Boundary(token);
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

        strcat(TargetFile[index],token);
        q=q+con;
        TargetFile[index][q]='\0';
        q=0;

        syn=FindOperator_Boundary(token);
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

        strcat(TargetFile[index],token);
        q=q+con;
        TargetFile[index][q]='\0';
        q=0;

        syn=FindOperator_Boundary(token);
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

        strcat(TargetFile[index],token);
        q=q+con;
        TargetFile[index][q]='\0';
        q=0;

        syn=FindOperator_Boundary(token);
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

        strcat(TargetFile[index],token);
        q=q+con;
        TargetFile[index][q]='\0';
        q=0;

        syn=FindOperator_Boundary(token);
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

/*
        if(syn==-1) //关键字中没有对应的种别码，说明不是关键字，是标识符
        {
            syn=101; //标识符的种别码为101

            strcat(TargetFile[index]);
            q=q+2;
            TargetFile[index][q]=0;
            q=0;

        }

        else
        {
            strcat(TargetFile[index]);
            q=q+con;
            TargetFile[index][q]=0;
            q=0;
        }
*/
