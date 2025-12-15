#ifndef ENCYCLOPEDIA_H
#define ENCYCLOPEDIA_H

#include <stdio.h>
#include <conio.h>

// 图鉴类型枚举
typedef enum {
    ENCY_ITEMS,      // 物品图鉴
    ENCY_MONSTERS,   // 怪物图鉴
    ENCY_RELICS,     // 遗物图鉴
    ENCY_ACHIEVEMENTS // 成就图鉴
} EncyclopediaType;

// 图鉴函数
void showEncyclopedia(void);
void displayEncyclopediaMenu(int selected);
void displayItemsEncyclopedia(int page);
void displayMonstersEncyclopedia(int page);
void displayRelicsEncyclopedia(int page);
void displayAchievementsEncyclopedia(int page);
int handleEncyclopediaInput(int* selected, int* page, EncyclopediaType* type);

#endif // ENCYCLOPEDIA_H