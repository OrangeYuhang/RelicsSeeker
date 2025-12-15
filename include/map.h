#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/player.h"
#include "../include/backpack.h"
#include "../include/commontypes.h"


// 视野窗口大小
#define VIEW_WIDTH 11
#define VIEW_HEIGHT 9

// 单个地图的维度
#define MAP_WIDTH 30
#define MAP_HEIGHT 20
#define MAX_LAYERS 3

// 最大地图数量
#define MAX_MAPS 5

// 地图元素类型
typedef enum {
    MAP_EMPTY,
    MAP_WALL,
    MAP_TREE,
    MAP_RIVER,
    MAP_GRASS,
    MAP_PATH,
    MAP_TREASURE,
    MAP_EVENT,
    MAP_ENTRANCE,
    MAP_STAIR,
    MAP_NPC,
    MAP_MONSTER,
    MAP_SHOP
} MapElement;

// 事件类型
typedef enum {
    MAP_EVENT_NONE,
    MAP_EVENT_HEAL,
    MAP_EVENT_TRAP,
    MAP_EVENT_RIDDLE,
    MAP_EVENT_BATTLE,
    MAP_EVENT_FIND_ITEM,
    MAP_EVENT_FIND_GOLD,
    MAP_EVENT_QUEST
} MapEventType;

// 宝藏类型
typedef enum {
    TREASURE_COINS,
    TREASURE_ITEM,
    TREASURE_RELIC,
    TREASURE_KEY
} TreasureType;

// 地图单元
typedef struct {
    MapElement element;
    MapEventType eventType;
    int eventValue;
    int itemId;            // 物品ID（如果是物品事件）
    char description[100];
    bool isTriggered;
    bool isExplored;

    // 宝藏相关
    TreasureType treasureType;
    int treasureValue;
    bool isOpened;
} MapCell;

// 单个地图层
typedef struct {
    MapCell cells[MAP_HEIGHT][MAP_WIDTH];
    char layerName[30];
    int layerNumber;
} MapLayer;

// 完整的多层地图
typedef struct {
    MapLayer layers[MAX_LAYERS];
    char mapName[50];
    char mapDescription[200];
    int difficulty;
    int totalLayers;
    int startLayer;
    int startX, startY;
    bool isUnlocked;
    int unlockLevel;  // 解锁所需等级
} MultiLayerMap;

// 地图系统
typedef struct {
    MultiLayerMap maps[MAX_MAPS];
    int totalMaps;
    int selectedMapIndex;
    bool isSelectingMap;

    // 当前游戏状态
    Player* player;
    Backpack* backpack;
    int currentMapIndex;
    int currentLayer;

    // 游戏统计
    int treasuresFound;
    int eventsTriggered;
    int monstersDefeated;
} MapSystem;

// 初始化函数
void initMapSystem(MapSystem* map, Player* player, Backpack* backpack);
void cleanupMapSystem(MapSystem* map);

// 地图选择界面
void runMapSelection(MapSystem* map);
void displayMapSelection(MapSystem* map);
int handleMapSelectionInput(MapSystem* map);

// 游戏内地图探索
//bool canMoveTo(MapSystem* map, int x, int y, int layer);
void runMapExploration(MapSystem* map);
void displayMapView(MapSystem* map);
void displayPlayerStatus(MapSystem* map);
void mapdisplayControls(MapSystem* map);
bool movePlayerInMap(MapSystem* map, int dx, int dy);
void interactCurrentCell(MapSystem* map);
void triggerEvent(MapSystem* map, MapCell* cell);
void openTreasure(MapSystem* map, MapCell* cell);
void changeLayer(MapSystem* map, int newLayer);

// 工具函数
MapCell* getCurrentCell(MapSystem* map);
char getElementChar(MapElement element);
const char* getElementName(MapElement element);
bool canMoveTo(MapSystem* map, int x, int y, int layer);

// 地图生成函数
void generateForestMap(MultiLayerMap* map);
void generateCaveMap(MultiLayerMap* map);
void generateRuinsMap(MultiLayerMap* map);

#endif // MAP_H