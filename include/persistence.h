#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "reader.h"
#include "queue.h"

#define DATA_FILE "library_data.dat"
#define MAX_READERS 1000

// 必须先定义这个结构体，因为其他头文件需要它
typedef struct LibrarySystem {
    Reader readers[MAX_READERS];  // 读者数组
    int readerCount;              // 读者数量
    LibraryQueue* queue;          // 队列
} LibrarySystem;

// 系统管理函数
LibrarySystem* createLibrarySystem();
void destroyLibrarySystem(LibrarySystem* system);
void initializeSampleReaders(LibrarySystem* system);

// 持久化函数
void saveToFile(LibrarySystem* system, const char* filename);
void loadFromFile(LibrarySystem* system, const char* filename);
void backupData(LibrarySystem* system);
void restoreData(LibrarySystem* system);

#endif // PERSISTENCE_H