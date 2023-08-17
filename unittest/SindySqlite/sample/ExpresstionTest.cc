#include "ExpresstionTest.h"

#include <float.h>

#include <iostream>

#include "../../../sindy/RunTime.h"
#include "../../../sindy/SQLite.h"
#include "../../../sindy/SindyMath.h"
#include "../ConstDefine.h"
#include "ExpresstionEvaluate.h"

using namespace std;

// TestFormula
namespace TestMathExpresstion
{
void entry()
{
    // mechanism();

    testCalculationError();

    {
        std::map<std::string, double> mapExpresstion2Value;
        readFormulaData(mapExpresstion2Value);

        sindy::RunTime time;
        for (int i = 0; i < 10000; ++i)
        {
            testCalculation(mapExpresstion2Value);
            // muParserTest(mapExpresstion2Value);
        }
        time.console("calc");
    }
}

void mechanism()
{
    // ExpresstionEval cal("1+2");
    // ExpresstionEval cal("1+(2-1+2)");
    // ExpresstionEval cal("(1+2)*2");
    ExpresstionEval cal("1+2+3");

    double dValue = 0.0;
    bool   isOk   = cal.GetResult(dValue);

    system("pause");
}

void testCalculationError()
{
    std::vector<std::string> vecData;
    // vecData.push_back("1E-9")); // 不支持
    vecData.push_back("-1-17");
    vecData.push_back("1*17");
    vecData.push_back("3.14");
    vecData.push_back("1*250");
    vecData.push_back("1*250/3.14-(-3*6)");
    vecData.push_back("-6/2.5*3.14+(-3*-6.3)");

    int iSize = vecData.size();
    for (int i = 0; i < iSize; ++i)
    {
        double dValue2 = 0.0;

        ExpresstionEval cal(vecData[i]);
        if (cal.CheckExpression())
        {
            bool isOk = cal.GetResult(dValue2);
        }
        else
        {
            dValue2 = 0.0;
        }
    }
}

void testCalculation(const std::map<std::string, double>& mapExpresstion2Value)
{
    std::map<std::string, double>::const_iterator iter = mapExpresstion2Value.begin();
    for (; iter != mapExpresstion2Value.end(); ++iter)
    {
        std::string strExpresstion = iter->first;

        double dRightValue = iter->second;

        double dValue = DBL_MAX;
        {
            ExpresstionEval cal(strExpresstion);
            // if(cal.CheckExpression())
            {
                bool isOk = cal.GetResult(dValue);
            }
        }

        if (sindy::compare(dRightValue, dValue) != 0)
            std::cout << "Calculate Error" << std::endl;
    }
}

void readFormulaData(std::map<std::string, double>& mapExpresstion2Value)
{
    std::string strSql("select expresstion,result from Formula");

    sindy::SQLite database(TEST_CUR_DB_PATH);
    database.beginTransaction();
    database.prepare(strSql);

    while (database.step() == SQLITE_ROW)
    {
        std::string strExpresstion;
        database.getValueText("expresstion", strExpresstion);

        double dValue = 0.0;
        database.getValueDouble("result", dValue);

        mapExpresstion2Value[strExpresstion] = dValue;
    }
}

} // namespace TestMathExpresstion