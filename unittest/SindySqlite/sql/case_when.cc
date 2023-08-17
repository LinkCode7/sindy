#include "case_when.h"

#include <format>

#include "../../../sindy/SQLite.h"
#include "../ConstDefine.h"

using namespace sindy;

void test_case_when1();
void make_statistics_table();

void test_case_when()
{
    make_statistics_table();
    test_case_when1();
}

void test_case_when1()
{
    std::string strPath = TEST_CUR_DB_PATH;
    SQLite      database(strPath);
    database.beginTransaction();

    auto strMsg = database.errorMessage();

    std::string strSql, strResultTable;
    {
        strResultTable = "statistics_sum_result";
        database.dropTable(strResultTable);
        strMsg = database.errorMessage();

        strSql = std::format("create table {} as select sum({}) all_population from {}", strResultTable,
                             sindy::statistics::population, sindy::statistics::table_name);
        database.execute(strSql);
    }
    {
        strResultTable = "statistics_sum_result2";
        database.dropTable(strResultTable);

        // 最后必须要group by
        std::string view =
            "create table {} as select sum({}) result, CASE {} WHEN '中国' THEN '亚洲' WHEN '印度' THEN '亚洲' WHEN '日本' "
            "THEN '亚洲' "
            "WHEN '美国' THEN '北美洲' WHEN '加拿大' THEN '北美洲' WHEN '墨西哥' THEN '北美洲' WHEN '英国' THEN '欧洲' WHEN "
            "'法国' THEN "
            "'欧洲' WHEN '德国' THEN '欧洲' ELSE '其它' END as state from {} GROUP BY {}";

        // strSql = std::format(view, strResultTable, sindy::statistics::population, sindy::statistics::country,
        // sindy::statistics::table_name,
        //                     sindy::statistics::country);

        database.execute(strSql);
        strMsg = database.errorMessage();
    }

    database.commit();
    strMsg = database.errorMessage();
}

void make_statistics_table()
{
    auto   strPath = TEST_CUR_DB_PATH;
    SQLite sqlite(strPath);
    sqlite.beginTransaction();

    sqlite.dropTable(sindy::statistics::table_name);

    StatisticsTable table;
    std::string     strSql = table.GetCreateSql();

    if (sqlite.execute(strSql) != SQLITE_DONE)
        return;

    strSql = table.GetInsertSql(sindy::statistics::table_name);
    sqlite.prepare(strSql);
    auto strMsg = sqlite.errorMessage();

    table.fill(sqlite, "中国", 600);
    table.fill(sqlite, "印度", 250);
    table.fill(sqlite, "日本", 250);
    table.fill(sqlite, "美国", 100);
    table.fill(sqlite, "加拿大", 100);
    table.fill(sqlite, "墨西哥", 50);
    table.fill(sqlite, "英国", 200);
    table.fill(sqlite, "法国", 300);
    table.fill(sqlite, "德国", 200);
    sqlite.commit();
    strMsg = sqlite.errorMessage();
}

void StatisticsTable::fill(SQLite& sqlite, const std::string& strCountry, int population)
{
    this->SetValue(sindy::statistics::country, strCountry);
    this->SetValue(sindy::statistics::population, population);
    this->InsertRecord(sqlite);
}

StatisticsTable::StatisticsTable()
{
    AddItem(sindy::statistics::country, sindy::kTextField);
    AddItem(sindy::statistics::population, sindy::kIntegerField);
}

std::string StatisticsTable::GetCreateSql()
{
    return _CreateSql(sindy::statistics::table_name, false, false);
}