#ifndef EXPRESSTION_EVALUATE_H
#define EXPRESSTION_EVALUATE_H

#include <string>

enum Tag
{
    NUM,
    SIGN
};
struct MathNode
{
    double           m_num;
    char             m_sign;
    Tag              m_tag;
    struct MathNode* m_pNext;
};

class ExpresstionEval
{
public:
    ExpresstionEval(const std::string& exp);
    ~ExpresstionEval();

public:
    bool GetResult(double& result);

    bool CheckExpression() const;
    void ResetExpression(const std::string& exp);

    // 显示属性内容
    void ShowResult();

private:
    enum
    {
        kUnknown,
        kMathLevel_1 = 1, // 数字
        kMathLevel_2,     // +-*/
        kMathLevel_3,     // (
        kMathLevel_4,     // )
        kMathLevel_5,     // .
        kMathLevel_6,     // 未支持符号
    };

    // 提取数值及运算符并存于链表中
    void Extraction();

    // 将字符转化成表示不同类型的数值标签
    void TurnToTag();

    // 检查括号是否匹配
    bool CheckBrackets() const;
    // 检查小数点位置是否合法
    bool CheckDot() const;
    // 下一个字符与当前字符是否关系是否正确
    bool CheckNextWord(int place) const;

private:
    // 计算从算术表达式中提取出的数字值
    double ExtractNum(int dotplace, char* numptr, int len);

    // 为数值申请节点
    void ApplyNode(double result);
    // 为运算符申请节点
    void ApplyNode(char calsign);

    // 比较两运算符的优先级
    int OperatorPriority(const char current, const char former);

    // 计算当前单运算式的值
    bool CalculateMath(double formernum, double laternum, char currentsign, double& result);

private:
    int         m_length; // 表达式中字符个数
    std::string m_strExp; // 用户输入的四则运算表达式
    int*        m_arrTag; // 用数值区别表示不同类型

    MathNode* m_pFirstExp;
    MathNode* m_pLastExp;
};

#endif // !EXPRESSTION_EVALUATE_H
