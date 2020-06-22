#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
using namespace std;
// ps:���Ҫ���ӻ�ɾ���ؼ��֣��ǵ��޸ĺ���int FindKeyWord();�е�ѭ����ֹ����,�Լ����ҽ���ֱ��뺯���е�22+i
static char KeyWord[24][20]={
  "begin","if","then","while","do","end","int","long","break","case",
  "char","continue","default","double","else","float","goto","return",
  "void","switch","static","true","false","bool"
};
//Ŀǰֻ��16��������ͽ��  ps:���Ҫ���ӻ�ɾ��������ͽ�����ǵ��޸ĺ���int FindOperator_Boundary(char sub[]);�е�ѭ����ֹ����
static char Operator_Boundary[26][10]={
    "+","-","*","/",":",":=","<","<>","<=",">",">=","=",";","(",")","#","!=",">>","<<","&&","&","||","]","[","{","}"
};
//�жϵ�ǰ�ַ��Ƿ�Ϊ����
bool isDigit(char c);
//�ж��Ƿ�Ϊ��ĸ
bool isLetter(char c);
//�����Ƿ�Ϊ�ؼ��֣������򷵻ض�Ӧ���ֱ��룬���򷵻�-1����ʾ���ǹؼ��֣��Ǳ�ʶ��
int FindKeyWord(char sub[]);
//�����Ƿ�Ϊ���������
int FindOperator_Boundary(char sub[]);
//������ȡ���ô���
void extractCode(char Source[],int n);
void Lexical_Analysis(int &syn,int &p,char SourceFile[],char token[],int &q,char TargetFile[200][200]);
int index=0;
int main()
{
    char SourceFile[2000]; //��Ŷ������ļ���Ϣ
    char TargetFile[200][200]; //����滻��ʶ������ַ���
    int q=0; //�൱��TargetFile������±�
    char token[20];//�洢���ʼǺ�
    int syn=-1;//�ֱ���
    int p=0;//���ڷ����ļ���ָ��,�൱��SourceFile[2000]������±�
    char ch;
    do{
        scanf("%c",&ch);
        SourceFile[p++]=ch;
    }while(ch!='$');
    SourceFile[p]='\0';
    //��ȡ�������Ĵ��루��ȥ���س���ע�͵���Ч�ַ���
    extractCode(SourceFile,p);
    p=0;
    for(int i=0;i<200;i++)
    {
        TargetFile[i][q]='\0';
    }

    while(syn!=0)
    {
        //���η���
        Lexical_Analysis(syn,p,SourceFile,token,q,TargetFile);
        index++;
        if(syn==0)
            continue;
        printf("(%d,%s)\n",syn,token);//���
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
    if((c>='A' && c<='Z')||(c>='a' && c<='z')||c=='_')  //��C������"_"Ҳ���Գ����ڱ�ʶ��������λ�ã���˼��ڴ˴�
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
            return i+1;  //�ɹ��ҵ������ض�Ӧ�ؼ��ֵ��ֱ���
        }
    }
    return -1;//û���ҵ�������-1��
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
        if(Source[i]=='/' && Source[i+1]=='/')   //��������ע�ͣ�ֱ���ҵ��س�����
        {
            while(Source[i]!='\n')
            {
                i++;
            }
        }
        if(Source[i]=='/' && Source[i+1]=='*')  //��������ע�ͣ�Ҫ�ҵ���Ӧ����һ��*/
        {
            i=i+2;
            while(Source[i]!='*' && Source[i+1]!='/')
            {
                i++;
                if(Source[i]=='$')  //˵���Ѿ������ļ�ĩβ������δ�ҵ���һ��ע�ͷ��ţ�˵������
                {
                    printf("Error!û���ҵ� */ !");
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
void Lexical_Analysis(int &syn,int &p,char SourceFile[],char token[],int &q,char TargetFile[200][200])  //ȱ�ٶԽ�����ҷ���-1���ж�
{
    int con=0;
    char ch;
    ch=SourceFile[p];
    while(ch==' ')  //ȥ����ͷ�Ŀո񣬱���Ӱ��
    {
        p++;
        ch=SourceFile[p];
    }
    for(int i=0;i<20;i++)  //token�����ʼ��
    {
        token[i]='\0';
    }
    if(isLetter(SourceFile[p]))  //���ַ�Ϊ ��ĸ����ʾΪ��ʶ����ؼ���
    {

        token[con++]=SourceFile[p++];
        while(isLetter(SourceFile[p]) || isDigit(SourceFile[p]))  //�ڶ����ַ�Ϊ���ֻ���ĸ
        {
            token[con++]=SourceFile[p++];   //���ַ�����token���飬ͬʱ�ƶ�ָ��
        }
        token[con]='\0';

        syn=FindKeyWord(token);  //���Ҷ�Ӧ�ֱ���
        if(syn==-1) //�ؼ�����û�ж�Ӧ���ֱ��룬˵�����ǹؼ��֣��Ǳ�ʶ��
        {
            syn=101; //��ʶ�����ֱ���Ϊ101

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
    else if(isDigit(SourceFile[p]))  //�����ֿ�ͷ��Ϊ����
    {
        while(isDigit(SourceFile[p]))
            token[con++]=SourceFile[p++];
        token[con]='\0';

        strcat(TargetFile[index],"NUM");
        q=q+3;
        TargetFile[index][q]='\0';
        q=0;

        syn=100; //�������ֱ���Ϊ100
        return;
    }
    else if(ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch==';' || ch=='(' || ch==')'
    || ch=='#' || ch=='=' || ch=='[' || ch==']' || ch=='{' || ch=='}')   //Ϊ������ͽ�����Ҳ���Ҫ��ǰ����
    {
        token[0]=ch;
        token[1]='\0';

        strcat(TargetFile[index],token);
        q=q+1;
        TargetFile[index][q]='\0';
        q=0;


        syn=FindOperator_Boundary(token);  //���Ҷ�Ӧ���ֱ���
        p++;  //ָ�����
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
            printf("Error!δ�ҵ�'|'!\n");  //�����˳�
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
    else  //�޷��������˳�
    {
        printf("Error! %c ������!\n",ch);
        exit(0);
    }
}

/*
        if(syn==-1) //�ؼ�����û�ж�Ӧ���ֱ��룬˵�����ǹؼ��֣��Ǳ�ʶ��
        {
            syn=101; //��ʶ�����ֱ���Ϊ101

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
