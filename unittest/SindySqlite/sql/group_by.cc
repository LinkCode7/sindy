#include "group_by.h"

#include <format>

#include "../ConstDefine.h"

using namespace sindy;

void make_group_table();
void _test_group_by();
void make_figure_table();
void test_figure();

void test_group_by()
{
    make_group_table();
    _test_group_by();

    make_figure_table();
    test_figure();
}

void _test_group_by()
{
    auto   strPath = TEST_CUR_DB_PATH;
    SQLite database(strPath);
    database.beginTransaction();

    std::string strResultTable = "group_by1_result";
    database.dropTable(strResultTable);

    // AddItem(sindy::group_by::id, sindy::kIntegerField);
    // AddItem(sindy::group_by::province, sindy::kTextField);
    // AddItem(sindy::group_by::city, sindy::kTextField);
    // AddItem(sindy::group_by::county, sindy::kTextField);
    // AddItem(sindy::group_by::town, sindy::kTextField);
    // AddItem(sindy::group_by::village, sindy::kTextField);

    std::string strSql =
        std::format("create table {} as select * from {} group by {},{},{},{}", strResultTable, sindy::group_by::table_name,
                    sindy::group_by::province, sindy::group_by::city, sindy::group_by::county, sindy::group_by::town);

    database.execute(strSql);
    database.commit();
}

void make_group_table()
{
    auto   strPath = TEST_CUR_DB_PATH;
    SQLite sqlite(strPath);
    sqlite.beginTransaction();

    sqlite.dropTable(sindy::group_by::table_name);

    GroupByTable table;
    std::string  strSql = table.GetCreateSql();

    if (sqlite.execute(strSql) != SQLITE_DONE)
        return;

    strSql = table.GetInsertSql(sindy::group_by::table_name);
    sqlite.prepare(strSql);

    int id = 0;
    table.fill(sqlite, id++, "浙江", "杭州市", "桐庐县", "小镇2", "村1");
    table.fill(sqlite, id++, "浙江", "湖州市", "安吉县", "小镇1", "村1");
    table.fill(sqlite, id++, "浙江", "宁波市", "象山县", "小镇1", "村1");
    table.fill(sqlite, id++, "浙江", "宁波市", "宁海县", "小镇2", "村1");
    table.fill(sqlite, id++, "浙江", "温州市", "永嘉县", "小镇1", "村1");
    table.fill(sqlite, id++, "浙江", "丽水市", "松阳县", "小镇1", "村1");
    table.fill(sqlite, id++, "浙江", "丽水市", "松阳县", "小镇2", "村1");
    table.fill(sqlite, id++, "河北", "雄安", "县1", "小镇1", "村1");
    table.fill(sqlite, id++, "河北", "雄安", "县1", "小镇1", "村2");
    table.fill(sqlite, id++, "上海", "徐汇", "3-a", "4-a", "5-a");
    table.fill(sqlite, id++, "上海", "黄浦", "3-a", "4-a", "5-a");
    table.fill(sqlite, id++, "上海", "黄浦", "3-b", "4-a", "5-b");
    table.fill(sqlite, id++, "北京", "东城区", "3-b", "4-a", "5-b");
    table.fill(sqlite, id++, "北京", "西城区", "3-b", "4-a", "5-b");

    sqlite.commit();
}

void GroupByTable::fill(SQLite& db, int id, const std::string& strProvince, const std::string& strCity, const std::string& strCounty,
                        const std::string& strTown, const std::string& strVillage)
{
    this->SetValue(sindy::group_by::id, id);
    this->SetValue(sindy::group_by::province, strProvince);
    this->SetValue(sindy::group_by::city, strCity);
    this->SetValue(sindy::group_by::county, strCounty);
    this->SetValue(sindy::group_by::town, strTown);
    this->SetValue(sindy::group_by::village, strVillage);
    this->InsertRecord(db);
}

GroupByTable::GroupByTable()
{
    AddItem(sindy::group_by::id, sindy::kIntegerField);
    AddItem(sindy::group_by::province, sindy::kTextField);
    AddItem(sindy::group_by::city, sindy::kTextField);
    AddItem(sindy::group_by::county, sindy::kTextField);
    AddItem(sindy::group_by::town, sindy::kTextField);
    AddItem(sindy::group_by::village, sindy::kTextField);
}
std::string GroupByTable::GetCreateSql()
{
    return _CreateSql(sindy::group_by::table_name, false, false);
}

void test_figure()
{
    auto   strPath = TEST_CUR_DB_PATH;
    SQLite database(strPath);
    database.beginTransaction();

    std::string strResultTable = "figure_result";
    database.dropTable(strResultTable);

    std::string strSql = std::format("create table {} as select * from {} group by {},{},{},{}", strResultTable, sindy::figure::table_name,
                                     sindy::figure::field1, sindy::figure::field2, sindy::figure::field3, sindy::figure::field4);

    strSql = std::format(
        "create table {} as select {},sum(field1) field1,sum(field2) field2,sum(field3) field3,sum(field4) field4,sum(field5) field5 "
        "from {} group by {},{},{},{}",
        strResultTable, sindy::figure::letter, sindy::figure::table_name, sindy::figure::field1, sindy::figure::field2,
        sindy::figure::field3, sindy::figure::field4);

    database.execute(strSql);
    auto strMsg = database.errorMessage();
    database.commit();
}

void make_figure_table()
{
    auto   strPath = TEST_CUR_DB_PATH;
    SQLite sqlite(strPath);
    sqlite.beginTransaction();

    sqlite.dropTable(sindy::figure::table_name);

    FigureTable table;
    std::string strSql = table.GetCreateSql();

    if (sqlite.execute(strSql) != SQLITE_DONE)
        return;

    strSql = table.GetInsertSql(sindy::figure::table_name);
    sqlite.prepare(strSql);

    table.fill(sqlite, "a", 1, 2, 3, 4, 5);
    table.fill(sqlite, "b", 11, 2, 3, 4, 5);
    table.fill(sqlite, "c", 11, 22, 3, 4, 5);
    table.fill(sqlite, "d", 11, 22, 333, 4, 5);
    table.fill(sqlite, "e", 1, 22, 33, 4, 5);
    table.fill(sqlite, "f", 1, 22, 33, 444, 5);
    table.fill(sqlite, "g", 1, 22, 33, 444, 55);
    table.fill(sqlite, "h", 11, 22, 33, 44, 55);
    table.fill(sqlite, "i", 11, 22, 33, 44, 5);

    sqlite.commit();
}

void FigureTable::fill(SQLite& db, const std::string& letter, int val1, int val2, int val3, int val4, int val5)
{
    this->SetValue(sindy::figure::letter, letter);
    this->SetValue(sindy::figure::field1, val1);
    this->SetValue(sindy::figure::field2, val2);
    this->SetValue(sindy::figure::field3, val3);
    this->SetValue(sindy::figure::field4, val4);
    this->SetValue(sindy::figure::field5, val5);
    this->InsertRecord(db);
}

FigureTable::FigureTable()
{
// #define _FIELD_TYPE_ sindy::kIntegerField
#define _FIELD_TYPE_ sindy::kTextField
    // 文本字段类型也能执行sum操作（group by时）
    AddItem(sindy::figure::letter, sindy::kTextField);
    AddItem(sindy::figure::field1, _FIELD_TYPE_);
    AddItem(sindy::figure::field2, _FIELD_TYPE_);
    AddItem(sindy::figure::field3, _FIELD_TYPE_);
    AddItem(sindy::figure::field4, _FIELD_TYPE_);
    AddItem(sindy::figure::field5, _FIELD_TYPE_);
#undef _FIELD_TYPE_
}
std::string FigureTable::GetCreateSql()
{
    return _CreateSql(sindy::figure::table_name, false, false);
}