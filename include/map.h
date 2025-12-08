#pragma once
#define MAX_MAP_HEIGHT 100	//最大地图高度
#define MAX_MAP_WIDTH 100	//最大地图宽度
#define MAX_WIN_HEIGHT 25	//最大窗口高度
#define MAX_WIN_WIDTH 25	//最大窗口宽度
#define CHECKPOINT 2	//关卡数
extern char mapData[CHECKPOINT][MAX_MAP_HEIGHT][MAX_MAP_WIDTH];//地图数据
extern char winData[MAX_WIN_HEIGHT][MAX_WIN_WIDTH];//窗口数据
extern int currentCheckPoint;//当前关卡
extern int xMapSize[CHECKPOINT];//地图X尺寸
extern int yMapSize[CHECKPOINT];//地图Y尺寸
extern int xWinSize;//窗口X尺寸
extern int yWinSize;//窗口Y尺寸
extern int xStart;//窗口X起始坐标
extern int yStart;//窗口Y起始坐标
//显示窗口
void ShowWin();
//获取窗口
void GetWin();
//安全修改地图
void SaveModifyMap(int xPos,int yPos,char data);