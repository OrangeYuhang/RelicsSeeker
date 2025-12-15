#ifndef SHOP_H
#define SHOP_H

#include "../include/backpack.h"
#include "../include/commontypes.h"

// 商店商品结构（内部使用）
typedef struct {
    Item* item;           // 商品
    int price;            // 价格（可能和物品价值不同）
    int stock;            // 库存
    bool isUnlimited;     // 是否无限供应
} ShopItem;

// 商店结构（内部使用）
typedef struct {
    ShopItem* items;      // 商品列表
    int itemCount;        // 商品数量
    int selectedIndex;    // 选中商品索引
} Shop;

//函数声明
void initShop(Shop* shop, Backpack* backpack);
void cleanupShop(Shop* shop);
void displayShop(Shop* shop, Backpack* backpack);
void displayShopItemDetails(Shop* shop, Backpack* backpack);
void buySelectedItem(Shop* shop, Backpack* backpack);
int handleShopInput(Shop* shop, Backpack* backpack);
void shop(Backpack *backpack);


// 辅助函数（可选，供商店内部使用）
int getPlayerItemCount(Backpack* backpack, int itemId, ItemType type);

#endif // SHOP_H