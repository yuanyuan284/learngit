#ifndef QUEUE_H
#define QUEUE_H

#include <time.h>

#define MAX_ID_LEN 15

typedef enum {
    BORROW_BOOK,     // 借书
    RETURN_BOOK,     // 还书
    RENEW_BOOK,      // 续借
    QUERY_INFO,      // 信息查询
    PAY_FINE,        // 缴纳罚款
    REGISTER_CARD    // 办证
} ServiceType;

typedef enum {
    WAITING,      // 等待中
    IN_SERVICE,   // 服务中
    COMPLETED,    // 已完成
    CANCELLED     // 已取消
} QueueStatus;

typedef struct {
    char readerId[MAX_ID_LEN];    // 读者ID
    int queueNumber;             // 排队号码
    time_t joinTime;             // 加入队列时间
    time_t startServiceTime;     // 开始服务时间
    time_t endServiceTime;       // 结束服务时间
    QueueStatus status;          // 状态
    ServiceType serviceType;     // 服务类型
    int windowId;               // 服务窗口ID
} QueueRecord;

typedef struct QueueNode {
    QueueRecord record;         // 排队记录
    struct QueueNode* next;     // 下一个节点
    struct QueueNode* prev;     // 前一个节点
} QueueNode;

typedef struct {
    QueueNode* front;           // 队首
    QueueNode* rear;            // 队尾
    QueueNode* priorityFront;   // 优先队列起始
    int size;                   // 队列大小
    int currentNumber;          // 当前排队号码
} LibraryQueue;

// 队列管理函数
LibraryQueue* createQueue();
void destroyQueue(LibraryQueue* queue);
int enqueue(LibraryQueue* queue, const char* readerId, ServiceType serviceType);
int priorityEnqueue(LibraryQueue* queue, const char* readerId, ServiceType serviceType);
QueueRecord* dequeue(LibraryQueue* queue);
QueueNode* findQueueNode(LibraryQueue* queue, const char* readerId);
int cancelQueue(LibraryQueue* queue, const char* readerId);
void displayQueue(LibraryQueue* queue);
int estimateWaitTime(LibraryQueue* queue, int queueNumber);
const char* serviceTypeToString(ServiceType type);
const char* queueStatusToString(QueueStatus status);

#endif // QUEUE_H