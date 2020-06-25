#ifndef MINEMAP_H
#define MINEMAP_H

//单元格属性
#define MINE    0   //是否有雷
#define NUM     1   //周围雷数
#define STATE   2   //挖开||未挖开||插旗
#define NOS     3   //状态数(Number of States)

//STATE属性
#define FLAT    0   //未挖开
#define DIGGED  1   //挖开
#define FLAGGED 2   //插旗
#define QUES    3   //问号

class MineMap
{
public:
    MineMap();
    ~MineMap();
    //初始化动态数组
    void Init();
    //挖雷
    void DigMine(int r, int c);
    //连续挖
    void MultiDigMine(int r, int c);
    //插旗
    void FlagIt(int r, int c);
    //问号
    void QuesIt(int r, int c);
    //拔旗
    void FlatIt(int r, int c);
    //释放内存
    void FreeMap();

    int ***Map;
    int rowNum, columnNum;  //行列数
    int mineNum;    //雷数
    int flagNum, digNum;    //旗数、挖开数
    int IsWin, IsLose;  //输赢状态
    int timer;  //计时器
    int tZero;  //存档时间

private:
    //递归展开
    void expand(int r,int c);

};

#endif // MINEMAP_H
