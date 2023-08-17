#pragma once
#include "../../../sindy/SQLite.h"
#include "../TableData.h"

void test_group_by();

class GroupByTable : public sindy::TableData
{
public:
    GroupByTable();
    std::string GetCreateSql();

    void fill(sindy::SQLite& db, int id, const std::string& strProvince, const std::string& strCity, const std::string& strCounty,
              const std::string& strTown, const std::string& strVillage);
};

class FigureTable : public sindy::TableData
{
public:
    FigureTable();
    std::string GetCreateSql();

    void fill(sindy::SQLite& db, const std::string& letter, int val1, int val2, int val3, int val4, int val5);
};

// group by分组、去重
void test_group_by();
void make_group_table();

// 数字字段，group by + sum
void test_figure();
void make_figure_table();
