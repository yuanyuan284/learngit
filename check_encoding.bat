@echo off
chcp 65001 >nul
echo 检查文件编码...
echo.

REM 检查文件编码
powershell -Command "Get-Content 'src/window.c' -Encoding UTF8 | Select -First 10"

echo.
echo 如果看到乱码，请使用Notepad++等编辑器：
echo 1. 用Notepad++打开文件
echo 2. 编码 → 转为UTF-8无BOM格式
echo 3. 保存
pause