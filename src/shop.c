#include "../include/item.h"
#include "../include/backpack.h"
#include "../include/shop.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>


// 清屏函数
static void clearScreen(void) {
    system("cls");
}

// 获取玩家背包中某物品的数量
int getPlayerItemCount(Backpack* backpack, int itemId, ItemType type) {
    if (!backpack) return 0;

    Item* current = backpack->head;
    int count = 0;

    while (current) {
        if (current->id == itemId && current->type == type) {
            count += current->quantity;
        }
        current = current->next;
    }

    return count;
}

// 初始化商店
void initShop(Shop* shop, Backpack* backpack) {
    if (!shop) return;
    //initBackpack(backpack);

    shop->itemCount = 8;  // 8种商品
    shop->items = (ShopItem*)malloc(sizeof(ShopItem) * shop->itemCount);
    shop->selectedIndex = 0;

    if (!shop->items) {
        printf("商店内存分配失败！\n");
        exit(1);
    }

    // 初始化商品
    for (int i = 0; i < shop->itemCount; i++) {
        shop->items[i].item = NULL;
        shop->items[i].stock = 0;
        shop->items[i].isUnlimited = false;
    }

    // 商品1: 初级治疗药水
    shop->items[0].item = createTool(101, "初级治疗药水", 50, 1,
        TOOL_HEALTH_POTION, 50.0,
        "恢复50点生命值");
    shop->items[0].price = 50;
    shop->items[0].stock = 10;
    shop->items[0].isUnlimited = false;

    // 商品2: 中级治疗药水
    shop->items[1].item = createTool(102, "中级治疗药水", 80, 1,
        TOOL_HEALTH_POTION, 100.0,
        "恢复100点生命值");
    shop->items[1].price = 80;
    shop->items[1].stock = 5;
    shop->items[1].isUnlimited = false;

    // 商品3: 低级维修套件
    shop->items[2].item = createTool(201, "低级维修套件", 100, 1,
        TOOL_REPAIR_KIT_LOW, 0.2,
        "基础维修工具，成功率40%");
    shop->items[2].price = 100;
    shop->items[2].stock = 8;
    shop->items[2].isUnlimited = false;

    // 商品4: 中级维修套件
    shop->items[3].item = createTool(202, "中级维修套件", 200, 1,
        TOOL_REPAIR_KIT_MEDIUM, 0.4,
        "专业维修工具，成功率70%");
    shop->items[3].price = 200;
    shop->items[3].stock = 5;
    shop->items[3].isUnlimited = false;

    // 商品5: 高级维修套件
    shop->items[4].item = createTool(203, "高级维修套件", 400, 1,
        TOOL_REPAIR_KIT_HIGH, 0.6,
        "专家级工具，成功率90%");
    shop->items[4].price = 400;
    shop->items[4].stock = 3;
    shop->items[4].isUnlimited = false;

    // 商品6: C级遗物箱
    shop->items[5].item = createRelic(1001, "C级遗物箱", 200, 1,
        QUALITY_C, 0.7, 0.4,
        "随机C级遗物，可能损坏");
    shop->items[5].price = 200;
    shop->items[5].stock = 5;
    shop->items[5].isUnlimited = true;

    // 商品7: 背包扩展券
    shop->items[6].item = createTool(301, "背包扩展券", 300, 1,
        TOOL_OTHER, 0.0,
        "扩展背包容量10格");
    shop->items[6].price = 300;
    shop->items[6].stock = 99;
    shop->items[6].isUnlimited = true;

    // 商品8: 金币福袋
    shop->items[7].item = createTool(302, "金币福袋", 800, 1,
        TOOL_OTHER, 0.0,
        "随机获得500-1500金币");
    shop->items[7].price = 800;
    shop->items[7].stock = 99;
    shop->items[7].isUnlimited = true;
}

// 清理商店
void cleanupShop(Shop* shop) {
    if (!shop) return;

    if (shop->items) {
        for (int i = 0; i < shop->itemCount; i++) {
            if (shop->items[i].item) {
                shop->items[i].item->destroy(shop->items[i].item);
            }
        }
        free(shop->items);
        shop->items = NULL;
    }
}

// 显示商店界面
void displayShop(Shop* shop, Backpack* backpack) {
    if (!shop || !backpack) return;

    printf("═══════════════════════════════════════════\n");
    printf("  商 店\n");
    printf("═══════════════════════════════════════════\n");
    printf(" 你的金币: %d\n", backpack->money);
    printf("═══════════════════════════════════════════\n\n");

    printf("商品列表:\n");
    printf("┌────┬────────────────────┬────────┬────────┬────────┬────────┐\n");
    printf("│    │        名称        │  类型  │  价格  │  库存  │  拥有  │\n");
    printf("├────┼────────────────────┼────────┼────────┼────────┼────────┤\n");

    for (int i = 0; i < shop->itemCount; i++) {
        ShopItem* shopItem = &shop->items[i];
        Item* item = shopItem->item;

        // 获取玩家拥有数量
        int playerCount = getPlayerItemCount(backpack, item->id, item->type);

        // 获取类型字符串
        const char* typeStr = (item->type == ITEM_RELIC) ? "遗物" : "工具";

        // 显示商品信息
        printf("│ %s %d%s │ %-18s │ %-6s │ %6d │ ",
            (shop->selectedIndex == i) ? ">" : " ",
            i + 1,
            (shop->selectedIndex == i) ? " <" : "  ",
            item->name,
            typeStr,
            shopItem->price);

        // 显示库存
        if (shopItem->isUnlimited) {
            printf("  无限 │");
        }
        else {
            printf(" %6d │", shopItem->stock);
        }

        // 显示玩家拥有数量
        printf(" %6d │\n", playerCount);
    }

    printf("└────┴────────────────────┴────────┴────────┴────────┴────────┘\n");
}

// 显示选中商品详情
void displayShopItemDetails(Shop* shop, Backpack* backpack) {
    if (!shop || !backpack || shop->itemCount == 0) return;

    ShopItem* selectedShopItem = &shop->items[shop->selectedIndex];
    Item* item = selectedShopItem->item;

    printf("\n═══════════════════════════════════════════\n");
    printf(" 商品详情:\n");
    printf("═══════════════════════════════════════════\n");

    // 显示基本信息
    printf("名称: %s\n", item->name);
    printf("类型: %s\n", item->type == ITEM_RELIC ? "遗物" : "工具");
    printf("价格: %d 金币\n", selectedShopItem->price);

    if (selectedShopItem->isUnlimited) {
        printf("库存: 无限\n");
    }
    else {
        printf("库存: %d\n", selectedShopItem->stock);
    }

    // 显示玩家信息
    int playerCount = getPlayerItemCount(backpack, item->id, item->type);
    printf("你已拥有: %d 个\n", playerCount);

    // 显示商品功能描述
    if (item->type == ITEM_TOOL) {
        Tool* tool = (Tool*)item;
        printf("功能: %s\n", tool->function);

        // 根据工具类型显示额外信息
        switch (tool->toolType) {
        case TOOL_HEALTH_POTION:
            printf("效果: 恢复 %.0f 点生命值\n", tool->effectValue);
            break;
        case TOOL_REPAIR_KIT_LOW:
        case TOOL_REPAIR_KIT_MEDIUM:
        case TOOL_REPAIR_KIT_HIGH:
            printf("修复效率: %.1f%%\n", tool->effectValue * 100);
            break;
        default:
            break;
        }
    }
    else if (item->type == ITEM_RELIC) {
        Relic* relic = (Relic*)item;
        printf("品质: %s\n", getQualityString(relic->quality));
        printf("描述: %s\n", relic->description);
    }

    // 显示购买能力
    if (backpack->money >= selectedShopItem->price) {
        if (selectedShopItem->isUnlimited || selectedShopItem->stock > 0) {
            printf("\n你可以购买此商品 (按E键购买)\n");
        }
        else {
            printf("\n商品已售罄\n");
        }
    }
    else {
        printf("\n金币不足！\n");
    }

    printf("═══════════════════════════════════════════\n");
}

// 购买选中商品
void buySelectedItem(Shop* shop, Backpack* backpack) {
    if (!shop || !backpack || shop->itemCount == 0) return;

    ShopItem* selectedShopItem = &shop->items[shop->selectedIndex];
    Item* item = selectedShopItem->item;

    // 检查库存
    if (!selectedShopItem->isUnlimited && selectedShopItem->stock <= 0) {
        printf("\n商品已售罄！\n");
		Sleep(500);
        return;
    }

    // 检查金币
    if (backpack->money < selectedShopItem->price) {
        printf("\n金币不足！需要 %d 金币，你只有 %d 金币。\n",
            selectedShopItem->price, backpack->money);
		Sleep(500);
        return;
    }

    // 确认购买
    printf("\n确认购买 %s ？\n", item->name);
    printf("价格: %d 金币\n", selectedShopItem->price);
    printf("输入 Y 确认，其他键取消: ");

    int confirm = _getch();
    if (confirm != 'y' && confirm != 'Y') {
        printf("\n取消购买。\n");
        Sleep(500);
        return;
    }

    // 扣钱
    backpack->money -= selectedShopItem->price;

    // 减少库存
    if (!selectedShopItem->isUnlimited) {
        selectedShopItem->stock--;
    }

    // 创建新物品添加到背包
    Item* newItem = NULL;
    if (item->type == ITEM_RELIC) {
        Relic* relic = (Relic*)item;
        newItem = createRelic(relic->base.id, relic->base.name,
            relic->base.value, 1,
            relic->quality, relic->damageLevel,
            relic->repairDifficulty, relic->description);
    }
    else {
        Tool* tool = (Tool*)item;
        newItem = createTool(tool->base.id, tool->base.name,
            tool->base.value, 1,
            tool->toolType, tool->effectValue,
            tool->function);
    }

    if (newItem) {
        addItem(backpack, newItem);
        printf("\n 购买成功！%s 已添加到背包。\n", item->name);
        printf("剩余金币: %d\n", backpack->money);

        // 特殊商品效果
        if (item->id == 301) { // 背包扩展券
            backpack->capacity += 10;
            printf("背包容量增加10格，当前容量: %d\n", backpack->capacity);
        }
        else if (item->id == 302) { // 金币福袋
            srand(time(NULL));
            int bonus = 500 + rand() % 1001; // 500-1500随机金币
            backpack->money += bonus;
            printf("打开福袋获得 %d 金币！\n", bonus);
            printf("当前金币: %d\n", backpack->money);
        }
    }
    else {
        printf("\n购买失败！无法创建物品。\n");
        backpack->money += selectedShopItem->price; // 返还金币
        if (!selectedShopItem->isUnlimited) {
            selectedShopItem->stock++; // 恢复库存
        }
    }

    Sleep(1500);
}

// 处理商店输入
int handleShopInput(Shop* shop, Backpack* backpack) {
    if (!shop || !backpack) return 1; // 继续显示商店

    while (1) {
        int ch = _getch();

        switch (ch) {
        case 'w':
        case 'W':
            if (shop->itemCount > 0) {
                shop->selectedIndex = (shop->selectedIndex - 1 + shop->itemCount) % shop->itemCount;
            }
            return 1; // 重新显示

        case 's':
        case 'S':
            if (shop->itemCount > 0) {
                shop->selectedIndex = (shop->selectedIndex + 1) % shop->itemCount;
            }
            return 1; // 重新显示

        case 'e':
        case 'E':
            buySelectedItem(shop, backpack);
            return 1; // 重新显示

        case 'q':
        case 'Q':
            return 0; // 退出商店

        default:
            break;
        }
    }
}
void shop(Backpack *backpack) {

    // 创建商店
    Shop shop;
    initShop(&shop, backpack);

    int inShop = 1;

    // 商店主循环
    while (inShop) {
        clearScreen();
        displayShop(&shop, backpack);
        displayShopItemDetails(&shop, backpack);

        printf("\n操作说明:\n");
        printf("W/S - 上/下选择商品\n");
        printf("E   - 购买选中商品\n");
        printf("Q   - 离开商店\n");
        printf("═══════════════════════════════════════════\n");

        inShop = handleShopInput(&shop, backpack);
    }

    // 清理商店
    cleanupShop(&shop);
    // 清理背包（会自动保存）
    //cleanupBackpack(backpack);
}
