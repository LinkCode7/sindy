#include "petal.h"

#include <format>

#include "../ConstDefine.h"

void make_add_sum_record_table();
void test_add_sum_record();
void test_add_sum_record2();

void test_petal()
{
    make_add_sum_record_table();

    test_add_sum_record();
    test_add_sum_record2();
}

void test_add_sum_record()
{
    auto          strPath = TEST_CUR_DB_PATH;
    sindy::SQLite database(strPath);
    database.beginTransaction();

    std::string strResultTable = "add_sum_record_result";
    database.dropTable(strResultTable);

    // union all不排序，union会排序
    std::string strSql = std::format("create table {} as select * from {} union all select -1,'合计',Sum({}),Sum({}) from {}",
                                     strResultTable, sindy::add_sum_record::table_name, sindy::add_sum_record::type,
                                     sindy::add_sum_record::width, sindy::add_sum_record::table_name);

    // 连接两次查询的结果（合在一起）
    database.execute(strSql);
    auto strMsg = database.errorMessage();
    database.commit();
}

void test_add_sum_record2()
{
    if (1)
    {
        auto          strPath = TEST_CUR_DB_PATH;
        sindy::SQLite database(strPath);
        database.beginTransaction();

        std::string strResultTable = "add_sum_record_result";
        database.dropTable(strResultTable);

        // select部分字段，合并
        std::string strSql = std::format("create table {} as select {},{},{} from {} union all select '合计',Sum({}),Sum({}) from {}",
                                         strResultTable, sindy::add_sum_record::name, sindy::add_sum_record::type,
                                         sindy::add_sum_record::width, sindy::add_sum_record::table_name, sindy::add_sum_record::type,
                                         sindy::add_sum_record::width, sindy::add_sum_record::table_name);

        database.execute(strSql);
        auto strMsg = database.errorMessage();
        database.commit();
    }
}

void make_add_sum_record_table()
{
    auto          strPath = TEST_CUR_DB_PATH;
    sindy::SQLite sqlite(strPath);
    sqlite.beginTransaction();

    sqlite.dropTable(sindy::add_sum_record::table_name);

    AddSumRecordTable table;
    std::string       strSql = table.GetCreateSql();

    if (sqlite.execute(strSql) != SQLITE_DONE)
        return;

    strSql = table.GetInsertSql(sindy::add_sum_record::table_name);
    sqlite.prepare(strSql);

    auto index = 1;
    table.fill(sqlite, index++, "mike", 1, 3.14);
    table.fill(sqlite, index++, "joy", 2, 0.1);
    sqlite.commit();
}

void AddSumRecordTable::fill(sindy::SQLite& db, int id, const std::string& strName, int type, double width)
{
    this->SetValue(sindy::add_sum_record::id, id);
    this->SetValue(sindy::add_sum_record::name, strName);
    this->SetValue(sindy::add_sum_record::type, type);
    this->SetValue(sindy::add_sum_record::width, width);
    this->InsertRecord(db);
}

AddSumRecordTable::AddSumRecordTable()
{
    AddItem(sindy::add_sum_record::id, sindy::kIntegerField);
    AddItem(sindy::add_sum_record::name, sindy::kTextField);
    AddItem(sindy::add_sum_record::type, sindy::kIntegerField);
    AddItem(sindy::add_sum_record::width, sindy::kDoubleField);
}
std::string AddSumRecordTable::GetCreateSql()
{
    return _CreateSql(sindy::add_sum_record::table_name, false, false);
}
