#ifndef SERVICE_H
#define SERVICE_H

#include "reader.h"
#include "queue.h"

// 前向声明，因为LibrarySystem在persistence.h中定义
typedef struct LibrarySystem LibrarySystem;

#define MAX_WINDOWS 5

typedef struct {
    int id;                 // 窗口ID
    int isActive;           // 是否激活
    char currentReaderId[MAX_ID_LEN];  // 当前服务读者ID
    ServiceType serviceType; // 当前服务类型
    time_t startTime;       // 开始服务时间
    int totalServed;        // 总服务人数
} ServiceWindow;

// 服务相关函数
void takeQueueNumber(LibrarySystem* system);
void displayCurrentQueue(LibrarySystem* system);

// 窗口管理函数
void initializeWindows(ServiceWindow windows[], int count);
void callNextReader(LibrarySystem* system, ServiceWindow windows[], int windowCount);
void completeCurrentService(LibrarySystem* system, ServiceWindow windows[], int windowCount);
void cancelCurrentService(LibrarySystem* system, ServiceWindow windows[], int windowCount);
void displayWindowStatus(ServiceWindow windows[], int count);

#endif // SERVICE_H