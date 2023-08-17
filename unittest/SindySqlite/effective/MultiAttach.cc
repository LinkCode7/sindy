#include "MultiAttach.h"

#include <format>

#include "../../../sindy/SQLite.h"

namespace TestMultiAttach
{
void entry()
{
    // testAttach();
    testAttach2();
}

void testAttach()
{
    // 操作多个数据库时，SQLite可以一个绑定、一个不绑定
    std::string strDbPath1 = "C:\\Users\\Administrator\\Desktop\\test.db";
    std::string strDbPath2 = "C:\\Users\\Administrator\\Desktop\\test2.db";

    sindy::SQLite sqlite(strDbPath1); // 默认

    sqlite.beginTransaction();
    sqlite.attach(strDbPath2, "P"); // 绑定

    std::string strMsg = sqlite.errorMessage();

    int         iValue = -1;
    std::string strValue;
    {
        std::string strSql = std::format("select * from {}", "components");

        sqlite.prepare(strSql);
        strMsg = sqlite.errorMessage();

        if (sqlite.step() == SQLITE_ROW)
        {
            strMsg = sqlite.errorMessage();
            sqlite.getValueInt("id", iValue);
            sqlite.getValueText("comname", strValue);
            strMsg = sqlite.errorMessage();
        }
    }

    int         iValue2 = -1;
    std::string strValue2;
    {
        std::string strSql2 = std::format("select * from P.{}", "components");

        sqlite.prepare(strSql2);
        strMsg = sqlite.errorMessage();

        if (sqlite.step() == SQLITE_ROW)
        {
            strMsg = sqlite.errorMessage();
            sqlite.getValueInt("id", iValue2);
            sqlite.getValueText("comname", strValue2);
            strMsg = sqlite.errorMessage();
        }
    }
    sqlite.detach("P");
}

void testAttach2()
{
    std::string   strDbPath = "C:\\Users\\Administrator\\Desktop\\test.db";
    sindy::SQLite sqlite(strDbPath); // 相当于sqlite.Attach(strDbPath, "main");

    std::string strSql = std::format("select * from main.{}", "components");

    sqlite.prepare(strSql);
    std::string strMsg = sqlite.errorMessage();

    int         iValue = -1;
    std::string strValue;

    if (sqlite.step() == SQLITE_ROW)
    {
        strMsg = sqlite.errorMessage();
        sqlite.getValueInt("id", iValue);
        sqlite.getValueText("comname", strValue);
        strMsg = sqlite.errorMessage();
    }
}

} // namespace TestMultiAttach
