#include "../include/service.h"
#include "../include/persistence.h"  // 添加这行
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void takeQueueNumber(LibrarySystem* system) {
    char readerId[MAX_ID_LEN];
    
    printf("\n--- 排队取号 ---\n");
    printf("请输入读者ID: ");
    fgets(readerId, MAX_ID_LEN, stdin);
    readerId[strcspn(readerId, "\n")] = '\0';
    
    int readerExists = 0;
    Reader* reader = NULL;
    
    for (int i = 0; i < system->readerCount; i++) {
        if (strcmp(system->readers[i].id, readerId) == 0) {
            readerExists = 1;
            reader = &system->readers[i];
            break;
        }
    }
    
    if (!readerExists) {
        printf("读者ID不存在，请先注册！\n");
        return;
    }
    
    printf("\n请选择服务类型:\n");
    printf("1. 借书\n");
    printf("2. 还书\n");
    printf("3. 续借\n");
    printf("4. 信息查询\n");
    printf("5. 缴纳罚款\n");
    printf("6. 办理读者证\n");
    printf("选择: ");
    
    int serviceChoice;
    scanf("%d", &serviceChoice);
    getchar();
    
    ServiceType serviceType;
    switch (serviceChoice) {
        case 1: serviceType = BORROW_BOOK; break;
        case 2: serviceType = RETURN_BOOK; break;
        case 3: serviceType = RENEW_BOOK; break;
        case 4: serviceType = QUERY_INFO; break;
        case 5: serviceType = PAY_FINE; break;
        case 6: serviceType = REGISTER_CARD; break;
        default: serviceType = BORROW_BOOK;
    }
    
    QueueNode* existingNode = findQueueNode(system->queue, readerId);
    if (existingNode != NULL && existingNode->record.status == WAITING) {
        printf("您已经在队列中，排队号码: %d\n", existingNode->record.queueNumber);
        return;
    }
    
    int queueNumber;
    if (reader->type == TEACHER || reader->type == STAFF) {
        queueNumber = priorityEnqueue(system->queue, readerId, serviceType);
        printf("优先排队成功！您的排队号码: %d (优先)\n", queueNumber);
    } else {
        queueNumber = enqueue(system->queue, readerId, serviceType);
        printf("排队成功！您的排队号码: %d\n", queueNumber);
    }
    
    int waitTime = estimateWaitTime(system->queue, queueNumber);
    if (waitTime > 0) {
        printf("预计等待时间: %d 分钟\n", waitTime);
    }
    
    printf("当前队列人数: %d\n", system->queue->size);
}

void displayCurrentQueue(LibrarySystem* system) {
    displayQueue(system->queue);
}

void initializeWindows(ServiceWindow windows[], int count) {
    for (int i = 0; i < count; i++) {
        windows[i].id = i + 1;
        windows[i].isActive = 0;
        windows[i].currentReaderId[0] = '\0';
        windows[i].serviceType = BORROW_BOOK;
        windows[i].startTime = 0;
        windows[i].totalServed = 0;
    }
}