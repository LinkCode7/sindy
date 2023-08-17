#pragma once
#include <memory>
#include <string>
#include <vector>

namespace sindy
{
class SQLite;
class DataRecord;
class Dataset2;

enum
{
    kUnknownField = 0,
    kTextField    = 1,
    kDoubleField  = 2,
    kIntegerField = 3,
    kBlobField    = 4,
};

// 字段类型的基类
class VirtualItem
{
public:
    std::string m_strKey;
    int         m_iType;
    VirtualItem();
    VirtualItem(const std::string& strKey, int iType);

    virtual std::string GetValue() { return std::string(); }
    virtual void*       GetBlobValue() { return nullptr; }
    virtual void        SetValue(const std::string& strValue) {}

    std::string GetProperty();
};
typedef std::shared_ptr<VirtualItem> VirtualItemPtr;

// 普通字段：整型、浮点数、文本
class DataItem : public VirtualItem
{
public:
    std::string m_strValue;

    DataItem();
    DataItem(const std::string& strKey, const std::string& strValue, int iType = kTextField);
    DataItem(const std::string& strKey, int iType = kTextField); // m_strValue为空

    std::string GetValue() override;
    void        SetValue(const std::string& strValue) override;
};

// 二进制字段
class BlobItem : public VirtualItem
{
public:
    void* m_pBuffer;
    long  m_lBufferSize;

    BlobItem(const std::string& strKey);
    ~BlobItem();

    void* GetBlobValue() override;
};

// 用于界面显示的item
class ColumnItem : public VirtualItem
{
public:
    std::string m_strValue; // 显示名称
    bool        m_isFixed;  // 是否固定
    bool        m_isHide;
    int         m_iWidth; // 列宽

    ColumnItem();
    ColumnItem(const std::string& strName, const std::string& strCaption, bool isFixed, int iWidth, bool isHide);

    std::string GetValue() override;
    void        SetValue(const std::string& strValue) override;
};
using ColumnItemPtr = std::shared_ptr<ColumnItem>;

// 一行数据，列的集合
class TableData
{
public:
    std::string                 m_strDbAliasName;
    std::vector<VirtualItemPtr> m_items;

    // 必须指定数据库别名，没有附加就填"main"
    TableData();

    inline void AddItem(const std::string& strKey, const std::string& strValue, int iType)
    {
        m_items.push_back(std::make_shared<DataItem>(strKey, strValue, iType));
    }
    inline void AddItem(const std::string& strKey, int iType)
    { // Value为空
        m_items.push_back(std::make_shared<DataItem>(strKey, iType));
    }
    inline void AddItem(const TableData& other) { m_items.insert(m_items.end(), other.m_items.begin(), other.m_items.end()); }
    inline void AddItemKey(const TableData& other);

    inline void AddBlobItem(const std::string& strKey)
    { // Value为空
        m_items.push_back(std::make_shared<BlobItem>(strKey));
    }

    // 用于界面显示的item
    void AddColumnItem(const std::string& strName, const std::string& strCaption, bool isFixed, int iWidth, bool isHide);

    std::string GetValueString(const std::string& strKey);
    double      GetValueDouble(const std::string& strKey);
    int         GetValueInt(const std::string& strKey);

    // 设置属性值，strKey必须已经在数组中初始化了
    bool SetValue(const std::string& strKey, const std::string& strValue);
    bool SetValue(const std::string& strKey, double dValue);
    bool SetValue(const std::string& strKey, int iValue);

    // 设置属性值，strKey不存在就新增
    void SetValue2(const std::string& strKey, const std::string& strValue);

    inline void Reset() { m_items.clear(); }

    // 返回"?,?,?"形式的预编译语句insert
    std::string GetInsertSql(const std::string& strTableName);

    void InsertRecord(SQLite& database);
    void InsertRecord(SQLite& database, const std::shared_ptr<DataRecord>& record);
    void InsertRecordKeyValue(SQLite& database);

    // 获取单个判定值的update语句
    std::string GetUpdateSql(const std::string& strTableName, const std::string& strKey);
    void        UpdateRecord(SQLite& database, const std::string& strKey);

    // 返回全部是TEXT字段的create语句
    void CreateSqlOfText(const std::string& strTableName, std::string& strCreateSql);

    // 请务必实现create语句，调用_CreateSql()
    virtual std::string GetCreateSql() { return std::string(); }

    void FillData(SQLite& database, const std::string& strTableName, Dataset2& dataset);

protected:
    // 返回设定字段类型的create语句
    std::string _CreateSql(const std::string& strDbAliasName, bool isPrimary = false, bool isAuto = false);
};
typedef std::shared_ptr<TableData> TableDataPtr;

// 记录集，多行数据的集合
class Dataset
{
public:
    std::vector<TableDataPtr> m_records;

    inline void  AddItem(const TableDataPtr& record) { m_records.push_back(record); }
    TableDataPtr GetItem(const std::string& strRowName);

    std::string GetValue(const std::string& strRowName, const std::string& strFieldName);
};
typedef std::shared_ptr<Dataset> DatasetPtr;

class DataRecord
{
public:
    std::vector<std::string> m_data;
};
typedef std::shared_ptr<DataRecord> DataRecordPtr;

class Dataset2
{
public:
    TableDataPtr               m_schema;
    std::vector<DataRecordPtr> m_records;

    Dataset2(const TableDataPtr& schema);
    void FillData(SQLite& database, const std::string& strTableName);

    inline void AddRecord(const DataRecordPtr& record) { m_records.push_back(record); }

    void InsertRecords(SQLite& database);
};
typedef std::shared_ptr<Dataset2> Dataset2Ptr;

} // namespace sindy
