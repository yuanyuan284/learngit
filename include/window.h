#ifndef WINDOW_H
#define WINDOW_H

#include "service.h"

// 前向声明
typedef struct LibrarySystem LibrarySystem;

// 窗口界面函数
void displayMainMenu();
void displayReaderMenu();
void displayWindowMenu();

// 窗口操作函数（这些函数在window.c中实现，但原型需要LibrarySystem）
void callNextReader(LibrarySystem* system, ServiceWindow windows[], int windowCount);
void completeCurrentService(LibrarySystem* system, ServiceWindow windows[], int windowCount);
void cancelCurrentService(LibrarySystem* system, ServiceWindow windows[], int windowCount);

// 工具函数
void clearScreen();
void waitForEnter();
int getValidatedInt(const char* prompt, int min, int max);
void getStringInput(char* buffer, int size, const char* prompt);

#endif // WINDOW_H