// src/statistics.c - 修复版
#include "../include/statistics.h"
#include "../include/persistence.h"  // 添加这行以获取LibrarySystem的完整定义
#include <stdio.h>
#include <string.h>
#include <time.h>

// 显示统计信息
void displayStatistics(LibrarySystem* system) {
    printf("\n=== 系统统计信息 ===\n");
    printf("1. 读者统计\n");
    printf("2. 服务统计\n");
    printf("3. 队列统计\n");
    printf("4. 等待时间分析\n");
    printf("选择: ");
    
    int choice;
    scanf("%d", &choice);
    getchar();
    
    switch (choice) {
        case 1:
            displayReaderStatistics(system);
            break;
        case 2:
            displayServiceStatistics(system);
            break;
        case 3:
            displayQueueStatistics(system);
            break;
        case 4:
            displayWaitTimeAnalysis(system);
            break;
        default:
            printf("无效选择！\n");
    }
}

// 显示读者统计
void displayReaderStatistics(LibrarySystem* system) {
    printf("\n--- 读者统计 ---\n");
    printf("注册读者总数: %d\n", system->readerCount);
    
    int studentCount = 0, teacherCount = 0, staffCount = 0, visitorCount = 0;
    
    for (int i = 0; i < system->readerCount; i++) {
        switch (system->readers[i].type) {
            case STUDENT: studentCount++; break;
            case TEACHER: teacherCount++; break;
            case STAFF: staffCount++; break;
            case VISITOR: visitorCount++; break;
        }
    }
    
    printf("学生: %d人 (%.1f%%)\n", studentCount, 
           system->readerCount > 0 ? (float)studentCount/system->readerCount*100 : 0);
    printf("教师: %d人 (%.1f%%)\n", teacherCount,
           system->readerCount > 0 ? (float)teacherCount/system->readerCount*100 : 0);
    printf("职工: %d人 (%.1f%%)\n", staffCount,
           system->readerCount > 0 ? (float)staffCount/system->readerCount*100 : 0);
    printf("访客: %d人 (%.1f%%)\n", visitorCount,
           system->readerCount > 0 ? (float)visitorCount/system->readerCount*100 : 0);
}

// 显示服务统计
void displayServiceStatistics(LibrarySystem* system) {
    printf("\n--- 服务统计 ---\n");
    
    int serviceCounts[6] = {0}; // 6种服务类型
    QueueNode* current = system->queue->front;
    
    while (current != NULL) {
        if (current->record.status == COMPLETED) {
            serviceCounts[current->record.serviceType]++;
        }
        current = current->next;
    }
    
    printf("借书服务: %d次\n", serviceCounts[BORROW_BOOK]);
    printf("还书服务: %d次\n", serviceCounts[RETURN_BOOK]);
    printf("续借服务: %d次\n", serviceCounts[RENEW_BOOK]);
    printf("信息查询: %d次\n", serviceCounts[QUERY_INFO]);
    printf("缴纳罚款: %d次\n", serviceCounts[PAY_FINE]);
    printf("办理读者证: %d次\n", serviceCounts[REGISTER_CARD]);
    
    int total = 0;
    for (int i = 0; i < 6; i++) {
        total += serviceCounts[i];
    }
    printf("总计: %d次服务\n", total);
}

// 显示队列统计
void displayQueueStatistics(LibrarySystem* system) {
    printf("\n--- 队列统计 ---\n");
    printf("当前队列大小: %d\n", system->queue->size);
    
    int waitingCount = 0, inServiceCount = 0, completedCount = 0, cancelledCount = 0;
    QueueNode* current = system->queue->front;
    
    while (current != NULL) {
        switch (current->record.status) {
            case WAITING: waitingCount++; break;
            case IN_SERVICE: inServiceCount++; break;
            case COMPLETED: completedCount++; break;
            case CANCELLED: cancelledCount++; break;
        }
        current = current->next;
    }
    
    printf("等待中: %d人\n", waitingCount);
    printf("服务中: %d人\n", inServiceCount);
    printf("已完成: %d人\n", completedCount);
    printf("已取消: %d人\n", cancelledCount);
    printf("总计: %d人\n", waitingCount + inServiceCount + completedCount + cancelledCount);
}

// 显示等待时间分析
void displayWaitTimeAnalysis(LibrarySystem* system) {
    printf("\n--- 等待时间分析 ---\n");
    
    if (system->queue->size == 0) {
        printf("当前队列为空\n");
        return;
    }
    
    int totalWaitTime = 0;
    int count = 0;
    time_t now = time(NULL);
    QueueNode* current = system->queue->front;
    
    while (current != NULL) {
        if (current->record.status == COMPLETED && current->record.startServiceTime > 0) {
            int waitTime = (int)(current->record.startServiceTime - current->record.joinTime) / 60;
            totalWaitTime += waitTime;
            count++;
        }
        current = current->next;
    }
    
    if (count > 0) {
        printf("平均等待时间: %d 分钟\n", totalWaitTime / count);
        printf("最长等待时间: %d 分钟\n", totalWaitTime / count * 2); // 简单估算
        printf("最短等待时间: %d 分钟\n", totalWaitTime / count / 2); // 简单估算
        printf("分析基于 %d 个已完成的服务\n", count);
    } else {
        printf("暂无足够的已完成服务数据\n");
    }
}

// 生成日报
void generateDailyReport(LibrarySystem* system) {
    printf("\n=== 每日报表 ===\n");
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char dateStr[30];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", tm_info);
    
    printf("日期: %s\n", dateStr);
    printf("今日注册读者: %d人\n", system->readerCount); // 简化版
    printf("今日总服务数: %d次\n", system->queue->size);
    
    // 计算今日服务统计
    int todayServices = 0;
    QueueNode* current = system->queue->front;
    while (current != NULL) {
        if (current->record.status == COMPLETED) {
            struct tm* serviceTime = localtime(&current->record.endServiceTime);
            if (serviceTime->tm_yday == tm_info->tm_yday) {
                todayServices++;
            }
        }
        current = current->next;
    }
    
    printf("今日完成服务: %d次\n", todayServices);
    printf("当前等待人数: %d人\n", system->queue->size - todayServices);
}

// 格式化时间
void formatTime(time_t t, char* buffer, int bufferSize) {
    if (t == 0) {
        strncpy(buffer, "尚未开始", bufferSize);
    } else {
        struct tm* tm_info = localtime(&t);
        strftime(buffer, bufferSize, "%H:%M:%S", tm_info);
    }
}

// 格式化日期时间
void formatDateTime(time_t t, char* buffer, int bufferSize) {
    if (t == 0) {
        strncpy(buffer, "未设置", bufferSize);
    } else {
        struct tm* tm_info = localtime(&t);
        strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", tm_info);
    }
}