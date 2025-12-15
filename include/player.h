#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "../include/commontypes.h"
#define PLAYER_DATA_FILE "player.dat"

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
//bool savePlayerData(Player* player, const char* filename);
//bool loadPlayerData(Player* player, const char* filename);
void quickSavePlayer(Player* player);

#endif // PLAYER_H