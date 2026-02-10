#include "../include/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

QueueNode* createQueueNode(QueueRecord record) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        fprintf(stderr, "内存分配失败！\n");
        return NULL;
    }
    
    newNode->record = record;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

LibraryQueue* createQueue() {
    LibraryQueue* queue = (LibraryQueue*)malloc(sizeof(LibraryQueue));
    if (queue == NULL) {
        return NULL;
    }
    
    queue->front = NULL;
    queue->rear = NULL;
    queue->priorityFront = NULL;
    queue->size = 0;
    queue->currentNumber = 1000;
    
    return queue;
}

int enqueue(LibraryQueue* queue, const char* readerId, ServiceType serviceType) {
    if (queue == NULL || readerId == NULL) {
        return -1;
    }
    
    QueueRecord record;
    strcpy(record.readerId, readerId);
    record.queueNumber = queue->currentNumber++;
    record.joinTime = time(NULL);
    record.startServiceTime = 0;
    record.endServiceTime = 0;
    record.status = WAITING;
    record.serviceType = serviceType;
    record.windowId = -1;
    
    QueueNode* newNode = createQueueNode(record);
    if (newNode == NULL) {
        return -1;
    }
    
    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        newNode->prev = queue->rear;
        queue->rear = newNode;
    }
    
    queue->size++;
    return record.queueNumber;
}

int priorityEnqueue(LibraryQueue* queue, const char* readerId, ServiceType serviceType) {
    if (queue == NULL || readerId == NULL) {
        return -1;
    }
    
    QueueRecord record;
    strcpy(record.readerId, readerId);
    record.queueNumber = queue->currentNumber++;
    record.joinTime = time(NULL);
    record.startServiceTime = 0;
    record.endServiceTime = 0;
    record.status = WAITING;
    record.serviceType = serviceType;
    record.windowId = -1;
    
    QueueNode* newNode = createQueueNode(record);
    if (newNode == NULL) {
        return -1;
    }
    
    if (queue->priorityFront == NULL) {
        if (queue->front == NULL) {
            queue->front = newNode;
            queue->rear = newNode;
        } else {
            newNode->next = queue->front;
            queue->front->prev = newNode;
            queue->front = newNode;
        }
        queue->priorityFront = newNode;
    } else {
        QueueNode* current = queue->priorityFront;
        while (current->next != NULL && current->next->record.status == WAITING) {
            current = current->next;
        }
        
        newNode->next = current->next;
        if (current->next != NULL) {
            current->next->prev = newNode;
        }
        current->next = newNode;
        newNode->prev = current;
        
        if (newNode->next == NULL) {
            queue->rear = newNode;
        }
    }
    
    queue->size++;
    return record.queueNumber;
}

QueueRecord* dequeue(LibraryQueue* queue) {
    if (queue == NULL || queue->front == NULL) {
        return NULL;
    }
    
    QueueNode* nodeToServe = NULL;
    
    QueueNode* current = queue->front;
    while (current != NULL) {
        if (current->record.status == WAITING) {
            nodeToServe = current;
            break;
        }
        current = current->next;
    }
    
    if (nodeToServe == NULL) {
        return NULL;
    }
    
    nodeToServe->record.status = IN_SERVICE;
    nodeToServe->record.startServiceTime = time(NULL);
    
    QueueRecord* recordCopy = (QueueRecord*)malloc(sizeof(QueueRecord));
    if (recordCopy == NULL) {
        return NULL;
    }
    *recordCopy = nodeToServe->record;
    
    return recordCopy;
}

QueueNode* findQueueNode(LibraryQueue* queue, const char* readerId) {
    if (queue == NULL || readerId == NULL) {
        return NULL;
    }
    
    QueueNode* current = queue->front;
    while (current != NULL) {
        if (strcmp(current->record.readerId, readerId) == 0 && 
            current->record.status == WAITING) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

int cancelQueue(LibraryQueue* queue, const char* readerId) {
    QueueNode* node = findQueueNode(queue, readerId);
    if (node == NULL) {
        return 0;
    }
    
    node->record.status = CANCELLED;
    node->record.endServiceTime = time(NULL);
    
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        queue->front = node->next;
    }
    
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        queue->rear = node->prev;
    }
    
    if (queue->priorityFront == node) {
        queue->priorityFront = NULL;
    }
    
    free(node);
    queue->size--;
    return 1;
}

void displayQueue(LibraryQueue* queue) {
    if (queue == NULL || queue->front == NULL) {
        printf("当前队列为空\n");
        return;
    }
    
    printf("\n=== 当前排队情况 ===\n");
    printf("%-6s %-15s %-8s %-20s %-10s\n", 
           "号码", "读者ID", "服务类型", "排队时间", "状态");
    printf("------------------------------------------------------------\n");
    
    QueueNode* current = queue->front;
    int waitingCount = 0;
    int priorityCount = 0;
    
    while (current != NULL) {
        char serviceTypeStr[20];
        char statusStr[10];
        char timeStr[20];
        
        switch (current->record.serviceType) {
            case BORROW_BOOK: strcpy(serviceTypeStr, "借书"); break;
            case RETURN_BOOK: strcpy(serviceTypeStr, "还书"); break;
            case RENEW_BOOK: strcpy(serviceTypeStr, "续借"); break;
            case QUERY_INFO: strcpy(serviceTypeStr, "查询"); break;
            case PAY_FINE: strcpy(serviceTypeStr, "缴费"); break;
            case REGISTER_CARD: strcpy(serviceTypeStr, "办证"); break;
            default: strcpy(serviceTypeStr, "未知");
        }
        
        switch (current->record.status) {
            case WAITING: strcpy(statusStr, "等待中"); waitingCount++; break;
            case IN_SERVICE: strcpy(statusStr, "服务中"); break;
            case COMPLETED: strcpy(statusStr, "已完成"); break;
            case CANCELLED: strcpy(statusStr, "已取消"); break;
            default: strcpy(statusStr, "未知");
        }
        
        struct tm* timeinfo = localtime(&current->record.joinTime);
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
        
        char displayId[20];
        if (current == queue->priorityFront || 
            (current->prev == queue->priorityFront && queue->priorityFront != NULL)) {
            strcpy(displayId, "*");
            strcat(displayId, current->record.readerId);
            priorityCount++;
        } else {
            strcpy(displayId, current->record.readerId);
        }
        
        printf("%-6d %-15s %-8s %-20s %-10s\n",
               current->record.queueNumber,
               displayId,
               serviceTypeStr,
               timeStr,
               statusStr);
        
        current = current->next;
    }
    
    printf("\n统计：等待中：%d人 | 优先读者：%d人 | 总排队：%d人\n", 
           waitingCount, priorityCount, queue->size);
}

int estimateWaitTime(LibraryQueue* queue, int queueNumber) {
    if (queue == NULL) {
        return -1;
    }
    
    int position = 0;
    QueueNode* current = queue->front;
    
    while (current != NULL) {
        if (current->record.queueNumber == queueNumber) {
            if (current->record.status != WAITING) {
                return 0;
            }
            
            int estimatedMinutes = position * 3;
            
            if (current == queue->priorityFront || 
                (current->prev == queue->priorityFront && queue->priorityFront != NULL)) {
                estimatedMinutes = (position - 1) * 3;
            }
            
            return estimatedMinutes > 0 ? estimatedMinutes : 1;
        }
        
        if (current->record.status == WAITING) {
            position++;
        }
        
        current = current->next;
    }
    
    return -1;
}

void destroyQueue(LibraryQueue* queue) {
    if (queue == NULL) {
        return;
    }
    
    QueueNode* current = queue->front;
    QueueNode* next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    free(queue);
}

const char* serviceTypeToString(ServiceType type) {
    switch(type) {
        case BORROW_BOOK: return "借书";
        case RETURN_BOOK: return "还书";
        case RENEW_BOOK: return "续借";
        case QUERY_INFO: return "查询";
        case PAY_FINE: return "缴费";
        case REGISTER_CARD: return "办证";
        default: return "未知";
    }
}

const char* queueStatusToString(QueueStatus status) {
    switch(status) {
        case WAITING: return "等待中";
        case IN_SERVICE: return "服务中";
        case COMPLETED: return "已完成";
        case CANCELLED: return "已取消";
        default: return "未知";
    }
}