#ifndef SINDY_MATH_H
#define SINDY_MATH_H

#ifndef SINDY_ZERO
    #define SINDY_ZERO 0.0000001 // 1e-7
#endif

namespace sindy
{

inline double getAbs(double dValue)
{
    if (dValue > 0)
        return dValue;
    else
        return -dValue;
}

// 比较两个浮点数：0表示相同 1表示前面大 -1表示后面大
inline int compare(const double src1, const double src2, double tol = SINDY_ZERO)
{
    double dblSub = src1 - src2;
    if (getAbs(dblSub) <= tol)
        return 0; // 相同
    else if (dblSub > tol)
        return 1;
    else
        return -1;
}

} // namespace sindy

#endif // !SINDY_MATH_H
