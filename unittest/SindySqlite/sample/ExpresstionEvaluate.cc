#include "ExpresstionEvaluate.h"

#include <float.h>

#include <iostream>
#include <string>

using namespace std;

ExpresstionEval::ExpresstionEval(const string& exp)
{
    m_pFirstExp = NULL;
    m_pLastExp  = NULL;
    m_arrTag    = NULL;

    m_strExp = exp;
    m_length = exp.size();
    TurnToTag();
}

ExpresstionEval::~ExpresstionEval()
{
    delete[] m_arrTag;

    if (m_pFirstExp != NULL)
    {
        while (m_pFirstExp->m_pNext != NULL)
        {
            MathNode* pTemp = m_pFirstExp;
            m_pFirstExp     = m_pFirstExp->m_pNext;
            delete pTemp;
        }
    }

    delete m_pFirstExp;
    m_pFirstExp = NULL;
    m_pLastExp  = NULL;
}

bool ExpresstionEval::CheckExpression() const
{
    int i = 0;
    if (!CheckBrackets() || !CheckDot()) // 当小数点位置合法且括号不匹配时
        return false;

    while (i < m_length) // 循环直到表达式中最后一个字符
    {
        if (!CheckNextWord(i)) // 当当前字符与其后的字符不匹配时
            break;             // 跳出循环
        i++;                   // 指针指向下一个字符位置
    }
    if (i < m_length) // 当指针不是指向最后字符的后一个位置时
        return false;
    else
        return true;
}

void ExpresstionEval::ResetExpression(const string& exp)
{
    m_strExp = exp;
    m_length = exp.size();
    if (m_arrTag)
    {
        delete[] m_arrTag;
        m_arrTag = NULL;
    }

    TurnToTag();
}

// 当前字符是数字时则下一个字符必须是：‘.’或运算符，或数字或右括号
// 当前字符是运算符时则下一个字符必须是：表示负号的‘-’或数字或左括号
// 当前字符是左括号时则下一个字符必须是：表示负号的‘-’或数字
// 当前字符是右括号则下一个字符必须是:运算符
// 当前字符是'.'号且不是最后一个字符时则下一个字符必须是:数字或运算符或右括号
bool ExpresstionEval::CheckNextWord(int place) const
{
    int word = m_arrTag[place];
    switch (word)
    {
        case kMathLevel_1: // 当前字符是数字时
        {
            if (place == m_length - 1) // 最后的字符是数字时
                return true;
            else if (m_arrTag[place + 1] == kMathLevel_1 || m_arrTag[place + 1] == kMathLevel_2 || m_arrTag[place + 1] == kMathLevel_4 ||
                     m_arrTag[place + 1] == kMathLevel_5)
                return true;
            return false;
        }
        case kMathLevel_2: // 当前字符是运算符时
        {
            if (m_arrTag[place + 1] == kMathLevel_1 || m_arrTag[place + 1] == kMathLevel_3)
                return true;
            else if (m_strExp[place + 1] == '-' && m_arrTag[place + 2] == kMathLevel_1)
                return true;
            return false;
        }
        case kMathLevel_3: // 当前字符是左括号时
        {
            if (m_arrTag[place + 1] == kMathLevel_1 || m_arrTag[place + 1] == kMathLevel_3)
                return true;
            else if (m_strExp[place + 1] == '-' && m_arrTag[place + 2] == kMathLevel_1)
                return true;
            return false;
        }
        case kMathLevel_4: // 当前字符是右括号
        {
            if (place == m_length - 1) // 当前是最后一个字符时
                return true;
            if (m_arrTag[place + 1] == kMathLevel_2 || m_arrTag[place + 1] == kMathLevel_4)
                return true;
            return false;
        }
        case kMathLevel_5: // 当前字符是'.'号时
        {
            if (place == m_length - 1)
                return true;
            else if (m_arrTag[place + 1] == kMathLevel_2 || m_arrTag[place + 1] == kMathLevel_1 ||
                     m_arrTag[place + 1] == kMathLevel_4) // 运算符，数字，右括号
                return true;
            return false;
        }
        default:
            return false;
    } // end switch
}

bool ExpresstionEval::CheckBrackets() const
{
    int leftBracket = 0, rightBracket = 0; // leftBracket记录左括号的数量,rightBracket记录右括号的数量;
    int i = 0;

    while (i < m_length) // 循环直到表达式中最后一个字符
    {
        if (rightBracket > leftBracket) // 当右括号数目多于左括号
            return false;
        if (m_arrTag[i] == kMathLevel_3) // 当前字符是左括号时
        {
            leftBracket++;
        }
        else if (m_arrTag[i] == kMathLevel_4) // 当前字符是右括号时
        {
            rightBracket++;
        }
        i++;
    }
    if (rightBracket == leftBracket) // 左右括号数量一样时
        return true;

    return false;
}

bool ExpresstionEval::CheckDot() const
{
    string str     = m_strExp;
    int    current = 0, prev, later; // prev指向小数点以前的字符位置,later指向小数点以后的字符位置

    //	bool dot=0;
    while (current < m_length) // 循环直到表达式中最后一个字符
    {
        if (m_arrTag[current] == kMathLevel_6)
        {
            return false;
        }
        else if (m_arrTag[current] == kMathLevel_5) // 当前字符是'.'时
        {
            //	dot=1;
            prev  = current - 1; // 初始化prev,later分别为当前小数点位置的前一字符和后一字符
            later = current + 1;
            if (m_arrTag[prev] != kMathLevel_1) // 当小数点前后都没有数字时--说明只有小数点不是数
                return false;
            while (prev >= 0 && later < m_length) // 循环直到指针指向表达式最前与最后的位置
            {
                if (m_arrTag[prev] == kMathLevel_1 && m_arrTag[later] == kMathLevel_1) // 当prev,later所指的字符都是数字时
                {
                    prev--;  // prev指针前移，
                    later++; // later指针后移
                    continue;
                }
                // 当prev,later所指的字符其中之一是小数点时--说明一数字里含有两个以上小数点
                else if (m_arrTag[prev] == kMathLevel_5 || m_arrTag[later] == kMathLevel_5)
                {
                    return false;
                }
                else if (m_arrTag[prev] == kMathLevel_1) // 当prev所指的字符是数字,later所指的字符不是数字
                {
                    prev--; // prev指针前移
                    continue;
                }
                else if (m_arrTag[later] == kMathLevel_1) // 当prev所指的字符不是数字,later所指的字符是数字
                {
                    later++; // later指针后移
                    continue;
                }
                // 当prev,later所指的字符都是是运算符时--说明该数字中只含有一个小数点
                else if ((m_arrTag[prev] == kMathLevel_2 && m_arrTag[later] == kMathLevel_2) ||
                         (m_arrTag[prev] == kMathLevel_2 && m_arrTag[later] == kMathLevel_4) ||
                         (m_arrTag[prev] == kMathLevel_3 && m_arrTag[later] == kMathLevel_2))
                {
                    return true;
                }
            } // end while
        }     // end if
        current++;
    } // end while
    return true;
}

void ExpresstionEval::TurnToTag() // 将算术表达式中字符分类并储存
{
    m_arrTag = new int[m_length];

    for (int i = 0; i < m_length; ++i)
    {
        if (m_strExp[i] >= '0' && m_strExp[i] <= '9')
            m_arrTag[i] = kMathLevel_1;
        else if (m_strExp[i] == '*' || m_strExp[i] == '+' || m_strExp[i] == '-' || m_strExp[i] == '/')
            m_arrTag[i] = kMathLevel_2;
        else if (m_strExp[i] == '(')
            m_arrTag[i] = kMathLevel_3;
        else if (m_strExp[i] == ')')
            m_arrTag[i] = kMathLevel_4;
        else if (m_strExp[i] == '.')
            m_arrTag[i] = kMathLevel_5;
        else
            m_arrTag[i] = kMathLevel_6; // illeger character
    }
}

void ExpresstionEval::ShowResult()
{
    cout << m_strExp << "  ";
    cout << "的长度是：" << m_length << endl;
    cout << "各字符的所属类别是：";

    for (int i = 0; i < m_length; ++i)
    {
        cout << m_arrTag[i] << "  ";
    }
    cout << endl;

    if (m_pFirstExp != NULL)
    {
        cout << endl << "Linked list:" << endl;

        MathNode* temptr = m_pFirstExp;
        while (temptr != NULL)
        {
            if (temptr->m_tag == NUM)
                cout << "NUM:" << temptr->m_num << endl;
            else
                cout << "SIGN:" << temptr->m_sign << endl;
            temptr = temptr->m_pNext;
        }
        cout << endl << endl;
    }
    else
    {
        cout << " 未输入表达式" << endl;
    }
}

bool ExpresstionEval::GetResult(double& result)
{
    Extraction();

    // ShowResult(); //Test

    if (!m_pFirstExp)
        return false;

    if (!m_pFirstExp->m_pNext) // 3.14
    {
        result = m_pFirstExp->m_num;
        return true;
    }

    // 符号栈、数字栈
    MathNode* arrNum    = new MathNode[10];
    MathNode* arrSign   = new MathNode[15];
    int       iPriority = -1;
    int       numptr(0), signptr(0);

    MathNode* pCurNode = m_pFirstExp;

    if (pCurNode->m_tag == NUM)
    {
        arrNum[numptr].m_num = pCurNode->m_num;
        arrNum[numptr].m_tag = pCurNode->m_tag;
        pCurNode             = pCurNode->m_pNext;

        arrSign[signptr].m_sign = pCurNode->m_sign;
        arrSign[signptr].m_tag  = pCurNode->m_tag;
        pCurNode                = pCurNode->m_pNext;
    }
    else
    {
        arrSign[signptr].m_sign = pCurNode->m_sign;
        arrSign[signptr].m_tag  = pCurNode->m_tag;
        pCurNode                = pCurNode->m_pNext;

        arrNum[numptr].m_num = pCurNode->m_num;
        arrNum[numptr].m_tag = pCurNode->m_tag;
        pCurNode             = pCurNode->m_pNext;
    }

    while (pCurNode != NULL)
    {
        if (pCurNode->m_tag == NUM) // 当前结点是数字
        {
            numptr++;
            arrNum[numptr].m_num = pCurNode->m_num;
            arrNum[numptr].m_tag = pCurNode->m_tag;

            pCurNode = pCurNode->m_pNext;
        }
        else // 当前结点是符号
        {
            if (signptr < 0) // 符号栈中没有符号时
            {
                signptr++;
                arrSign[signptr].m_sign = pCurNode->m_sign;
                arrSign[signptr].m_tag  = pCurNode->m_tag;
                pCurNode                = pCurNode->m_pNext;
            }
            else
            {
                iPriority = OperatorPriority(pCurNode->m_sign, arrSign[signptr].m_sign);
                switch (iPriority)
                {
                    case 0:
                    {
                        signptr++;
                        arrSign[signptr].m_sign = pCurNode->m_sign;
                        arrSign[signptr].m_tag  = pCurNode->m_tag;

                        pCurNode = pCurNode->m_pNext;
                        break;
                    }
                    case 1:
                    {
                        double tempresult = DBL_MAX;
                        if (!CalculateMath(arrNum[numptr - 1].m_num, arrNum[numptr].m_num, arrSign[signptr].m_sign, tempresult))
                            break;

                        numptr--;
                        arrNum[numptr].m_num = tempresult;
                        signptr--;
                        break;
                    }
                    case -1: // 表示当前为')'
                    {
                        signptr--; // 将符号栈顶的'('弹出
                        pCurNode = pCurNode->m_pNext;
                        break;
                    }
                }
            }
        }
    }

    while (signptr >= 0)
    {
        double result = DBL_MAX;
        if (!CalculateMath(arrNum[numptr - 1].m_num, arrNum[numptr].m_num, arrSign[signptr].m_sign, result))
            break;

        numptr--;
        arrNum[numptr].m_num = result;
        signptr--;
    }

    // pCurNode = NULL;
    result = arrNum[numptr].m_num;

    delete[] arrNum;
    delete[] arrSign;

    return true;
}

void ExpresstionEval::Extraction()
{
    // 存储从运算表达式中提取的数字字符串：1,3.14,666
    char numStr[20];
    int  n_str_p  = 0; // 同一数字字符串中的某一位
    int  dotplace = 0; // 小数点在数字字符串中的位置

    int    len = 0; // 当前数字字符串的长度
    double result;  // 当前所提取的数字串的值

    for (int i = 0; i < m_length; ++i)
    {
        // 如果当前字符是数字则直接存入数字字符串中
        if (m_arrTag[i] == kMathLevel_1 || m_arrTag[i] == kMathLevel_5)
        {
            numStr[n_str_p] = m_strExp[i];
            len++;

            if (m_arrTag[i] == kMathLevel_5) // 如果当前字符是小数点
            {
                dotplace = n_str_p; // 纪录小数点位置
            }
            n_str_p++;

            if (i == m_length - 1)
            {
                result = ExtractNum(dotplace, numStr, len);
                ApplyNode(result); // 申请新节点并保存数值
                n_str_p  = 0;
                len      = 0;
                dotplace = 0;
            }
        }
        // 如果当前是字符'-'，且表示负
        else if ((m_arrTag[i - 1] == kMathLevel_2 && m_strExp[i] == '-') || (i == 0 && m_strExp[i] == '-') ||
                 (m_arrTag[i - 1] == kMathLevel_3 && m_strExp[i] == '-'))
        {
            numStr[n_str_p] = m_strExp[i];
            len++;
            n_str_p++;
        }
        // else if(m_arrTag[i]==kMathLevel_2||m_arrTag[i]==kMathLevel_4||m_arrTag[i]==kMathLevel_3) // 当前字符是运算符或右括号
        else
        {
            if (m_arrTag[i] == kMathLevel_3) // 如果当前是左括号
            {
                ApplyNode(m_strExp[i]); // 申请新节点并保存运算符
            }
            else if (m_arrTag[i] == kMathLevel_2 && m_arrTag[i - 1] == kMathLevel_4)
            {
                ApplyNode(m_strExp[i]);
            }
            else
            {
                result = ExtractNum(dotplace, numStr, len); // 计算提取出的数值
                ApplyNode(result);                          // 申请新节点并保存数值
                n_str_p  = 0;                               // 为下一数值字符串存储作准备
                len      = 0;
                dotplace = 0;
                ApplyNode(m_strExp[i]); // 申请新节点并保存运算符
            }
        }
    }
}

// 计算从算术表达式中提取出的数字值
double ExpresstionEval::ExtractNum(int dotplace, char* numptr, int len)
{
    int    i(-1);
    double result      = 0.0;
    int    Gdecimalism = 1;

    if (dotplace == 0) // 当小数点在0的位置时--表示当前所提取的数中没有小数点
    {
        for (i = len - 1; i > 0; --i) // 循环直到数字字符串的最后一个字符
        {
            result = (double)result + (numptr[i] - '0') * Gdecimalism;
            Gdecimalism *= 10;
        }
        if (numptr[i] != '-')                                          // 当该数不是负数时，即numptr[0]!='-'
            result = (double)result + (numptr[i] - '0') * Gdecimalism; // result值再加上numptr[0]的数值
        else
            result = 0 - result;
    }
    else
    {
        double Ldecimalism = 0.1;

        i     = dotplace - 1; // 小数点前一位
        int j = dotplace + 1; // 小数点后一位

        while (i >= 0 || j < len) // 循环直到数值字符串最前和最后的数值
        {
            if (i > 0) // i指向小数点之前数的位置
            {
                result = (double)result + (numptr[i] - '0') * Gdecimalism;
                Gdecimalism *= 10;
                i--;
            }
            else if (i == 0)
            {
                if (numptr[i] != '-') // 当该数不是负数时，即numptr[0]!='-'
                {
                    result = (double)result + (numptr[i] - '0') * Gdecimalism;
                    i--;
                }
                else // 该数是负数时
                {
                    result = 0 - result; // 将result求负值
                    i--;
                }
            }
            if (j <= len - 1) // j指向小数点后数的位置
            {
                result = result + (double)(numptr[j] - '0') * Ldecimalism;
                Ldecimalism *= 0.1;
                j++;
            }
        }
    }
    return result;
}

void ExpresstionEval::ApplyNode(double result)
{ // 用于指向链表的最后节点
    MathNode* newnode = new MathNode;
    newnode->m_num    = result;
    newnode->m_tag    = NUM;
    newnode->m_pNext  = NULL;

    if (m_pFirstExp == NULL)
    {
        m_pFirstExp = newnode; // 将链表开始指针指向头节点
        m_pLastExp  = newnode;
    }
    else
    {
        m_pLastExp->m_pNext = newnode; // 将新节点挂在链表最后头
        m_pLastExp          = newnode;
    }
}

void ExpresstionEval::ApplyNode(char calsign)
{
    MathNode* newnode = new MathNode;
    newnode->m_sign   = calsign;
    newnode->m_tag    = SIGN;
    newnode->m_pNext  = NULL;

    if (m_pFirstExp == NULL && m_pLastExp == NULL)
    {
        m_pFirstExp = newnode; // 将链表开始指针指向头节点
        m_pLastExp  = newnode;
    }
    else
    {
        m_pLastExp->m_pNext = newnode; // 将新节点挂在链表最后头
        m_pLastExp          = newnode;
    }
}

int ExpresstionEval::OperatorPriority(const char current, const char former)
{
    switch (former)
    {
        case '+':
        {
            switch (current)
            {
                case '-':
                case '+':
                    return 1;
                case '*':
                case '/':
                case '(':
                    return 0;
                case ')':
                    return 1;
            }
        }
        case '-':
        {
            switch (current)
            {
                case '-':
                case '+':
                    return 1;
                case '*':
                case '/':
                case '(':
                    return 0;
                case ')':
                    return 1;
            }
        }
        case '*':
        {
            switch (current)
            {
                case '+':
                case '-':
                case '/':
                case '*':
                    return 1;
                case '(':
                    return 0;
                case ')':
                    return 1;
            }
        }
        case '/':
        {
            switch (current)
            {
                case '+':
                case '-':
                case '*':
                case '/':
                    return 1;
                case '(':
                    return 0;
                case ')':
                    return 1;
            }
        }
        case '(':
        {
            switch (current)
            {
                case '+':
                case '-':
                case '*':
                case '/':
                case '(':
                    return 0;
                case ')':
                    return -1;
            }
        }
        default:
            return false;
    }
}

bool ExpresstionEval::CalculateMath(double formernum, double laternum, char currentsign, double& result)
{
    switch (currentsign)
    {
        case '+':
        {
            result = formernum + laternum;
            break;
        }
        case '-':
        {
            result = formernum - laternum;
            break;
        }
        case '*':
        {
            result = formernum * laternum;
            break;
        }
        case '/':
        {
            if (!laternum) // 除数为零
                return false;
            else
                result = formernum / laternum;
            break;
        }
    }

    return true;
}