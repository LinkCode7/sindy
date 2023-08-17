#ifndef C_FUNCTIONS_H
#define C_FUNCTIONS_H
#include <string>

struct sqlite3;

namespace SQLiteCFunction
{
void entry();

int  testInsert();
void testExpression();
void testSelect(const char* sql);
int  _callback(void* data, int argc, char** argv, char** azColName);

int DropTable(sqlite3* db, const std::string& strTableName);
int ClearTable(sqlite3* db, const std::string& strTableName);

class ComponentTableInfo
{
public:
    int m_iComId;
    int m_iComType;
    int m_iFloorId;

    std::string m_strName;

    int         m_iShowId;
    int         m_iMode;
    std::string m_strNote;

    ComponentTableInfo()
    {
        m_iComId   = -1;
        m_iComType = -1;
        m_iFloorId = -1;

        m_iShowId = -1;
        m_iMode   = -1;
    }
};

// 基础用法
void createDatabase();
void createTable();
void insertRow();
void select();
void update();
void deleteRow();

} // namespace SQLiteCFunction

#endif // !C_FUNCTIONS_H
