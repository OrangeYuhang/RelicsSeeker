#include "../include/player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// 检查玩家数据文件是否存在
bool playerDataExists(void) {
    FILE* fp = fopen(PLAYER_DATA_FILE, "rb");
    if (fp) {
        fclose(fp);
        return true;
    }
    return false;
}

// 创建新玩家（不保存到文件）
Player* createNewPlayer(const char* name, void* backpack) {
    Player* player = (Player*)malloc(sizeof(Player));
    if (!player) return NULL;

    // 初始化基本信息
    player->id = 1;
    strncpy_s(player->name, sizeof(player->name),name, sizeof(player->name) - 1);
    player->name[sizeof(player->name) - 1] = '\0';

    player->level = 1;
    player->exp = 0;
    player->maxExp = 100;

    // 初始化生命值
    player->maxHp = 100;
    player->hp = player->maxHp;
    player->maxMp = 50;
    player->mp = player->maxMp;

    // 初始化属性
    player->strength = 10;
    player->defense = 5;
    player->agility = 8;
    player->luck = 5;

    // 初始化状态
    player->status = PLAYER_NORMAL;
	player->stats.totalSteps = 0;
	player->stats.treasuresFound = 0;
	player->stats.eventsTriggered = 0;
	player->stats.monstersDefeated = 0;

    // 初始化位置
    player->x = 1;
    player->y = 1;
    player->layer = 0;
    player->direction = 2; // 向下

    // 设置背包指针
    player->backpack = backpack;

    // 初始化统计
    initPlayerStats(player);

    return player;
}

// 保存玩家数据到文件
bool savePlayerToFile(Player* player) {
    if (!player) return false;

    FILE* fp = fopen(PLAYER_DATA_FILE, "wb");
    if (!fp) {
        printf("无法创建玩家数据文件！\n");
        return false;
    }

    // 保存玩家数据
    size_t written = fwrite(player, sizeof(Player), 1, fp);
    fclose(fp);

    return written == 1;
}

// 从文件加载玩家数据
bool loadPlayerFromFile(Player* player) {
    if (!player) return false;

    FILE* fp = fopen(PLAYER_DATA_FILE, "rb");
    if (!fp) {
        return false;
    }

    size_t read = fread(player, sizeof(Player), 1, fp);
    fclose(fp);

    return read == 1;
}

// 加载或创建玩家（主入口函数）
Player* loadOrCreatePlayer(void* backpack) {
    Player* player = NULL;

    if (playerDataExists()) {
        // 已有存档，加载玩家数据
        player = (Player*)malloc(sizeof(Player));
        if (!player) return NULL;

        if (loadPlayerFromFile(player)) {
            printf("🎮 欢迎回来，%s！\n", player->name);

            // 恢复背包指针
            player->backpack = backpack;

            // 显示加载的信息
            printf("等级: %d | 经验: %d/%d\n",
                player->level, player->exp, player->maxExp);
            printf("HP: %d/%d\n", player->hp, player->maxHp);
            printf("位置: 层%d (%d, %d)\n",
                player->layer + 1, player->x, player->y);

            printf("按任意键继续...");
            getch();

            return player;
        }
        else {
            printf("读取玩家数据失败，创建新玩家...\n");
            free(player);
            // 继续创建新玩家
        }
    }

    // 创建新玩家
    clearScreen();
    printf("═══════════════════════════════════════════\n");
    printf("           创建新角色\n");
    printf("═══════════════════════════════════════════\n\n");

    char name[50];
    int nameValid = 0;

    while (!nameValid) {
        printf("请输入角色名字 (2-20个字符): ");

        if (fgets(name, sizeof(name), stdin) == NULL) {
            printf("输入错误！\n");
            continue;
        }

        // 移除换行符
        name[strcspn(name, "\n")] = '\0';

        // 检查名字长度
        size_t nameLen = strlen(name);
        if (nameLen < 2 || nameLen > 20) {
            printf("名字长度必须在2-20个字符之间！\n");
            continue;
        }

        // 检查名字是否包含非法字符
        int isValid = 1;
        for (size_t i = 0; i < nameLen; i++) {
            if (name[i] < 32 || name[i] > 126) {
                isValid = 0;
                break;
            }
        }

        if (!isValid) {
            printf("名字包含非法字符！\n");
            continue;
        }

        nameValid = 1;
    }

    // 选择职业（可选功能）
    printf("\n═══════════════════════════════════════════\n");
    printf("选择初始职业:\n");
    printf("1. 战士 - 高生命，高力量\n");
    printf("2. 法师 - 高魔法，高智力\n");
    printf("3. 游侠 - 高敏捷，高幸运\n");
    printf("═══════════════════════════════════════════\n");
    printf("选择 (1-3，直接回车选择战士): ");

    char classChoice[10];
    fgets(classChoice, sizeof(classChoice), stdin);

    // 创建玩家
    player = createNewPlayer(name, backpack);
    if (!player) {
        printf("创建玩家失败！\n");
        return NULL;
    }

    // 根据职业调整属性
    if (strlen(classChoice) > 0) {
        switch (classChoice[0]) {
        case '2': // 法师
            player->maxHp = 80;
            player->hp = 80;
            player->maxMp = 100;
            player->mp = 100;
            player->strength = 6;
            player->defense = 4;
            player->agility = 7;
            player->luck = 8;
            printf("\n选择了法师职业\n");
            break;

        case '3': // 游侠
            player->maxHp = 90;
            player->hp = 90;
            player->maxMp = 60;
            player->mp = 60;
            player->strength = 8;
            player->defense = 5;
            player->agility = 12;
            player->luck = 10;
            printf("\n选择了游侠职业\n");
            break;

        default: // 战士
            player->maxHp = 120;
            player->hp = 120;
            player->maxMp = 30;
            player->mp = 30;
            player->strength = 12;
            player->defense = 8;
            player->agility = 6;
            player->luck = 5;
            printf("\n选择了战士职业\n");
            break;
        }
    }

    // 保存新玩家数据
    if (savePlayerToFile(player)) {
        printf("✅ 角色创建成功并已保存！\n");
    }
    else {
        printf("⚠️  角色创建成功，但保存失败！\n");
    }

    printf("\n═══════════════════════════════════════════\n");
    printf("角色信息:\n");
    printf("名字: %s\n", player->name);
    printf("等级: %d\n", player->level);
    printf("HP: %d/%d\n", player->hp, player->maxHp);
    printf("力量: %d | 防御: %d | 敏捷: %d | 幸运: %d\n",
        player->strength, player->defense,
        player->agility, player->luck);
    printf("═══════════════════════════════════════════\n");

    printf("\n按任意键开始冒险...");
    getch();

    return player;
}

// 修改destroyPlayer函数，保存数据
void destroyPlayer(Player* player) {
    if (player) {
        // 退出前保存玩家数据
        savePlayerToFile(player);
        free(player);
    }
}

// 添加一个快速保存函数
void quickSavePlayer(Player* player) {
    if (player && savePlayerToFile(player)) {
        printf("✅ 游戏已保存！\n");
    }
    else {
        printf("❌ 保存失败！\n");
    }
}


// 初始化玩家统计
void initPlayerStats(Player* player) {
    if (!player) return;

    player->stats.totalSteps = 0;
    player->stats.treasuresFound = 0;
    player->stats.eventsTriggered = 0;
    player->stats.monstersDefeated = 0;
}

// 等级提升
void levelUp(Player* player) {
    if (!player) return;

    player->level++;
    player->exp -= player->maxExp;
    player->maxExp = player->level * 100;

    // 属性增长
    player->maxHp += 20;
    player->hp = player->maxHp; // 升级时恢复满血
    player->maxMp += 10;
    player->mp = player->maxMp;

    player->strength += 3;
    player->defense += 2;
    player->agility += 2;
    player->luck += 1;

    printf("\n🎉 等级提升！现在是 %d 级！\n", player->level);
    printf("最大HP: %d → %d\n", player->maxHp - 20, player->maxHp);
    printf("力量: %d → %d\n", player->strength - 3, player->strength);
    printf("防御: %d → %d\n", player->defense - 2, player->defense);
}

// 获得经验
void gainExp(Player* player, int amount) {
    if (!player || amount <= 0) return;

    player->exp += amount;
    printf("获得 %d 经验值！\n", amount);

    // 检查是否升级
    while (player->exp >= player->maxExp) {
        levelUp(player);
    }
}

// 治疗玩家
void heal(Player* player, int amount) {
    if (!player || amount <= 0) return;

    int oldHp = player->hp;
    player->hp += amount;
    if (player->hp > player->maxHp) {
        player->hp = player->maxHp;
    }

    printf("恢复 %d HP！(%d → %d)\n", amount, oldHp, player->hp);
}

// 伤害玩家
void damage(Player* player, int amount) {
    if (!player || amount <= 0) return;

    // 计算实际伤害（考虑防御）
    int actualDamage = amount - player->defense / 2;
    if (actualDamage < 1) {
        actualDamage = 1; // 至少造成1点伤害
    }

    player->hp -= actualDamage;
    printf("受到 %d 点伤害！\n", actualDamage);

    if (player->hp <= 0) {
        player->hp = 0;
        player->status = PLAYER_DEAD;
        printf("💀 玩家已死亡！\n");
    }
    else if (player->hp < player->maxHp * 0.3) {
        player->status = PLAYER_INJURED;
    }
}

// 检查玩家是否存活
bool isPlayerAlive(Player* player) {
    return player && player->hp > 0;
}

// 设置玩家状态
void setPlayerStatus(Player* player, PlayerStatus status) {
    if (!player) return;

    player->status = status;
}

// 获取状态字符串
const char* getStatusString(PlayerStatus status) {
    switch (status) {
    case PLAYER_NORMAL:   return "正常";
    case PLAYER_INJURED:  return "受伤";
    case PLAYER_POISONED: return "中毒";
    case PLAYER_BLESSED:  return "祝福";
    case PLAYER_DEAD:     return "死亡";
    default:              return "未知";
    }
}

// 更新玩家状态
void updatePlayerStatus(Player* player) {
    if (!player || player->status == PLAYER_DEAD) return;

    // 状态效果
    switch (player->status) {
    case PLAYER_POISONED:
        // 中毒每回合扣血
        player->hp -= 3;
        if (player->hp <= 0) {
            player->hp = 0;
            player->status = PLAYER_DEAD;
        }
        break;
    case PLAYER_INJURED:
        // 受伤状态可能影响移动
        break;
    case PLAYER_BLESSED:
        // 祝福状态可能提供增益
        break;
    default:
        break;
    }

    // 检查HP状态
    if (player->hp > player->maxHp * 0.3 && player->status == PLAYER_INJURED) {
        player->status = PLAYER_NORMAL;
    }
}

// 移动玩家
void movePlayerPosition(Player* player, int dx, int dy) {
    if (!player) return;

    player->x += dx;
    player->y += dy;
    player->stats.totalSteps++;

    // 更新方向
    if (dx > 0) player->direction = 1; // 右
    else if (dx < 0) player->direction = 3; // 左
    else if (dy > 0) player->direction = 2; // 下
    else if (dy < 0) player->direction = 0; // 上
}

// 设置玩家位置
void setPlayerPosition(Player* player, int x, int y, int layer) {
    if (!player) return;

    player->x = x;
    player->y = y;
    player->layer = layer;
}

// 改变玩家所在层
void changePlayerLayer(Player* player, int newLayer) {
    if (!player) return;

    player->layer = newLayer;
}

// 显示玩家信息
void displayPlayerInfo(Player* player) {
    if (!player) return;

    printf("═══════════════════════════════════════════\n");
    printf(" 玩家: %s\n", player->name);
    printf("等级: %d | 经验: %d/%d\n",
        player->level, player->exp, player->maxExp);

    // 显示HP条
    printf("HP: ");
    int hpBars = (player->hp * 20) / player->maxHp;
    for (int i = 0; i < 20; i++) {
        if (i < hpBars) {
            printf("█");
        }
        else {
            printf("░");
        }
    }
    printf(" %d/%d\n", player->hp, player->maxHp);

    // 显示MP条（如果有的话）
    if (player->maxMp > 0) {
        printf("MP: ");
        int mpBars = (player->mp * 10) / player->maxMp;
        for (int i = 0; i < 10; i++) {
            if (i < mpBars) {
                printf("▓");
            }
            else {
                printf("░");
            }
        }
        printf(" %d/%d\n", player->mp, player->maxMp);
    }

    printf("状态: %s\n", getStatusString(player->status));
    printf("位置: (%d, %d) 层%d\n",
        player->x, player->y, player->layer + 1);
    printf("═══════════════════════════════════════════\n");
}

// 显示玩家统计
void displayPlayerStats(Player* player) {
    if (!player) return;

    printf("═══════════════════════════════════════════\n");
    printf("📊 玩家统计\n");
    printf("═══════════════════════════════════════════\n");
    printf("总步数: %d\n", player->stats.totalSteps);
    printf("发现宝藏: %d\n", player->stats.treasuresFound);
    printf("触发事件: %d\n", player->stats.eventsTriggered);
    printf("击败怪物: %d\n", player->stats.monstersDefeated);

    printf("\n📈 属性:\n");
    printf("力量: %d (影响攻击力)\n", player->strength);
    printf("防御: %d (减少受到的伤害)\n", player->defense);
    printf("敏捷: %d (影响闪避和先攻)\n", player->agility);
    printf("幸运: %d (影响暴击和稀有掉落)\n", player->luck);
    printf("═══════════════════════════════════════════\n");
}
