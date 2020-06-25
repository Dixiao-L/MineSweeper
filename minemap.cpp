#include "minemap.h"
#include "stdlib.h"
#include "time.h"

#include <QTime>

MineMap::MineMap() {
    rowNum = 9;
    columnNum = 9;
    mineNum = 10;
    Map = NULL;
}

MineMap::~MineMap()
{

}

//初始化
void MineMap::Init() {
    flagNum = 0;
    digNum = 0;
    IsWin = 0;
    IsLose = 0;
    timer = 0;
    tZero = 0;
    Map = (int ***)malloc(rowNum * sizeof(int **));

    for (int i = 0; i < rowNum; i++)
    {
        Map[i] = (int **)malloc(columnNum * sizeof(int *));
        for (int j = 0; j < columnNum; j++)
        {
            Map[i][j] = (int *)malloc(NOS * sizeof(int));
            Map[i][j][MINE] = 0;
            Map[i][j][NUM] = 0;
            Map[i][j][STATE] = FLAT;
        }
    }

    //布雷
    srand((unsigned)time(NULL));
    for (int i = 0; i < mineNum; i++)
    {
        int a = rand() % (rowNum * columnNum);
        //判断重复
        if (Map[a / columnNum][a % columnNum][MINE] == 1)
        {
            --i;
        }
        else
            Map[a / columnNum][a % columnNum][MINE] = 1;
    }

    //计算卷积
    for (int r = 0; r < rowNum; r++) {
        for (int c = 0; c < columnNum; c++) {
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if (r + i >= 0 && r + i < rowNum && c + j >= 0 && c + j < columnNum) {
                        if (Map[r + i][c + j][MINE])
                            ++Map[r][c][NUM];
                    }
                }
            }
        }
    }
}

//挖雷
void MineMap::DigMine(int r, int c){
    //平地才能挖雷
    if (Map[r][c][STATE] == FLAT)
    {
        //挖到雷判负
        if (Map[r][c][MINE] == 1)
        {
            IsLose = 1;
        }
        //否则递归挖雷
        else
        {
            expand(r, c);
        }
    }
}

//连续挖
void MineMap::MultiDigMine(int r, int c) {
    //数字块
    if (Map[r][c][STATE] == DIGGED && Map[r][c][NUM] != 0) {
        //而且四周旗子标齐了
        int ff = 0;
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if (r + i >= 0 && r + i < rowNum && c + j >= 0 && c + j < columnNum) {
                    if (Map[r + i][c + j][STATE] == FLAGGED)
                        ++ff;
                }
            }
        }
        if(Map[r][c][NUM] == ff) {
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if (r + i >= 0 && r + i < rowNum && c + j >= 0 && c + j < columnNum){
                        DigMine(r + i, c + j);
                    }
                }
            }
        }
    }
}

//插旗
void MineMap::FlagIt(int r, int c) {
    Map[r][c][STATE] = FLAGGED;
    ++flagNum;
}

//问号
void MineMap::QuesIt(int r, int c) {
    Map[r][c][STATE] = QUES;
    --flagNum;
}

//拔旗
void MineMap::FlatIt(int r, int c){
    Map[r][c][STATE] = FLAT;
}

void MineMap::FreeMap(){
    //释放内存
        for (int i = 0; i < rowNum; i++)
        {
            for (int j = 0; j < columnNum; j++)
            {
                free(Map[i][j]);
                Map[i][j] = NULL;
            }
            free(Map[i]);
            Map[i] = NULL;
        }
        free(Map);
        Map = NULL;
}


//递归展开
void MineMap::expand(int r, int c){
    if (Map[r][c][STATE] == FLAT)
    {
        Map[r][c][STATE] = DIGGED;
        ++digNum;
        if (Map[r][c][NUM] == 0)
        {
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if (r + i >= 0 && r + i < rowNum && c + j >= 0 && c + j < columnNum)
                        expand(r + i, c + j);
                }
            }
        }
    }
}
