#include "../include/guide.h"
#include <windows.h>

// 清屏函数
static void clearScreen(void) {
    system("cls");
}

// 设置颜色
static void setColor(int color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#endif
}

// 显示指南页面
void displayGuidePage(int page) {
    clearScreen();
    
    setColor(11); // 青色
    printf("═══════════════════════════════════════════\n");
    printf("           游戏指南 (第%d页/共5页)\n", page + 1);
    printf("═══════════════════════════════════════════\n");
    setColor(7); // 白色
    
    switch (page) {
        case 0: // 基本操作
            printf("\n基本操作：\n");
            printf("═══════════════════════════════════════════\n");
            printf("• WASD键 - 移动角色\n");
            printf("• 回车键 - 确认选择\n");
            printf("• E键   - 与物体交互\n");
            printf("• Q键   - 返回上级菜单\n");
            printf("• I键   - 查看玩家信息\n");
            printf("• B键   - 打开背包\n");
            printf("\n 界面说明：\n");
            printf("• > 符号表示当前选中项\n");
            printf("• 黄色文字表示高亮选项\n");
            printf("• 按任意提示键继续\n");
            break;
            
        case 1: // 战斗系统
            printf("\n战斗系统：\n");
            printf("═══════════════════════════════════════════\n");
            printf("• 自动战斗 - 靠近怪物自动触发\n");
            printf("• 属性影响：\n");
            printf("  - 力量：增加物理伤害\n");
            printf("  - 防御：减少受到的伤害\n");
            printf("  - 敏捷：影响闪避概率\n");
            printf("  - 幸运：影响暴击率和物品掉落\n");
            printf("\n怪物类型：\n");
            printf("• 普通怪物 - 容易击败，奖励一般\n");
            printf("• 精英怪物 - 较强，奖励丰厚\n");
            printf("• BOSS     - 非常强大，击败后有特殊奖励\n");
            break;
            
        case 2: // 物品系统
            printf("\n物品系统：\n");
            printf("═══════════════════════════════════════════\n");
            printf("背包管理：\n");
            printf("• 容量限制 - 初始50格，可扩展\n");
            printf("• 物品分类 - 遗物和工具分开管理\n");
            printf("• 使用物品 - 选择物品按E键使用\n");
            printf("\n 遗物系统：\n");
            printf("• 品质分级 - C,B,A,S,SS,SSS\n");
            printf("• 损坏程度 - 影响遗物价值和效果\n");
            printf("• 修复系统 - 使用维修套件修复遗物\n");
            printf("• 修复风险 - 可能失败导致更严重的损坏\n");
            break;
            
        case 3: // 经济系统
            printf("\n经济系统：\n");
            printf("═══════════════════════════════════════════\n");
            printf(" 商店功能：\n");
            printf("• 购买物品 - 使用金币购买各种道具\n");
            printf("• 库存系统 - 部分商品有限量\n");
            printf("• 特殊商品 - 背包扩展券、金币福袋等\n");
            printf("\n获取金币：\n");
            printf("• 击败怪物 - 获得少量金币\n");
            printf("• 发现宝藏 - 获得大量金币\n");
            printf("• 完成任务 - 获得金币奖励\n");
            printf("• 出售物品 - 半价出售不需要的物品\n");
            break;
            
        case 4: // 地图探索
            printf("\n地图探索：\n");
            printf("═══════════════════════════════════════════\n");
            printf(" 地图类型：\n");
            printf("• 魔法森林 - 适合新手，难度较低\n");
            printf("• 火山洞穴 - 中级难度，需要等级5\n");
            printf("• 冰封遗迹 - 高级难度，需要等级8\n");
            printf("\n 地图元素：\n");
            printf("• # 墙     - 无法通过\n");
            printf("• T 树     - 障碍物\n");
            printf("• = 河     - 需要特殊道具通过\n");
            printf("• * 宝藏   - 包含各种奖励\n");
            printf("• ? 事件   - 触发特殊事件\n");
            printf("• N NPC    - 提供任务和信息\n");
            printf("• M 怪物   - 触发战斗\n");
            printf("• $ 商店   - 购买物品\n");
            printf("• @ 玩家   - 你的当前位置\n");
            break;
    }
    
    setColor(11);
    printf("\n═══════════════════════════════════════════\n");
    setColor(14); // 黄色
    printf("操作: A/D键翻页 ←/→ | Q键返回主菜单\n");
    setColor(7);
}

// 处理指南输入
int handleGuideInput(int* page) {
    int ch = getch();
    
    switch (ch) {
        case 'a':
        case 'A':
            *page = (*page - 1 + 5) % 5;
            return 1; // 继续显示
        case 'd':
        case 'D':
            *page = (*page + 1) % 5;
            return 1; // 继续显示
        case 'q':
        case 'Q':
            return 0; // 返回主菜单
        default:
            return 1; // 继续显示
    }
}

// 显示游戏指南
void showGameGuide(void) {
    int page = 0;
    int running = 1;
    
    while (running) {
        displayGuidePage(page);
        running = handleGuideInput(&page);
    }
}