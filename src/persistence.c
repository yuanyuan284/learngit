#include "../include/persistence.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

LibrarySystem* createLibrarySystem() {
    LibrarySystem* system = (LibrarySystem*)malloc(sizeof(LibrarySystem));
    if (system == NULL) {
        return NULL;
    }
    
    system->readerCount = 0;
    system->queue = createQueue();
    
    initializeSampleReaders(system);
    
    return system;
}

void destroyLibrarySystem(LibrarySystem* system) {
    if (system == NULL) {
        return;
    }
    
    destroyQueue(system->queue);
    free(system);
}

void initializeSampleReaders(LibrarySystem* system) {
    // 添加一些示例读者以便测试
    if (system->readerCount < MAX_READERS) {
        Reader reader1 = {
            .id = "S001",
            .name = "张三",
            .type = STUDENT,
            .phone = "13800138001",
            .borrowedCount = 0,
            .maxBorrowLimit = 10,
            .registerTime = time(NULL)
        };
        system->readers[system->readerCount++] = reader1;
    }
    
    if (system->readerCount < MAX_READERS) {
        Reader reader2 = {
            .id = "T001",
            .name = "李老师",
            .type = TEACHER,
            .phone = "13800138002",
            .borrowedCount = 0,
            .maxBorrowLimit = 20,
            .registerTime = time(NULL)
        };
        system->readers[system->readerCount++] = reader2;
    }
}

void saveToFile(LibrarySystem* system, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("无法打开文件");
        return;
    }
    
    // 保存读者数量
    fwrite(&system->readerCount, sizeof(int), 1, file);
    
    // 保存所有读者
    fwrite(system->readers, sizeof(Reader), system->readerCount, file);
    
    // 保存队列信息
    fwrite(&system->queue->currentNumber, sizeof(int), 1, file);
    fwrite(&system->queue->size, sizeof(int), 1, file);
    
    // 保存队列节点
    QueueNode* current = system->queue->front;
    while (current != NULL) {
        fwrite(&current->record, sizeof(QueueRecord), 1, file);
        current = current->next;
    }
    
    fclose(file);
    printf("数据已保存到 %s\n", filename);
}

void loadFromFile(LibrarySystem* system, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("未找到数据文件，创建新系统\n");
        return;
    }
    
    // 读取读者数量
    fread(&system->readerCount, sizeof(int), 1, file);
    
    // 读取读者数据
    fread(system->readers, sizeof(Reader), system->readerCount, file);
    
    // 读取队列信息
    fread(&system->queue->currentNumber, sizeof(int), 1, file);
    int queueSize;
    fread(&queueSize, sizeof(int), 1, file);
    
    // 重建队列
    for (int i = 0; i < queueSize; i++) {
        QueueRecord record;
        fread(&record, sizeof(QueueRecord), 1, file);
        
        QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
        if (newNode) {
            newNode->record = record;
            newNode->next = NULL;
            newNode->prev = NULL;
            
            if (system->queue->rear == NULL) {
                system->queue->front = newNode;
                system->queue->rear = newNode;
            } else {
                system->queue->rear->next = newNode;
                newNode->prev = system->queue->rear;
                system->queue->rear = newNode;
            }
        }
    }
    
    fclose(file);
    printf("数据已从 %s 加载\n", filename);
}