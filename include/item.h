#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/commontypes.h"
#define ITEM_TYPES 10//物品种类数

// 物品类型枚举
typedef enum {
    ITEM_TOOL,
    ITEM_RELIC
} ItemType;

// Relic品质枚举
typedef enum {
    QUALITY_C = 0,
    QUALITY_B,
    QUALITY_A,
    QUALITY_S,
    QUALITY_SS,
    QUALITY_SSS
} RelicQuality;

// 工具类型枚举
typedef enum {
    TOOL_HEALTH_POTION,
    TOOL_REPAIR_KIT_LOW,
    TOOL_REPAIR_KIT_MEDIUM,
    TOOL_REPAIR_KIT_HIGH,
    TOOL_OTHER
} ToolType;

// 修复结果枚举
typedef enum {
    REPAIR_SUCCESS,
    REPAIR_FAILURE,
    REPAIR_CRITICAL_FAILURE
} RepairResult;

// 物品基类结构体
typedef struct Item {
    int id;
    char name[50];
    ItemType type;
    int value;
    int quantity;
    int repairCount;  // 修复次数（用于影响修复难度）
    struct Item* next;

    // 虚函数表指针
    void (*displayInfo)(struct Item*);
    void (*use)(struct Item*, void* context, Player *player);
    void (*destroy)(struct Item*);
} Item;

// Relic类（继承自Item）
typedef struct {
    Item base;
    RelicQuality quality;
    float damageLevel;      // 损坏程度 0.0-1.0
    float repairDifficulty; // 修复难度 0.0-1.0
    int totalRepairTimes;   // 总修复次数
    int successfulRepairs;  // 成功修复次数
    char description[200];
} Relic;

// Tool类（继承自Item）
typedef struct {
    Item base;
    ToolType toolType;
    float effectValue;      // 效果值（恢复量、修复量等）
    char function[200];
} Tool;
/*如果后期需要扩展relic，tool请按以下方法：拓展item：调用createRelic函数并填补相关参数
拓展tool，首先修改工具类型枚举ToolType，以TOOL_NAME格式添加，然后跳转至useTool函数，根据工具类型枚举添加特定功能*/

// 创建函数
Item* createRelic(int id, const char* name, int value, int quantity,
    RelicQuality quality, float damageLevel,
    float repairDifficulty, const char* description);

Item* createTool(int id, const char* name, int value, int quantity,
    ToolType toolType, float effectValue, const char* function);

// 工具函数
const char* getQualityString(RelicQuality quality);
const char* getToolTypeString(ToolType type);
RelicQuality charToQuality(char c);
char qualityToChar(RelicQuality quality);
float getRepairKitEfficiency(ToolType toolType);
float getRepairKitAmount(ToolType toolType);
float getCriticalFailureChance(ToolType toolType);

// 修复相关函数
bool canRepairRelic(Item* tool);
float calculateRepairChance(Relic* relic, Tool* tool);
RepairResult performRepair(Relic* relic, Tool* tool);
void applyRepairSuccess(Relic* relic, Tool* tool);
void applyRepairFailure(Relic* relic, Tool* tool);
void applyCriticalFailure(Relic* relic, Tool* tool);

// 价值计算
int calculateRelicValue(Relic* relic);

//随机获得物品
Item*getRandomItem(ItemType type,int id);

#endif // ITEM_H