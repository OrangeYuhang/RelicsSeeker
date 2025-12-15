#ifndef PLAYER_H
#define PLAYER_H

#include "../include/item.h"
#define PLAYER_DATA_FILE "player.dat"

// 玩家状态枚举
typedef enum {
    PLAYER_NORMAL,
    PLAYER_INJURED,
    PLAYER_POISONED,
    PLAYER_BLESSED,
    PLAYER_DEAD
} PlayerStatus;

// 玩家统计结构
typedef struct {
    int totalSteps;         // 总步数
    int treasuresFound;     // 发现的宝藏数
    int eventsTriggered;    // 触发的事件数
    int monstersDefeated;   // 击败的怪物数
} PlayerStats;

// 玩家属性结构
typedef struct {
    int id;
    char name[50];
    int level;
    int exp;
    int maxExp;

    // 生命值
    int hp;
    int maxHp;
    int mp;
    int maxMp;

    // 属性
    int strength;
    int defense;
    int agility;
    int luck;

    // 状态
    PlayerStatus status;
    PlayerStats stats;

    // 位置
    int x;
    int y;
    int layer;
    int direction;  // 方向：0-上，1-右，2-下，3-左

    // 背包指针
    void* backpack;
} Player;

// 初始化函数
Player* createNewPlayer(const char* name, void* backpack);
Player* loadOrCreatePlayer(void* backpack);
bool playerDataExists(void);
bool savePlayerToFile(Player* player);
bool loadPlayerFromFile(Player* player);
void destroyPlayer(Player* player);
void initPlayerStats(Player* player);

// 属性操作
void levelUp(Player* player);
void gainExp(Player* player, int amount);
void heal(Player* player, int amount);
void damage(Player* player, int amount);
bool isPlayerAlive(Player* player);

// 状态操作
void setPlayerStatus(Player* player, PlayerStatus status);
const char* getStatusString(PlayerStatus status);
void updatePlayerStatus(Player* player);

// 位置操作
void movePlayerPosition(Player* player, int dx, int dy);
void setPlayerPosition(Player* player, int x, int y, int layer);
void changePlayerLayer(Player* player, int newLayer);

// 显示函数
void displayPlayerInfo(Player* player);
void displayPlayerStats(Player* player);

// 保存/加载
bool savePlayerData(Player* player, const char* filename);
bool loadPlayerData(Player* player, const char* filename);
void quickSavePlayer(Player* player);

#endif // PLAYER_H