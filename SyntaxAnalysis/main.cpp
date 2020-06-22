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
// ps:���Ҫ���ӻ�ɾ���ؼ��֣��ǵ��޸ĺ���int FindKeyWord();�е�ѭ����ֹ����,�Լ����ҽ���ֱ��뺯���е�22+i
static char KeyWord[24][20]={
  "begin","if","then","while","do","end","int","long","break","case",
  "char","continue","default","double","else","float","goto","return",
  "void","switch","static","true","false","bool"
};
//Ŀǰֻ��16��������ͽ��  ps:���Ҫ���ӻ�ɾ��������ͽ�����ǵ��޸ĺ���int�е�ѭ����ֹ����
int FindOperator_Boundary(char sub[]);
static char Operator_Boundary[26][10]={
    "+","-","*","/",":",":=","<","<>","<=",">",">=","=",";","(",")","#","!=",">>","<<","&&","&","||","]","[","{","}"};
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
    int q=0; //�൱��TargetFile������±�
    char token[20];//�洢���ʼǺ�
    int syn=-1;//�ֱ���
    int p=0;//���ڷ����ļ���ָ��,�൱��SourceFile[2000]������±�
    do{
        scanf("%c",&ch);
        SourceFile[p++]=ch;
    }while(ch!='$');
    SourceFile[p]='\0';
    //��ȡ�������Ĵ��루��ȥ���س���ע�͵���Ч�ַ���
    extractCode(SourceFile,p);
    p=0;
    TargetFile[q]='\0';
    while(syn!=0)
    {
        //���η���
        Lexical_Analysis(syn,p,SourceFile,token,q,TargetFile);
        if(syn==0)
            continue;
    }
    TargetFile[q++]='$';
    TargetFile[q]='\0';

    int len = strlen(TargetFile);
    temp.push('$');
    temp.push('E'); //��ʼ������ջ
    Analysis(TargetFile,len+1);
    return 0;
}

int search_NonTerminal(char x)
{
    for(int i = 0; i < 23; i++)//�ҵ���Ӧ��,�����к�
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
    for(int i = 0; i < 27; i++)//�ҵ���Ӧ�У������к�
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
    printf("\t����\n");
    return len;
}
void Analysis(char str[],int len)
{
    int cnt = 1;//���Stepר��
    int i = 0;
    char p[200] = "$E";//ģ��ջ���Ѵﵽ���ջ�е�ǰ���ݵĹ��ܣ���ʼ״ֻ̬�п�ʼ���ţ�$��ʾջ�գ��ַ����������
    int index = 2;  //ģ��ջ��ָ�룬ָ��ǰ��Ч�ַ���ŵ���һ��λ�ã���0��ʼ��
    printf("Step\tStack\tString\tRule\n");
    while(i<len)
    {
        int x,y;
        char ch = temp.top();
        if(ch>='A'&&ch<='Z')  //˵��ջ��Ϊ���ս��,��Ҫ���ղ���ʽ��Լ
        {
            temp.pop();   //�������ս��
            x = search_NonTerminal(ch);   //�ҵ�Ԥ��������ж�Ӧ���ս����������
            y = search_Terminator(str[i]);  //�ҵ�Ԥ��������е�ǰ�������ս������һ��
            if(x!=-1&&y!=-1)   //�ҵ���Ӧ����
            {
                int len2 = strlen(LL1[x][y]);
                if(strcmp(LL1[x][y],"null")==0)  //�����������ж�Ӧ����Ϊ��
                {
                    i = error(i,cnt,len,p,str);
                    continue;
                }
                printf("%d\t%s\t",cnt,p);  //�����ǰ�����Ĳ�������Լ���ǰջ�е�����
                if(p[index-1] != '$')  //ջδ��
                {
                    p[index] = '\0';  //���ոյ���ջ�ķ��Ÿ���
                    index--;          //ջ��ָ������
                }
                if(LL1[x][y][2]!='#')  //����ʽ���Ƴ���(����Ϊ"->#")ʱ
                {
                    for(int q = len2-1; q>1; q--)  //���򽫲���ʽ��ջ
                    {
                        p[index++] = LL1[x][y][q];
                        temp.push(LL1[x][y][q]);
                    }
                }
                else
                {
                    p[index] = '\0';  //����ʽΪ"->#"ʱ������ջ,ֱ�ӽ���ǰջ�����Ǽ���
                    //index--;
                }
                for(int q = i; q<len;q++)  //���ʣ��Ļ�δ�����Ĵ�
                {
                    cout<<str[q];
                }
                printf("\t%c%s\n",ch,LL1[x][y]);  //���Ӧʹ�õĲ���ʽ
            }
            else   //δ�ڷ��������ҵ���Ӧ�ķ���
            {
                i = error(i,cnt,len,p,str);
                continue;
            }

        }
        else  //�����ǰջ��Ϊ�ս��
        {
            if(ch==str[i])  //���ջ��Ԫ���뵱ǰ�����ķ��ս��ƥ��
            {
                temp.pop();    //���ս����ջ
                printf("%d\t%s\t",cnt,p);
                if(ch=='$'&&str[i]=='$')  //ȫ���������
                {
                    printf("$\t����\n");
                    return ;
                }
                for(int q = i; q<len;q++)  //���ʣ�໹δ�����Ĵ�
                {
                    cout<<str[q];
                }
                printf("\t%cƥ��\n",ch);
                index--;
                p[index] = '\0';
                i++;
            }
            else   //��ƥ�䣬����
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
void Lexical_Analysis(int &syn,int &p,char SourceFile[],char token[],int &q,char TargetFile[])  //ȱ�ٶԽ�����ҷ���-1���ж�
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
    else if(isDigit(SourceFile[p]))  //�����ֿ�ͷ��Ϊ����
    {
        //token[con++]=SourceFile[p++];
        while(isDigit(SourceFile[p]))
            token[con++]=SourceFile[p++];
        token[con]='\0';

        strcat(TargetFile,"n");
        q=q+1;
        TargetFile[q]='\0';

        syn=100; //�������ֱ���Ϊ100
        return;
    }
    else if(ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch==';' || ch=='(' || ch==')'
    || ch=='#' || ch=='=' || ch=='[' || ch==']' || ch=='{' || ch=='}')   //Ϊ������ͽ�����Ҳ���Ҫ��ǰ����
    {
        token[0]=ch;
        token[1]='\0';

        strcat(TargetFile,token);
        q=q+1;
        TargetFile[q]='\0';

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
            printf("Error!δ�ҵ�'|'!\n");  //�����˳�
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
    else  //�޷��������˳�
    {
        printf("Error! %c ������!\n",ch);
        exit(0);
    }
}

