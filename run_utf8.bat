@echo off
chcp 65001 >nul
title 图书馆排队管理系统
mode con: cols=80 lines=30

echo 正在启动图书馆排队系统...
echo.

cd build\bin
library_queue_system.exe

cd ..\..
pause