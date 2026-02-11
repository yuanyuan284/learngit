#include "../include/library_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 全局变量定义
LibrarySystem* g_system = NULL;
ServiceWindow g_windows[MAX_WINDOWS];
int g_windowCount = MAX_WINDOWS;

int main() {
    // 设置控制台为中文编码
    // Linux系统默认支持UTF-8编码，无需额外设置
    // 以下是跨平台兼容的设置
    #ifdef _WIN32
        system("chcp 936 > nul");  // GBK编码
        SetConsoleOutputCP(936);    // Windows API设置输出
        SetConsoleCP(936);          // Windows API设置输入
    #endif
    
    // 原来的代码继续...
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
        if (scanf("%d", &choice) != 1) {
            printf("输入错误，请重新输入！\n");
            while (getchar() != '\n'); // 清空输入缓冲区
            continue;
        }
        getchar();  // 清除输入缓冲
        
        switch (choice) {
            case 1:  // 读者管理
                do {
                    displayReaderMenu();
                    if (scanf("%d", &subChoice) != 1) {
                        printf("输入错误，请重新输入！\n");
                        while (getchar() != '\n'); // 清空输入缓冲区
                        continue;
                    }
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
                    if (scanf("%d", &subChoice) != 1) {
                        printf("输入错误，请重新输入！\n");
                        while (getchar() != '\n'); // 清空输入缓冲区
                        continue;
                    }
                    getchar();
                    
                    switch (subChoice) {
                        case 1:  // 叫号服务
                            {
                                int windowId;
                                printf("请输入窗口ID (1-%d): ", g_windowCount);
                                if (scanf("%d", &windowId) != 1) {
                                    printf("输入错误，请重新输入！\n");
                                    while (getchar() != '\n'); // 清空输入缓冲区
                                    return;
                                }
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
                                if (scanf("%d", &windowId) != 1) {
                                    printf("输入错误，请重新输入！\n");
                                    while (getchar() != '\n'); // 清空输入缓冲区
                                    return;
                                }
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
                                if (scanf("%d", &windowId) != 1) {
                                    printf("输入错误，请重新输入！\n");
                                    while (getchar() != '\n'); // 清空输入缓冲区
                                    return;
                                }
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