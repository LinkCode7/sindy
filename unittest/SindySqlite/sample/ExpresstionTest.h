#ifndef EXPRESSTION_TEST_H
#define EXPRESSTION_TEST_H

#include <map>
#include <string>

// 数学表达式，如"1*250/3.14-(-3*6)"的结果为97.6178343949044
namespace TestMathExpresstion
{
void entry();

void mechanism();

void testCalculation(const std::map<std::string, double>& mapExpresstion2Value);

void testCalculationError();

void readFormulaData(std::map<std::string, double>& mapExpresstion2Value);

} // namespace TestMathExpresstion

#endif // !EXPRESSTION_TEST_H
