#ifndef BACKPACK_H
#define BACKPACK_H

#include "../include/item.h"
#include "../include/commontypes.h"
#include <stdio.h>

// 背包状态
typedef struct {
    Item* head;           // 物品链表头
    int money;            // 金钱
    int capacity;         // 容量
    int count;            // 物品数量
    int selectedIndex;    // 选中索引
    int currentCategory;  // 当前类别 0:全部 1:遗物 2:工具
    bool isSelectingTool; // 是否正在选择修复工具
    Item* selectedRelic;  // 当前选中的遗物（用于修复）
} Backpack;

// 初始化
void initBackpack(Backpack* backpack);
void loadSampleItems(Backpack* backpack);
void cleanupBackpack(Backpack* backpack);

// 文件操作
bool saveBackpackData(Backpack* backpack, const char*backpackFile, const char*moneyFile);
bool loadBackpackData(Backpack* backpack, const char*backpackFile, const char*moneyFile);
Item* createItemFromData(int id, const char* name, int type, int value, int quantity,
    int repairCount, float damageLevel, float repairDifficulty,
    int quality, int toolType, float effectValue,
    const char* description, const char* function);
void saveGame(Backpack* backpack);

// 物品管理
bool addItem(Backpack* backpack, Item* item);
bool removeItem(Backpack* backpack, int id, int quantity);
Item* findItemById(Backpack* backpack, int id);
Item* getSelectedItem(Backpack* backpack);
Item* getItemByIndex(Backpack* backpack, int index);

// 显示功能
void displayMoney(Backpack* backpack);
void displayItems(Backpack* backpack);
void displayItemDetails(Backpack* backpack);
void displayControls(Backpack* backpack);
void displayRepairInfo(Backpack* backpack);

// 交互功能
void useSelectedItem(Backpack* backpack,Player *player);
void repairSelectedRelic(Backpack* backpack);
void sellSelectedItem(Backpack* backpack);

// 背包操作
int handleInput(Backpack* backpack,Player *player);
void clearScreen(void);
int getCurrentCategoryCount(Backpack* backpack);
void backpack(Player *player,Backpack*backpack);

#endif // BACKPACK_H