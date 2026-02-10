#include "../include/persistence.h"
#include "../include/window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void registerNewReader(LibrarySystem* system) {
    if (system->readerCount >= MAX_READERS) {
        printf("读者数量已达上限，无法注册新读者！\n");
        return;
    }
    
    Reader newReader;
    
    printf("\n--- 注册新读者 ---\n");
    
    // 读者ID
    printf("请输入读者ID/学号: ");
    fgets(newReader.id, MAX_ID_LEN, stdin);
    newReader.id[strcspn(newReader.id, "\n")] = '\0';
    
    // 检查是否已存在
    for (int i = 0; i < system->readerCount; i++) {
        if (strcmp(system->readers[i].id, newReader.id) == 0) {
            printf("读者ID已存在！\n");
            return;
        }
    }
    
    // 姓名
    printf("请输入姓名: ");
    fgets(newReader.name, MAX_NAME_LEN, stdin);
    newReader.name[strcspn(newReader.name, "\n")] = '\0';
    
    // 读者类型
    printf("请选择读者类型:\n");
    printf("1. 学生\n");
    printf("2. 教师\n");
    printf("3. 职工\n");
    printf("4. 访客\n");
    printf("选择: ");
    
    int typeChoice;
    scanf("%d", &typeChoice);
    getchar();
    
    switch (typeChoice) {
        case 1: newReader.type = STUDENT; break;
        case 2: newReader.type = TEACHER; break;
        case 3: newReader.type = STAFF; break;
        case 4: newReader.type = VISITOR; break;
        default: newReader.type = STUDENT;
    }
    
    // 联系电话
    printf("请输入联系电话: ");
    fgets(newReader.phone, MAX_PHONE_LEN, stdin);
    newReader.phone[strcspn(newReader.phone, "\n")] = '\0';
    
    // 设置借阅限制
    switch (newReader.type) {
        case TEACHER:
            newReader.maxBorrowLimit = 20;
            break;
        case STUDENT:
            newReader.maxBorrowLimit = 10;
            break;
        case STAFF:
            newReader.maxBorrowLimit = 15;
            break;
        case VISITOR:
            newReader.maxBorrowLimit = 3;
            break;
    }
    
    newReader.borrowedCount = 0;
    newReader.registerTime = time(NULL);
    
    system->readers[system->readerCount] = newReader;
    system->readerCount++;
    
    printf("\n读者注册成功！\n");
    printf("读者ID: %s\n", newReader.id);
    printf("姓名: %s\n", newReader.name);
    printf("最大借阅量: %d本\n", newReader.maxBorrowLimit);
}

void queryReaderInfo(LibrarySystem* system) {
    char searchId[MAX_ID_LEN];
    
    printf("\n请输入要查询的读者ID: ");
    fgets(searchId, MAX_ID_LEN, stdin);
    searchId[strcspn(searchId, "\n")] = '\0';
    
    for (int i = 0; i < system->readerCount; i++) {
        if (strcmp(system->readers[i].id, searchId) == 0) {
            displayReaderInfo(&system->readers[i]);
            return;
        }
    }
    
    printf("未找到读者ID: %s\n", searchId);
}

void displayReaderInfo(const Reader* reader) {
    printf("\n=== 读者信息 ===\n");
    printf("读者ID: %s\n", reader->id);
    printf("姓名: %s\n", reader->name);
    
    printf("读者类型: %s\n", readerTypeToString(reader->type));
    printf("联系电话: %s\n", reader->phone);
    printf("已借阅数量: %d/%d\n", reader->borrowedCount, reader->maxBorrowLimit);
    
    char timeStr[30];
    struct tm* regTime = localtime(&reader->registerTime);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", regTime);
    printf("注册时间: %s\n", timeStr);
}

void modifyReaderInfo(LibrarySystem* system) {
    char searchId[MAX_ID_LEN];
    
    printf("\n请输入要修改的读者ID: ");
    fgets(searchId, MAX_ID_LEN, stdin);
    searchId[strcspn(searchId, "\n")] = '\0';
    
    for (int i = 0; i < system->readerCount; i++) {
        if (strcmp(system->readers[i].id, searchId) == 0) {
            printf("找到读者: %s\n", system->readers[i].name);
            printf("请输入新姓名 (直接回车保持原值): ");
            char newName[MAX_NAME_LEN];
            fgets(newName, MAX_NAME_LEN, stdin);
            if (strlen(newName) > 1) {
                newName[strcspn(newName, "\n")] = '\0';
                strcpy(system->readers[i].name, newName);
            }
            
            printf("请输入新电话 (直接回车保持原值): ");
            char newPhone[MAX_PHONE_LEN];
            fgets(newPhone, MAX_PHONE_LEN, stdin);
            if (strlen(newPhone) > 1) {
                newPhone[strcspn(newPhone, "\n")] = '\0';
                strcpy(system->readers[i].phone, newPhone);
            }
            
            printf("读者信息修改成功！\n");
            return;
        }
    }
    
    printf("未找到读者ID: %s\n", searchId);
}

void deleteReader(LibrarySystem* system) {
    char searchId[MAX_ID_LEN];
    
    printf("\n请输入要删除的读者ID: ");
    fgets(searchId, MAX_ID_LEN, stdin);
    searchId[strcspn(searchId, "\n")] = '\0';
    
    for (int i = 0; i < system->readerCount; i++) {
        if (strcmp(system->readers[i].id, searchId) == 0) {
            printf("确认删除读者 %s (Y/N): ", system->readers[i].name);
            char confirm;
            scanf("%c", &confirm);
            getchar();
            
            if (confirm == 'Y' || confirm == 'y') {
                // 将最后一个读者移动到删除位置
                if (i < system->readerCount - 1) {
                    system->readers[i] = system->readers[system->readerCount - 1];
                }
                system->readerCount--;
                printf("读者删除成功！\n");
            } else {
                printf("取消删除操作。\n");
            }
            return;
        }
    }
    
    printf("未找到读者ID: %s\n", searchId);
}

void displayAllReaders(LibrarySystem* system) {
    if (system->readerCount == 0) {
        printf("暂无读者信息！\n");
        return;
    }
    
    printf("\n=== 所有读者信息 ===\n");
    printf("%-10s %-10s %-8s %-15s %-10s\n", 
           "读者ID", "姓名", "类型", "电话", "借阅量");
    printf("----------------------------------------------------------\n");
    
    for (int i = 0; i < system->readerCount; i++) {
        printf("%-10s %-10s %-8s %-15s %d/%d\n",
               system->readers[i].id,
               system->readers[i].name,
               readerTypeToString(system->readers[i].type),
               system->readers[i].phone,
               system->readers[i].borrowedCount,
               system->readers[i].maxBorrowLimit);
    }
    printf("总计: %d 位读者\n", system->readerCount);
}

const char* readerTypeToString(ReaderType type) {
    switch(type) {
        case STUDENT: return "学生";
        case TEACHER: return "教师";
        case STAFF: return "职工";
        case VISITOR: return "访客";
        default: return "未知";
    }
}