#ifndef HIGH_PERFORMANCE_H
#define HIGH_PERFORMANCE_H

#include <string>

namespace TestSQLitePerformance
{
void entry();

// 预编译SQL语句
void testInsert(const std::string& strDbPath);

void testUpdate(const std::string& strDbPath);
void testSelect(const std::string& strDbPath);

void testPragma(const std::string& strDbPath);
void bindInsertSQLWithNumeric(const std::string& strDbPath);

// 判断是否存在某行数据
void testExistsData(const std::string& strDbPath);

// void testUnlargeField(const std::string& strDbPath);
void testLargeField(const std::string& strDbPath, const std::string& strTableName, bool isLarge);

// 获取满足某条件的行数量
void selectCount(const std::string& strDbPath);

} // namespace TestSQLitePerformance

#endif // !HIGH_PERFORMANCE_H
