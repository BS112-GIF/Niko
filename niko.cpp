#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

namespace fs = std::filesystem;

// 全局配置
std::set<std::string> skip_folders;  // 要跳过的文件夹列表
bool print_tree = false;             // 是否打印文件树（-o触发）

/**
 * 兼容C++17的字符串前缀判断（替代starts_with）
 * @param str 原字符串
 * @param prefix 前缀
 * @return 是否以prefix开头
 */
bool string_starts_with(const std::string& str, const std::string& prefix) {
    if (prefix.size() > str.size()) return false;
    return str.substr(0, prefix.size()) == prefix;
}

/**
 * 打印帮助信息（直接运行时显示）
 */
void print_help() {
    std::cout << "niko 文件树生成工具  版本1.0.1" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "用法: niko [选项] [目标路径]" << std::endl;
    std::cout << std::endl;
    std::cout << "选项:" << std::endl;
    std::cout << "  -h/--help       显示此帮助信息（默认直接运行也显示）" << std::endl;
    std::cout << "  -o              打印目标路径的文件树" << std::endl;
    std::cout << "  -l <文件夹名>   跳过指定文件夹（可多个，自动触发-o）" << std::endl;
    std::cout << std::endl;
    std::cout << "示例:" << std::endl;
    std::cout << "  ./niko -o                # 打印当前目录文件树" << std::endl;
    std::cout << "  ./niko -o /home/user     # 打印指定目录文件树" << std::endl;
    std::cout << "  ./niko -l .git           # 跳过.git并打印当前目录树（-l自动带-o）" << std::endl;
    std::cout << "  ./niko -l .git -l build  # 跳过多个文件夹并打印树" << std::endl;
    std::cout << "  ./niko -l .git -o /tmp   # 显式-o + 指定路径 + 跳过文件夹" << std::endl;
}

/**
 * 递归打印文件树
 * @param path 目标路径
 * @param prefix 层级前缀
 */
void print_file_tree(const fs::path& path, const std::string& prefix) {
    std::vector<fs::path> entries;
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            entries.push_back(entry.path());
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "错误：访问目录 '" << path << "' 失败 - " << e.what() << std::endl;
        return;
    }

    std::sort(entries.begin(), entries.end());

    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        bool is_last = (i == entries.size() - 1);
        std::string entry_name = entry.filename().string();

        // 跳过指定文件夹
        if (fs::is_directory(entry) && skip_folders.count(entry_name)) {
            std::cout << prefix << (is_last ? "└── " : "├── ") 
                      << entry_name << "/ [已跳过]" << std::endl;
            continue;
        }

        if (fs::is_directory(entry)) {
            std::cout << prefix << (is_last ? "└── " : "├── ") 
                      << entry_name << "/" << std::endl;
            std::string new_prefix = prefix + (is_last ? "    " : "│   ");
            print_file_tree(entry, new_prefix);
        } else if (fs::is_regular_file(entry)) {
            std::cout << prefix << (is_last ? "└── " : "├── ") 
                      << entry_name << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    fs::path target_path = "."; // 默认当前目录
    bool show_help = true;      // 默认显示帮助

    // 解析命令行参数
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            show_help = true;
        } else if (arg == "-o") {
            print_tree = true;
            show_help = false;
        } else if (arg == "-l" && i + 1 < argc) {
            skip_folders.insert(argv[++i]);
            print_tree = true;  // -l自动触发-o
            show_help = false;
        } else if (!string_starts_with(arg, "-")) { // 替换starts_with
            target_path = arg;  // 自定义目标路径
            show_help = false;
        } else {
            std::cerr << "错误：无效参数 '" << arg << "'" << std::endl;
            print_help();
            return 1;
        }
    }

    // 1. 显示帮助（直接运行/加-h时）
    if (show_help) {
        print_help();
        return 0;
    }

    // 2. 打印文件树（-o/-l触发）
    if (print_tree) {
        // 校验路径
        if (!fs::exists(target_path)) {
            std::cerr << "错误：路径 '" << target_path << "' 不存在！" << std::endl;
            return 1;
        }
        // 打印树标题 + 内容
        std::cout << "目录: " << fs::absolute(target_path) << std::endl;
        print_file_tree(target_path, "");
    }

    return 0;
}