## Niko

让豆包写的一个小工具，以下内容为豆包编写

## 工具介绍

一个轻量级跨平台文件树生成工具，基于 C++17 开发，支持跳过指定文件夹，纯命令行无 GUI，开箱即用。

## 📌 功能特点

+   📂 生成层级清晰的 ASCII 风格文件树，排版美观
+   🚫 支持跳过指定文件夹（如 .git、node\_modules、build 等）
+   💻 跨平台兼容：Windows/Linux/macOS 均可运行
+   📋 无参数直接运行显示帮助文档，使用门槛低
+   ⚡ 轻量高效，无多余依赖，编译后可直接执行

## 🛠 编译说明

### 前置条件

+   支持 C++17 的编译器（Windows 推荐 MinGW-w64，Linux 用 GCC 7+，macOS 用 Clang 5+）
+   编译器需支持 `<filesystem>` 标准库

### 编译命令

#### Linux / macOS

```bash
# 克隆仓库
git clone https://github.com/BS112-GIF/Niko.git
cd Niko

# 编译（src 目录下存放 filetree.cpp）
g++ -std=c++17 src/filetree.cpp -o niko

# macOS 若提示 filesystem 库缺失，执行：
clang++ -std=c++17 src/filetree.cpp -o niko -lc++fs
```

#### Windows（MinGW）

```powershell
# 克隆仓库
git clone https://github.com/BS112-GIF/Niko.git
cd Niko

# 编译
g++ -std=c++17 src/filetree.cpp -o niko.exe -lstdc++fs
```

## 🚀 使用方法

### 基本语法

```bash
# Linux/macOS
./niko [选项] [目标路径]

# Windows
.\niko.exe [选项] [目标路径]
```

### 核心选项

| 选项 | 说明 |
| --- | --- |
| `-h`/`--help` | 显示帮助信息（无参数运行时默认显示） |
| `-o` | 打印指定路径的文件树 |
| `-l <文件夹名>` | 跳过指定文件夹（可多个，自动触发 `-o`） |

### 示例

#### 1\. 显示帮助信息

```bash
# Linux/macOS
./niko

# Windows
.\niko.exe
```

#### 2\. 打印当前目录文件树

```bash
# Linux/macOS
./niko -o

# Windows
.\niko.exe -o
```

#### 3\. 打印指定目录文件树

```bash
# Linux/macOS
./niko -o /home/用户/项目

# Windows
.\niko.exe -o C:\Users\你的用户名\文档
```

#### 4\. 跳过单个文件夹（如 .git）

```bash
# Linux/macOS
./niko -l .git

# Windows
.\niko.exe -l .git
```

#### 5\. 跳过多个文件夹（如 .git + build）

```bash
# Linux/macOS
./niko -l .git -l build

# Windows
.\niko.exe -l .git -l build
```

## 📸 输出示例

### 正常文件树（无跳过）

```plaintext
📂 File Tree: /home/用户/test
├── docs/
│   ├── note.txt
│   └── report.pdf
├── src/
│   ├── main.cpp
│   └── utils.h
└── readme.md
```

### 跳过指定文件夹（如 .git）

```plaintext
📂 File Tree: /home/用户/test
├── .git/ [已跳过]
├── src/
│   ├── main.cpp
│   └── utils.h
└── readme.md
```

## 📄 许可证

本项目采用 MIT 许可证开源 - 详见 [LICENSE](LICENSE) 文件。

## 💡 注意事项

1.  Linux/macOS 下以 `.` 开头的隐藏文件默认包含在文件树中；
2.  ASCII 符号（├──、└──）若显示乱码，建议更换终端字体（Windows 用 Consolas，macOS/Linux 用 Menlo）；
3.  Windows PowerShell 用户若遇中文/符号乱码，可先执行：
    
    ```powershell
    [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
    chcp 65001 > $null
    ```
