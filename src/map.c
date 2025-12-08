#include "../include/map.h"
#include <stdio.h>
void ShowWin() {
	for (int i = 0; i< yWinSize; i++) {
		for (int j = 0; j < xWinSize; j++) {
			char tile = mapData[currentCheckPoint][yStart + i][xStart + j];
			putch(tile);
		}
		putch('\n');
	}
}

void GetWin() {
	if (xStart > xMapSize[currentCheckPoint] - xWinSize) {
		xStart = xMapSize[currentCheckPoint] - xWinSize;
	}
	if(yStart > yMapSize[currentCheckPoint] - yWinSize) {
		yStart = yMapSize[currentCheckPoint] - yWinSize;
	}
	if(xStart < 0) {
		xStart = 0;
	}
	if(yStart < 0) {
		yStart = 0;
	}
	for(int i = 0; i < yWinSize; i++) {
		for(int j = 0; j < xWinSize; j++) {
			winData[i][j] = mapData[currentCheckPoint][yStart + i][xStart + j];
		}
	}
}

void SaveModifyMap(int xPos,int yPos,char data) {
	if(xPos > xMapSize[currentCheckPoint] - 1 || yPos > yMapSize[currentCheckPoint] - 1 || xPos < 0 || yPos < 0) {
		return;
	}
	else{
		mapData[currentCheckPoint][yPos][xPos] = data;
	}
}