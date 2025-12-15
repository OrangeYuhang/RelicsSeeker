#include "../include/item.h"
#include <time.h>
#include <math.h>
#include "../include/player.h"

//请在此处添加新的Relic：Item *Name = createRelic( id, "名称", 价值, 数量, QUALITY_X, 损坏程度, 修复难度, "描述");






//-------------------------------------------------------------------------------------------------------
//请在此处添加新的Tool：Item *Name = createTool( id, "名称", 价值, 数量, TOOL_TYPE, 效果值, "功能描述");





//-------------------------------------------------------------------------------------------------------
static void displayRelicInfo(Item* item) {
    Relic* relic = (Relic*)item;
    printf("\n═══════════════════════════════════════════\n");
    printf("🏺 遗物信息:\n");
    printf("═══════════════════════════════════════════\n");
    printf("ID: %d\n", relic->base.id);
    printf("名称: %s\n", relic->base.name);
    printf("品质: %s (%c)\n", getQualityString(relic->quality), qualityToChar(relic->quality));
    printf("损坏程度: %.1f%%\n", relic->damageLevel * 100);
    printf("修复难度: %.1f%%\n", relic->repairDifficulty * 100);
    printf("修复次数: %d (成功: %d)\n", relic->totalRepairTimes, relic->successfulRepairs);
    printf("价值: %d 金币\n", relic->base.value);
    printf("数量: %d\n", relic->base.quantity);
    printf("描述: %s\n", relic->description);

    // 显示损坏状态
    printf("状态: ");
    if (relic->damageLevel < 0.1) {
        printf("完美\n");
    }
    else if (relic->damageLevel < 0.3) {
        printf("良好\n");
    }
    else if (relic->damageLevel < 0.6) {
        printf("轻微损坏\n");
    }
    else if (relic->damageLevel < 0.9) {
        printf("严重损坏\n");
    }
    else {
        printf("濒临损毁\n");
    }

    // 显示修复建议
    if (relic->damageLevel > 0.8) {
        printf("建议: 急需修复！\n");
    }
    else if (relic->damageLevel > 0.5) {
        printf("建议: 建议尽快修复\n");
    }
    else if (relic->damageLevel > 0.3) {
        printf("建议: 可以修复以提升价值\n");
    }

    printf("═══════════════════════════════════════════\n");
}

static void useRelic(Item* item, void* context, Player *player) {
    Relic* relic = (Relic*)item;
    printf("遗物 %s 目前无法直接使用，需要修复后才能发挥效果。\n", relic->base.name);
}

static void destroyRelic(Item* item) {
    free(item);
}

// Tool的函数实现
static void displayToolInfo(Item* item) {
    Tool* tool = (Tool*)item;
    printf("\n═══════════════════════════════════════════\n");
    printf("🛠️  工具信息:\n");
    printf("═══════════════════════════════════════════\n");
    printf("ID: %d\n", tool->base.id);
    printf("名称: %s\n", tool->base.name);
    printf("类型: %s\n", getToolTypeString(tool->toolType));
    printf("价值: %d 金币\n", tool->base.value);
    printf("数量: %d\n", tool->base.quantity);

    // 根据工具类型显示不同信息
    switch (tool->toolType) {
    case TOOL_HEALTH_POTION:
        printf("效果: 恢复 %.0f 点生命值\n", tool->effectValue);
        break;
    case TOOL_REPAIR_KIT_LOW:
    case TOOL_REPAIR_KIT_MEDIUM:
    case TOOL_REPAIR_KIT_HIGH:
        printf("修复效率: %.1f%%\n", tool->effectValue * 100);
        printf("修复成功率: %.1f%%\n", getRepairKitEfficiency(tool->toolType) * 100);
        break;
    default:
        break;
    }

    printf("功能: %s\n", tool->function);
    printf("═══════════════════════════════════════════\n");
}

//拓展tool的使用效果请在此函数中添加case，修理相关的功能仅仅从选中物品后调用，不可直接使用修理工具
static void useTool(Item* item, void* context, Player *player) {
    Tool* tool = (Tool*)item;
    printf("使用了 %s\n", tool->base.name);

    switch (tool->toolType) {
    case TOOL_HEALTH_POTION:
		heal(player, (int)(tool->effectValue));
		Sleep(500);
        break;
	case TOOL_REPAIR_KIT_LOW:
	case TOOL_REPAIR_KIT_MEDIUM:
    case TOOL_REPAIR_KIT_HIGH:
        printf("请在修复遗物界面选择要修复的遗物！\n");
		Sleep(500);
        return;
    default:
        printf("物品使用效果已生效。\n");
		Sleep(500);
        break;
    }

    tool->base.quantity--;
    if (tool->base.quantity <= 0) {
        printf("物品已用完！\n");
        Sleep(500);
    }
}

static void destroyTool(Item* item) {
    free(item);
}

// 创建Relic
Item* createRelic(int id, const char* name, int value, int quantity,
    RelicQuality quality, float damageLevel,
    float repairDifficulty, const char* description) {
    Relic* relic = (Relic*)malloc(sizeof(Relic));
    if (!relic) return NULL;

    relic->base.id = id;
    strncpy_s(relic->base.name, sizeof(relic->base.name), name, sizeof(relic->base.name) - 1);
    relic->base.type = ITEM_RELIC;
    relic->base.value = value;
    relic->base.quantity = quantity;
    relic->base.next = NULL;

    relic->quality = quality;
    relic->damageLevel = damageLevel;
    relic->repairDifficulty = repairDifficulty;
    strncpy_s(relic->description, sizeof(relic->description), description, sizeof(relic->description) - 1);

    // 设置虚函数
    relic->base.displayInfo = displayRelicInfo;
    relic->base.use = useRelic;
    relic->base.destroy = destroyRelic;

    return (Item*)relic;
}

// 创建Tool
Item* createTool(int id, const char* name, int value, int quantity,
    ToolType toolType, float effectValue, const char* function) {
    Tool* tool = (Tool*)malloc(sizeof(Tool));
    if (!tool) return NULL;

    tool->base.id = id;
    strncpy_s(tool->base.name, sizeof(tool->base.name), name, sizeof(tool->base.name) - 1);
    tool->base.type = ITEM_TOOL;
    tool->base.value = value;
    tool->base.quantity = quantity;
    tool->base.next = NULL;

    tool->toolType = toolType;
    tool->effectValue = effectValue;
    strncpy_s(tool->function, sizeof(tool->function), function, sizeof(tool->function) - 1);

    // 设置虚函数
    tool->base.displayInfo = displayToolInfo;
    tool->base.use = useTool;
    tool->base.destroy = destroyTool;

    return (Item*)tool;
}

// 辅助函数
const char* getQualityString(RelicQuality quality) {
    switch (quality) {
    case QUALITY_C: return "C级";
    case QUALITY_B: return "B级";
    case QUALITY_A: return "A级";
    case QUALITY_S: return "S级";
    case QUALITY_SS: return "SS级";
    case QUALITY_SSS: return "SSS级";
    default: return "未知";
    }
}

const char* getToolTypeString(ToolType type) {
    switch (type) {
    case TOOL_HEALTH_POTION: return "治疗药水";
    case TOOL_REPAIR_KIT_LOW: return "低级维修套件";
    case TOOL_REPAIR_KIT_MEDIUM: return "中级维修套件";
    case TOOL_REPAIR_KIT_HIGH: return "高级维修套件";
    case TOOL_OTHER: return "其他工具";
    default: return "未知";
    }
}

RelicQuality charToQuality(char c) {
    switch (c) {
    case 'C': return QUALITY_C;
    case 'B': return QUALITY_B;
    case 'A': return QUALITY_A;
    case 'S': return QUALITY_S;
    case '2': return QUALITY_SS;  // '2' 表示 SS
    case '3': return QUALITY_SSS; // '3' 表示 SSS
    default: return QUALITY_C;
    }
}

char qualityToChar(RelicQuality quality) {
    switch (quality) {
    case QUALITY_C: return 'C';
    case QUALITY_B: return 'B';
    case QUALITY_A: return 'A';
    case QUALITY_S: return 'S';
    case QUALITY_SS: return '2';  // 用'2'表示SS
    case QUALITY_SSS: return '3'; // 用'3'表示SSS
    default: return '?';
    }
}

float getRepairKitEfficiency(ToolType toolType) {
    switch (toolType) {
    case TOOL_REPAIR_KIT_LOW: return 0.4;    // 40%成功率
    case TOOL_REPAIR_KIT_MEDIUM: return 0.7; // 70%成功率
    case TOOL_REPAIR_KIT_HIGH: return 0.9;   // 90%成功率
    default: return 0.0;
    }
}

// 检查工具是否可以修复遗物
bool canRepairRelic(Item* tool) {
    if (!tool || tool->type != ITEM_TOOL) return false;

    Tool* toolItem = (Tool*)tool;
    return (toolItem->toolType == TOOL_REPAIR_KIT_LOW ||
        toolItem->toolType == TOOL_REPAIR_KIT_MEDIUM ||
        toolItem->toolType == TOOL_REPAIR_KIT_HIGH);
}

// 应用修复
void applyRepair(Relic* relic, Tool* tool) {
    if (!relic || !tool) return;

    float repairAmount = 0.0;
    switch (tool->toolType) {
    case TOOL_REPAIR_KIT_LOW:
        repairAmount = 0.2;  // 修复20%
        break;
    case TOOL_REPAIR_KIT_MEDIUM:
        repairAmount = 0.4;  // 修复40%
        break;
    case TOOL_REPAIR_KIT_HIGH:
        repairAmount = 0.6;  // 修复60%
        break;
    default:
        return;
    }

    // 应用修复
    relic->damageLevel -= repairAmount;
    if (relic->damageLevel < 0.0) {
        relic->damageLevel = 0.0;
    }

    // 减少工具数量
    tool->base.quantity--;
}

// 获取修复套件的修复量
float getRepairKitAmount(ToolType toolType) {
    switch (toolType) {
    case TOOL_REPAIR_KIT_LOW:
        return 0.2;  // 修复20%损坏
    case TOOL_REPAIR_KIT_MEDIUM:
        return 0.35; // 修复35%损坏
    case TOOL_REPAIR_KIT_HIGH:
        return 0.5;  // 修复50%损坏
    default:
        return 0.0;
    }
}

// 获取修复套件的严重失败概率
float getCriticalFailureChance(ToolType toolType) {
    switch (toolType) {
    case TOOL_REPAIR_KIT_LOW:
        return 0.1;   // 10%严重失败概率
    case TOOL_REPAIR_KIT_MEDIUM:
        return 0.05;  // 5%严重失败概率
    case TOOL_REPAIR_KIT_HIGH:
        return 0.02;  // 2%严重失败概率
    default:
        return 0.0;
    }
}

// 计算遗物价值（考虑修复次数和损坏程度）
int calculateRelicValue(Relic* relic) {
    if (!relic) return 0;

    int baseValue = relic->base.value;
    float qualityMultiplier = 1.0 + (float)relic->quality * 0.2; // 品质加成

    // 损坏程度影响（越完好价值越高）
    float damageFactor = 1.0 - (relic->damageLevel * 0.5);

    // 修复难度影响（修复难度高的遗物更稀有）
    float difficultyFactor = 1.0 + (relic->repairDifficulty * 0.3);

    // 成功修复次数加成（每成功修复一次增加5%价值）
    float successBonus = 1.0 + (relic->successfulRepairs * 0.05);

    // 计算最终价值
    float finalValue = baseValue * qualityMultiplier * damageFactor *
        difficultyFactor * successBonus;

    // 确保最小价值
    int minValue = baseValue * 0.5;
    int maxValue = baseValue * 2;

    if (finalValue < minValue) finalValue = minValue;
    if (finalValue > maxValue) finalValue = maxValue;

    return (int)finalValue;
}

// 计算修复成功率
float calculateRepairChance(Relic* relic, Tool* tool) {
    if (!relic || !tool) return 0.0;

    // 基础成功率
    float baseChance = getRepairKitEfficiency(tool->toolType);

    // 修复难度影响（难度越高，成功率越低）
    float difficultyFactor = 1.0 - (relic->repairDifficulty * 0.5);

    // 损坏程度影响（损坏越严重越难修）
    float damageFactor = 1.0 - (relic->damageLevel * 0.3);

    // 修复次数影响（修的次数越多越难修）
    float repairCountFactor = 1.0 - (log10f(relic->totalRepairTimes + 1) * 0.1);

    // 最终成功率
    float successChance = baseChance * difficultyFactor *
        damageFactor * repairCountFactor;

    // 确保在合理范围内
    if (successChance < 0.05) successChance = 0.05; // 最低5%成功率
    if (successChance > 0.95) successChance = 0.95; // 最高95%成功率

    return successChance;
}

// 执行修复操作，返回修复结果
RepairResult performRepair(Relic* relic, Tool* tool) {
    if (!relic || !tool) return REPAIR_FAILURE;

    srand(time(NULL) + rand());

    // 计算成功率
    float successChance = calculateRepairChance(relic, tool);

    // 计算严重失败概率
    float criticalFailureChance = getCriticalFailureChance(tool->toolType);

    // 生成随机数
    float roll = (float)rand() / RAND_MAX;

    // 记录总修复次数
    relic->totalRepairTimes++;
    relic->base.repairCount++;

    // 判断修复结果
    if (roll < successChance) {
        // 成功
        return REPAIR_SUCCESS;
    }
    else if (roll < successChance + criticalFailureChance) {
        // 严重失败
        return REPAIR_CRITICAL_FAILURE;
    }
    else {
        // 普通失败
        return REPAIR_FAILURE;
    }
}

// 应用成功修复的效果
void applyRepairSuccess(Relic* relic, Tool* tool) {
    if (!relic || !tool) return;

    // 减少损坏程度
    float repairAmount = getRepairKitAmount(tool->toolType);
    relic->damageLevel -= repairAmount;

    // 确保损坏程度不低于0%
    if (relic->damageLevel < 0.0) {
        relic->damageLevel = 0.0;
    }

    // 提高修复难度（每次成功修复增加10%难度）
    relic->repairDifficulty += 0.05;
    if (relic->repairDifficulty > 0.95) {
        relic->repairDifficulty = 0.95; // 最高95%难度
    }

    // 提高基础价值（成功修复增加价值）
    relic->base.value += relic->base.value * 0.05; // 增加5%

    // 记录成功修复次数
    relic->successfulRepairs++;

    // 重新计算实际价值
    relic->base.value = calculateRelicValue(relic);

    // 减少工具数量
    tool->base.quantity--;

    printf("✨ 修复成功！\n");
    printf("  损坏程度降低 %.1f%%，当前 %.1f%%\n",
        repairAmount * 100, relic->damageLevel * 100);
    printf("  修复难度提高至 %.1f%%\n", relic->repairDifficulty * 100);
    printf("  价值提升至 %d 金币\n", relic->base.value);
}

// 应用普通失败的效果
void applyRepairFailure(Relic* relic, Tool* tool) {
    if (!relic || !tool) return;

    // 随机增加损坏程度（10%-30%）
    srand(time(NULL) + rand());
    float additionalDamage = 0.1 + (float)rand() / RAND_MAX * 0.2;
    relic->damageLevel += additionalDamage;

    // 确保损坏程度不超过100%
    if (relic->damageLevel > 1.0) {
        relic->damageLevel = 1.0;
    }

    // 提高修复难度（每次失败也增加难度，但比成功少）
    relic->repairDifficulty += 0.03;
    if (relic->repairDifficulty > 0.95) {
        relic->repairDifficulty = 0.95;
    }

    // 降低价值（失败修复降低价值）
    relic->base.value -= relic->base.value * 0.1; // 减少10%

    // 确保价值不会太低
    int minValue = 50; // 最低价值50金币
    if (relic->base.value < minValue) {
        relic->base.value = minValue;
    }

    // 重新计算实际价值
    relic->base.value = calculateRelicValue(relic);

    // 减少工具数量
    tool->base.quantity--;

    printf("❌ 修复失败！\n");
    printf("  损坏程度增加 %.1f%%，当前 %.1f%%\n",
        additionalDamage * 100, relic->damageLevel * 100);
    printf("  修复难度提高至 %.1f%%\n", relic->repairDifficulty * 100);
    printf("  价值降低至 %d 金币\n", relic->base.value);
}

// 应用严重失败的效果
void applyCriticalFailure(Relic* relic, Tool* tool) {
    if (!relic || !tool) return;

    // 大量增加损坏程度（30%-60%）
    srand(time(NULL) + rand());
    float additionalDamage = 0.3 + (float)rand() / RAND_MAX * 0.3;
    relic->damageLevel += additionalDamage;

    // 确保损坏程度不超过100%
    if (relic->damageLevel > 1.0) {
        relic->damageLevel = 1.0;
    }

    // 大幅提高修复难度
    relic->repairDifficulty += 0.08;
    if (relic->repairDifficulty > 0.95) {
        relic->repairDifficulty = 0.95;
    }

    // 大幅降低价值
    relic->base.value -= relic->base.value * 0.25; // 减少25%

    // 确保价值不会太低
    int minValue = 30; // 最低价值30金币
    if (relic->base.value < minValue) {
        relic->base.value = minValue;
    }

    // 重新计算实际价值
    relic->base.value = calculateRelicValue(relic);

    // 减少工具数量
    tool->base.quantity--;

    printf("💥 修复严重失败！\n");
    printf("  损坏程度大幅增加 %.1f%%，当前 %.1f%%\n",
        additionalDamage * 100, relic->damageLevel * 100);
    printf("  修复难度大幅提高至 %.1f%%\n", relic->repairDifficulty * 100);
    printf("  价值大幅降低至 %d 金币\n", relic->base.value);
}

Item*getRandomItem(ItemType type,int id) {
    srand((unsigned)time(NULL));
    int op = rand() % 3;
    RelicQuality quality = rand() % 6;
    float damageLevel = (rand()%10)*0.1;
    float repairDifficulty = (rand() % 10) * 0.1;
    int value = rand() % 1000;
    if (type == ITEM_RELIC)
    {
        switch (op) {
        case 0: {
            Item* relic = createRelic(1, "龙之戒指", value, 1,
                quality, damageLevel, repairDifficulty,
                "传说中龙族守护的戒指，增加火属性抗性");
            ((Relic*)relic)->totalRepairTimes = 0;
            ((Relic*)relic)->successfulRepairs = 0;
            ((Relic*)relic)->base.repairCount = 0;
            relic->value = calculateRelicValue((Relic*)relic);
            return relic;
        }
        case 1: {
            Item* relic = createRelic(1, "古代护身符", value, 1,
                quality, damageLevel, repairDifficulty,
                "带有神秘符文，提升魔法恢复速度");
            ((Relic*)relic)->totalRepairTimes = 0;
            ((Relic*)relic)->successfulRepairs = 0;
            ((Relic*)relic)->base.repairCount = 0;
            relic->value = calculateRelicValue((Relic*)relic);
            return relic;
        }
        case 2: {
            Item* relic = createRelic(1, "精灵之泪", value, 1,
                quality, damageLevel, repairDifficulty,
                "精灵的眼泪结晶，蕴含强大的自然能量");
            ((Relic*)relic)->totalRepairTimes = 0;
            ((Relic*)relic)->successfulRepairs = 0;
            ((Relic*)relic)->base.repairCount = 0;
            relic->value = calculateRelicValue((Relic*)relic);
            return relic;
        }
        }
    }
    else if(type == ITEM_TOOL)
    {
        switch (op) {
        case 0: {
            Item* tool = createTool(101, "小型治疗药水", value, 1,
                TOOL_HEALTH_POTION, 50.0f,
                "恢复50点生命值的基础治疗药水");
            return tool;
        }
        case 1: {
            Item* tool = createTool(102, "低级维修套件", value, 1,
                TOOL_REPAIR_KIT_LOW, 0.4f,
                "适用于轻微损坏遗物的维修套件，40%成功率");
            return tool;
        }
        case 2: {
            Item* tool = createTool(103, "中级维修套件", value, 1,
                TOOL_REPAIR_KIT_MEDIUM, 0.7f,
                "适用于中度损坏遗物的维修套件，70%成功率");
            return tool;
        }
        }
	}
}