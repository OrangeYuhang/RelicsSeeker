#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<Windows.h>
#include "../include/item.h"
#include "../include/backpack.h"
#include "../include/event.h"
#include "../include/player.h"
#include "../include/map.h"
#include "../include/shop.h"
#define _CRT_SECURE_NO_WARNINGS


void pictureGuidance() {}

// 假设您已经写好了这些功能
void showGameGuide(void) {};    // 游戏指南功能
void showEncyclopedia(void) {}; // 图鉴功能

// 全局游戏数据
Player* globalPlayer = NULL;
Backpack globalBackpack;
MapSystem globalMap;

// 菜单选项枚举
typedef enum {
    MENU_START_GAME = 0,
    MENU_PLAYER_INFO,
    MENU_BACKPACK,
    MENU_SHOP,
    MENU_ENCYCLOPEDIA,   // 图鉴
    MENU_GAME_GUIDE,     // 游戏指南
    MENU_QUICK_SAVE,
    MENU_SETTINGS,
    MENU_EXIT_GAME,
    MENU_COUNT
} MenuOption;

// 菜单选项文本
const char* menuTexts[MENU_COUNT] = {
    "🗺️  开始冒险",
    "👤 查看角色信息",
    "🎒 打开背包",
    "🏪 进入商店",
    "📚 图鉴系统",        // 图鉴
    "❓ 游戏指南",        // 游戏指南
    "💾 快速保存",
    "⚙️  游戏设置",
    "🚪 退出游戏"
};

// 菜单项描述
const char* menuDescriptions[MENU_COUNT] = {
    "选择地图开始新的冒险旅程",
    "查看角色属性和统计数据",
    "管理背包中的物品和装备",
    "购买物品和装备",
    "查看游戏中所有物品、怪物和成就",  // 图鉴描述
    "查看游戏操作说明和技巧",          // 游戏指南描述
    "快速保存当前游戏进度",
    "调整游戏音量和显示设置",
    "保存游戏并退出"
};

// 初始化游戏系统
int initGameSystem(void) {
    system("chcp 65001 > nul");

    // 1. 初始化背包
    initBackpack(&globalBackpack);

    // 2. 加载或创建玩家
    globalPlayer = loadOrCreatePlayer(&globalBackpack);
    if (!globalPlayer) {
        printf("❌ 玩家创建失败，游戏无法继续！\n");
        return 0;
    }

    // 3. 初始化地图系统
    initMapSystem(&globalMap, globalPlayer, &globalBackpack);

    return 1;
}

// 显示主菜单（W/S控制版本）
void displayMainMenu(int selectedIndex) {
    clearScreen();

    printf("═══════════════════════════════════════════\n");
    printf("             冒险游戏主菜单\n");
    printf("═══════════════════════════════════════════\n\n");

    // 显示所有菜单选项
    for (int i = 0; i < MENU_COUNT; i++) {
        if (i == selectedIndex) {
            // 高亮显示选中的选项
            printf("  ▶ ");
#ifdef _WIN32
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 14); // 黄色高亮
#endif
            printf("%s", menuTexts[i]);
#ifdef _WIN32
            SetConsoleTextAttribute(hConsole, 7); // 恢复白色
#endif
        }
        else {
            printf("    %s", menuTexts[i]);
        }
        printf("\n");
    }

    printf("\n═══════════════════════════════════════════\n");

    // 显示选中选项的描述
    printf("\n📝 %s\n", menuDescriptions[selectedIndex]);

    // 显示玩家状态
    if (globalPlayer) {
        printf("\n═══════════════════════════════════════════\n");
        printf("👤 %s | 等级: %d | HP: %d/%d | 💰: %d\n",
            globalPlayer->name,
            globalPlayer->level,
            globalPlayer->hp,
            globalPlayer->maxHp,
            globalBackpack.money);
    }

    printf("═══════════════════════════════════════════\n");
    printf("操作: W/S键选择 ↑/↓ | 回车键确认 | Q键返回上级\n");
}

// 显示角色信息子菜单
void showPlayerInfoSubMenu(void) {
    int running = 1;
    int selected = 0;
    const char* infoOptions[] = { "查看属性", "查看统计", "删除存档", "返回" };
    const int infoCount = 4;

    while (running) {
        clearScreen();

        printf("═══════════════════════════════════════════\n");
        printf("           角色信息\n");
        printf("═══════════════════════════════════════════\n\n");

        // 显示选项
        for (int i = 0; i < infoCount; i++) {
            if (i == selected) {
                printf("  ▶ ");
#ifdef _WIN32
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 14);
#endif
                printf("%s", infoOptions[i]);
#ifdef _WIN32
                SetConsoleTextAttribute(hConsole, 7);
#endif
            }
            else {
                printf("    %s", infoOptions[i]);
            }
            printf("\n");
        }

        printf("\n═══════════════════════════════════════════\n");
        printf("操作: W/S键选择 ↑/↓ | 回车键确认 | Q键返回\n");

        int ch = getch();
        switch (ch) {
        case 'w':
        case 'W':
            selected = (selected - 1 + infoCount) % infoCount;
            break;

        case 's':
        case 'S':
            selected = (selected + 1) % infoCount;
            break;

        case '\r': // 回车键
        case '\n':
            switch (selected) {
            case 0: // 查看属性
                clearScreen();
                displayPlayerInfo(globalPlayer);
                printf("\n按任意键返回...");
                _getch();
                break;

            case 1: // 查看统计
                clearScreen();
                displayPlayerStats(globalPlayer);
                printf("\n按任意键返回...");
                _getch();
                break;

            case 2: // 删除存档
                printf("\n⚠️  确定要删除存档并重新开始吗？(Y/N): ");
                char confirm = _getch();
                if (confirm == 'y' || confirm == 'Y') {
                    // 删除存档文件
                    remove(PLAYER_DATA_FILE);
                    printf("\n存档已删除，游戏将重新启动...\n");
                    printf("按任意键继续...");
                    _getch();

                    // 清理当前游戏
                    if (globalPlayer) {
                        destroyPlayer(globalPlayer);
                        globalPlayer = NULL;
                    }
                    cleanupBackpack(&globalBackpack);

                    // 重新初始化游戏
                    if (!initGameSystem()) {
                        printf("游戏初始化失败，退出...\n");
                        return;
                    }
                    running = 0; // 退出子菜单
                }
                break;

            case 3: // 返回
                running = 0;
                break;
            }
            break;

        case 'q':
        case 'Q':
            running = 0;
            break;
        }
    }
}

// 游戏设置子菜单
void showSettingsSubMenu(void) {
    int selected = 0;
    const char* settingOptions[] = { "调整音量", "控制设置", "画面设置", "返回" };
    const int settingCount = 4;

    int running = 1;
    while (running) {
        clearScreen();

        printf("═══════════════════════════════════════════\n");
        printf("           游戏设置\n");
        printf("═══════════════════════════════════════════\n\n");

        for (int i = 0; i < settingCount; i++) {
            if (i == selected) {
                printf("  ▶ ");
#ifdef _WIN32
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 14);
#endif
                printf("%s", settingOptions[i]);
#ifdef _WIN32
                SetConsoleTextAttribute(hConsole, 7);
#endif
            }
            else {
                printf("    %s", settingOptions[i]);
            }
            printf("\n");
        }

        printf("\n═══════════════════════════════════════════\n");
        printf("操作: W/S键选择 ↑/↓ | 回车键确认 | Q键返回\n");

        int ch = getch();
        switch (ch) {
        case 'w':
        case 'W':
            selected = (selected - 1 + settingCount) % settingCount;
            break;

        case 's':
        case 'S':
            selected = (selected + 1) % settingCount;
            break;

        case '\r': // 回车键
        case '\n':
            if (selected == 3) { // 返回
                running = 0;
            }
            else {
                printf("\n%s功能待实现...\n", settingOptions[selected]);
                printf("按任意键继续...");
                _getch();
            }
            break;

        case 'q':
        case 'Q':
            running = 0;
            break;
        }
    }
}

// 退出游戏确认
int confirmExitGame(void) {
    int selected = 0;
    const char* exitOptions[] = { "保存并退出", "直接退出", "取消" };
    const int exitCount = 3;

    while (1) {
        clearScreen();

        printf("═══════════════════════════════════════════\n");
        printf("           退出游戏\n");
        printf("═══════════════════════════════════════════\n\n");

        for (int i = 0; i < exitCount; i++) {
            if (i == selected) {
                printf("  ▶ ");
#ifdef _WIN32
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 14);
#endif
                printf("%s", exitOptions[i]);
#ifdef _WIN32
                SetConsoleTextAttribute(hConsole, 7);
#endif
            }
            else {
                printf("    %s", exitOptions[i]);
            }
            printf("\n");
        }

        printf("\n═══════════════════════════════════════════\n");
        printf("操作: W/S键选择 ↑/↓ | 回车键确认\n");

        int ch = getch();
        switch (ch) {
        case 'w':
        case 'W':
            selected = (selected - 1 + exitCount) % exitCount;
            break;

        case 's':
        case 'S':
            selected = (selected + 1) % exitCount;
            break;

        case '\r': // 回车键
        case '\n':
            if (selected == 0) { // 保存并退出
                if (globalPlayer) {
                    quickSavePlayer(globalPlayer);
                    saveGame(&globalBackpack);
                    printf("\n✅ 游戏已保存！\n");
                    printf("再见，%s！\n", globalPlayer->name);
                    printf("按任意键退出...");
                    _getch();
                }
                return 1; // 退出游戏
            }
            else if (selected == 1) { // 直接退出
                printf("\n游戏进度未保存！\n");
                printf("确定直接退出吗？(Y/N): ");
                char confirm = _getch();
                if (confirm == 'y' || confirm == 'Y') {
                    return 1; // 退出游戏
                }
            }
            else if (selected == 2) { // 取消
                return 0; // 返回主菜单
            }
            break;
        }
    }
}

// 处理主菜单选择
void handleMainMenu(void) {
    int selectedIndex = 0;
    int running = 1;

    while (running) {
        displayMainMenu(selectedIndex);

        int ch = getch();

        switch (ch) {
        case 'w':
        case 'W':
            selectedIndex = (selectedIndex - 1 + MENU_COUNT) % MENU_COUNT;
            break;

        case 's':
        case 'S':
            selectedIndex = (selectedIndex + 1) % MENU_COUNT;
            break;

        case '\r': // 回车键
        case '\n':
            switch (selectedIndex) {
            case MENU_START_GAME: // 开始冒险
                printf("\n🚀 开始冒险...\n");
                runMapSelection(&globalMap);
                break;

            case MENU_PLAYER_INFO: // 查看角色信息
                showPlayerInfoSubMenu();
                break;

            case MENU_BACKPACK: // 打开背包
                printf("\n打开背包...\n");
				backpack(&globalBackpack);
                pressAnyKey();
                break;

            case MENU_SHOP: // 进入商店
                printf("\n进入商店...\n");
                shop(&globalBackpack);
                break;

            case MENU_ENCYCLOPEDIA: // 图鉴系统
                printf("\n打开图鉴...\n");
                showEncyclopedia(); // 调用您的图鉴函数
                break;

            case MENU_GAME_GUIDE: // 游戏指南
                printf("\n打开游戏指南...\n");
                showGameGuide(); // 调用您的游戏指南函数
                break;

            case MENU_QUICK_SAVE: // 快速保存
                if (globalPlayer) {
                    quickSavePlayer(globalPlayer);
                    saveGame(&globalBackpack);
                    printf("\n✅ 游戏进度已保存！\n");
                }
                pressAnyKey();
                break;

            case MENU_SETTINGS: // 游戏设置
                showSettingsSubMenu();
                break;

            case MENU_EXIT_GAME: // 退出游戏
                if (confirmExitGame()) {
                    running = 0; // 退出主循环
                }
                break;
            }
            break;

        case 'q':
        case 'Q':
            // Q键也可以作为返回键
            if (selectedIndex == MENU_EXIT_GAME) {
                if (confirmExitGame()) {
                    running = 0;
                }
            }
            else {
                selectedIndex = MENU_EXIT_GAME; // 跳到退出选项
            }
            break;
        }
    }
}

// 游戏启动界面
void showGameTitle(void) {
    clearScreen();
    printf("═══════════════════════════════════════════\n");
    printf("          🏰 冒险游戏 v1.0 🏰\n");
    printf("═══════════════════════════════════════════\n");
    printf("          探索未知的世界\n");
    printf("          战胜强大的怪物\n");
    printf("          收集稀有的宝藏\n");
    printf("          成为传奇的英雄\n");
    printf("═══════════════════════════════════════════\n\n");

    printf("制作人员:\n");
    printf("  程序: 冒险者团队\n");
    printf("  美术: 幻想工作室\n");
    printf("  音乐: 史诗之声\n");
    printf("\n═══════════════════════════════════════════\n");

    printf("\n按任意键开始游戏...");
    _getch();
}

// 游戏主函数
int main(void) {
    // 显示游戏标题
    showGameTitle();

    // 初始化游戏系统
    if (!initGameSystem()) {
        printf("游戏初始化失败！\n");
        printf("按任意键退出...");
        _getch();
        return 1;
    }

    // 进入主菜单循环
    handleMainMenu();

    // 游戏结束，清理资源
    printf("\n═══════════════════════════════════════════\n");
    printf("          感谢游玩！\n");
    printf("═══════════════════════════════════════════\n");

    if (globalPlayer) {
        destroyPlayer(globalPlayer);
        globalPlayer = NULL;
    }

    cleanupBackpack(&globalBackpack);

    printf("\n按任意键退出...");
    _getch();

    return 0;
}