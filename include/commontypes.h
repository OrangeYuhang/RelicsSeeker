#ifndef COMMONTYPES_H
#define COMMONTYPES_H


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

#endif // COMMONTYPES_H
