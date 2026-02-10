#ifndef STATISTICS_H
#define STATISTICS_H

#include <time.h>  // 添加这行，包含time_t的定义

// 前向声明
typedef struct LibrarySystem LibrarySystem;

// 统计函数
void displayStatistics(LibrarySystem* system);
void displayReaderStatistics(LibrarySystem* system);
void displayServiceStatistics(LibrarySystem* system);
void displayQueueStatistics(LibrarySystem* system);
void displayWaitTimeAnalysis(LibrarySystem* system);

// 时间格式函数
void formatTime(time_t t, char* buffer, int bufferSize);
void formatDateTime(time_t t, char* buffer, int bufferSize);

#endif // STATISTICS_H