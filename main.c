//--------------------头文件和宏替换
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#define enter printf("\n")
#define DEBUG       \
    Sleep(500);     \
    printf("\n\n"); \
    printf
#define prt printf
#define outMode                         \
    printf("留在该模式?<YES:1\\NO:0>"); \
    TO_GET_OUT= multiInput();
#define inMode     \
    system("cls"); \
    int TO_GET_OUT = 1;    \
    while (TO_GET_OUT)
#define optb break
#define maxn 10005
#define anHour 1
#define RATES 2
//--------------------常量定义
const int monthDayRun[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int monthDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //月份天数
const float measuringPriceCoefficient[15] = {0.20, 0.20, 8.00, 6.00};
const float couponUnit[7] = {0, 6.00, 100, 15.00, 200, 25.00, 300};
const char simmeasuringUnit[10][15] = {"jin", "ge", "lan", "xiang"};
const char star[10][15] = {"", "*", "**", "***", "****", "*****"};
const char freshGood[10][15] = {"全腐", "略腐", "一般", "新鲜"};
const char simpleMeasuringUnit[10][15] = {"j", "g", "l", "x"};
const char measuringUnit[10][15] = {"斤", "个", "篮", "箱"};
const float freshCoefficient[15] = {0, 0.35, 0.60, 1.00};
//--------------------结构体定义与声明
struct DATES//时间结点
{
    int year, mon, day, hour;
};
typedef struct DATES date;
struct GOODS//商品结点
{
    int id;            // 1 编号
    int Measure;       // 2 单位
    int Level;         // 3 新鲜等级
    int Decay;         // 4 是否腐烂
    int SellingWay;    // 5 销售方式
    int StarLevel;     // 6 品质星级
    float Quantity;    // 7 数量
    float BasePrice;   // 8 零售价
    float NetPrice;    // 9 批发价
    float PackPrice;   // 10 包装费
    char Name[30];     // 11 种类
    char Variety[30];  // 12 品种
    char Supplier[50]; // 13 供应商
    char Buyer[50];    // 14 购买人
    struct TIME
    {
        int freshTime; // 15 保质期
        int addTime;   // 16 进货时间
    } time;
    struct GOODS *nxt, *frm;
    date indate, outdate; // 23 24
};
typedef struct GOODS good;
struct RECORDS//记录结点
{
    int id;
    struct RECORDS *nxt, *frm;
    char Buyer[50];
    int buyerPhonenumber; // 21 顾客手机尾号
    char Supplier[50];
    float money;
    good theThing; // 17 代表的商品信息
    date theDate;  // 18 代表的日期
    int type;      // 区别进货和出售
};
typedef struct RECORDS record;
struct BUYERS//顾客结点
{
    int id;
    char name[50];
    float points;
    int level;
    int phonenumber;
    struct BUYERS *nxt, *frm;
};
typedef struct BUYERS customer;
struct SUPPLIERS//供应商结点
{
    int id;
    char name[50];
    struct SUPPLIERS *nxt, *frm;
};
typedef struct SUPPLIERS sup;
sup supplyList;
good currentGoods, sourceOfGoods, salesGoods, ordersGoods;
record allRecord, salesRecord, ordersRecord;
customer members;
//--------------------变量定义
int idcnt = 1, idsource = 1, idsales = 1, idorders = 1, membercnt = 1, cntYear, cntMon, cntDay, cntHour, cntSec, aDay = anHour * 24, aMonth = anHour * 24 * 30;
int typeTime[30], occupied[maxn];
char typeName[30][30], ss[255];
float typeWeight[30];
double totalMoney;
//--------------------函数声明
void calcCouponMoney(good *L, int allcnt, int cunit, float tMoney); //修改满减后的链表
good *addGoods2ordersGoods(good *, float, int, char *);             //将选的进货产品添加到进货单链表
good *addGoods2salesGoods(good *, float, int, char *);              //将销售产品添加到销售单链表
void add2salesRecord(good *, int, char *, int);                     //将销售情况添加到销售记录链表
void printAllTypeRecord(record *L, int optA[]);                     //通过数组打印record种类的信息
void add2ordersRecord(good *, int, char *);                         //把进货信息添加到进货记录链表
int printAllTypeGood(good *p, int optA[]);                          //通过数组打印good种类的信息
void fixOrdersRecordSupplier(record *p);                            //修改进货记录中的供货商
record *idFindRecord(int x, record *L);                             //寻找record链表中某记录并返回指针
float transWeigh(float quan, char *s);                              //转换个和斤的单位
float calcPack(int meas, float quan);                               //计算包装费用
void saveCusInformation(customer *L);                               // 保存顾客信息
customer *findCustomerR(customer *L);                               //返回customer链表的尾结点
void combineTheSame(good *L, int x);                                //把销售或进货时的同种商品合并，不考虑购买人姓名的差异
void fixSalesRecordMoney(record *p);                                //修改销售记录中的交易钱数
void fixSalesRecordBuyer(record *p);                                //修改销售记录中的买家名称
void clearCustomerList(customer *L);                                //清空顾客链表
void fixSalesRecordTime(record *p);                                 //修改销售记录中的时间信息
customer *findCustomer(char s[50]);                                 //根据顾客姓名寻找顾客
void printOrdersRecord(record *L);                                  //打印所有进货记录
void printWhatWasOrdered(good *L);                                  //打印当次进货记录
int calcCouponUnit(float tMoney);                                   //判断满减
void printWhatWasBought(good *L);                                   //打印当次销售记录
void saveOrdersRecord(record *L);                                   //保存进货记录
void printSalesRecord(record *L);                                   //打印所有销售记录
void readOrdersRecord(record *L);                                   //读取进货记录链表
void readSalesRecord(record *L);                                    //读取销售记录链表
double howAboutThisOne(good *p);                                    //计算腐烂的函数
void fixOrdersRecord(record *L);                                    //修改进货记录
void addOrders2Current(good *L);                                    //添加进货信息到当前库存链表
void fixGoodsBasePrice(good *p);                                    //修改商品基本价格
void saveSalesRecord(record *L);                                    //保存销售记录到文件
void fixGoodsStarLevel(good *p);                                    //把进货内容添加到当前库存中，并保存
void clearRecordList(record *L);                                    //清除记录链表
record *findRecordR(record *L);                                     //返回record链表尾结点
void fixSalesRecord(record *L);                                     //修改销售记录信息
void readCurrentGoods(good *L);                                     //读取当前库存文件
void saveCurrentGoods(good *L);                                     //保存当前库存到文件
void fixGoodsQuantity(good *p);                                     //修改商品数量信息
void printAllSuppliers(sup *L);                                     //打印所有进货商信息
void fixGoodsVariety(good *p);                                      //修改商品品种
void seekCustomers(record *L);                                      //查找顾客信息
void seekSuppliers(record *L);                                      //查找供货商信息
int goCheckFreshness(good *L);                                      //计算腐烂的函数
void printSourceList(good *L);                                      //打印货源列表
void readMembers(customer *L);                                      //读取顾客信息
good *idFind(int x, good *L);                                       //通过id寻找库存中的货物
void printByPointer(good *p);                                       //通过指针打印某种货物库存
void readSourceList(good *L);                                       //读取货源列表
void delRecordNode(record *);                                       //删除某条记录
void clearGoodList(good *L);                                        //清除购物车链表
void seekSameKinds(good *L);                                        //寻找相同类型的库存物品
void dealWithDecay(good *L);                                        //处理腐烂物
void fixGoodsLevel(good *p);                                        //修改库存货物
void saveInformation(void);                                         //保存店铺信息
void delGoodsNode(good *p);                                         //删除某个库存
void readSuppliers(sup *L);                                         //读取供货商列表
int addFreshTime(char *s);                                          //增加保质期属性
void addTimeInfo(date *D);                                          //增加进货日期属性
void printCurrentGoods();                                           //打印现有库存
void priceFluctuations();                                           //计算价格波动
good *findGoodR(good *L);                                           //寻找good链表的尾节点
void fixGoods(good *L);                                             //修改good链表的某个节点
void statisticsMode();                                              //统计模式界面
void printTimeNote();                                               //打印不同时间的提示语句
double multiInput();                                                //混合输入，提取数字
void suspendMode();                                                 //挂起模式
void changeColor();                                                 //修改界面颜色
int getClockTime();                                                 //获取当前时间
void queryMode();                                                   //查询模式界面
void orderMode();                                                   //进货模式界面
void sellMode();                                                    //销售模式界面
void getToday();                                                    //获取店铺信息，包括经营时间、金钱等
void initType();                                                    //初始化各种商品的规格
void welcome2();                                                    //欢迎界面
void initPtr();                                                     //初始化各种链表的头节点
void fixInfo();                                                     //修改信息
void goodbye();                                                     //离开界面
void reColor();                                                     //恢复文字颜色
void remake();                                                      //重置系统

//--------------------主函数
int main()
{
    system("mode con cols=140");
    getToday();
    initType();
    initPtr();
    readCurrentGoods(&currentGoods);
    prt("当前库存读取完毕!\n");
    readSalesRecord(&salesRecord);
    prt("销售记录读取完毕!\n");
    readOrdersRecord(&ordersRecord);
    prt("进货记录读取完毕!\n");
    readMembers(&members);
    prt("会员信息读取完毕!\n");
    readSuppliers(&supplyList);
    prt("货商信息读取完毕!\n");
    //DEBUG("文件读取测试");
    inMode
    {
        changeColor();
        welcome2();
        int choice;
        choice = multiInput();
        switch (choice)
        {
        case 1:
            reColor();
            statisticsMode();
            break;
        case 2:
            reColor();
            queryMode();
            break;
        case 3:
            reColor();
            fixInfo();
            break;
        case 4:
            reColor();
            sellMode();
            break;
        case 5:
            reColor();
            orderMode();
            break;
        case 6:
            reColor();
            suspendMode();
            break;
        case 7:
            reColor();
            remake();
            break;
        case 0:
        {
            system("cls");
            goodbye();
            return 0;
        }
        }
        saveInformation();
        welcome2();
    }
    system("cls");
    goodbye();
    return 0;
}
//--------------------打印主菜单
void welcome2() //
{
    getToday();
    system("cls");
    printf("╭*╮———-———-———-———-———-———-———-———-———*\n");
    printf("╰*╯ | DEV Gerald & Solokey & YiberWon |    果蔬超市管理系统    |\n ");
    printf("   | 现在时间是 %d年%02d月%02d日%02d时   |------------------------|\n", cntYear, cntMon, cntDay, cntHour);
    printf("    | 店内现金:%12.2lf元         |\n", totalMoney);
    printf("    |——-———-——-——-———-——|\n\n");
    cntSec = cntMon * aMonth + cntDay * aDay + cntHour * anHour;
    if(cntDay==8) printf("     今天是会员日!!!全场满减折扣中!\n");
    else printf("     每月8日是全场满减会员日,请多多关注小店~~\n");
    printTimeNote();
    printf("    *———-———-———-———-——-———-———-———*\n");
    printf("    |   统计模式--1  |   查询模式--2    |   修改信息--3   |\n");
    printf("    |----------------|------------------|-----------------|\n");
    printf("    |  ----现有库存  |  -----同种蔬果   |  -----蔬果属性  |\n");
    printf("    |  ----销售记录  |  -----顾客记录   |  -----销售记录  |\n");
    printf("    |  ----进货记录  |  -----进货商记录 |  -----进货记录  |\n");
    printf("    |----------------|------------------|-----------------|\n");
    printf("    |\n");
    printf("    |----------------|------------------|-----------------|\n");
    printf("    |   销售模式--4  |   进货模式--5    |   挂起模式--6   |\n");
    printf("    |----------------|------------------|-----------------|\n");
    printf("    |\n");
    printf("    |----------------|------------------|\n");
    printf("    |   重置系统--7  |     退出--0      |\n");
    printf("    |----------------|------------------|\n\n");
}
//--------------------再见界面
void goodbye()
{
    system("cls");
    saveInformation();
    printf("*********************************\n");
    printf("*                               *\n");
    printf("*           GoodBye!            *\n");
    printf("*                               *\n");
    printf("*********************************\n");
    system("pause");
}
//--------------------读取时间和金钱
void getToday()
{
    FILE *fp = fopen("information.csv", "r");
    while (fgetc(fp) != '\n')
        fseek(fp, 1, SEEK_CUR);
    char ch;
    fscanf(fp, "%d %d %d %d %c %lf %c %d %c %d", &cntYear, &cntMon, &cntDay, &cntHour, &ch, &totalMoney, &ch, &idsales, &ch, &idorders);
    fclose(fp);
    return;
}
//--------------------读入现有库存
void readCurrentGoods(good *L)
{
    FILE *fp = fopen("currentGoods.csv", "r");
    cntSec = cntMon * aMonth + cntDay * aDay + cntHour * anHour;
    while (fgetc(fp) != '\n')
        fseek(fp, 1, SEEK_CUR);
    good *g, *t = L;
    good gd;
    char ch;
    if (fp == NULL)
    {
        printf("现有库存文件读入错误!\n");
        Sleep(2000);
        fclose(fp);
        return;
    }
    else
    {
        while (fscanf(fp, "%d %d %d %d %c %s %c %s %c %f %c %f %c %d %c %d %c %d",
                      &gd.indate.year, &gd.indate.mon, &gd.indate.day, &gd.indate.hour, &ch, &gd.Name, &ch, &gd.Variety, &ch,
                      &gd.BasePrice, &ch, &gd.Quantity, &ch, &gd.Measure, &ch,
                      &gd.Level, &ch, &gd.StarLevel) != EOF)
        {
            g = (good *)malloc(sizeof(good)); //逐个创建空间,连接成链表
            int nametype = addFreshTime(gd.Name);
            gd.id = idcnt++;
            gd.time.freshTime = typeTime[nametype];
            gd.time.addTime = gd.indate.mon * aMonth + gd.indate.day * aDay + gd.indate.hour * anHour;
            *g = gd;
            double rates = howAboutThisOne(g);
            if (rates >= 0)
            {
                int levelshouldbe = (int)(ceil)(rates * 3);
                g->Level = levelshouldbe;
            }
            else
                g->Level = 0;
            occupied[g->id] = 1;
            t->nxt = g;
            g->frm = t;
            t = g;
            t->nxt = NULL;
        }
    }
    fclose(fp);
    return;
}
//--------------------读入供货商
void readSuppliers(sup *L)
{
    FILE *fp = fopen("suppliers.csv", "r");
    while (fgetc(fp) != '\n')
        fseek(fp, 1, SEEK_CUR);
    sup *g, *t = L;
    sup gd;
    char ch;
    int idsource = 1;
    if(fp==NULL)
    {
        printf("供货商列表文件读入错误!\n");
        Sleep(2000);
        return ;
    }
    while(fscanf(fp,"%d %c %s\n", &gd.id, &ch, &gd.name)!=EOF)
    {
        g=(sup*)malloc(sizeof(sup));
        *g=gd;
        t->nxt = g;
        g->frm = t;
        t = g;
        t->nxt = NULL;
    }
}
//--------------------读入进货选择
void readSourceList(good *L)
{
    FILE *fp = fopen("sourceOfGoods.csv", "r");
    while (fgetc(fp) != '\n')
        fseek(fp, 1, SEEK_CUR);
    good *g, *t = L;
    good gd;
    char ch;
    int idsource = 1;
    if (fp == NULL)
    {
        printf("进货选择文件读入错误!\n");
        Sleep(2000);
        fclose(fp);
        return;
    }
    else
    {
        while (fscanf(fp, "%s %s %s %s %f %s %f %c %d %c %d %c %d %c %s\n",
                      &gd.Name, &ch, &gd.Variety, &ch, &gd.NetPrice, &ch, &gd.BasePrice, &ch, &gd.Measure, &ch, &gd.Level, &ch, &gd.StarLevel, &ch, &gd.Supplier) != EOF)
        {
            gd.Level = 3;                     //默认的等级是新鲜
            g = (good *)malloc(sizeof(good)); //逐个创建空间,连接成链表
            int nametype = addFreshTime(gd.Name);
            gd.id = idsource++;
            gd.time.freshTime = typeTime[nametype];
            gd.time.addTime = cntMon * aMonth + cntDay * aDay + cntHour * anHour;
            addTimeInfo(&gd.indate);
            *g = gd;
            occupied[g->id] = 1;
            t->nxt = g;
            g->frm = t;
            t = g;
            t->nxt = NULL;
        }
    }
    fclose(fp);
    return;
}
//--------------------类型文件读入
void initType()
{
    int i = 0;
    char ch;
    FILE *fp = fopen("typeInit.csv", "r");
    while (fgetc(fp) != '\n')
        fseek(fp, 1, SEEK_CUR);
    while (fscanf(fp, "%s %c %f %c %d", typeName[i], &ch, &typeWeight[i], &ch, &typeTime[i]) != EOF)
        ++i;
    fclose(fp);
}
//--------------------统计模式
void statisticsMode()
{
    printf("统计模式\n");
    inMode
    {
        int choice;
        system("cls"); // 清屏
        printf("    *———-———-———-———-——-———-———-———*\n");
        printf("    |   现有库存--1  |   销售记录--2    |   进货记录--3   |\n");
        printf("    |----------------|------------------|-----------------|\n");
        printf("    |                      退出--0      |\n");
        printf("    |----------------|------------------|\n\n");
        choice = multiInput();
        switch (choice)
        {
        case 1:
            printCurrentGoods(&currentGoods);
            break;
        case 3:
            printOrdersRecord(&ordersRecord);
            break;
        case 2:
            printSalesRecord(&salesRecord);
            break;
        case 0:
        {
            system("cls");
            printf("已退出该模式\n");
            return;
        }
        }
        outMode
    }
}
//--------------------查询模式
void queryMode()
{
    printf("查询模式\n");
    int choice;
    system("cls");
    printf("    *———-———-———-———-——-———-———-———*\n");
    printf("    |   同种蔬果--1  |   顾客记录--2    |  进货商信息--3  |\n");
    printf("    |----------------|------------------|-----------------|\n");
    printf("    |                      退出--0      |\n");
    printf("    |----------------|------------------|\n\n");
    choice = multiInput();
    switch (choice)
    {
    case 1:
        seekSameKinds(&currentGoods);
        break;
    case 2:
        seekCustomers(&salesRecord);
        break;
    case 3:
        seekSuppliers(&ordersRecord);
        break;
    case 0:
    {
        system("cls");
        printf("已退出该模式\n");
        return;
    }
    }
}
//--------------------修改信息
void fixInfo()
{
    printf("修改信息\n");
    inMode
    {
        int choice;
        system("cls");
        printf("    *———-———-———-———-——-———-———-------------———*\n");
        printf("    |   修改蔬果属性--1  |   修改销售记录--2    |   修改进货记录--3   |\n");
        printf("    |--------------------|----------------------|---------------------|\n");
        printf("    |                            退出--0        |\n");
        printf("    |--------------------|----------------------|\n\n");
        choice = multiInput(ss);
        switch (choice)
        {
        case 1:
            fixGoods(&currentGoods);
            saveCurrentGoods(&currentGoods);
            return;
        case 2:
            fixSalesRecord(&salesRecord);
            return;
        case 3:
            fixOrdersRecord(&ordersRecord);
            return;
        case 0:
        {
            system("cls");
            printf("已退出该模式\n");
            return;
        }
        }
        outMode
    }
}
//--------------------销售模式
void sellMode()
{
    system("cls");
    printf("销售模式\n");
    if (cntHour >= 22 || cntHour < 7)
    {
        printf("不在营业时间!\n");
        Sleep(1000);
        return;
    }
    if (!currentGoods.nxt)
    {
        printf("没有商品可供选择!\n");
        Sleep(1000);
        return;
    }
    char buyername[50];
    printf("请输入购买者名称<输入0以退出>");
    scanf("%s", buyername);
    printf("\n");
    if (!strcmp(buyername, "0"))
        return;
    printf("请输入手机尾号后四位:");
    int pnumber;
    pnumber = multiInput();
    int cnt = 0;
    inMode
    {
        int gid = 0;
        float gcnt = 0;
        system("cls");
        if(cntDay==8) printf("今天是会员日!!全场满减折扣中!\n");
        printf("当前购买者:%s\n", buyername);
        if (!currentGoods.nxt)
        {
            printf("没有商品可供选择!\n");
            Sleep(1000);
        }
        else
        {
            printCurrentGoods(&currentGoods);
            printf("请输入购买的商品ID\n");
            gid = multiInput();
            good *p = idFind(gid, &currentGoods);
            while (!p)
            {
                printf("该物品不存在,请重新输入\n");
                gid = multiInput();
                p = idFind(gid, &currentGoods);
            }
            printf("该物品 %s %s 的库存为 %.1f %s,单价 %.2f:\n", p->Name, p->Variety, p->Quantity, measuringUnit[p->Measure], p->BasePrice * freshCoefficient[p->Level]);
            printf("请输入购买数量:");
            gcnt = multiInput();
            while (gcnt <= 0 || p->Quantity < gcnt)
            {
                printf("输入的数量不在范围内，请重新输入\n");
                gcnt = multiInput();
            }
            p->Quantity -= gcnt;
            saveCurrentGoods(&currentGoods);
            good *newThing = addGoods2salesGoods(p, gcnt, cnt, buyername); //添加销售物品至本次销售链表，返回一个good指针
            combineTheSame(&salesGoods, 1);                                //合并相同ID物品
            add2salesRecord(newThing, gcnt, buyername, pnumber);           //添加销售物品至销售记录链表
            if (!p->Quantity)
                delGoodsNode(p);
            printf("添加成功!\n");
            cnt++;
            printWhatWasBought(&salesGoods); //打印本次销售物品的信息
        }
        outMode
    }
    system("cls");
    prt("%s\n", buyername);
    customer *pcus = NULL;
    pcus = findCustomer(buyername);
    int couponFlag = 0;
    if (cntDay == 8)
    {
        printf("今天是%d月%d日,全场满减!\n", cntMon, cntDay);
        couponFlag = 1;
    }
    if (pcus != NULL && (pcus->phonenumber == pnumber)) // 判断顾客是否为大会员,并且计算满减
    {

        if (pcus->points >= 100)
        {
            couponFlag = 1;
            printf("检测到该顾客是会员!享受满减优惠!\n");
        }
        else
        {
            printf("检测到该顾客是会员!购买将增加会员积分!\n");
        }
        if (couponFlag == 1)
        {
            good *g = salesGoods.nxt;
            float tMoney = 0;
            while (g != NULL)
            {
                tMoney += (float)g->BasePrice * g->Quantity + g->PackPrice;
                g = g->nxt;
            }
            int cunit = calcCouponUnit(tMoney);
            calcCouponMoney(&salesGoods, cnt, cunit, tMoney);
        }
        pcus->points = pcus->points + 10.0;
    }
    else
    {
        printf("检测到该顾客不是会员!将自动加入会员列表!\n");
        customer *c = members.nxt;
        customer *d = (customer *)malloc(sizeof(customer));
        strcpy(d->name, buyername);
        d->phonenumber = pnumber;
        d->points = 10.0;
        d->nxt = NULL;
        c = findCustomerR(&members);
        d->id = c->id++;
        c->nxt = d;
        d->frm = c;
        printf("将顾客 %s 添加到顾客列表成功!\n", buyername);
        printWhatWasBought(&salesGoods);
        Sleep(300);
    }
    saveCusInformation(&members);
    saveSalesRecord(&salesRecord);
    clearGoodList(&salesGoods);
    printf("按任意键退出...\n");
    getch();
}
//--------------------进货模式
void orderMode()
{
    float moneyNow = 0;
    readSourceList(&sourceOfGoods);
    inMode
    {
        int cnt = 0, gid = 0;
        float gcnt = 0;
        char suppliername[50];
        system("cls");
        printSourceList(&sourceOfGoods);
        printf("请输入进货的商品ID<输入0以退出>\n");
        gid = multiInput();
        if (gid == 0)
            break;
        good *p = idFind(gid, &sourceOfGoods);
        while (!p)
        {
            printf("该物品不存在,请重新输入<输入0以退出>\n");
            gid = multiInput();
            if (gid == 0)
            {
                saveOrdersRecord(&ordersRecord);
                clearGoodList(&ordersGoods);
                printf("按任意键退出...\n");
                getch();
                return;
            }
            p = idFind(gid, &sourceOfGoods);
        }
        printf("该物品 %s %s 的库存为 999+ %s,单价 %.2f:\n", p->Name, p->Variety, measuringUnit[p->Measure], p->NetPrice * freshCoefficient[p->Level]);
        printf("请输入购买数量<输入0以退出>");
        gcnt = multiInput();
        if (!gcnt)
        {
            totalMoney-=moneyNow;
            printWhatWasOrdered(&ordersGoods);
            saveOrdersRecord(&ordersRecord);
            clearGoodList(&ordersGoods);
            printf("按任意键退出...\n");
            getch();
            return;
        }
        while (gcnt < 0)
        {
            gcnt = multiInput();
            if (gcnt == 0)
            {
                totalMoney-=moneyNow;
                saveOrdersRecord(&ordersRecord);
                clearGoodList(&ordersGoods);
                return;
            }
            printf("输入的数量不在范围内，请重新输入\n");
            gcnt = multiInput();
        }
        moneyNow += p->NetPrice * gcnt;
        strcpy(suppliername, p->Supplier);
        if (moneyNow > totalMoney)
        {
            printf("资金不足,无法添加!\n");
            moneyNow -= p->NetPrice * gcnt;
        }
        else
        {
            good *newThing = addGoods2ordersGoods(p, gcnt, cnt, suppliername);
            add2ordersRecord(newThing, gcnt, suppliername);
            printWhatWasOrdered(&ordersGoods);
        }
        outMode
    }
    totalMoney-=moneyNow;
    system("cls");
    addOrders2Current(&ordersGoods);
    saveOrdersRecord(&ordersRecord);
    clearGoodList(&ordersGoods);
}
//--------------------挂起模式
void suspendMode()
{
    system("cls");
    printf("挂起模式\n");
    getToday();
    dealWithDecay(&currentGoods);
    saveCurrentGoods(&currentGoods);
    if (!currentGoods.nxt)
    {
        printf("没有库存商品!继续挂起吗?<Y--1/N--0>\n");
        Sleep(1000);
        int flag;
        flag = multiInput();
        if (!flag)
            return;
    }
    inMode
    {
        system("cls");
        printf("系统即将进入挂起模式\n");
        printf("现在时间是，%d年%d月%d日%d时\n", cntYear, cntMon, cntDay, cntHour);
        int initial_Time = getClockTime(); //本次进入挂起模式的初始时间
        int now_Time = 0, duration = 0, lasthour = 0, result = 0;
        int pastmonth = 0, pastday = 0, pasthour = 0;
        int printmonth = 0, printday = 0, printhour = 0;
        while (!_kbhit())
        {
            now_Time = getClockTime();
            duration = (now_Time - initial_Time) * RATES;
            pastmonth = (duration) / aMonth;
            pastday = (duration % aMonth) / aDay;
            pasthour = (duration % aMonth % aDay) / anHour;
            if (lasthour != pasthour) //当时间有变化时
            {
                lasthour = pasthour;
                system("cls");
                printf("系统已进入挂起模式,按任意键退出\n");
                printmonth = cntMon + pastmonth;
                printday = (cntDay + pastday) + (cntHour + pasthour) / aDay;
                if (printday > 30) //如果一个月的时间超过30天
                {
                    printday = 1;
                    printmonth++;
                }
                printhour = (cntHour + pasthour) % aDay;
                printf("现在时间是，%d年%d月%d日%d时\n", cntYear, printmonth, printday, printhour);
                cntSec = printmonth * aMonth + printday * aDay + printhour * anHour;
                printCurrentGoods(&currentGoods);
                result = goCheckFreshness(&currentGoods);
                if (result)
                    break;
            }
        }
        system("cls");
        printf("挂起模式已退出\n");
        cntMon = printmonth;
        cntDay = printday;
        cntHour = printhour;
        printf("现在时间是，%d年%d月%d日%d时\n", cntYear, cntMon, cntDay, cntHour);
        saveInformation();
        if (result)
            dealWithDecay(&currentGoods);
        printf("现在开始进行价格波动...\n");
        priceFluctuations();
        outMode
    }
}
//--------------------账单查看**
void billMode()
{
    printf("账单查看\n");
}
//--------------------保鲜时间替换
int addFreshTime(char *s)
{
    for (int i = 0; i < 25; i++)
    {
        if (strcmp(s, typeName[i]) == 0)
            return i;
    }
    return 0;
}
//--------------------打印当前库存***
void printCurrentGoods(good *L)
{
    good *p = L->nxt;
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("添加时间           |序号|名称    |品种      |基础价格        |库存量及单位 |等级 |保质期 |星级  |\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    if (p != NULL)
    {
        while (p != NULL) // 全部输出直至链表结束
        {
            printf("%d年%02d月%02d日%02d时", p->indate.year, p->indate.mon, p->indate.day, p->indate.hour);
            printByPointer(p);
            p = p->nxt;
        }
    }
}
//--------------------打印货源列表***
void printSourceList(good *L)
{
    printf("----------------------------------------------------------------------------------------------------------\n");
    printf("|编号\t|名称    |品种     |批发价     |零售价及单位   |等级|星级  |保质期|供应者                        |\n");
    printf("----------------------------------------------------------------------------------------------------------\n");
    int optA[40] = {1, 11, 12, 98, 9, 8, 2, 97, 3, 6, 15, 13, 0};
    good *q = L->nxt;
    while (q)
    {
        printAllTypeGood(q, optA);
        printf("\n");
        printf("----------------------------------------------------------------------------------------------------------\n");
        q = q->nxt;
    }
    return;
}
//--------------------打印进货记录***
void printOrdersRecord(record *L)
{
    record *q = L->nxt;
    printf("----------------------------------------------------------------------------------------------------------------------------\n");
    printf("序号\t|时间\t\t   |名称    |品种    |商品数量(单位) |等级 |星级  |买家  |供应者                         |金额    |\n");
    printf("----------------------------------------------------------------------------------------------------------------------------\n");
    while (q != NULL)
    {
        int optA[40] = {20, 18, 17, 11, 12, 7, 2, 3, 97, 6, 0, 14, 13, 97, 19, 0};
        printAllTypeRecord(q, optA);
        printf("\n");
        printf("----------------------------------------------------------------------------------------------------------------------------\n");
        q = q->nxt;
    }
}
//--------------------打印销售记录***
void printSalesRecord(record *L)
{
    record *q = L->nxt;
    printf("------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("序号\t|时间\t\t   |名称    |品种    |商品数量(单位) |等级 |星级  |买家  |买家尾号 |供应者                           |金额 |\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------\n");
    while (q != NULL)
    {
        int optA[40] = {20, 18, 17, 11, 12, 7, 2, 3, 97, 6, 0, 14, 98, 98, 21, 97, 13, 19, 0};
        printAllTypeRecord(q, optA);
        printf("\n");
        printf("------------------------------------------------------------------------------------------------------------------------------------\n");
        q = q->nxt;
    }
}
//--------------------通过指针单独打印***
void printByPointer(good *p)
{
    int optA[40] = {1, 11, 12, 8, 90, 2, 7, 2, 3, 15, 98, 6, 0};
    printAllTypeGood(p, optA);
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------\n");
}
//--------------------查找同种蔬果
void seekSameKinds(good *L)
{
    char serName[20];
    int flag = 0;
    system("cls");
    printf("请输入待查询蔬果名称<输入0以退出>\n");
    scanf("%s", serName);
    if (!strcmp(serName, "0"))
        return;
    good *g = L->nxt;
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("添加时间           |序号|名称    |品种      |基础价格        |库存量及单位 |等级 |保质期 |星级  |\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    while (g != NULL)
    {
        if (!strcmp(serName, g->Name))
        {
            flag = 1;
            printf("%d年%02d月%02d日%02d时", g->indate.year, g->indate.mon, g->indate.day, g->indate.hour);
            printByPointer(g);
        }
        g = g->nxt;
    }
    if (!flag)
        printf("未查找到相应种类蔬果!\n");
    printf("按任意键退出...\n");
    getch();
    return;
}
//--------------------查找某位顾客信息
void seekCustomers(record *L)
{
    printf("---------------------------------\n");
    printf("|序号|顾客名称|积分数量|手机尾号|\n");
    printf("---------------------------------\n");
    customer *p = &(members);
    p = p->nxt;
    int cuscnt = 1;
    while (p != NULL)
    {
        printf(" %4d %-8s     %.1f  %04d\n", cuscnt, p->name, p->points, p->phonenumber);
        printf("---------------------------------\n");
        cuscnt++;
        p = p->nxt;
    }
    printf("请输入待查询的顾客姓名<输入0以退出>");
    char buyerName[20];
    int buyerPhone;
    int flag = 0;
    scanf("%s", buyerName);
    if (strcmp(buyerName, "0") == 0)
        return;
    printf("请输入待查询顾客的手机号后四位<输入0以退出>");
    buyerPhone = multiInput();
    if (buyerPhone == 0)
        return;
    record *g = L->nxt;
    printf("------------------------------------------------------------------------------------------------------------------\n");
    printf("|时间              |名称    |品种    |商品数量(单位) |等级|星级 |买家   |供应者                          |金额   |\n");
    printf("------------------------------------------------------------------------------------------------------------------\n");
    while (g != NULL)
    {
        if (!strcmp(buyerName, g->Buyer) && (buyerPhone == g->buyerPhonenumber))
        {
            flag = 1;
            int optA[40] = {18, 97, 17, 11, 12, 7, 2, 3, 6, 0, 14, 13, 19, 0};
            printAllTypeRecord(g, optA);
            printf("\n");
            printf("------------------------------------------------------------------------------------------------------------------\n");
        }
        g = g->nxt;
    }
    if (!flag)
        printf("未查找到该顾客!\n");
    printf("按任意键退出...\n");
    getch();
    return;
}
//--------------------查找某位供货商信息
void seekSuppliers(record *L)
{
    char supName[50];
    int flag = 0;
    record *g = L->nxt;
    system("cls");
    printAllSuppliers(&supplyList);
    printf("-------------------------------------------------------------------------------------------------------------------\n");
    printf("|时间                |名称    |品种    |商品数量(单位) |等级|星级  |买家  |供应者                       |金额     |\n");
    printf("-------------------------------------------------------------------------------------------------------------------\n");
    while (g != NULL)
    {
        {
            int optA[40] = {18, 17, 98, 97, 11, 12, 7, 2, 3, 6, 0, 14, 13, 19, 0};
            printAllTypeRecord(g, optA);
            printf("\n");
            printf("-------------------------------------------------------------------------------------------------------------------\n");
        }
        g = g->nxt;
    }
    printf("请输入你要单独查看的供货商id<输入0以退出>");
    int sid;
    sid=multiInput();
    sup* supIndex=supplyList.nxt;
    while(supIndex!=NULL)
    {
        if(sid==supIndex->id)
        {
            strcpy(supName,supIndex->name);
            break;
        }
        supIndex=supIndex->nxt;
    }
    g = L->nxt;
    while (g != NULL)
    {
        if (!strcmp(supName, g->Supplier))
        {
            if (!flag)
            {
                printf("-------------------------------------------------------------------------------------------------------------------\n");
                printf("|时间                |名称    |品种    |商品数量(单位) |等级|星级  |买家  |供应者                       |金额     |\n");
                printf("-------------------------------------------------------------------------------------------------------------------\n");
            }
            flag = 1;
            int optA[40] = {18, 17, 98, 97, 11, 12, 7, 2, 3, 6, 0, 14, 13, 19, 0};
            printAllTypeRecord(g, optA);
            printf("\n");
            printf("-------------------------------------------------------------------------------------------------------------------\n");
        }
        g = g->nxt;
    }
    if (!flag)
        printf("未查找到该供货商!\n");
    printf("按任意键退出...\n");
    getch();
    return;
}
//--------------------修改蔬果属性
void fixGoods(good *L)
{
    inMode
    {
        int choice = 0;
        printCurrentGoods(&currentGoods);
        printf("请输入需要修改的商品id<输入0退出>\n");
        int gid = 0;
        gid = multiInput();
        if (gid == 0) return;
        good *p = idFind(gid, &currentGoods);
        while (!p)
        {
            printf("该商品不存在,请重新输入\n");
            gid = multiInput();
            p = idFind(gid, &currentGoods);
        }
        printf("-------------------------------------------------------------------------------------------------\n");
        printf("添加时间           |序号|名称    |品种      |基础价格        |库存量及单位 |等级 |保质期 |星级  |\n");
        printf("-------------------------------------------------------------------------------------------------\n");
        printf("%d年%02d月%02d日%02d时 ", p->indate.year, p->indate.mon, p->indate.day, p->indate.hour);
        printByPointer(p);
        printf("|修改品种--1\n");
        printf("|\n");
        printf("|修改价格--2\n");
        printf("|\n");
        printf("|修改库存--3\n");
        printf("|\n");
        printf("|修改星级--4\n");
        printf("|\n");
        printf("|删除该商品--5\n");
        printf("|\n");
        printf("|退出--0\n");

        choice = multiInput();
        switch (choice)
        {
        case 1:
            fixGoodsVariety(p);
            break;
        case 2:
            fixGoodsBasePrice(p);
            break;
        case 3:
            fixGoodsQuantity(p);
            break;
        case 4:
            fixGoodsStarLevel(p);
            break;
        case 5:
        {
            system("cls");
            printf("确认删除吗?<YES:1\\NO:0>\n");
            int confirm = 0;
            confirm = multiInput();
            if (confirm)
            {
                delGoodsNode(p);
                printf("删除成功!\n");
            }
            else
            {
                printf("未删除该商品!\n");
            }
            break;
        }
        case 0:
            system("cls");
            return;
        }
        outMode
    }
}
//--------------------修改销售记录
void fixSalesRecord(record *L)
{
    int choice = 0;
    if (salesRecord.nxt == NULL)
    {
        printf("暂无销售记录！\n");
        printf("按任意键退出...\n");
        getch();
        return;
    }
    printSalesRecord(&salesRecord);
    printf("请输入需要修改的记录id<输入0以退出>");
    int gid = 0;
    gid = multiInput();
    if (gid == 0)
        return;
    record *p = idFindRecord(gid, &salesRecord);
    while (!p)
    {
        printf("该记录不存在,请重新输入<输入0以退出>\n");
        gid = multiInput();
        if (gid == 0)
            return;
        p = idFindRecord(gid, &salesRecord);
    }
    printf("|修改时间--1\n");
    printf("|\n");
    printf("|修改金额--2\n");
    printf("|\n");
    printf("|修改买家名称--3\n");
    printf("|\n");
    printf("|删除该条记录(退货)--4\n");
    printf("|\n");
    printf("|退出--0\n");

    choice = multiInput();
    switch (choice)
    {
    case 1:
        fixSalesRecordTime(p);
        break;
    case 2:
        fixSalesRecordMoney(p);
        break;
    case 3:
        fixSalesRecordBuyer(p);
        break;
    case 4:
    {
        system("cls");
        printf("确认删除吗?<YES:1\\NO:0>\n");
        int confirm = 0;
        confirm = multiInput();
        if (confirm)
        {
            totalMoney -= p->money;
            printf("退货钱款为:%.2f", p->money);
            strcpy(p->Supplier, "该商品已退款");
            // delRecordNode(p);
            saveSalesRecord(&salesRecord);
            saveInformation();
            printf("删除成功!\n");
            printf("按任意键退出...\n");
            getch();
        }
        else
        {
            printf("未删除该记录!\n");
        }
        break;
    }
    case 0:
        system("cls");
        return;
    }
}
//--------------------修改进货记录
void fixOrdersRecord(record *L)
{
    int choice = 0;
    if (ordersRecord.nxt == NULL)
    {
        printf("暂无进货记录!\n");
        printf("按任意键退出...\n");
        getch();
        return;
    }
    printOrdersRecord(&ordersRecord);
    printf("请输入需要修改的记录id<输入0以退出>");
    int gid = 0;
    gid = multiInput();
    if (gid == 0)
        return;
    record *p = idFindRecord(gid, &ordersRecord);
    while (!p)
    {
        printf("该记录不存在,请重新输入\n");
        gid = multiInput();
        p = idFindRecord(gid, &salesRecord);
    }
    printf("|修改时间--1\n");
    printf("|\n");
    printf("|修改金额--2\n");
    printf("|\n");
    printf("|修改供货商名称--3\n");
    printf("|\n");
    printf("|删除该条记录--4\n");
    printf("|\n");
    printf("|退出--0\n");

    choice = multiInput();
    switch (choice)
    {
    case 1:
        fixSalesRecordTime(p);
        break; //这里用salesRecord的函数，因为功能是一样的
    case 2:
        fixSalesRecordMoney(p);
        break; //这里用salesRecord的函数，因为功能是一样的
    case 3:
        fixOrdersRecordSupplier(p);
        break;
    case 4:
    {
        system("cls");
        printf("确认删除吗?<YES:1\\NO:0>\n");
        int confirm = 0;
        confirm = multiInput();
        if (confirm)
        {
            delRecordNode(p);
            saveOrdersRecord(&ordersRecord);
            printf("删除成功!\n");
            printf("按任意键退出...\n");
            getch();
        }
        else
        {
            printf("未删除该记录!\n");
        }
        break;
    }
    case 0:
        system("cls");
        return;
    }
}

//--------------------获得程序运行时间
int getClockTime() { return clock() / CLOCKS_PER_SEC; }
//--------------------检查库存果蔬新鲜程度
int goCheckFreshness(good *L)
{
    int anydecayed = 0;
    good *p = L->nxt;
    while (p != NULL)
    {
        double rate = howAboutThisOne(p);
        int levelshouldbe = (int)(ceil)(rate * 3);
        p->Level = levelshouldbe;
        if (levelshouldbe <= 0)
            anydecayed = 1;
        p = p->nxt;
    }
    return anydecayed;
}
//--------------------检查单独果蔬新鲜程度
double howAboutThisOne(good *p)
{
    int nowtime = cntSec;
    double rates = (p->time.freshTime * 1.0 - (nowtime - p->time.addTime) * 1.0) / p->time.freshTime;//公式计算
    return rates;
}
//--------------------处理腐烂物
void dealWithDecay(good *L)
{
    good *g = L->nxt;
    good *g0;
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("添加时间           |序号|名称    |品种      |基础价格        |库存量及单位 |等级 |保质期 |星级  |\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    while (g != NULL)
    {
        if (g->Level <= 0)
        {
            printf("%d年%02d月%02d日%02d时 ", g->indate.year, g->indate.mon, g->indate.day, g->indate.hour);
            printByPointer(g);
            printf("\n");
            g0 = g;
            g = g->nxt;
            delGoodsNode(g0);
        }
        else
            g = g->nxt;
    }
    printf("完全腐烂物已被清理\n");
    return;
}
//--------------------通过指针删除蔬果链表的一个结点
void delGoodsNode(good *p)
{
    good *p0 = p->frm;
    p0->nxt = p->nxt;
    if (p->nxt != NULL)
        (p->nxt)->frm = p0;
    occupied[p->id] = 0;
    free(p);
    saveCurrentGoods(&currentGoods);
}
//--------------------保存当前库存到文件
void saveCurrentGoods(good *L)
{
    FILE *fpw = fopen("currentGoods.csv", "w");
    if (fpw == NULL)
    {
        printf("打开文件失败!");
        return;
    }
    fprintf(fpw, "添加时间 , 名称 , 品种 , 基础价格 , 数量 , 单位 , 新鲜度 , 星级\n");
    good *p = L->nxt;
    while (p != NULL)
    {
        fprintf(fpw, "%d %d %d %d , %s , %s , %f , %f , %d , %d , %d\n", p->indate.year, p->indate.mon, p->indate.day, p->indate.hour, p->Name, p->Variety, p->BasePrice, p->Quantity, p->Measure, p->Level, p->StarLevel);
        p = p->nxt;
    }
    fclose(fpw);
    return;
}
//--------------------保存进货记录到文件
void saveOrdersRecord(record *L)
{
    FILE *fpw = fopen("ordersRecord.csv", "w");
    if (fpw == NULL)
    {
        printf("打开文件失败!");
        return;
    }
    fprintf(fpw, "序号 , 时间 , 名称 , 品种 , 批发价 , 零售价 , 商品数量 , (单位) , 等级 , 星级 , 供应者 , 金额\n");
    record *p = L->nxt;
    while (p != NULL)
    {
        good gd = p->theThing;
        fprintf(fpw, "%d , %d %d %d %d , %s , %s , %f , %f , %f , %d , %d , %d , %s , %f\n",
                p->id, p->theDate.year, p->theDate.mon, p->theDate.day, p->theDate.hour, gd.Name, gd.Variety, gd.NetPrice, gd.BasePrice, gd.Quantity, gd.Measure, gd.Level, gd.StarLevel, p->Supplier, p->money);
        p = p->nxt;
    }
    fclose(fpw);
    return;
}
//--------------------保存销售记录到文件
void saveSalesRecord(record *L)
{
    FILE *fpw = fopen("salesRecord.csv", "w");
    if (fpw == NULL)
    {
        printf("打开文件失败!");
        return;
    }
    fprintf(fpw, "序号 , 时间 , 名称 , 品种 , 商品数量 , (单位) , 等级 , 星级 , 买家 , 手机尾号 , 供应者 , 金额\n");
    record *p = L->nxt;
    while (p != NULL)
    {
        good g = p->theThing;
        fprintf(fpw, "%d , %d %d %d %d , %s , %s , %f , %d , %d , %d , %s , %d , %s , %f\n",
                p->id, p->theDate.year, p->theDate.mon, p->theDate.day, p->theDate.hour,
                g.Name, g.Variety, g.Quantity, g.Measure, g.Level, g.StarLevel,
                p->Buyer, p->buyerPhonenumber, p->Supplier, p->money);
        p = p->nxt;
    }
    fclose(fpw);
    return;
}
//--------------------查找某good链表中的ID
good *idFind(int x, good *L)
{
    good *g = L->nxt;
    while (g != NULL)
        if (g->id == x)
            return g;
        else
            g = g->nxt;
    return NULL;
}
//--------------------将单个物品添加至购物车链表
good *addGoods2salesGoods(good *gd, float gcnt, int cnt, char buyername[50])
{
    good *q = (good *)malloc(sizeof(good));
    *q = *gd;                                                     //名称
    strcpy(q->Buyer, buyername);                                  //购买者
    strcpy(q->Supplier, "本店");                                  //提供者
    q->Quantity = gcnt;                                           //购买数量
    q->BasePrice = (gd->BasePrice) * freshCoefficient[gd->Level]; //基础价格*新鲜系数
    if (q->Measure == 1)
    {
        float quan = transWeigh(q->Quantity, q->Name);
        q->Quantity = quan;
        q->Measure = 0;
    }
    q->PackPrice = calcPack(q->Measure, q->Quantity); //包装费用
    q->frm = NULL;
    q->nxt = NULL;
    good *p = findGoodR(&salesGoods);
    q->frm = p;
    p->nxt = q;
    good *cpgd = (good *)malloc(sizeof(good));
    *cpgd = *q;
    return cpgd;
}
//--------------------找到good链表的尾结点
good *findGoodR(good *L)
{
    good *p = L;
    while (p->nxt != NULL)
        p = p->nxt;
    return p;
}
//--------------------合并salesGoods链表中ID相同的物品
void combineTheSame(good *L, int x)
{
    int occupiedSell[10005] = {0};
    good *g = L->nxt;
    good *p = NULL;
    while (g != NULL)
    {
        if (occupiedSell[g->id] == 0) //若该物品还没出现过，便记录出现，看下一个
        {
            occupiedSell[g->id] = 1;
            g = g->nxt;
        }
        else
        {
            if (x == 1)
            {
                p = idFind(g->id, &salesGoods);
            }
            if (x == 2)
            {
                p = idFind(g->id, &ordersGoods);
            }
            p->Quantity = p->Quantity + g->Quantity;
            (g->frm)->nxt = g->nxt;
            if (g->nxt != NULL)
                (g->nxt)->frm = g->frm;
            good *tmp = g;
            g = g->nxt;
            free(tmp);
        }
    }
}
//--------------------清空good链表，仅剩下表头
void clearGoodList(good *L)
{
    good *t = findGoodR(L);
    while (t->frm != NULL)
    {
        t = t->frm;
        free(t->nxt);
        t->nxt = NULL;
    }
}
//--------------------打印被本次出售物品
void printWhatWasBought(good *L)
{
    if (L->nxt == NULL)
    {
        printf("购物清单为空！");
        return;
    }
    good *g = L->nxt;
    printf("购物清单:\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("添加时间           |序号|名称    |品种      |基础价格        |库存量及单位 |等级 |保质期 |星级  |\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    float money = 0;
    float packprice = 0;
    int allcnt = 0;
    int couponLevel;
    while (g != NULL)
    {
        money += (float)(g->Quantity) * (g->BasePrice) + g->PackPrice;
        packprice += g->PackPrice;
        printf("%d年%02d月%02d日%02d时 ", g->indate.year, g->indate.mon, g->indate.day, g->indate.hour);
        printByPointer(g);
        if (!g->nxt)
            totalMoney += (float)(g->Quantity) * (g->BasePrice) + g->PackPrice;
        allcnt++;
        g = g->nxt;
    }
    printf("|总价一共是 %.2f|\n", money);
    printf("|包括满减折扣 %.2f|\n", couponLevel);
    printf("|包括包装费%.2f|\n", packprice);
    printf("\n");
    saveInformation();
    return;
}
//--------------------添加单个商品到销售记录链表
void add2salesRecord(good *L, int gcnt, char buyername[50], int pnumber)
{
    printf("添加出售商品到销售记录\n");
    record *q = (record *)malloc(sizeof(record));
    strcpy(q->Buyer, buyername);
    strcpy(q->Supplier, "本店");
    q->buyerPhonenumber = pnumber;
    q->theThing = *L;
    addTimeInfo(&q->theDate);
    q->id = idsales++;
    q->money = (q->theThing.BasePrice) * (q->theThing.Quantity) + q->theThing.PackPrice;
    q->nxt = NULL;
    q->frm = NULL;
    if (salesRecord.nxt == NULL)
    {
        salesRecord.nxt = q;
        q->frm = &salesRecord;
    }
    else
    {
        record *p = findRecordR(&salesRecord);
        q->frm = p;
        p->nxt = q;
    }
    return;
}
//--------------------个和斤的转换
float transWeigh(float quan, char *s)
{
    int typ;
    for (int i = 0; i < 100; ++i)
    {
        if (!strcmp(s, typeName[i]))
        {
            typ = i;
            break;
        }
    }
    quan = quan / typeWeight[typ];
    return quan;
}
//--------------------计算包装费用
float calcPack(int meas, float quan)
{
    float packprice;
    if (meas == 0 || meas == 1)
    {
        packprice = (ceil)(quan / 10) * 0.20;
        return packprice;
    }
    else if (meas == 2)
        return 8.0 * quan;
    else if (meas == 3)
        return 6.0 * quan;
    return -1;
}
//--------------------找到record链表的尾结点
record *findRecordR(record *L)
{
    record *p = L;
    while (p->nxt)
        p = p->nxt;
    return p;
}
//--------------------所有链表初始化
void initPtr()
{
    currentGoods.frm = NULL;
    currentGoods.nxt = NULL;
    sourceOfGoods.frm = NULL;
    sourceOfGoods.nxt = NULL;
    salesGoods.frm = NULL;
    salesGoods.nxt = NULL;
    ordersGoods.frm = NULL;
    ordersGoods.nxt = NULL;
    ordersRecord.frm = NULL;
    ordersRecord.nxt = NULL;
    allRecord.frm = NULL;
    allRecord.nxt = NULL;
    salesRecord.frm = NULL;
    salesRecord.nxt = NULL;
    ordersRecord.frm = NULL;
    ordersRecord.nxt = NULL;
    members.frm = NULL;
    members.nxt = NULL;
    supplyList.frm = NULL;
    supplyList.nxt = NULL;
}
//--------------------赋时间
void addTimeInfo(date *D)
{
    D->year = cntYear;
    D->mon = cntMon;
    D->day = cntDay;
    D->hour = cntHour;
}
//--------------------输出任意类型数据
int printAllTypeGood(good *p, int optA[])
{
    int i = 0;
    while (optA[i])
    {
        switch (optA[i]) // optb:output 该项后 break;
        {
        case 1:
            printf("   %-4d", p->id);
            optb; // id
        case 2:
            printf("%-4s ", measuringUnit[p->Measure]);
            optb; //单位
        case 3:
            printf("%-4s ", freshGood[p->Level]);
            optb; //新鲜度
        case 4:
            printf("%4d ", p->Decay);
            optb; //是否腐烂
        case 5:
            printf("%8d ", p->SellingWay);
            optb; //销售方式
        case 6:
            printf("%-6s ", star[p->StarLevel]);
            optb; //品质星级
        case 7:
            printf("%8.2f ", p->Quantity);
            optb; //数量
        case 8:
            printf("%8.2f ", p->BasePrice);
            optb; //零售价
        case 9:
            printf("%8.2f  ", p->NetPrice);
            optb; //批发价
        case 10:
            printf("%6.2f ", p->PackPrice);
            optb; //包装费
        case 11:
            printf("%-8s ", p->Name);
            optb; //名称
        case 12:
            printf("%-10s ", p->Variety);
            optb; //品种
        case 13:
            printf(" %-30s ", p->Supplier);
            optb; //供货商
        case 14:
            printf("%-10s ", p->Buyer);
            optb; // 购买人
        case 15:
            printf("%6dh", p->time.freshTime);
            optb; //保鲜时间
        case 16:
            printf("%6d ", p->time.addTime);
            optb; //进货时间
        case 17:
            printf("%8.2f ", (p->BasePrice) * (p->Quantity));
            optb; //单价*数量
        case 99:
            printf("     ");
            optb;
        case 98:
            printf("  ");
            optb;
        case 97:
            printf(" ");
            optb;
        case 90:
            printf("元/");
            optb;
        }
        ++i;
    }
    return i;
}
void printAllTypeRecord(record *q, int optA[])
{
    int i = 0;
    while (optA[i])
    {
        switch (optA[i])
        {
        case 13:
            printf("%-30s ", q->Supplier);
            optb;
        case 14:
            printf("%-6s ", q->Buyer);
            optb;
        case 17:
            i = i + 1 + printAllTypeGood(&(q->theThing), &(optA[0]) + i + 1);
            optb;
        case 18:
            printf("%d年%02d月%02d日%02d时 ", q->theDate.year, q->theDate.mon, q->theDate.day, q->theDate.hour);
            optb;
        case 19:
            printf("%8.2f", q->money);
            optb;
        case 20:
            printf("   %d\t ", q->id);
            optb;
        case 21:
            printf(" %04d", q->buyerPhonenumber);
            optb;
        case 99:
            printf("     ");
            optb;
        case 98:
            printf("  ");
            optb;
        case 97:
            printf(" ");
            optb;
        }
        ++i;
    }
}
//--------------------将单个物品添加至进货单链表
good *addGoods2ordersGoods(good *gd, float gcnt, int cnt, char *suppliername)
{
    good *q = (good *)malloc(sizeof(good));
    {
        *q = *gd;                                                     //名称
        strcpy(q->Supplier, suppliername);                            //购买者
        strcpy(q->Buyer, "本店");                                     //提供者
        q->Quantity = gcnt;                                           //购买数量
        q->NetPrice = (gd->NetPrice) * (freshCoefficient[gd->Level]); //基础价格*新鲜系数
        if (q->Measure == 1)
        {
            float quan = transWeigh(q->Quantity, q->Name);
            q->Quantity = quan;
            q->Measure = 0;
        }
        q->PackPrice = calcPack(q->Measure, q->Quantity); //包装费用
        q->frm = NULL;
        q->nxt = NULL;
    }
    good *p = findGoodR(&ordersGoods);
    q->frm = p;
    p->nxt = q;
    return q;
}
//--------------------添加单个商品到进货记录链表
void add2ordersRecord(good *L, int gcnt, char suppliername[50])
{
    printf("添加出售商品到进货记录\n");
    record *q = (record *)malloc(sizeof(record));
    strcpy(q->Buyer, "本店");
    strcpy(q->Supplier, suppliername);
    q->theThing = *L;
    addTimeInfo(&q->theDate);
    q->id = idorders++;
    q->money = (q->theThing.NetPrice) * (q->theThing.Quantity) + q->theThing.PackPrice;
    q->nxt = NULL;
    q->frm = NULL;
    if (ordersRecord.nxt == NULL)
    {
        ordersRecord.nxt = q;
        q->frm = &ordersRecord;
    }
    else
    {
        record *p = findRecordR(&ordersRecord);
        q->frm = p;
        p->nxt = q;
    }
    return;
}
//--------------------打印本次进货商品
void printWhatWasOrdered(good *L)
{
    if (L->nxt == NULL)
    {
        printf("进货清单为空！");
        return;
    }
    good *gd = L->nxt;
    printf("进货清单:\n");
    printf("-------------------------------------------------------------------------------------------------------------\n");
    printf("|编号\t|名称      |品种        |批发价  |建议零售价及单位  |等级|星级  |保质期|供应者                      |\n");
    printf("-------------------------------------------------------------------------------------------------------------\n");
    float money = 0;
    float packprice = 0;
    float totalMoney;
    while (gd != NULL)
    {
        money += (float)(gd->Quantity) * (gd->NetPrice) + gd->PackPrice;
        packprice += gd->PackPrice;
        int optA[40] = {1, 11, 98, 12, 97, 9, 99, 8, 2, 3, 6, 15, 13, 0};
        printAllTypeGood(gd, optA);
        printf("\n");
        printf("-------------------------------------------------------------------------------------------------------------\n");
        // printf("%d %s %s %.2f %.2f %s %s %s\n",gd->id, gd->Name, gd->Variety, gd->NetPrice, gd->BasePrice, measuringUnit[gd->Measure], star[gd->StarLevel], gd->Supplier);
        if (!gd->nxt)
            totalMoney -= (float)(gd->Quantity) * (gd->NetPrice) + gd->PackPrice;
        gd = gd->nxt;
    }
    printf("|总价一共是 %.2f|\n", money);
    printf("|包括包装费%.2f|\n", packprice);
    printf("\n");
    saveInformation();
    return;
}
//--------------------把进货单中的结点添加到当前货物中
void addOrders2Current(good *L)
{
    good *p = L->nxt;
    while (p != NULL)
    {
        good *g = (good *)malloc(sizeof(good)); //逐个创建空间，连接成链表
        *g = *p;
        g->id = idcnt++;
        occupied[g->id] = 1;
        good *t = findGoodR(&currentGoods);
        t->nxt = g;
        g->frm = t;
        t = g;
        t->nxt = NULL;
        p = p->nxt;
    }
    saveCurrentGoods(&currentGoods);
}
//--------------------修改商品品种信息
void fixGoodsVariety(good *p)
{
    char name[30];
    printf("请输入你想要改成的品种:");
    scanf("%s", name);
    strcpy(p->Variety, name);
    printf("修改成功!\n");
    saveCurrentGoods(&currentGoods);
}
//--------------------修改商品基本价格
void fixGoodsBasePrice(good *p)
{
    int price = 0;
    printf("请输入你想要改成的售价:");
    price = multiInput();
    while (price - 0 <= 1e-6)
    {
        printf("输入的售价不合理，请重新输入!\n");
        price = multiInput();
    }
    p->BasePrice = price;
    printf("修改成功!\n");
    saveCurrentGoods(&currentGoods);
}
//--------------------修改商品数量
void fixGoodsQuantity(good *p)
{
    int cnt;
    printf("请输入你想要改成的数量:");
    cnt = multiInput();
    while (cnt <= 0)
    {
        printf("输入的数量不合理，请重新输入!\n");
        cnt = multiInput();
    }
    p->Quantity = cnt;
    printf("修改成功!\n");
    saveCurrentGoods(&currentGoods);
}
//--------------------修改商品星级
void fixGoodsStarLevel(good *p)
{
    int level;
    printf("请输入你想要改成的星级:\n");
    printf("|*****--5\n");
    printf("|\n");
    printf("|****--4\n");
    printf("|\n");
    printf("|***--3\n");
    printf("|\n");
    printf("|**--2\n");
    printf("|\n");
    printf("|*--1\n");
    printf("|\n");
    level = multiInput();
    while (level <= 0 || level > 5)
    {
        printf("输入的数量不合理，请重新输入!\n");
        level = multiInput();
    }
    p->StarLevel = level;
    printf("修改成功!\n");
    saveCurrentGoods(&currentGoods);
}
//--------------------查找某record链表中的ID
record *idFindRecord(int x, record *L)
{
    record *g = L->nxt;
    while (g != NULL)
        if (g->id == x)
            return g;
        else
            g = g->nxt;
    return NULL;
}
//--------------------删除record链表结点
void delRecordNode(record *p)
{
    record *p0 = p->frm;
    p0->nxt = p->nxt;
    if (p->nxt != NULL)
        (p->nxt)->frm = p0;
    free(p);
}
//--------------------修改销售记录时间
void fixSalesRecordTime(record *p)
{
    int year, mon, day, hour;
    printf("请输入你想改成的时间, 格式同 2022 3 30 10, 表示2022年3月30日10时\n");
    year = multiInput();
    mon = multiInput();
    day = multiInput();
    hour = multiInput();
    date t = p->theDate;
    while (year > t.year)
    {
        printf("输入的年份不在范围内，请重新输入:\n");
        year = multiInput();
    }
    while (!((year < t.year && mon <= 12 && mon >= 1) || (year == t.year && mon <= t.mon && mon >= 1))) //绕晕了
    {
        printf("输入的月份不在范围内，请重新输入:\n");
        mon = multiInput();
    }
    while (day < 1 || day > monthDayRun[mon - 1])
    {
        printf("输入的日期不在范围内，请重新输入:\n");
        day = multiInput();
    }
    while (hour > 23 || hour < 0)
    {
        printf("输入的时间不在范围内，请重新输入:\n");
        hour = multiInput();
    }
    p->theDate.year = year;
    p->theDate.mon = mon;
    p->theDate.day = day;
    p->theDate.hour = hour;
    printf("修改成功\n");
    saveSalesRecord(&salesRecord);
    saveOrdersRecord(&ordersRecord);
}
//--------------------修改销售记录金额
void fixSalesRecordMoney(record *p)
{
    double money;
    printf("请输入你想改成的金额:");
    money = multiInput();
    while (money - 0 < 1e-6)
    {
        printf("输入的金额不在范围内，请重新输入:\n");
        money = multiInput();
    }
    p->money = money;
    printf("修改成功\n");
    saveSalesRecord(&salesRecord);
    saveOrdersRecord(&ordersRecord);
}
//--------------------修改销售记录买家名称
void fixSalesRecordBuyer(record *p)
{
    char name[30];
    printf("请输入你想改成的姓名:");
    scanf("%s", name);
    strcpy(p->Buyer, name);
    printf("修改成功\n");
    saveSalesRecord(&salesRecord);
    saveOrdersRecord(&ordersRecord);
}
//--------------------修改进货记录供货商名称
void fixOrdersRecordSupplier(record *p)
{
    char name[30];
    printf("请输入你想改成的姓名:");
    scanf("%s", name);
    strcpy(p->Supplier, name);
    printf("修改成功\n");
    saveSalesRecord(&salesRecord);
    saveOrdersRecord(&ordersRecord);
}

//--------------------//通过顾客名字找顾客
customer *findCustomer(char s[50])
{
    customer *p = members.nxt;
    while (p != NULL)
    {
        if (strcmp(p->name, s) == 0)
            return p;
        p = p->nxt;
    }
    return NULL;
}
//--------------------//读取顾客文件
void readMembers(customer *L)
{
    FILE *fp = fopen("members.csv", "r");
    while (fgetc(fp) != '\n')
        fseek(fp, 1, SEEK_CUR);
    int membercnt = 1;
    customer *g = NULL, *t = L;
    customer amember;
    if (fp == NULL)
    {
        printf("顾客文件读入错误!\n\n");
        Sleep(2000);
        fclose(fp);
        return;
    }
    else
    {
        char ch;
        while (fscanf(fp, "%s %c %f %c %d", &amember.name, &ch, &amember.points, &ch, &amember.phonenumber) != EOF)
        {
            amember.id = membercnt++;
            g = (customer *)malloc(sizeof(customer));
            *g = amember;
            t->nxt = g;
            g->frm = t;
            t = g;
            t->nxt = NULL;
        }
    }
    fclose(fp);
    return;
}
//--------------------保存时间和金钱
void saveInformation(void)
{
    FILE *fpw = fopen("information.csv", "w");
    if (fpw == NULL)
    {
        printf("打开店铺信息文件失败!\n\n");
        Sleep(2000);
        return;
    }
    fprintf(fpw, "日期(年/月/天/时) , 金钱 , 销售记录id , 进货记录id\n");
    fprintf(fpw, "%d %d %d %d , %f , %d , %d", cntYear, cntMon, cntDay, cntHour, totalMoney, idsales, idorders);
    fclose(fpw);
    return;
}
//--------------------保存顾客信息
void saveCusInformation(customer *L)
{
    FILE *fpw = fopen("members.csv", "w");
    if (fpw == NULL)
    {
        printf("打开顾客信息文件失败!\n\n");
        Sleep(2000);
        return;
    }
    customer *p = L->nxt;
    fprintf(fpw, "顾客名称 , 积分数量 , 手机尾号\n");
    while (p != NULL)
    {
        fprintf(fpw, "%s , %f , %d\n", p->name, p->points, p->phonenumber);
        p = p->nxt;
    }
    fclose(fpw);
    return;
}
//--------------------读取进货记录
void readOrdersRecord(record *L)
{
    FILE *fp = fopen("ordersRecord.csv", "r");
    if (fp == NULL)
    {
        printf("进货记录文件读入错误!\n\n");
        Sleep(2000);
        fclose(fp);
        return;
    }
    while (fgetc(fp) != '\n')
        fseek(fp, 1, SEEK_CUR);
    record *g = NULL, *t = L;
    record gd;
    char ch;
    while (fscanf(fp, "%d %c %d %d %d %d %c %s %c %s %c %f %c %f %c %f %c %d %c %d %c %d %c %s %c %f",
                  &gd.id, &ch, &gd.theDate.year, &gd.theDate.mon, &gd.theDate.day, &gd.theDate.hour, &ch,
                  gd.theThing.Name, &ch, gd.theThing.Variety, &ch, &gd.theThing.NetPrice, &ch, &gd.theThing.BasePrice, &ch, &gd.theThing.Quantity, &ch, &gd.theThing.Measure, &ch,
                  &gd.theThing.Level, &ch, &gd.theThing.StarLevel, &ch, gd.Supplier, &ch, &gd.money) != EOF)
    {
        strcpy(gd.Buyer, "本店");
        g = (record *)malloc(sizeof(record));
        *g = gd;
        t->nxt = g;
        g->frm = t;
        t = g;
        t->nxt = NULL;
    }
    fclose(fp);
    return;
}
//--------------------读取销售记录
void readSalesRecord(record *L)
{
    FILE *fp = fopen("salesRecord.csv", "r");
    if (fp == NULL)
    {
        printf("销售记录文件读入错误!\n\n");
        Sleep(2000);
        fclose(fp);
        return;
    }
    while (fgetc(fp) != '\n')
        fseek(fp, 1, SEEK_CUR);
    record *g = NULL, *t = L;
    record gd;
    char ch;
    while (fscanf(fp, "%d %c %d %d %d %d %c %s %c %s %c %f %c %d %c %d %c %d %c %s %c %d %c %s %c %f",
                  &gd.id, &ch, &gd.theDate.year, &gd.theDate.mon, &gd.theDate.day, &gd.theDate.hour,
                  &ch, gd.theThing.Name, &ch, gd.theThing.Variety, &ch, &gd.theThing.Quantity, &ch, &gd.theThing.Measure, &ch,
                  &gd.theThing.Level, &ch, &gd.theThing.StarLevel, &ch, gd.Buyer, &ch, &gd.buyerPhonenumber, &ch, gd.Supplier, &ch, &gd.money) != EOF)
    {
        g = (record *)malloc(sizeof(record));
        *g = gd;
        t->nxt = g;
        g->frm = t;
        t = g;
        t->nxt = NULL;
    }
    fclose(fp);
    return;
}
//--------------------判断满减等级
int calcCouponUnit(float tMoney)
{
    int i = 6;
    while (i >= 2)
    {
        if (tMoney >= couponUnit[i])
            return i - 1;
        i -= 2;
    }
    return 0;
}
//--------------------修改满减后的链表
void calcCouponMoney(good *L, int allcnt, int cUnit, float tMoney)
{
    good *g = L->nxt;
    printf("当前满减等级: %.1f-%.1f\n", couponUnit[cUnit + 1], couponUnit[cUnit]);
    printf("购物清单:\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("添加时间           |序号|名称    |品种      |基础价格        |库存量及单位 |等级 |保质期 |星级  |\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    float money;
    float packprice = 0;
    while (g != NULL)
    {
        float aveReduce = couponUnit[cUnit] * ((float)(g->Quantity) * (g->BasePrice) + g->PackPrice) / (tMoney);
        money = (float)(g->Quantity) * (g->BasePrice) + g->PackPrice - aveReduce;
        g->BasePrice = g->BasePrice - (aveReduce / g->Quantity);
        packprice += g->PackPrice;
        printf("%d年%02d月%02d日%02d时 ", g->indate.year, g->indate.mon, g->indate.day, g->indate.hour);
        printByPointer(g);
        totalMoney += money;
        g = g->nxt;
    }
    printf("|总价一共是 %.2f|\n", tMoney - couponUnit[cUnit]);
    printf("|包括满减折扣 %.2f|\n", couponUnit[cUnit]);
    printf("|包括包装费%.2f|\n", packprice);
    printf("\n");
    saveInformation();
    return;
}
//--------------------清空record链表，仅剩下表头
void clearRecordList(record *L)
{
    record *t = findRecordR(L);
    while (t->frm != NULL)
    {
        t = t->frm;
        free(t->nxt);
        t->nxt = NULL;
    }
}
//--------------------找到customer链表的尾结点
customer *findCustomerR(customer *L)
{
    customer *p = L;
    while (p->nxt)
        p = p->nxt;
    return p;
}
//--------------------清空customer链表，仅剩下表头
void clearCustomerList(customer *L)
{
    customer *t = findCustomerR(L);
    while (t->frm != NULL)
    {
        t = t->frm;
        free(t->nxt);
        t->nxt = NULL;
    }
}
//--------------------重置系统
void remake()
{
    system("cls");
    idcnt = 1, idsource = 1, idsales = 1, idorders = 1, membercnt = 1;
    clearGoodList(&currentGoods);
    prt("当前库存链表清理完毕!\n");
    clearGoodList(&salesGoods);
    prt("销售单链表清理完毕!\n");
    clearGoodList(&ordersGoods);
    prt("进货单链表清理完毕!\n");
    clearRecordList(&salesRecord);
    prt("销售记录链表清理完毕!\n");
    clearRecordList(&ordersRecord);
    prt("进货记录链表清理完毕!\n");
    clearCustomerList(&members);
    prt("会员信息链表清理完毕!\n");
    FILE *fpw = fopen("information.csv", "w");
    fprintf(fpw, "日期(年/月/天/时) , 金钱 , 销售记录id , 进货记录id\n");
    fclose(fpw);
    prt("店铺信息重置完毕!\n");
    fpw = fopen("currentGoods.csv", "w");
    fprintf(fpw, "添加时间 , 名称 , 品种 , 基础价格 , 数量 , 单位 , 新鲜度 , 星级\n");
    fclose(fpw);
    prt("当前库存信息重置完毕!\n");
    fpw = fopen("salesRecord.csv", "w");
    fprintf(fpw, "序号 , 时间 , 名称 , 品种 , 商品数量 , (单位) , 等级 , 星级 , 买家 , 手机尾号 , 供应者 , 金额\n");
    fclose(fpw);
    prt("销售记录信息重置完毕!\n");
    fpw = fopen("ordersRecord.csv", "w");
    fprintf(fpw, "序号 , 时间 , 名称 , 品种 , 批发价 , 零售价 , 商品数量 , (单位) , 等级 , 星级 , 供应者 , 金额\n");
    fclose(fpw);
    prt("进货记录信息重置完毕!\n");
    fpw = fopen("members.csv", "w");
    fprintf(fpw, "顾客名称 , 积分数量 , 手机尾号\n");
    prt("会员信息重置完毕!\n\n");
    fclose(fpw);
    printf("果蔬超市管理系统重置\n\n");
    printf("请输入起始资金(元)\n");
    totalMoney = multiInput();
    printf("请输入起始时间,格式如2022 04 09 16\n");
    cntYear = multiInput();
    cntMon = multiInput();
    cntDay = multiInput();
    cntHour = multiInput();
    saveInformation();
    prt("重置成功,回到主界面!\n");
}
//--------------------价格波动
void priceFluctuations()
{
    srand((int)time(0));
    float r = ((rand() % 40) + 80) / 100.0;
    printf("当前的价格系数是:%.2f\n", r);
    good *p = currentGoods.nxt;
    while (p != NULL)
    {
        if (r < 1 && p->BasePrice * r * r <= p->NetPrice)
            p->BasePrice = p->NetPrice;
        else
            p->BasePrice = p->BasePrice * r;
        p = p->nxt;
    }
    saveCurrentGoods(&currentGoods);
    printCurrentGoods(&currentGoods);
    return;
}
//--------------------混合输入，提取数字
double multiInput()
{
    char s[255];
    scanf("%s", &s);
    int lens = strlen(s);
    double inte = 0, floa = 0;
    int flag = 0, real = 0;
    for (int i = 0; i < lens; i++)
    {
        if (s[i] == '.')
            real = 1;
        if (s[i] >= '0' && s[i] <= '9')
        {
            flag = 1;
            if (real != 0)
            {
                real++;
                floa = floa * 10 + (s[i] - 48);
            }
            if (real == 0)
            {
                inte = inte * 10 + (s[i] - 48);
            }
        }
    }
    for (int i = 1; i <= real - 1; i++)
        floa = floa / 10.0;
    if (flag)
        return inte + floa;
    else
        return 0;
}
//--------------------打印时间提示
void printTimeNote()
{
    if (cntHour >= 22 || cntHour <= 2)
        printf("\n     晚安, 打工人. 城市的光里没有你.\n\n");
    if (cntHour > 2 && cntHour < 7)
        printf("\n     不眠之夜.\n\n");
    if (cntHour >= 7 && cntHour < 12)
        printf("\n     没吃早饭, 却想着中午吃什么.\n\n");
    if (cntHour >= 12 && cntHour < 18)
        printf("\n     emmm...晚上吃什么呢?\n\n");
    if (cntHour >= 18 && cntHour < 22)
        printf("\n     When the flowers start to fade.\n\n");
}
//--------------------改变颜色
void changeColor()
{
    srand((int)time(0));
    int r=rand()%3;
    switch(r)
    {
        case 0:
            system("color 0B");
            break;
        case 1:
            system("color 0E");
            break;
        case 2:
            system("color 0F");
            break;
    }
    return;
}
void reColor()
{
    system("color 0F");
}
//--------------------输出所有供货商
void printAllSuppliers(sup *L)
{
    sup *p = supplyList.nxt;
    printf("----------------------------------------------------\n");
    printf("序号 | 供货商                                     |\n");
    printf("----------------------------------------------------\n");
    while(p!=NULL)
    {
        printf(" %4d  %-30s\n",p->id,p->name);
        p=p->nxt;
        printf("----------------------------------------------------\n");
    }
    return ;
}
