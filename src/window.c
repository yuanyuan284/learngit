#include "../include/window.h"
#include "../include/persistence.h"  // 需要包含这个以获取LibrarySystem的完整定义
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// 跨平台兼容处理
#ifdef _WIN32
#include <windows.h>
#endif

void displayMainMenu() {
    #ifdef _WIN32
        (void)system("cls");
    #else
        (void)system("clear");
    #endif
    printf("\n========== 图书馆排队管理系统 ==========\n");
    printf("1. 读者管理\n");
    printf("2. 排队取号\n");
    printf("3. 服务窗口\n");
    printf("4. 队列显示\n");
    printf("5. 数据统计\n");
    printf("0. 退出系统\n");
    printf("=====================================\n");
    printf("请选择操作: ");
}

void displayReaderMenu() {
    printf("\n--- 读者管理 ---\n");
    printf("1. 注册新读者\n");
    printf("2. 查询读者信息\n");
    printf("3. 修改读者信息\n");
    printf("4. 删除读者\n");
    printf("5. 显示所有读者\n");
    printf("0. 返回主菜单\n");
}

void displayWindowMenu() {
    printf("\n--- 服务窗口管理 ---\n");
    printf("1. 叫号服务\n");
    printf("2. 完成当前服务\n");
    printf("3. 取消当前服务\n");
    printf("4. 查看窗口状态\n");
    printf("0. 返回主菜单\n");
}

void clearScreen() {
    #ifdef _WIN32
        (void)system("cls");
    #else
        (void)system("clear");
    #endif
}

void waitForEnter() {
    printf("\n按Enter键继续...");
    while (getchar() != '\n');
}

void callNextReader(LibrarySystem* system, ServiceWindow windows[], int windowId) {
    if (windowId < 1 || windowId > MAX_WINDOWS) {
        printf("无效窗口ID！\n");
        return;
    }
    
    ServiceWindow* window = &windows[windowId - 1];
    
    if (window->isActive) {
        printf("窗口 %d 正在服务中！\n", windowId);
        return;
    }
    
    QueueRecord* nextRecord = dequeue(system->queue);
    if (nextRecord == NULL) {
        printf("当前没有等待的读者！\n");
        return;
    }
    
    Reader* reader = NULL;
    for (int i = 0; i < system->readerCount; i++) {
        if (strcmp(system->readers[i].id, nextRecord->readerId) == 0) {
            reader = &system->readers[i];
            break;
        }
    }
    
    if (reader == NULL) {
        printf("找不到读者信息！\n");
        free(nextRecord);
        return;
    }
    
    window->isActive = 1;
    strcpy(window->currentReaderId, nextRecord->readerId);
    window->serviceType = nextRecord->serviceType;
    window->startTime = time(NULL);
    nextRecord->windowId = windowId;
    
    printf("\n=== 叫号通知 ===\n");
    printf("请 %s 号读者到 %d 号窗口办理！\n", 
           nextRecord->readerId, windowId);
    printf("读者姓名: %s\n", reader->name);
    
    printf("服务类型: ");
    switch (nextRecord->serviceType) {
        case BORROW_BOOK: printf("借书\n"); break;
        case RETURN_BOOK: printf("还书\n"); break;
        case RENEW_BOOK: printf("续借\n"); break;
        case QUERY_INFO: printf("信息查询\n"); break;
        case PAY_FINE: printf("缴纳罚款\n"); break;
        case REGISTER_CARD: printf("办理读者证\n"); break;
    }
    
    printf("排队号码: %d\n", nextRecord->queueNumber);
    
    // 跨平台提示音
    #ifdef _WIN32
        Beep(1000, 300);
        Beep(1200, 300);
    #else
        // Linux系统可以使用printf输出提示音字符
        printf("\a"); // 响铃
        printf("\a"); // 响铃
    #endif
    
    free(nextRecord);
}

void completeCurrentService(LibrarySystem* system, ServiceWindow windows[], int windowId) {
    if (windowId < 1 || windowId > MAX_WINDOWS) {
        printf("无效窗口ID！\n");
        return;
    }
    
    ServiceWindow* window = &windows[windowId - 1];
    
    if (!window->isActive) {
        printf("窗口 %d 当前没有服务！\n", windowId);
        return;
    }
    
    // 更新服务记录状态
    QueueNode* current = system->queue->front;
    while (current != NULL) {
        if (strcmp(current->record.readerId, window->currentReaderId) == 0 &&
            current->record.status == IN_SERVICE) {
            current->record.status = COMPLETED;
            current->record.endServiceTime = time(NULL);
            break;
        }
        current = current->next;
    }
    
    printf("窗口 %d 服务完成！\n", windowId);
    printf("服务读者: %s\n", window->currentReaderId);
    printf("服务时长: %ld 秒\n", time(NULL) - window->startTime);
    
    // 重置窗口状态
    window->isActive = 0;
    window->currentReaderId[0] = '\0';
    window->startTime = 0;
    window->totalServed++;
}

void cancelCurrentService(LibrarySystem* system, ServiceWindow windows[], int windowId) {
    if (windowId < 1 || windowId > MAX_WINDOWS) {
        printf("无效窗口ID！\n");
        return;
    }
    
    ServiceWindow* window = &windows[windowId - 1];
    
    if (!window->isActive) {
        printf("窗口 %d 当前没有服务！\n", windowId);
        return;
    }
    
    // 更新服务记录状态
    QueueNode* current = system->queue->front;
    while (current != NULL) {
        if (strcmp(current->record.readerId, window->currentReaderId) == 0 &&
            current->record.status == IN_SERVICE) {
            current->record.status = CANCELLED;
            current->record.endServiceTime = time(NULL);
            break;
        }
        current = current->next;
    }
    
    printf("窗口 %d 服务已取消！\n", windowId);
    
    // 重置窗口状态
    window->isActive = 0;
    window->currentReaderId[0] = '\0';
    window->startTime = 0;
}

void displayWindowStatus(ServiceWindow windows[], int count) {
    printf("\n=== 窗口状态 ===\n");
    printf("%-6s %-10s %-15s %-12s %-10s %-10s\n", 
           "窗口号", "状态", "当前读者", "服务类型", "服务时长", "服务人数");
    printf("-------------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        char statusStr[10];
        char serviceTypeStr[10];
        char timeStr[20] = "";
        
        if (windows[i].isActive) {
            strcpy(statusStr, "服务中");
            
            // 服务类型
            switch (windows[i].serviceType) {
                case BORROW_BOOK: strcpy(serviceTypeStr, "借书"); break;
                case RETURN_BOOK: strcpy(serviceTypeStr, "还书"); break;
                case RENEW_BOOK: strcpy(serviceTypeStr, "续借"); break;
                case QUERY_INFO: strcpy(serviceTypeStr, "查询"); break;
                case PAY_FINE: strcpy(serviceTypeStr, "缴费"); break;
                case REGISTER_CARD: strcpy(serviceTypeStr, "办证"); break;
                default: strcpy(serviceTypeStr, "未知");
            }
            
            // 计算服务时长
            time_t now = time(NULL);
            int duration = (int)(now - windows[i].startTime);
            snprintf(timeStr, sizeof(timeStr), "%d秒", duration);
        } else {
            strcpy(statusStr, "空闲");
            strcpy(serviceTypeStr, "无");
            strcpy(timeStr, "0秒");
        }
        
        printf("%-6d %-10s %-15s %-12s %-10s %-10d\n",
               windows[i].id,
               statusStr,
               windows[i].isActive ? windows[i].currentReaderId : "无",
               serviceTypeStr,
               timeStr,
               windows[i].totalServed);
    }
}