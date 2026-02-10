#ifndef LIBRARY_SYSTEM_H
#define LIBRARY_SYSTEM_H

// 首先包含定义基本类型的头文件
#include "reader.h"
#include "queue.h"
#include "service.h"
#include "window.h"
#include "statistics.h"

// 最后包含persistence.h，因为它定义了LibrarySystem
#include "persistence.h"

// 全局变量声明
extern LibrarySystem* g_system;
extern ServiceWindow g_windows[MAX_WINDOWS];
extern int g_windowCount;

#endif // LIBRARY_SYSTEM_H