#include "../include/library_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 全局变量定义
LibrarySystem* g_system = NULL;
ServiceWindow g_windows[MAX_WINDOWS];
int g_windowCount = MAX_WINDOWS;

int main() {
    // 初始化系统
    g_system = createLibrarySystem();
    if (!g_system) {
        fprintf(stderr, "系统初始化失败！\n");
        return 1;
    }
    
    // 加载数据
    loadFromFile(g_system, DATA_FILE);
    
    // 初始化窗口
    initializeWindows(g_windows, g_windowCount);
    
    int choice;
    int subChoice;
    
    do {
        displayMainMenu();
        scanf("%d", &choice);
        getchar();  // 清除输入缓冲
        
        switch (choice) {
            case 1:  // 读者管理
                do {
                    displayReaderMenu();
                    scanf("%d", &subChoice);
                    getchar();
                    
                    switch (subChoice) {
                        case 1:
                            registerNewReader(g_system);
                            break;
                        case 2:
                            queryReaderInfo(g_system);
                            break;
                        case 3:
                            modifyReaderInfo(g_system);
                            break;
                        case 4:
                            deleteReader(g_system);
                            break;
                        case 5:
                            displayAllReaders(g_system);
                            break;
                        case 0:
                            break;
                        default:
                            printf("无效选择！\n");
                    }
                } while (subChoice != 0);
                break;
                
            case 2:  // 排队取号
                takeQueueNumber(g_system);
                break;
                
            case 3:  // 服务窗口
                do {
                    displayWindowMenu();
                    scanf("%d", &subChoice);
                    getchar();
                    
                    switch (subChoice) {
                        case 1:  // 叫号服务
                            {
                                int windowId;
                                printf("请输入窗口ID (1-%d): ", g_windowCount);
                                scanf("%d", &windowId);
                                getchar();
                                if (windowId >= 1 && windowId <= g_windowCount) {
                                    // 注意：这里需要传递系统、窗口数组和窗口ID
                                    callNextReader(g_system, g_windows, windowId);
                                }
                            }
                            break;
                        case 2:  // 完成当前服务
                            {
                                int windowId;
                                printf("请输入窗口ID (1-%d): ", g_windowCount);
                                scanf("%d", &windowId);
                                getchar();
                                if (windowId >= 1 && windowId <= g_windowCount) {
                                    completeCurrentService(g_system, g_windows, windowId);
                                }
                            }
                            break;
                        case 3:  // 取消当前服务
                            {
                                int windowId;
                                printf("请输入窗口ID (1-%d): ", g_windowCount);
                                scanf("%d", &windowId);
                                getchar();
                                if (windowId >= 1 && windowId <= g_windowCount) {
                                    cancelCurrentService(g_system, g_windows, windowId);
                                }
                            }
                            break;
                        case 4:  // 查看窗口状态
                            displayWindowStatus(g_windows, g_windowCount);
                            break;
                        case 0:
                            break;
                        default:
                            printf("无效选择！\n");
                    }
                } while (subChoice != 0);
                break;
                
            case 4:  // 队列显示
                displayCurrentQueue(g_system);
                break;
                
            case 5:  // 数据统计
                displayStatistics(g_system);
                break;
                
            case 0:  // 退出
                printf("正在保存数据...\n");
                saveToFile(g_system, DATA_FILE);
                printf("数据保存完成，系统退出！\n");
                break;
                
            default:
                printf("无效选择，请重新输入！\n");
        }
        
        if (choice != 0) {
            printf("\n按Enter键继续...");
            getchar();
        }
        
    } while (choice != 0);
    
    destroyLibrarySystem(g_system);
    return 0;
}