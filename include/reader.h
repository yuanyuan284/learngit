#ifndef READER_H
#define READER_H

#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_ID_LEN 15
#define MAX_PHONE_LEN 15

typedef enum {
    STUDENT,
    TEACHER,
    STAFF,
    VISITOR
} ReaderType;

typedef struct {
    char id[MAX_ID_LEN];           // 读者ID/学号
    char name[MAX_NAME_LEN];       // 姓名
    ReaderType type;               // 读者类型
    char phone[MAX_PHONE_LEN];     // 联系电话
    int borrowedCount;            // 已借阅数量
    int maxBorrowLimit;           // 最大借阅限制
    time_t registerTime;          // 注册时间
} Reader;

// 读者管理函数
void registerNewReader();
void queryReaderInfo();
void modifyReaderInfo();
void deleteReader();
void displayAllReaders();
void displayReaderInfo(const Reader* reader);
const char* readerTypeToString(ReaderType type);

#endif // READER_H