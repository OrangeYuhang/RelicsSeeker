#include "../include/map.h"
#include "../include/backpack.h"
#include "../include/player.h"
#include "../include/shop.h"
#include "../include/item.h"
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

// 颜色定义
#ifdef _WIN32
#define COLOR_NORMAL 7
#define COLOR_HIGHLIGHT 14
#define COLOR_TITLE 11
#define COLOR_INFO 10
#define COLOR_WARNING 12
#define COLOR_SUCCESS 10
#define COLOR_PLAYER 13
#define COLOR_WALL 8
#define COLOR_TREE 2
#define COLOR_RIVER 9
#define COLOR_TREASURE 6
#define COLOR_EVENT 13
#define COLOR_MONSTER 12
#define COLOR_SHOP 14
#endif

// 辅助函数
static void setColor(int color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#endif
}

static void clearScreen(void) {
    system("cls");
}

static void pressAnyKey(void) {
    printf("按任意键继续...");
    _getch();
}

// 元素显示字符
char getElementChar(MapElement element) {
    switch (element) {
    case MAP_EMPTY:   return ' ';
    case MAP_WALL:    return '#';
    case MAP_TREE:    return '♣';
    case MAP_RIVER:   return '≈';
    case MAP_GRASS:   return '.';
    case MAP_PATH:    return '·';
    case MAP_TREASURE:return '★';
    case MAP_EVENT:   return '?';
    case MAP_ENTRANCE:return '↕';
    case MAP_STAIR:   return '⇅';
    case MAP_NPC:     return '☻';
    case MAP_MONSTER: return '☠';
    case MAP_SHOP:    return '$';
    default:          return '?';
    }
}

const char* getElementName(MapElement element) {
    switch (element) {
    case MAP_EMPTY:   return "空地";
    case MAP_WALL:    return "墙";
    case MAP_TREE:    return "树";
    case MAP_RIVER:   return "河";
    case MAP_GRASS:   return "草";
    case MAP_PATH:    return "路";
    case MAP_TREASURE:return "宝藏";
    case MAP_EVENT:   return "事件";
    case MAP_ENTRANCE:return "入口";
    case MAP_STAIR:   return "楼梯";
    case MAP_NPC:     return "NPC";
    case MAP_MONSTER: return "怪物";
    case MAP_SHOP:    return "商店";
    default:          return "未知";
    }
}

// ==================== 地图生成函数 ====================

void generateForestMap(MultiLayerMap* map) {
    strcpy(map->mapName, "魔法森林");
    strcpy(map->mapDescription, "充满神秘生物的古老森林，据说深处藏有宝藏");
    map->difficulty = 2;
    map->totalLayers = 3;
    map->startLayer = 0;
    map->startX = 5;
    map->startY = 5;
    map->isUnlocked = true;
    map->unlockLevel = 1;

    srand(time(NULL));

    // 层0：森林地表
    strcpy(map->layers[0].layerName, "森林地表");
    map->layers[0].layerNumber = 0;

    // 生成基础地形
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            MapCell* cell = &map->layers[0].cells[y][x];

            // 边界墙
            if (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1) {
                cell->element = MAP_WALL;
            }
            else {
                // 随机地形
                int r = rand() % 100;
                if (r < 15) {
                    cell->element = MAP_TREE;
                    strcpy(cell->description, "茂密的树木");
                }
                else if (r < 25) {
                    cell->element = MAP_GRASS;
                    strcpy(cell->description, "柔软的草地");
                }
                else if (r < 30 && x > 5 && x < MAP_WIDTH - 6) {
                    cell->element = MAP_RIVER;
                    strcpy(cell->description, "清澈的小河");
                }
                else {
                    cell->element = MAP_PATH;
                    strcpy(cell->description, "林间小径");
                }
            }

            cell->isExplored = false;
            cell->isTriggered = false;
            cell->isOpened = false;
            cell->eventType = MAP_EVENT_NONE;
        }
    }

    // 设置关键地点
    // 入口
    map->layers[0].cells[5][5].element = MAP_ENTRANCE;
    strcpy(map->layers[0].cells[5][5].description, "森林入口");

    // 宝藏1
    map->layers[0].cells[8][12].element = MAP_TREASURE;
    map->layers[0].cells[8][12].treasureType = TREASURE_COINS;
    map->layers[0].cells[8][12].treasureValue = 100;
    strcpy(map->layers[0].cells[8][12].description, "精灵的宝箱");

    // 宝藏2
    map->layers[0].cells[15][8].element = MAP_TREASURE;
    map->layers[0].cells[15][8].treasureType = TREASURE_ITEM;
    map->layers[0].cells[15][8].itemId = 101; // 治疗药水
    strcpy(map->layers[0].cells[15][8].description, "古老的宝箱");

    // 治疗事件
    map->layers[0].cells[10][20].element = MAP_EVENT;
    map->layers[0].cells[10][20].eventType = MAP_EVENT_HEAL;
    map->layers[0].cells[10][20].eventValue = 30;
    strcpy(map->layers[0].cells[10][20].description, "治疗泉水");

    // 陷阱事件
    map->layers[0].cells[12][5].element = MAP_EVENT;
    map->layers[0].cells[12][5].eventType = MAP_EVENT_TRAP;
    map->layers[0].cells[12][5].eventValue = 20;
    strcpy(map->layers[0].cells[12][5].description, "危险的陷阱");

    // NPC
    map->layers[0].cells[7][25].element = MAP_NPC;
    map->layers[0].cells[7][25].eventType = MAP_EVENT_QUEST;
    strcpy(map->layers[0].cells[7][25].description, "森林守护者");

    // 商店
    map->layers[0].cells[3][15].element = MAP_SHOP;
    strcpy(map->layers[0].cells[3][15].description, "旅行商人");

    // 怪物
    map->layers[0].cells[10][10].element = MAP_MONSTER;
    strcpy(map->layers[0].cells[10][10].description, "森林狼");

    // 下楼楼梯
    map->layers[0].cells[18][5].element = MAP_STAIR;
    strcpy(map->layers[0].cells[18][5].description, "通往地下洞穴");

    // 层1：地下洞穴
    strcpy(map->layers[1].layerName, "地下洞穴");
    map->layers[1].layerNumber = 1;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            MapCell* cell = &map->layers[1].cells[y][x];

            if (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1) {
                cell->element = MAP_WALL;
            }
            else {
                int r = rand() % 100;
                if (r < 40) {
                    cell->element = MAP_WALL;
                    strcpy(cell->description, "岩石墙");
                }
                else if (r < 50) {
                    cell->element = MAP_RIVER;
                    strcpy(cell->description, "地下河");
                }
                else {
                    cell->element = MAP_PATH;
                    strcpy(cell->description, "洞穴通道");
                }
            }

            cell->isExplored = false;
            cell->isTriggered = false;
            cell->isOpened = false;
            cell->eventType = MAP_EVENT_NONE;
        }
    }

    // 层1关键地点
    // 上楼楼梯
    map->layers[1].cells[3][3].element = MAP_STAIR;
    strcpy(map->layers[1].cells[3][3].description, "返回地表");

    // 宝藏
    map->layers[1].cells[12][20].element = MAP_TREASURE;
    map->layers[1].cells[12][20].treasureType = TREASURE_RELIC;
    map->layers[1].cells[12][20].itemId = 1001; // C级遗物
    strcpy(map->layers[1].cells[12][20].description, "矿工宝藏");

    // 谜题事件
    map->layers[1].cells[8][15].element = MAP_EVENT;
    map->layers[1].cells[8][15].eventType = MAP_EVENT_RIDDLE;
    strcpy(map->layers[1].cells[8][15].description, "古代石碑");

    // 下楼楼梯
    map->layers[1].cells[18][10].element = MAP_STAIR;
    strcpy(map->layers[1].cells[18][10].description, "通往深层");

    // 层2：古代遗迹
    strcpy(map->layers[2].layerName, "古代遗迹");
    map->layers[2].layerNumber = 2;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            MapCell* cell = &map->layers[2].cells[y][x];

            if (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1) {
                cell->element = MAP_WALL;
                strcpy(cell->description, "遗迹外墙");
            }
            else if ((x % 4 == 0 && y % 4 == 0) ||
                (x == MAP_WIDTH / 2 || y == MAP_HEIGHT / 2)) {
                cell->element = MAP_PATH;
                strcpy(cell->description, "遗迹走廊");
            }
            else {
                cell->element = MAP_GRASS;
                strcpy(cell->description, "遗迹草地");
            }

            cell->isExplored = false;
            cell->isTriggered = false;
            cell->isOpened = false;
            cell->eventType = MAP_EVENT_NONE;
        }
    }

    // 层2关键地点
    // 上楼楼梯
    map->layers[2].cells[2][2].element = MAP_STAIR;
    strcpy(map->layers[2].cells[2][2].description, "返回洞穴");

    // 最终宝藏
    map->layers[2].cells[15][25].element = MAP_TREASURE;
    map->layers[2].cells[15][25].treasureType = TREASURE_RELIC;
    map->layers[2].cells[15][25].itemId = 1003; // A级遗物
    strcpy(map->layers[2].cells[15][25].description, "古代神器");

    // BOSS怪物
    map->layers[2].cells[10][15].element = MAP_MONSTER;
    strcpy(map->layers[2].cells[10][15].description, "遗迹守护者");
}

void generateCaveMap(MultiLayerMap* map) {
    strcpy(map->mapName, "火山洞穴");
    strcpy(map->mapDescription, "炽热的火山洞穴，充满危险和稀有矿物");
    map->difficulty = 4;
    map->totalLayers = 2;
    map->startLayer = 0;
    map->startX = 3;
    map->startY = 3;
    map->isUnlocked = false;
    map->unlockLevel = 5;

    // 简化的生成逻辑
    // 实际实现时可以参考森林地图的生成方式
}

void generateRuinsMap(MultiLayerMap* map) {
    strcpy(map->mapName, "冰封遗迹");
    strcpy(map->mapDescription, "被冰雪覆盖的古代遗迹，隐藏着失传的秘密");
    map->difficulty = 6;
    map->totalLayers = 3;
    map->startLayer = 0;
    map->startX = 2;
    map->startY = 2;
    map->isUnlocked = false;
    map->unlockLevel = 8;
}

// ==================== 地图系统初始化 ====================

void initMapSystem(MapSystem* map, Player* player, Backpack* backpack) {
    if (!map || !player || !backpack) return;

    // 初始化基本数据
    map->player = player;
    map->backpack = backpack;
    map->totalMaps = 3;
    map->selectedMapIndex = 0;
    map->isSelectingMap = true;
    map->currentMapIndex = -1;
    map->currentLayer = 0;

    map->treasuresFound = 0;
    map->eventsTriggered = 0;
    map->monstersDefeated = 0;

    // 生成所有地图
    generateForestMap(&map->maps[0]);
    generateCaveMap(&map->maps[1]);
    generateRuinsMap(&map->maps[2]);
}

void cleanupMapSystem(MapSystem* map) {
    // 目前没有动态分配的内存需要清理
    if (map) {
        // 可以添加清理代码
    }
}

// ==================== 地图选择界面 ====================

void displayMapSelection(MapSystem* map) {
    clearScreen();

    setColor(COLOR_TITLE);
    printf("═══════════════════════════════════════════\n");
    printf("         选择冒险地图\n");
    printf("═══════════════════════════════════════════\n\n");
    setColor(COLOR_NORMAL);

    // 显示玩家信息
    if (map->player) {
        setColor(COLOR_INFO);
        printf("玩家: %s | 等级: %d | HP: %d/%d\n\n",
            map->player->name,
            map->player->level,
            map->player->hp,
            map->player->maxHp);
        setColor(COLOR_NORMAL);
    }

    // 显示所有地图
    for (int i = 0; i < map->totalMaps; i++) {
        MultiLayerMap* currentMap = &map->maps[i];

        // 高亮显示选中的地图
        if (map->selectedMapIndex == i) {
            setColor(COLOR_HIGHLIGHT);
            printf("▶ ");
        }
        else {
            setColor(COLOR_NORMAL);
            printf("  ");
        }

        // 显示地图名称和状态
        printf("%s", currentMap->mapName);

        if (!currentMap->isUnlocked) {
            setColor(COLOR_WARNING);
            printf(" [🔒 需等级%d]", currentMap->unlockLevel);
        }

        printf("\n");

        // 显示地图信息
        setColor(COLOR_INFO);
        printf("  难度: ");
        for (int star = 0; star < 10; star++) {
            if (star < currentMap->difficulty) printf("★");
            else 
                printf ("☆");
        }
        printf(" | 层数: %d\n", currentMap->totalLayers);
        printf("  %s\n\n", currentMap->mapDescription);
        setColor(COLOR_NORMAL);
    }

    // 显示操作说明
    setColor(COLOR_TITLE);
    printf("═══════════════════════════════════════════\n");
    setColor(COLOR_NORMAL);
    printf("🎮 操作说明:\n");
    printf("W/S - 上下选择地图\n");
    printf("E   - 进入选中的地图\n");
    printf("I   - 查看玩家信息\n");
    printf("Q   - 返回主菜单\n");

    // 显示选中地图的详细信息
    MultiLayerMap* selected = &map->maps[map->selectedMapIndex];
    setColor(COLOR_TITLE);
    printf("═══════════════════════════════════════════\n");
    setColor(COLOR_HIGHLIGHT);
    printf("📋 选中地图详情:\n");
    setColor(COLOR_NORMAL);
    printf("名称: %s\n", selected->mapName);
    printf("难度: %d/10\n", selected->difficulty);
    printf("层数: %d\n", selected->totalLayers);

    // 显示各层名称
    printf("层结构: ");
    for (int i = 0; i < selected->totalLayers; i++) {
        printf("%s", selected->layers[i].layerName);
        if (i < selected->totalLayers - 1) printf(" → ");
    }
    printf("\n");

    printf("起始位置: 层%d (%d, %d)\n",
        selected->startLayer + 1,
        selected->startX,
        selected->startY);

    printf("状态: ");
    if (selected->isUnlocked) {
        setColor(COLOR_SUCCESS);
        printf("✅ 已解锁\n");
    }
    else {
        setColor(COLOR_WARNING);
        printf("🔒 未解锁 (需要等级 %d)\n", selected->unlockLevel);
    }
    setColor(COLOR_NORMAL);
    setColor(COLOR_TITLE);
    printf("═══════════════════════════════════════════\n");
}
int handleMapSelectionInput(MapSystem* map) {
    int ch = _getch();
    switch (ch) {
    case 'w':
    case 'W':
        map->selectedMapIndex--;
        if (map->selectedMapIndex < 0) {
            map->selectedMapIndex = map->totalMaps - 1;
        }
        return 1; // 继续显示选择界面

    case 's':
    case 'S':
        map->selectedMapIndex = (map->selectedMapIndex + 1) % map->totalMaps;
        return 1;

    case 'e':
    case 'E': {
        MultiLayerMap* selected = &map->maps[map->selectedMapIndex];

        // 检查是否解锁
        if (!selected->isUnlocked) {
            setColor(COLOR_WARNING);
            printf("\n地图未解锁！\n");
            printf("需要达到等级 %d 才能进入\n", selected->unlockLevel);
            if (map->player) {
                printf("当前等级: %d\n", map->player->level);
            }
            setColor(COLOR_NORMAL);
            pressAnyKey();
            return 1;
        }

        // 检查玩家等级
        if (map->player && map->player->level < selected->difficulty) {
            setColor(COLOR_WARNING);
            printf("\n等级不足！\n");
            printf("建议等级: %d，当前等级: %d\n",
                selected->difficulty, map->player->level);
            printf("仍然要进入吗？(Y/N): ");

            int confirm = getch();
            if (confirm != 'y' && confirm != 'Y') {
                setColor(COLOR_NORMAL);
                return 1;
            }
        }

        // 进入地图
        map->currentMapIndex = map->selectedMapIndex;
        map->currentLayer = selected->startLayer;
        map->isSelectingMap = false;

        // 设置玩家起始位置
        map->player->x = selected->startX;
        map->player->y = selected->startY;
        map->player->layer = selected->startLayer;

        setColor(COLOR_SUCCESS);
        printf("\n进入 %s...\n", selected->mapName);
        printf("祝你好运，冒险者！\n");
        setColor(COLOR_NORMAL);
        pressAnyKey();

        return 2; // 开始游戏

    case 'i':
    case 'I':
        if (map->player) {
            clearScreen();
            displayPlayerInfo(map->player);
            pressAnyKey();
        }
        return 1;

    case 'q':
    case 'Q':
        return 0; // 返回主菜单

    default:
        return 1;
    }
    }
}
void runMapSelection(MapSystem * map) {
    if (!map) return;

    map->isSelectingMap = true;
    int state = 1; // 1:选择界面, 2:开始游戏, 0:返回

    while (state == 1) {
        displayMapSelection(map);
        state = handleMapSelectionInput(map);
    }

    if (state == 2) {
        // 开始地图探索
        runMapExploration(map);
    }
}

// ==================== 地图探索系统 ====================

MapCell* getCurrentCell(MapSystem* map) {
    if (!map || map->currentMapIndex < 0) return NULL;
    if (map->currentLayer < 0 || map->currentLayer >= MAX_LAYERS) return NULL;
    if (map->player->x < 0 || map->player->x >= MAP_WIDTH) return NULL;
    if (map->player->y < 0 || map->player->y >= MAP_HEIGHT) return NULL;

    MultiLayerMap* currentMap = &map->maps[map->currentMapIndex];
    return &currentMap->layers[map->currentLayer].cells[map->player->y][map->player->x];
}

bool canMoveTo(MapSystem* map, int x, int y, int layer) {
    if (!map || map->currentMapIndex < 0) 
        return false;
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) 
        return false;
    if (layer < 0 || layer >= MAX_LAYERS) 
        return false;

    MultiLayerMap* currentMap = &map->maps[map->currentMapIndex];
    MapCell* targetCell = &currentMap->layers[layer].cells[y][x];

    // 检查元素是否可通过
    switch (targetCell->element) {
    case MAP_EMPTY:
    case MAP_GRASS:
    case MAP_PATH:
    case MAP_EVENT:
    case MAP_TREASURE:
    case MAP_ENTRANCE:
    case MAP_STAIR:
    case MAP_NPC:
    case MAP_SHOP:
        return true;
    case MAP_WALL:
    case MAP_TREE:
    case MAP_RIVER:
    case MAP_MONSTER:
        return false;
    default:
        return true;
    }
}

void displayMapView(MapSystem* map) {
    if (!map || map->currentMapIndex < 0) 
        return;

    MultiLayerMap* currentMap = &map->maps[map->currentMapIndex];
    MapLayer* currentLayer = &currentMap->layers[map->currentLayer];

    // 计算视野窗口
    int viewStartX = map->player->x - VIEW_WIDTH / 2;
    int viewStartY = map->player->y - VIEW_HEIGHT / 2;

    // 确保视野不超出边界
    if (viewStartX < 0) viewStartX = 0;
    if (viewStartY < 0) viewStartY = 0;
    if (viewStartX + VIEW_WIDTH > MAP_WIDTH) viewStartX = MAP_WIDTH - VIEW_WIDTH;
    if (viewStartY + VIEW_HEIGHT > MAP_HEIGHT) viewStartY = MAP_HEIGHT - VIEW_HEIGHT;

    // 显示地图信息
    setColor(COLOR_TITLE);
    printf("═══════════════════════════════════════════\n");
    printf("%s - %s\n", currentMap->mapName, currentLayer->layerName);
    printf("层数: %d/%d | 坐标: (%d, %d)\n",map->currentLayer + 1, currentMap->totalLayers, map->player->x, map->player->y);
    printf("═══════════════════════════════════════════\n\n");

    // 显示视野窗口
    for (int y = viewStartY; y < viewStartY + VIEW_HEIGHT; y++) {
        printf("  ");
        for (int x = viewStartX; x < viewStartX + VIEW_WIDTH; x++) {
            // 检查是否在地图范围内
            if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                MapCell* cell = &currentLayer->cells[y][x];

                // 检查是否是玩家位置
                if (x == map->player->x && y == map->player->y) {
                    setColor(COLOR_PLAYER);
                    printf("@");  // 玩家
                    setColor(COLOR_NORMAL);
                }
                else if (cell->isExplored) {
                    // 已探索的单元格
                    switch (cell->element) {
                    case MAP_WALL:   setColor(COLOR_WALL); break;
                    case MAP_TREE:   setColor(COLOR_TREE); break;
                    case MAP_RIVER:  setColor(COLOR_RIVER); break;
                    case MAP_TREASURE: setColor(COLOR_TREASURE); break;
                    case MAP_EVENT:  setColor(COLOR_EVENT); break;
                    case MAP_MONSTER:setColor(COLOR_MONSTER); break;
                    case MAP_SHOP:   setColor(COLOR_SHOP); break;
                    default:         setColor(COLOR_NORMAL);
                    }

                    printf("%c", getElementChar(cell->element));
                    setColor(COLOR_NORMAL);
                }
                else {
                    // 未探索的单元格
                    printf(" ");
                }
            }
            else {
                // 边界外
                setColor(COLOR_WALL);
                printf("#");
                setColor(COLOR_NORMAL);
            }
            printf(" "); // 单元格间距
        }
        printf("\n");
    }
    printf("\n");
}

void displayPlayerStatus(MapSystem* map) {
    if (!map || !map->player) return;

    setColor(COLOR_TITLE);
    printf("═══════════════════════════════════════════\n");
    setColor(COLOR_HIGHLIGHT);
    printf("👤 玩家状态\n");
    setColor(COLOR_NORMAL);

    printf("名称: %s\n", map->player->name);
    printf("等级: %d | 经验: %d/%d\n",
        map->player->level, map->player->exp, map->player->maxExp);

    // HP条
    printf("HP: ");
    int hpBars = (map->player->hp * 20) / map->player->maxHp;
    setColor(COLOR_WARNING);
    for (int i = 0; i < 20; i++) {
        if (i < hpBars) 
            printf("█");
        else 
            printf("░");
    }
    setColor(COLOR_NORMAL);
    printf(" %d/%d\n", map->player->hp, map->player->maxHp);

    if (map->backpack) {
        printf("金币: %d\n", map->backpack->money);
    }

    // 当前位置信息
    MapCell* currentCell = getCurrentCell(map);
    if (currentCell && currentCell->isExplored) {
        printf("位置: %s", getElementName(currentCell->element));
        if (strlen(currentCell->description) > 0) {
            printf(" - %s", currentCell->description);
        }
        printf("\n");
    }

    setColor(COLOR_TITLE);
    printf("═══════════════════════════════════════════\n");
}

void displayControls(MapSystem* map) {
    printf("🎮 操作说明:\n");
    printf("W/A/S/D - 移动 (上/左/下/右)\n");
    printf("E       - 与当前位置交互\n");
    printf("U/J     - 上/下一层\n");
    printf("I       - 查看详细信息\n");
    printf("B       - 打开背包\n");
    printf("M       - 地图菜单\n");
    printf("Q       - 返回地图选择\n");

    // 显示游戏统计
    setColor(COLOR_INFO);
    printf("\n📊 本局统计:\n");
    printf("宝藏发现: %d | 事件触发: %d | 怪物击败: %d\n",
        map->treasuresFound, map->eventsTriggered, map->monstersDefeated);
    setColor(COLOR_NORMAL);
    printf("═══════════════════════════════════════════\n");
}

bool movePlayerInMap(MapSystem* map, int dx, int dy) {
    if (!map || !map->player) return false;

    int newX = map->player->x + dx;
    int newY = map->player->y + dy;

    // 检查是否可以移动
    if (!canMoveTo(map, newX, newY, map->currentLayer)) {
        return false;
    }

    // 移动玩家
    map->player->x = newX;
    map->player->y = newY;
    map->player->stats.totalSteps++;

    // 更新方向
    if (dx > 0) map->player->direction = 1; // 右
    else if (dx < 0) map->player->direction = 3; // 左
    else if (dy > 0) map->player->direction = 2; // 下
    else if (dy < 0) map->player->direction = 0; // 上

    // 标记当前位置为已探索
    MapCell* currentCell = getCurrentCell(map);
    if (currentCell) {
        currentCell->isExplored = true;
    }

    return true;
}

void changeLayer(MapSystem* map, int newLayer) {
    if (!map || map->currentMapIndex < 0) return;

    MultiLayerMap* currentMap = &map->maps[map->currentMapIndex];

    // 检查层数是否有效
    if (newLayer < 0 || newLayer >= currentMap->totalLayers) {
        return;
    }

    // 检查当前位置是否有楼梯
    MapCell* currentCell = getCurrentCell(map);
    if (currentCell && currentCell->element != MAP_STAIR) {
        printf("需要找到楼梯才能切换层数！\n");
        pressAnyKey();
        return;
    }

    // 切换层数
    int oldLayer = map->currentLayer;
    map->currentLayer = newLayer;
    map->player->layer = newLayer;

    // 设置在新层的起始位置（简单实现：放在楼梯附近）
    map->player->x = 3;
    map->player->y = 3;

    printf("从%s前往%s...\n",
        currentMap->layers[oldLayer].layerName,
        currentMap->layers[newLayer].layerName);
    pressAnyKey();
}

void triggerEvent(MapSystem* map, MapCell* cell) {
    if (!map || !cell || cell->isTriggered) return;

    clearScreen();
    setColor(COLOR_TITLE);
    printf("═══════════════════════════════════════════\n");
    printf("🎭 事件触发\n");
    printf("═══════════════════════════════════════════\n");
    setColor(COLOR_NORMAL);

    printf("%s\n\n", cell->description);

    switch (cell->eventType) {
    case MAP_EVENT_HEAL:
        if (map->player) {
            int oldHp = map->player->hp;
            heal(map->player, cell->eventValue);
            printf("你喝下了治疗泉水，恢复了 %d 点生命值！\n",
                map->player->hp - oldHp);
        }
        break;

    case MAP_EVENT_TRAP:
        if (map->player) {
            int oldHp = map->player->hp;
            damage(map->player, cell->eventValue);
            printf("你触发了陷阱，受到了 %d 点伤害！\n",
                oldHp - map->player->hp);
        }
        break;

    case MAP_EVENT_RIDDLE:
        printf("石碑上刻着古老的文字：\n");
        printf("\"我行走，却没有腿；我说话，却没有嘴。我是什么？\"\n\n");
        printf("1. 河流  2. 风  3. 时间  4. 影子\n");
        printf("你的选择 (1-4): ");

        int answer = getch() - '0';
        if (answer == 2) {
            printf("\n✅ 回答正确！风无形无体却能行走和发声。\n");
            if (map->player) {
                gainExp(map->player, 50);
                printf("获得 50 经验值！\n");
            }
        }
        else {
            printf("\n❌ 回答错误！正确答案是：风\n");
        }
        break;

    case MAP_EVENT_FIND_ITEM:
        printf("你发现了一些有用的物品！\n");
        // 这里可以添加物品获取逻辑
        break;

    case MAP_EVENT_FIND_GOLD:
        if (map->backpack) {
            int gold = 20 + rand() % 81; // 20-100金币
            map->backpack->money += gold;
            printf("发现了 %d 金币！\n", gold);
            printf("当前金币: %d\n", map->backpack->money);
        }
        break;

    case MAP_EVENT_QUEST:
        printf("NPC: \"欢迎，冒险者！\"\n");
        printf("NPC: \"这片森林需要帮助，你能找到三件宝物吗？\"\n");
        printf("任务：寻找森林中的三件宝物\n");
        printf("进度：%d/3\n", map->treasuresFound);
        break;

    default:
        printf("发生了某些神秘的事件...\n");
        break;
    }

    cell->isTriggered = true;
    map->eventsTriggered++;

    setColor(COLOR_TITLE);
    printf("═══════════════════════════════════════════\n");
    setColor(COLOR_NORMAL);
    pressAnyKey();
}
//待完善
void openTreasure(MapSystem* map, MapCell* cell) {
    if (!map || !cell || cell->isOpened) return;

    clearScreen();
    setColor(COLOR_TITLE);
    printf("═══════════════════════════════════════════\n");
    printf("💎 打开宝箱\n");
    printf("═══════════════════════════════════════════\n");
    setColor(COLOR_NORMAL);

    printf("%s\n\n", cell->description);

    srand(time(NULL));

    switch (cell->treasureType) {
    case TREASURE_COINS:
        if (map->backpack) {
            int coins = cell->treasureValue > 0 ? cell->treasureValue : (50 + rand() % 151);
            map->backpack->money += coins;
            printf("发现 %d 金币！\n", coins);
            printf("当前金币: %d\n", map->backpack->money);
        }
        break;

    case TREASURE_ITEM:
        printf("获得了一件神秘物品！\n");
		addItem(map->backpack, getRandomItem(ITEM_TOOL,cell->itemId));
        break;

    case TREASURE_RELIC:
        printf("发现了古代遗物！\n");
		addItem(map->backpack, getRandomItem(ITEM_RELIC,cell->itemId));
        break;

    case TREASURE_KEY:
        printf("获得了一把神秘的钥匙！\n");
        printf("这把钥匙可能能打开某些特殊的门...\n");
        break;

    default:
        printf("宝箱是空的...\n");
        break;
    }

    cell->isOpened = true;
    cell->element = MAP_PATH; // 打开后变为路径
    map->treasuresFound++;
    map->player->stats.treasuresFound++;

    setColor(COLOR_TITLE);
    printf("═══════════════════════════════════════════\n");
    setColor(COLOR_NORMAL);
    pressAnyKey();
}
//交互，后期修改
void interactCurrentCell(MapSystem* map) {
    MapCell* currentCell = getCurrentCell(map);
    if (!currentCell) return;

    clearScreen();

    if (!currentCell->isExplored) {
        printf("这里一片漆黑，什么也看不清...\n");
        pressAnyKey();
        return;
    }

    setColor(COLOR_TITLE);
    printf("═══════════════════════════════════════════\n");
    printf("📍 当前位置交互\n");
    printf("═══════════════════════════════════════════\n");
    setColor(COLOR_NORMAL);

    printf("位置: (%d, %d) 层%d\n",
        map->player->x, map->player->y, map->currentLayer + 1);
    printf("地形: %s\n", getElementName(currentCell->element));

    if (strlen(currentCell->description) > 0) {
        printf("描述: %s\n", currentCell->description);
    }

    printf("\n");

    switch (currentCell->element) {
    case MAP_TREASURE:
        if (!currentCell->isOpened) {
            printf("这是一个宝箱，要打开吗？ (Y/N)\n");
            int ch = _getch();
            if (ch == 'y' || ch == 'Y') {
                openTreasure(map, currentCell);
            }
        }
        else {
            printf("这个宝箱已经被打开过了。\n");
            pressAnyKey();
        }
        break;

    case MAP_EVENT:
        if (!currentCell->isTriggered) {
            printf("发现了一个事件，要触发吗？ (Y/N)\n");
            int ch = _getch();
            if (ch == 'y' || ch == 'Y') {
                triggerEvent(map, currentCell);
            }
        }
        else {
            printf("这个事件已经触发过了。\n");
            pressAnyKey();
        }
        break;

    case MAP_NPC:
        printf("要与NPC对话吗？ (Y/N)\n");
        int ch = getch();
        if (ch == 'y' || ch == 'Y') {
            triggerEvent(map, currentCell);
        }
        break;

    case MAP_SHOP:
        printf("要进入商店吗？ (Y/N)\n");
        ch = getch();
        if (ch == 'y' || ch == 'Y') {
            shop();
            printf("商店系统需要集成...\n");
            pressAnyKey();
        }
        break;

    case MAP_MONSTER:
        printf("发现怪物！要战斗吗？ (Y/N)\n");
        ch = getch();
        if (ch == 'y' || ch == 'Y') {
            // 简单的战斗模拟
            printf("\n⚔️ 战斗开始！\n");

            // 计算伤害
            int playerDamage = map->player->strength / 2 + rand() % 6;
            int monsterDamage = 10 + rand() % 11; // 10-20伤害

            printf("你对怪物造成了 %d 点伤害！\n", playerDamage);
            printf("怪物对你造成了 %d 点伤害！\n", monsterDamage);

            damage(map->player, monsterDamage);

            // 击败怪物
            currentCell->element = MAP_PATH;
            map->monstersDefeated++;
            map->player->stats.monstersDefeated++;

            // 获得经验
            gainExp(map->player, 30);

            printf("\n✅ 成功击败了怪物！\n");
            printf("获得 30 经验值！\n");
        }
        pressAnyKey();
        break;

    case MAP_STAIR:
        printf("这是一个楼梯，要使用吗？ (Y/N)\n");
        printf("U - 上楼  |  J - 下楼\n");
        ch = getch();
        if (ch == 'u' || ch == 'U') {
            changeLayer(map, map->currentLayer - 1);
        }
        else if (ch == 'j' || ch == 'J') {
            changeLayer(map, map->currentLayer + 1);
        }
        break;

    default:
        printf("这里没有什么特别的东西。\n");
        pressAnyKey();
        break;
    }
}
//待完善
void runMapExploration(MapSystem* map) {
    if (!map || !map->player || map->currentMapIndex < 0) return;

    // 确保玩家状态正常
    if (!isPlayerAlive(map->player)) {
        printf("玩家已死亡，无法继续探索！\n");
        pressAnyKey();
        return;
    }

    int running = 1;

    while (running && isPlayerAlive(map->player)) {
        clearScreen();

        // 显示地图
        displayMapView(map);

        // 显示玩家状态
        displayPlayerStatus(map);

        // 显示操作说明
        displayControls(map);

        // 处理输入
        int ch = getch();

        switch (ch) {
        case 'w': case 'W': movePlayerInMap(map, 0, -1); break;
        case 'a': case 'A': movePlayerInMap(map, -1, 0); break;
        case 's': case 'S': movePlayerInMap(map, 0, 1); break;
        case 'd': case 'D': movePlayerInMap(map, 1, 0); break;

        case 'e': case 'E':
            interactCurrentCell(map);
            break;

        case 'u': case 'U':
            changeLayer(map, map->currentLayer - 1);
            break;

        case 'j': case 'J':
            changeLayer(map, map->currentLayer + 1);
            break;

        case 'i': case 'I':
            clearScreen();
            displayPlayerInfo(map->player);
            displayPlayerStats(map->player);
            pressAnyKey();
            break;

        case 'b': case 'B':
            backpack();
            pressAnyKey();
            break;

        case 'm': case 'M':
            printf("地图菜单：\n");
            printf("1. 返回地图选择\n");
            printf("2. 查看完整地图\n");
            printf("3. 保存游戏\n");
            printf("选择: ");

            int menuChoice = _getch();
            if (menuChoice == '1') {
                running = 0;
            }
            else if (menuChoice == '2') {
                printf("\n完整地图功能待实现...\n");
                pressAnyKey();
            }
            else if (menuChoice == '3') {
                printf("\n保存游戏功能待实现...\n");
                pressAnyKey();
            }
            break;

        case 'q': case 'Q':
            printf("确定要返回地图选择吗？(Y/N): ");
            int confirm = _getch();
            if (confirm == 'y' || confirm == 'Y') {
                running = 0;
            }
            break;
        }

        // 更新玩家状态
        updatePlayerStatus(map->player);
    }

    // 返回地图选择界面
    map->isSelectingMap = true;

    if (!isPlayerAlive(map->player)) {
        setColor(COLOR_WARNING);
        printf("\n💀 玩家已死亡！游戏结束。\n");
        setColor(COLOR_NORMAL);

        // 重置玩家状态
        map->player->hp = map->player->maxHp;
        map->player->status = PLAYER_NORMAL;

        pressAnyKey();
    }
}
    