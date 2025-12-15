#include "../include/backpack.h"
#include <conio.h>
#include <windows.h>
#include <time.h>
#define BACKPACK_FILE "backpack.dat"
#define MONEY_FILE "money.dat"

// 清屏函数
void clearScreen(void) {
    system("cls");
    // Linux/Mac: system("clear");
}

// 初始化背包,自动从文件加载数据
void initBackpack(Backpack* backpack) {
    if (!backpack) return;

    backpack->head = NULL;
    backpack->capacity = 50;
    backpack->count = 0;
    backpack->selectedIndex = 0;
    backpack->currentCategory = 0;
    backpack->isSelectingTool = false;
    backpack->selectedRelic = NULL;

    // 尝试从文件加载
    if (!loadBackpackData(backpack, BACKPACK_FILE, MONEY_FILE)) {
        printf("failes");
        _getch();
        // 如果加载失败，使用默认值
        backpack->money = 2000;
        loadSampleItems(backpack);
        printf("创建新的背包存档...\n");
        Sleep(1000);
    }
    else {
        printf("成功加载背包存档！\n");
        Sleep(500);
    }
}

// 加载示例物品
void loadSampleItems(Backpack* backpack) {
    if (!backpack || backpack->count > 0) return; // 已经有物品就不加载示例

    printf("正在加载示例物品...\n");

    // 创建遗物（添加修复次数和成功次数的初始化）
    Item* relic1 = createRelic(1, "龙之戒指", 500, 1,
        QUALITY_A, 0.6, 0.3,
        "传说中龙族守护的戒指，增加火属性抗性");
    ((Relic*)relic1)->totalRepairTimes = 0;
    ((Relic*)relic1)->successfulRepairs = 0;
    ((Relic*)relic1)->base.repairCount = 0;
    relic1->value = calculateRelicValue((Relic*)relic1);

    Item* relic2 = createRelic(2, "古代护身符", 300, 1,
        QUALITY_B, 0.4, 0.5,
        "带有神秘符文，提升魔法恢复速度");
    ((Relic*)relic2)->totalRepairTimes = 0;
    ((Relic*)relic2)->successfulRepairs = 0;
    ((Relic*)relic2)->base.repairCount = 0;
    relic2->value = calculateRelicValue((Relic*)relic2);

    Item* relic3 = createRelic(3, "精灵之泪", 450, 1,
        QUALITY_S, 0.8, 0.2,
        "精灵的眼泪结晶，蕴含强大的自然能量");
    ((Relic*)relic3)->totalRepairTimes = 0;
    ((Relic*)relic3)->successfulRepairs = 0;
    ((Relic*)relic3)->base.repairCount = 0;
    relic3->value = calculateRelicValue((Relic*)relic3);

    Item* relic4 = createRelic(4, "矮人战锤", 600, 1,
        QUALITY_SS, 0.9, 0.4,
        "矮人国王的武器，造成额外伤害");
    ((Relic*)relic4)->totalRepairTimes = 0;
    ((Relic*)relic4)->successfulRepairs = 0;
    ((Relic*)relic4)->base.repairCount = 0;
    relic4->value = calculateRelicValue((Relic*)relic4);

    // 创建工具
    Item* tool1 = createTool(101, "初级治疗药水", 50, 3,
        TOOL_HEALTH_POTION, 50.0,
        "恢复50点生命值");

    Item* tool2 = createTool(102, "中级治疗药水", 80, 2,
        TOOL_HEALTH_POTION, 100.0,
        "恢复100点生命值");

    Item* tool3 = createTool(201, "低级维修套件", 100, 2,
        TOOL_REPAIR_KIT_LOW, 0.2,
        "基础维修工具，适合新手使用");

    Item* tool4 = createTool(202, "中级维修套件", 200, 1,
        TOOL_REPAIR_KIT_MEDIUM, 0.4,
        "专业维修工具，修复效率较高");

    Item* tool5 = createTool(203, "高级维修套件", 400, 1,
        TOOL_REPAIR_KIT_HIGH, 0.6,
        "专家级工具，修复效率最高且风险低");

    // 添加到背包
    Item* items[] = { relic1, relic2, relic3, relic4,
                     tool1, tool2, tool3, tool4, tool5 };

    for (int i = 0; i < sizeof(items) / sizeof(items[0]); i++) {
        addItem(backpack, items[i]);
    }
}

// 清理背包
void cleanupBackpack(Backpack* backpack) {
    if (!backpack) return;

    // 在清理前保存数据
    saveBackpackData(backpack, BACKPACK_FILE, MONEY_FILE);

    // 清理内存
    Item* current = backpack->head;
    while (current != NULL) {
        Item* next = current->next;
        current->destroy(current);
        current = next;
    }

    backpack->head = NULL;
    backpack->count = 0;
}

// 添加物品
bool addItem(Backpack* backpack, Item* item) {
    if (!backpack || !item || backpack->count >= backpack->capacity) {
        return false;
    }

    // 如果物品已存在，增加数量
    Item* existing = findItemById(backpack, item->id);
    if (existing && existing->type == item->type) {
        existing->quantity += item->quantity;
        item->destroy(item); // 销毁新创建的对象
        return true;
    }

    // 添加到链表头部
    item->next = backpack->head;
    backpack->head = item;
    backpack->count++;

    return true;
}

// 移除物品
bool removeItem(Backpack* backpack, int id, int quantity) {
    if (!backpack || quantity <= 0) return false;

    Item* prev = NULL;
    Item* current = backpack->head;

    while (current) {
        if (current->id == id) {
            if (current->quantity > quantity) {
                current->quantity -= quantity;
                return true;
            }
            else if (current->quantity == quantity) {
                // 移除整个物品
                if (prev) {
                    prev->next = current->next;
                }
                else {
                    backpack->head = current->next;
                }

                current->destroy(current);
                backpack->count--;
                return true;
            }
        }

        prev = current;
        current = current->next;
    }

    return false;
}

// 查找物品
Item* findItemById(Backpack* backpack, int id) {
    if (!backpack) return NULL;

    Item* current = backpack->head;
    while (current) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// 获取选中物品
Item* getSelectedItem(Backpack* backpack) {
    return getItemByIndex(backpack, backpack->selectedIndex);
}

// 通过索引获取物品
Item* getItemByIndex(Backpack* backpack, int index) {
    if (!backpack || index < 0) return NULL;

    int count = 0;
    Item* current = backpack->head;

    while (current) {
        if (backpack->currentCategory == 0 ||
            (backpack->currentCategory == 1 && current->type == ITEM_RELIC) ||
            (backpack->currentCategory == 2 && current->type == ITEM_TOOL)) {

            if (count == index) {
                return current;
            }
            count++;
        }
        current = current->next;
    }

    return NULL;
}

// 显示金钱
void displayMoney(Backpack* backpack) {
    if (!backpack) return;

    printf("═══════════════════════════════════════════\n");
    printf("金钱: %d 金币\n", backpack->money);
    printf("容量: %d/%d\n", backpack->count, backpack->capacity);
    printf("═══════════════════════════════════════════\n\n");
}

// 显示物品列表
void displayItems(Backpack* backpack) {
    if (!backpack) return;

    printf("物品列表");
    switch (backpack->currentCategory) {
    case 0: printf(" [全部]"); break;
    case 1: printf(" [遗物]"); break;
    case 2: printf(" [工具]"); break;
    }
    printf(":\n");
    printf("┌────┬────────────┬────────────┬──────┬──────┐\n");
    printf("│ ID │    名称    │    类型    │ 价值 │ 数量 │\n");
    printf("├────┼────────────┼────────────┼──────┼──────┤\n");

    int displayed = 0;
    Item* current = backpack->head;

    while (current) {
        if (backpack->currentCategory == 0 ||
            (backpack->currentCategory == 1 && current->type == ITEM_RELIC) ||
            (backpack->currentCategory == 2 && current->type == ITEM_TOOL)) {

            const char* typeStr = (current->type == ITEM_RELIC) ? "遗物" : "工具";

            printf("│ %s%3d%s │ %-10s │ %-10s │ %4d │ %4d │\n",
                (backpack->selectedIndex == displayed) ? ">" : " ",
                current->id,
                (backpack->selectedIndex == displayed) ? "<" : " ",
                current->name,
                typeStr,
                current->value,
                current->quantity);

            displayed++;
        }
        current = current->next;
    }

    if (displayed == 0) {
        printf("│           暂无物品                      │\n");
    }

    printf("└────┴────────────┴────────────┴──────┴──────┘\n");
}

// 显示物品详情
void displayItemDetails(Backpack* backpack) {
    if (!backpack) return;

    Item* selected = getSelectedItem(backpack);
    if (selected) {
        selected->displayInfo(selected);
    }
    else {
        printf("\n背包是空的！\n");
    }
}

// 显示修复信息
void displayRepairInfo(Backpack* backpack) {
    if (!backpack || !backpack->selectedRelic) return;

    Relic* relic = (Relic*)backpack->selectedRelic;

    printf("\n═══════════════════════════════════════════\n");
    printf("修复遗物: %s\n", relic->base.name);
    printf("═══════════════════════════════════════════\n");
    printf("当前损坏: %.1f%%\n", relic->damageLevel * 100);
    printf("修复难度: %.1f%%\n", relic->repairDifficulty * 100);
    printf("已修复次数: %d (成功: %d)\n", relic->totalRepairTimes, relic->successfulRepairs);
    printf("当前价值: %d 金币\n", relic->base.value);

    // 显示修复风险提示
    if (relic->totalRepairTimes > 5) {
        printf("!!!多次修复：该遗物已修复 %d 次，修复难度较高\n", relic->totalRepairTimes);
    }

    if (relic->damageLevel > 0.8) {
        printf("!!! 高风险：损坏严重，修复失败可能造成更大损坏\n");
    }

    printf("\n选择维修套件 (W/S 选择, E 确认, Q 取消):\n");
}

// 显示控制说明
void displayControls(Backpack* backpack) {
    if (!backpack) return;

    printf("\n🎮 操作说明:\n");

    if (backpack->isSelectingTool) {
        printf("W/S - 选择维修工具\n");
        printf("E   - 确认使用该工具修复\n");
        printf("Q   - 取消修复\n");
        printf("\n修复说明:\n");
        printf("• 成功：降低损坏，提高价值，增加难度\n");
        printf("• 失败：增加损坏，降低价值，增加难度\n");
        printf("• 严重失败：大幅增加损坏和难度\n");
    }
    else {
        printf("W/S - 上/下选择物品\n");
        printf("A/D - 切换物品类别\n");
        printf("E   - 使用/修复选中物品\n");
        printf("R   - 修复遗物（遗物专用）\n");
        printf("X   - 出售选中物品\n");  // 改为 X 键
        printf("F   - 手动保存游戏\n");
        printf("Q   - 退出背包系统\n");
    }
    printf("═══════════════════════════════════════════\n");
}

// 使用选中物品
void useSelectedItem(Backpack* backpack,Player *player) {
    Item* selected = getSelectedItem(backpack);
    if (!selected) return;

    if (selected->type == ITEM_TOOL) {
        selected->use(selected, NULL, player);

        // 如果物品用完，从背包移除
        if (selected->quantity <= 0) {
            removeItem(backpack, selected->id, 1);
        }
    }
    else {
        printf("遗物需要先修复才能使用！\n");
    }
}

// 修复遗物
void repairSelectedRelic(Backpack* backpack) {
    Item* selected = getSelectedItem(backpack);
    if (!selected || selected->type != ITEM_RELIC) {
        printf("请选择一个遗物进行修复！\n");
        return;
    }

    Relic* relic = (Relic*)selected;

    // 检查遗物是否已经完全修复
    if (relic->damageLevel <= 0.01) {
        printf("该遗物已经完好无损，无需修复！\n");
        Sleep(500);
        return;
    }

    backpack->isSelectingTool = true;
    backpack->selectedRelic = selected;
    backpack->currentCategory = 2; // 切换到工具类别
    backpack->selectedIndex = 0;

    // 等待用户选择工具
    while (backpack->isSelectingTool) {
        clearScreen();
        displayMoney(backpack);
        displayItems(backpack);
        displayRepairInfo(backpack);
        displayControls(backpack);

        // 处理工具选择
        int ch = getch();
        switch (ch) {
        case 'w':
        case 'W':
            if (getCurrentCategoryCount(backpack) > 0) {
                backpack->selectedIndex = (backpack->selectedIndex - 1 +
                    getCurrentCategoryCount(backpack)) %
                    getCurrentCategoryCount(backpack);
            }
            break;

        case 's':
        case 'S':
            if (getCurrentCategoryCount(backpack) > 0) {
                backpack->selectedIndex = (backpack->selectedIndex + 1) %
                    getCurrentCategoryCount(backpack);
            }
            break;

        case 'e':
        case 'E': {
            Item* tool = getSelectedItem(backpack);
            if (tool && canRepairRelic(tool)) {
                Tool* repairTool = (Tool*)tool;

                // 显示修复详情
                float successChance = calculateRepairChance(relic, repairTool);
                float repairAmount = getRepairKitAmount(repairTool->toolType);
                float criticalChance = getCriticalFailureChance(repairTool->toolType);

                printf("\n═══════════════════════════════════════════\n");
                printf("修复详情:\n");
                printf("遗物: %s\n", relic->base.name);
                printf("当前损坏: %.1f%%\n", relic->damageLevel * 100);
                printf("修复工具: %s\n", repairTool->base.name);
                printf("修复量: %.1f%%\n", repairAmount * 100);
                printf("成功率: %.1f%%\n", successChance * 100);
                printf("严重失败率: %.1f%%\n", criticalChance * 100);
                printf("═══════════════════════════════════════════\n");
                printf("是否确认修复？ (Y/N): ");

                int confirm = getch();
                if (confirm == 'y' || confirm == 'Y') {
                    // 执行修复
                    RepairResult result = performRepair(relic, repairTool);

                    printf("\n");
                    switch (result) {
                    case REPAIR_SUCCESS:
                        applyRepairSuccess(relic, repairTool);
                        break;
                    case REPAIR_FAILURE:
                        applyRepairFailure(relic, repairTool);
                        break;
                    case REPAIR_CRITICAL_FAILURE:
                        applyCriticalFailure(relic, repairTool);
                        break;
                    }

                    // 如果工具用完，移除
                    if (tool->quantity <= 0) {
                        removeItem(backpack, tool->id, 1);
                    }

                    // 检查遗物是否完全损毁
                    if (relic->damageLevel >= 0.99) {
                        printf("\n!!!警告：遗物 %s 已几乎完全损毁！\n",
                            relic->base.name);
                        printf("   继续修复可能导致永久损坏！\n");
                    }

                    // 修复完成后回到物品列表
                    backpack->isSelectingTool = false;
                    backpack->selectedRelic = NULL;
                    backpack->currentCategory = 0;
                    backpack->selectedIndex = 0;
                }
            }
            else {
                printf("请选择一个维修套件！\n");
            }
            break;
        }

        case 'q':
        case 'Q':
            backpack->isSelectingTool = false;
            backpack->selectedRelic = NULL;
            backpack->currentCategory = 0;
            backpack->selectedIndex = 0;
            break;

        default:
            break;
        }
    }
}

// 出售选中物品
void sellSelectedItem(Backpack* backpack) {
    Item* selected = getSelectedItem(backpack);
    if (!selected) return;

    int sellValue = selected->value * selected->quantity;

    printf("\n出售 %s x%d 可获得 %d 金币\n",
        selected->name, selected->quantity, sellValue);
    printf("确认出售？ (Y/N): ");

    int ch = getch();
    if (ch == 'y' || ch == 'Y') {
        backpack->money += sellValue;
        removeItem(backpack, selected->id, selected->quantity);
        printf("已出售！\n");
    }
    else {
        printf("取消出售。\n");
    }
}

// 处理输入
int handleInput(Backpack* backpack,Player *player) {
    if (!backpack) return 0;

    int currentCount = getCurrentCategoryCount(backpack);

    while (1) {
        int ch = getch();

        switch (ch) {
        case 'w':
        case 'W':
            if (currentCount > 0) {
                backpack->selectedIndex = (backpack->selectedIndex - 1 + currentCount) % currentCount;
            }
            return 1;

        case 's':
        case 'S':
            if (currentCount > 0) {
                backpack->selectedIndex = (backpack->selectedIndex + 1) % currentCount;
            }
            return 1;

        case 'a':
        case 'A':
            backpack->currentCategory = (backpack->currentCategory - 1 + 3) % 3;
            backpack->selectedIndex = 0;
            return 1;

        case 'd':
        case 'D':
            backpack->currentCategory = (backpack->currentCategory + 1) % 3;
            backpack->selectedIndex = 0;
            return 1;

        case 'e':
        case 'E':
            useSelectedItem(backpack,player);
            return 1;

        case 'r':
        case 'R':
            repairSelectedRelic(backpack);
            return 1;

        case 'f':
        case 'F':
            saveGame(backpack);
            return 1;

        case 'x': 
        case 'X':
            if (!backpack->isSelectingTool) {
                sellSelectedItem(backpack);
            }
            return 1;

        case 'q':
        case 'Q':
            // 退出前自动保存
            saveGame(backpack);
            return 0;

        default:
            break;
        }
    }
}

// 获取当前类别物品数量
int getCurrentCategoryCount(Backpack* backpack) {
    if (!backpack) return 0;

    int count = 0;
    Item* current = backpack->head;

    while (current) {
        if (backpack->currentCategory == 0 ||
            (backpack->currentCategory == 1 && current->type == ITEM_RELIC) ||
            (backpack->currentCategory == 2 && current->type == ITEM_TOOL)) {
            count++;
        }
        current = current->next;
    }

    return count;
}

// 从文件创建物品
Item* createItemFromData(int id, const char* name, int type, int value, int quantity,
    int repairCount, float damageLevel, float repairDifficulty,
    int quality, int toolType, float effectValue,
    const char* description, const char* function) {

    Item* item = NULL;

    if (type == ITEM_RELIC) {
        // 创建遗物
        item = createRelic(id, name, value, quantity,
            (RelicQuality)quality, damageLevel,
            repairDifficulty, description);
        if (item) {
            Relic* relic = (Relic*)item;
            relic->totalRepairTimes = repairCount;
            relic->successfulRepairs = 0; // 需要从文件读取，这里简化处理
            item->repairCount = repairCount;
            // 重新计算价值
            item->value = calculateRelicValue(relic);
        }
    }
    else if (type == ITEM_TOOL) {
        // 创建工具
        item = createTool(id, name, value, quantity,
            (ToolType)toolType, effectValue, function);
        if (item) {
            item->repairCount = repairCount;
        }
    }

    return item;
}

// 保存背包数据到文件
bool saveBackpackData(Backpack* backpack, const char* backpackFile, const char* moneyFile) {
    if (!backpack) return false;

    // 保存金钱
    FILE* moneyFp = fopen(moneyFile, "wb");
    if (!moneyFp) {
        printf("无法保存金钱数据到文件: %s\n", moneyFile);
        return false;
    }

    fwrite(&backpack->money, sizeof(int), 1, moneyFp);
    fclose(moneyFp);

    // 保存物品数据
    FILE* backpackFp = fopen(backpackFile, "wb");
    if (!backpackFp) {
        printf("无法保存背包数据到文件: %s\n", backpackFile);
        return false;
    }

    // 写入物品数量
    fwrite(&backpack->count, sizeof(int), 1, backpackFp);

    // 遍历所有物品并保存
    Item* current = backpack->head;
    while (current) {
        // 保存通用物品属性
        fwrite(&current->id, sizeof(int), 1, backpackFp);

        // 保存名称（包括终止符）
        fwrite(current->name, sizeof(char), 50, backpackFp);

        fwrite(&current->type, sizeof(ItemType), 1, backpackFp);
        fwrite(&current->value, sizeof(int), 1, backpackFp);
        fwrite(&current->quantity, sizeof(int), 1, backpackFp);
        fwrite(&current->repairCount, sizeof(int), 1, backpackFp);

        if (current->type == ITEM_RELIC) {
            Relic* relic = (Relic*)current;

            // 保存遗物特有属性
            fwrite(&relic->damageLevel, sizeof(float), 1, backpackFp);
            fwrite(&relic->repairDifficulty, sizeof(float), 1, backpackFp);
            fwrite(&relic->quality, sizeof(RelicQuality), 1, backpackFp);
            fwrite(&relic->totalRepairTimes, sizeof(int), 1, backpackFp);
            fwrite(&relic->successfulRepairs, sizeof(int), 1, backpackFp);

            // 保存描述
            fwrite(relic->description, sizeof(char), 200, backpackFp);

            // 为工具类型写入默认值（保持文件格式一致）
            int toolType = -1;
            float effectValue = 0.0f;
            char emptyFunction[200] = { 0 };

            fwrite(&toolType, sizeof(int), 1, backpackFp);
            fwrite(&effectValue, sizeof(float), 1, backpackFp);
            fwrite(emptyFunction, sizeof(char), 200, backpackFp);

        }
        else if (current->type == ITEM_TOOL) {
            Tool* tool = (Tool*)current;

            // 为遗物属性写入默认值（保持文件格式一致）
            float damageLevel = 0.0f;
            float repairDifficulty = 0.0f;
            int quality = 0;
            int totalRepairTimes = 0;
            int successfulRepairs = 0;
            char emptyDescription[200] = { 0 };

            fwrite(&damageLevel, sizeof(float), 1, backpackFp);
            fwrite(&repairDifficulty, sizeof(float), 1, backpackFp);
            fwrite(&quality, sizeof(int), 1, backpackFp);
            fwrite(&totalRepairTimes, sizeof(int), 1, backpackFp);
            fwrite(&successfulRepairs, sizeof(int), 1, backpackFp);
            fwrite(emptyDescription, sizeof(char), 200, backpackFp);

            // 保存工具特有属性
            fwrite(&tool->toolType, sizeof(ToolType), 1, backpackFp);
            fwrite(&tool->effectValue, sizeof(float), 1, backpackFp);
            fwrite(tool->function, sizeof(char), 200, backpackFp);
        }

        current = current->next;
    }

    fclose(backpackFp);
    return true;
}

// 从文件加载背包数据
bool loadBackpackData(Backpack* backpack, const char* backpackFile, const char* moneyFile) {
    if (!backpack) return false;

    // 先检查文件是否存在
    FILE* testFp = fopen(backpackFile, "rb");
    if (!testFp) {
        return false; // 文件不存在，创建新的背包
    }
    fclose(testFp);

    // 加载金钱
    FILE* moneyFp = fopen(moneyFile, "rb");
    if (!moneyFp) {
        printf("无法加载金钱数据: %s\n", moneyFile);
        return false;
    }

    fread(&backpack->money, sizeof(int), 1, moneyFp);
    fclose(moneyFp);

    // 加载物品数据
    FILE* backpackFp = fopen(backpackFile, "rb");
    if (!backpackFp) {
        printf("无法加载背包数据: %s\n", backpackFile);
        return false;
    }

    // 读取物品数量
    int itemCount = 0;
    fread(&itemCount, sizeof(int), 1, backpackFp);

    if (itemCount <= 0) {
        fclose(backpackFp);
        return true; // 空背包，加载成功
    }

    // 清空当前背包
    cleanupBackpack(backpack);

    // 读取所有物品
    for (int i = 0; i < itemCount; i++) {
        int id;
        char name[50];
        ItemType type;
        int value, quantity, repairCount;

        // 读取通用属性
        fread(&id, sizeof(int), 1, backpackFp);
        fread(name, sizeof(char), 50, backpackFp);
        fread(&type, sizeof(ItemType), 1, backpackFp);
        fread(&value, sizeof(int), 1, backpackFp);
        fread(&quantity, sizeof(int), 1, backpackFp);
        fread(&repairCount, sizeof(int), 1, backpackFp);

        float damageLevel, repairDifficulty;
        int quality, totalRepairTimes, successfulRepairs;
        char description[200];
        int toolType;
        float effectValue;
        char function[200];

        // 读取所有字段（包括两种类型的所有字段）
        fread(&damageLevel, sizeof(float), 1, backpackFp);
        fread(&repairDifficulty, sizeof(float), 1, backpackFp);
        fread(&quality, sizeof(int), 1, backpackFp);
        fread(&totalRepairTimes, sizeof(int), 1, backpackFp);
        fread(&successfulRepairs, sizeof(int), 1, backpackFp);
        fread(description, sizeof(char), 200, backpackFp);
        fread(&toolType, sizeof(int), 1, backpackFp);
        fread(&effectValue, sizeof(float), 1, backpackFp);
        fread(function, sizeof(char), 200, backpackFp);

        // 根据类型创建物品
        Item* item = createItemFromData(id, name, type, value, quantity, repairCount,
            damageLevel, repairDifficulty, quality,
            toolType, effectValue, description, function);

        if (item) {
            if (type == ITEM_RELIC) {
                Relic* relic = (Relic*)item;
                relic->totalRepairTimes = totalRepairTimes;
                relic->successfulRepairs = successfulRepairs;
            }
            addItem(backpack, item);
        }
    }

    fclose(backpackFp);
    return true;
}

// 添加一个单独保存的函数，可以在游戏中手动保存
void saveGame(Backpack* backpack) {
    if (saveBackpackData(backpack, BACKPACK_FILE, MONEY_FILE)) {
        printf("游戏已保存！\n");
        Sleep(500);
    }
    else {
        printf("保存失败！\n");
        Sleep(500);
    }
}

void backpack(Player *player,Backpack*backpack) {
    //initBackpack(backpack);

    int running = 1;

    // 主循环
    while (running) {
        clearScreen();

        // 显示界面
        displayMoney(backpack);
        displayItems(backpack);
        displayItemDetails(backpack);
        displayControls(backpack);

        // 处理输入
        running = handleInput(backpack, player);
    }

    // 清理（会自动保存）
    //cleanupBackpack(backpack);

    //printf("\n背包数据已保存到文件。\n");
    Sleep(500);

    return 0;
}
