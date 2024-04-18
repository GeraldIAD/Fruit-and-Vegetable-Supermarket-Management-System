//--------------------ͷ�ļ��ͺ��滻
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
    printf("���ڸ�ģʽ?<YES:1\\NO:0>"); \
    TO_GET_OUT= multiInput();
#define inMode     \
    system("cls"); \
    int TO_GET_OUT = 1;    \
    while (TO_GET_OUT)
#define optb break
#define maxn 10005
#define anHour 1
#define RATES 2
//--------------------��������
const int monthDayRun[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int monthDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //�·�����
const float measuringPriceCoefficient[15] = {0.20, 0.20, 8.00, 6.00};
const float couponUnit[7] = {0, 6.00, 100, 15.00, 200, 25.00, 300};
const char simmeasuringUnit[10][15] = {"jin", "ge", "lan", "xiang"};
const char star[10][15] = {"", "*", "**", "***", "****", "*****"};
const char freshGood[10][15] = {"ȫ��", "�Ը�", "һ��", "����"};
const char simpleMeasuringUnit[10][15] = {"j", "g", "l", "x"};
const char measuringUnit[10][15] = {"��", "��", "��", "��"};
const float freshCoefficient[15] = {0, 0.35, 0.60, 1.00};
//--------------------�ṹ�嶨��������
struct DATES//ʱ����
{
    int year, mon, day, hour;
};
typedef struct DATES date;
struct GOODS//��Ʒ���
{
    int id;            // 1 ���
    int Measure;       // 2 ��λ
    int Level;         // 3 ���ʵȼ�
    int Decay;         // 4 �Ƿ���
    int SellingWay;    // 5 ���۷�ʽ
    int StarLevel;     // 6 Ʒ���Ǽ�
    float Quantity;    // 7 ����
    float BasePrice;   // 8 ���ۼ�
    float NetPrice;    // 9 ������
    float PackPrice;   // 10 ��װ��
    char Name[30];     // 11 ����
    char Variety[30];  // 12 Ʒ��
    char Supplier[50]; // 13 ��Ӧ��
    char Buyer[50];    // 14 ������
    struct TIME
    {
        int freshTime; // 15 ������
        int addTime;   // 16 ����ʱ��
    } time;
    struct GOODS *nxt, *frm;
    date indate, outdate; // 23 24
};
typedef struct GOODS good;
struct RECORDS//��¼���
{
    int id;
    struct RECORDS *nxt, *frm;
    char Buyer[50];
    int buyerPhonenumber; // 21 �˿��ֻ�β��
    char Supplier[50];
    float money;
    good theThing; // 17 �������Ʒ��Ϣ
    date theDate;  // 18 ���������
    int type;      // ��������ͳ���
};
typedef struct RECORDS record;
struct BUYERS//�˿ͽ��
{
    int id;
    char name[50];
    float points;
    int level;
    int phonenumber;
    struct BUYERS *nxt, *frm;
};
typedef struct BUYERS customer;
struct SUPPLIERS//��Ӧ�̽��
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
//--------------------��������
int idcnt = 1, idsource = 1, idsales = 1, idorders = 1, membercnt = 1, cntYear, cntMon, cntDay, cntHour, cntSec, aDay = anHour * 24, aMonth = anHour * 24 * 30;
int typeTime[30], occupied[maxn];
char typeName[30][30], ss[255];
float typeWeight[30];
double totalMoney;
//--------------------��������
void calcCouponMoney(good *L, int allcnt, int cunit, float tMoney); //�޸������������
good *addGoods2ordersGoods(good *, float, int, char *);             //��ѡ�Ľ�����Ʒ��ӵ�����������
good *addGoods2salesGoods(good *, float, int, char *);              //�����۲�Ʒ��ӵ����۵�����
void add2salesRecord(good *, int, char *, int);                     //�����������ӵ����ۼ�¼����
void printAllTypeRecord(record *L, int optA[]);                     //ͨ�������ӡrecord�������Ϣ
void add2ordersRecord(good *, int, char *);                         //�ѽ�����Ϣ��ӵ�������¼����
int printAllTypeGood(good *p, int optA[]);                          //ͨ�������ӡgood�������Ϣ
void fixOrdersRecordSupplier(record *p);                            //�޸Ľ�����¼�еĹ�����
record *idFindRecord(int x, record *L);                             //Ѱ��record������ĳ��¼������ָ��
float transWeigh(float quan, char *s);                              //ת�����ͽ�ĵ�λ
float calcPack(int meas, float quan);                               //�����װ����
void saveCusInformation(customer *L);                               // ����˿���Ϣ
customer *findCustomerR(customer *L);                               //����customer�����β���
void combineTheSame(good *L, int x);                                //�����ۻ����ʱ��ͬ����Ʒ�ϲ��������ǹ����������Ĳ���
void fixSalesRecordMoney(record *p);                                //�޸����ۼ�¼�еĽ���Ǯ��
void fixSalesRecordBuyer(record *p);                                //�޸����ۼ�¼�е��������
void clearCustomerList(customer *L);                                //��չ˿�����
void fixSalesRecordTime(record *p);                                 //�޸����ۼ�¼�е�ʱ����Ϣ
customer *findCustomer(char s[50]);                                 //���ݹ˿�����Ѱ�ҹ˿�
void printOrdersRecord(record *L);                                  //��ӡ���н�����¼
void printWhatWasOrdered(good *L);                                  //��ӡ���ν�����¼
int calcCouponUnit(float tMoney);                                   //�ж�����
void printWhatWasBought(good *L);                                   //��ӡ�������ۼ�¼
void saveOrdersRecord(record *L);                                   //���������¼
void printSalesRecord(record *L);                                   //��ӡ�������ۼ�¼
void readOrdersRecord(record *L);                                   //��ȡ������¼����
void readSalesRecord(record *L);                                    //��ȡ���ۼ�¼����
double howAboutThisOne(good *p);                                    //���㸯�õĺ���
void fixOrdersRecord(record *L);                                    //�޸Ľ�����¼
void addOrders2Current(good *L);                                    //��ӽ�����Ϣ����ǰ�������
void fixGoodsBasePrice(good *p);                                    //�޸���Ʒ�����۸�
void saveSalesRecord(record *L);                                    //�������ۼ�¼���ļ�
void fixGoodsStarLevel(good *p);                                    //�ѽ���������ӵ���ǰ����У�������
void clearRecordList(record *L);                                    //�����¼����
record *findRecordR(record *L);                                     //����record����β���
void fixSalesRecord(record *L);                                     //�޸����ۼ�¼��Ϣ
void readCurrentGoods(good *L);                                     //��ȡ��ǰ����ļ�
void saveCurrentGoods(good *L);                                     //���浱ǰ��浽�ļ�
void fixGoodsQuantity(good *p);                                     //�޸���Ʒ������Ϣ
void printAllSuppliers(sup *L);                                     //��ӡ���н�������Ϣ
void fixGoodsVariety(good *p);                                      //�޸���ƷƷ��
void seekCustomers(record *L);                                      //���ҹ˿���Ϣ
void seekSuppliers(record *L);                                      //���ҹ�������Ϣ
int goCheckFreshness(good *L);                                      //���㸯�õĺ���
void printSourceList(good *L);                                      //��ӡ��Դ�б�
void readMembers(customer *L);                                      //��ȡ�˿���Ϣ
good *idFind(int x, good *L);                                       //ͨ��idѰ�ҿ���еĻ���
void printByPointer(good *p);                                       //ͨ��ָ���ӡĳ�ֻ�����
void readSourceList(good *L);                                       //��ȡ��Դ�б�
void delRecordNode(record *);                                       //ɾ��ĳ����¼
void clearGoodList(good *L);                                        //������ﳵ����
void seekSameKinds(good *L);                                        //Ѱ����ͬ���͵Ŀ����Ʒ
void dealWithDecay(good *L);                                        //��������
void fixGoodsLevel(good *p);                                        //�޸Ŀ�����
void saveInformation(void);                                         //���������Ϣ
void delGoodsNode(good *p);                                         //ɾ��ĳ�����
void readSuppliers(sup *L);                                         //��ȡ�������б�
int addFreshTime(char *s);                                          //���ӱ���������
void addTimeInfo(date *D);                                          //���ӽ�����������
void printCurrentGoods();                                           //��ӡ���п��
void priceFluctuations();                                           //����۸񲨶�
good *findGoodR(good *L);                                           //Ѱ��good�����β�ڵ�
void fixGoods(good *L);                                             //�޸�good�����ĳ���ڵ�
void statisticsMode();                                              //ͳ��ģʽ����
void printTimeNote();                                               //��ӡ��ͬʱ�����ʾ���
double multiInput();                                                //������룬��ȡ����
void suspendMode();                                                 //����ģʽ
void changeColor();                                                 //�޸Ľ�����ɫ
int getClockTime();                                                 //��ȡ��ǰʱ��
void queryMode();                                                   //��ѯģʽ����
void orderMode();                                                   //����ģʽ����
void sellMode();                                                    //����ģʽ����
void getToday();                                                    //��ȡ������Ϣ��������Ӫʱ�䡢��Ǯ��
void initType();                                                    //��ʼ��������Ʒ�Ĺ��
void welcome2();                                                    //��ӭ����
void initPtr();                                                     //��ʼ�����������ͷ�ڵ�
void fixInfo();                                                     //�޸���Ϣ
void goodbye();                                                     //�뿪����
void reColor();                                                     //�ָ�������ɫ
void remake();                                                      //����ϵͳ

//--------------------������
int main()
{
    system("mode con cols=140");
    getToday();
    initType();
    initPtr();
    readCurrentGoods(&currentGoods);
    prt("��ǰ����ȡ���!\n");
    readSalesRecord(&salesRecord);
    prt("���ۼ�¼��ȡ���!\n");
    readOrdersRecord(&ordersRecord);
    prt("������¼��ȡ���!\n");
    readMembers(&members);
    prt("��Ա��Ϣ��ȡ���!\n");
    readSuppliers(&supplyList);
    prt("������Ϣ��ȡ���!\n");
    //DEBUG("�ļ���ȡ����");
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
//--------------------��ӡ���˵�
void welcome2() //
{
    getToday();
    system("cls");
    printf("�q*�r������-������-������-������-������-������-������-������-������*\n");
    printf("�t*�s | DEV Gerald & Solokey & YiberWon |    ���߳��й���ϵͳ    |\n ");
    printf("   | ����ʱ���� %d��%02d��%02d��%02dʱ   |------------------------|\n", cntYear, cntMon, cntDay, cntHour);
    printf("    | �����ֽ�:%12.2lfԪ         |\n", totalMoney);
    printf("    |����-������-����-����-������-����|\n\n");
    cntSec = cntMon * aMonth + cntDay * aDay + cntHour * anHour;
    if(cntDay==8) printf("     �����ǻ�Ա��!!!ȫ�������ۿ���!\n");
    else printf("     ÿ��8����ȫ��������Ա��,�����עС��~~\n");
    printTimeNote();
    printf("    *������-������-������-������-����-������-������-������*\n");
    printf("    |   ͳ��ģʽ--1  |   ��ѯģʽ--2    |   �޸���Ϣ--3   |\n");
    printf("    |----------------|------------------|-----------------|\n");
    printf("    |  ----���п��  |  -----ͬ���߹�   |  -----�߹�����  |\n");
    printf("    |  ----���ۼ�¼  |  -----�˿ͼ�¼   |  -----���ۼ�¼  |\n");
    printf("    |  ----������¼  |  -----�����̼�¼ |  -----������¼  |\n");
    printf("    |----------------|------------------|-----------------|\n");
    printf("    |\n");
    printf("    |----------------|------------------|-----------------|\n");
    printf("    |   ����ģʽ--4  |   ����ģʽ--5    |   ����ģʽ--6   |\n");
    printf("    |----------------|------------------|-----------------|\n");
    printf("    |\n");
    printf("    |----------------|------------------|\n");
    printf("    |   ����ϵͳ--7  |     �˳�--0      |\n");
    printf("    |----------------|------------------|\n\n");
}
//--------------------�ټ�����
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
//--------------------��ȡʱ��ͽ�Ǯ
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
//--------------------�������п��
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
        printf("���п���ļ��������!\n");
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
            g = (good *)malloc(sizeof(good)); //��������ռ�,���ӳ�����
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
//--------------------���빩����
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
        printf("�������б��ļ��������!\n");
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
//--------------------�������ѡ��
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
        printf("����ѡ���ļ��������!\n");
        Sleep(2000);
        fclose(fp);
        return;
    }
    else
    {
        while (fscanf(fp, "%s %s %s %s %f %s %f %c %d %c %d %c %d %c %s\n",
                      &gd.Name, &ch, &gd.Variety, &ch, &gd.NetPrice, &ch, &gd.BasePrice, &ch, &gd.Measure, &ch, &gd.Level, &ch, &gd.StarLevel, &ch, &gd.Supplier) != EOF)
        {
            gd.Level = 3;                     //Ĭ�ϵĵȼ�������
            g = (good *)malloc(sizeof(good)); //��������ռ�,���ӳ�����
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
//--------------------�����ļ�����
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
//--------------------ͳ��ģʽ
void statisticsMode()
{
    printf("ͳ��ģʽ\n");
    inMode
    {
        int choice;
        system("cls"); // ����
        printf("    *������-������-������-������-����-������-������-������*\n");
        printf("    |   ���п��--1  |   ���ۼ�¼--2    |   ������¼--3   |\n");
        printf("    |----------------|------------------|-----------------|\n");
        printf("    |                      �˳�--0      |\n");
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
            printf("���˳���ģʽ\n");
            return;
        }
        }
        outMode
    }
}
//--------------------��ѯģʽ
void queryMode()
{
    printf("��ѯģʽ\n");
    int choice;
    system("cls");
    printf("    *������-������-������-������-����-������-������-������*\n");
    printf("    |   ͬ���߹�--1  |   �˿ͼ�¼--2    |  ��������Ϣ--3  |\n");
    printf("    |----------------|------------------|-----------------|\n");
    printf("    |                      �˳�--0      |\n");
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
        printf("���˳���ģʽ\n");
        return;
    }
    }
}
//--------------------�޸���Ϣ
void fixInfo()
{
    printf("�޸���Ϣ\n");
    inMode
    {
        int choice;
        system("cls");
        printf("    *������-������-������-������-����-������-������-------------������*\n");
        printf("    |   �޸��߹�����--1  |   �޸����ۼ�¼--2    |   �޸Ľ�����¼--3   |\n");
        printf("    |--------------------|----------------------|---------------------|\n");
        printf("    |                            �˳�--0        |\n");
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
            printf("���˳���ģʽ\n");
            return;
        }
        }
        outMode
    }
}
//--------------------����ģʽ
void sellMode()
{
    system("cls");
    printf("����ģʽ\n");
    if (cntHour >= 22 || cntHour < 7)
    {
        printf("����Ӫҵʱ��!\n");
        Sleep(1000);
        return;
    }
    if (!currentGoods.nxt)
    {
        printf("û����Ʒ�ɹ�ѡ��!\n");
        Sleep(1000);
        return;
    }
    char buyername[50];
    printf("�����빺��������<����0���˳�>");
    scanf("%s", buyername);
    printf("\n");
    if (!strcmp(buyername, "0"))
        return;
    printf("�������ֻ�β�ź���λ:");
    int pnumber;
    pnumber = multiInput();
    int cnt = 0;
    inMode
    {
        int gid = 0;
        float gcnt = 0;
        system("cls");
        if(cntDay==8) printf("�����ǻ�Ա��!!ȫ�������ۿ���!\n");
        printf("��ǰ������:%s\n", buyername);
        if (!currentGoods.nxt)
        {
            printf("û����Ʒ�ɹ�ѡ��!\n");
            Sleep(1000);
        }
        else
        {
            printCurrentGoods(&currentGoods);
            printf("�����빺�����ƷID\n");
            gid = multiInput();
            good *p = idFind(gid, &currentGoods);
            while (!p)
            {
                printf("����Ʒ������,����������\n");
                gid = multiInput();
                p = idFind(gid, &currentGoods);
            }
            printf("����Ʒ %s %s �Ŀ��Ϊ %.1f %s,���� %.2f:\n", p->Name, p->Variety, p->Quantity, measuringUnit[p->Measure], p->BasePrice * freshCoefficient[p->Level]);
            printf("�����빺������:");
            gcnt = multiInput();
            while (gcnt <= 0 || p->Quantity < gcnt)
            {
                printf("������������ڷ�Χ�ڣ�����������\n");
                gcnt = multiInput();
            }
            p->Quantity -= gcnt;
            saveCurrentGoods(&currentGoods);
            good *newThing = addGoods2salesGoods(p, gcnt, cnt, buyername); //���������Ʒ������������������һ��goodָ��
            combineTheSame(&salesGoods, 1);                                //�ϲ���ͬID��Ʒ
            add2salesRecord(newThing, gcnt, buyername, pnumber);           //���������Ʒ�����ۼ�¼����
            if (!p->Quantity)
                delGoodsNode(p);
            printf("��ӳɹ�!\n");
            cnt++;
            printWhatWasBought(&salesGoods); //��ӡ����������Ʒ����Ϣ
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
        printf("������%d��%d��,ȫ������!\n", cntMon, cntDay);
        couponFlag = 1;
    }
    if (pcus != NULL && (pcus->phonenumber == pnumber)) // �жϹ˿��Ƿ�Ϊ���Ա,���Ҽ�������
    {

        if (pcus->points >= 100)
        {
            couponFlag = 1;
            printf("��⵽�ù˿��ǻ�Ա!���������Ż�!\n");
        }
        else
        {
            printf("��⵽�ù˿��ǻ�Ա!�������ӻ�Ա����!\n");
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
        printf("��⵽�ù˿Ͳ��ǻ�Ա!���Զ������Ա�б�!\n");
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
        printf("���˿� %s ��ӵ��˿��б�ɹ�!\n", buyername);
        printWhatWasBought(&salesGoods);
        Sleep(300);
    }
    saveCusInformation(&members);
    saveSalesRecord(&salesRecord);
    clearGoodList(&salesGoods);
    printf("��������˳�...\n");
    getch();
}
//--------------------����ģʽ
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
        printf("�������������ƷID<����0���˳�>\n");
        gid = multiInput();
        if (gid == 0)
            break;
        good *p = idFind(gid, &sourceOfGoods);
        while (!p)
        {
            printf("����Ʒ������,����������<����0���˳�>\n");
            gid = multiInput();
            if (gid == 0)
            {
                saveOrdersRecord(&ordersRecord);
                clearGoodList(&ordersGoods);
                printf("��������˳�...\n");
                getch();
                return;
            }
            p = idFind(gid, &sourceOfGoods);
        }
        printf("����Ʒ %s %s �Ŀ��Ϊ 999+ %s,���� %.2f:\n", p->Name, p->Variety, measuringUnit[p->Measure], p->NetPrice * freshCoefficient[p->Level]);
        printf("�����빺������<����0���˳�>");
        gcnt = multiInput();
        if (!gcnt)
        {
            totalMoney-=moneyNow;
            printWhatWasOrdered(&ordersGoods);
            saveOrdersRecord(&ordersRecord);
            clearGoodList(&ordersGoods);
            printf("��������˳�...\n");
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
            printf("������������ڷ�Χ�ڣ�����������\n");
            gcnt = multiInput();
        }
        moneyNow += p->NetPrice * gcnt;
        strcpy(suppliername, p->Supplier);
        if (moneyNow > totalMoney)
        {
            printf("�ʽ���,�޷����!\n");
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
//--------------------����ģʽ
void suspendMode()
{
    system("cls");
    printf("����ģʽ\n");
    getToday();
    dealWithDecay(&currentGoods);
    saveCurrentGoods(&currentGoods);
    if (!currentGoods.nxt)
    {
        printf("û�п����Ʒ!����������?<Y--1/N--0>\n");
        Sleep(1000);
        int flag;
        flag = multiInput();
        if (!flag)
            return;
    }
    inMode
    {
        system("cls");
        printf("ϵͳ�����������ģʽ\n");
        printf("����ʱ���ǣ�%d��%d��%d��%dʱ\n", cntYear, cntMon, cntDay, cntHour);
        int initial_Time = getClockTime(); //���ν������ģʽ�ĳ�ʼʱ��
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
            if (lasthour != pasthour) //��ʱ���б仯ʱ
            {
                lasthour = pasthour;
                system("cls");
                printf("ϵͳ�ѽ������ģʽ,��������˳�\n");
                printmonth = cntMon + pastmonth;
                printday = (cntDay + pastday) + (cntHour + pasthour) / aDay;
                if (printday > 30) //���һ���µ�ʱ�䳬��30��
                {
                    printday = 1;
                    printmonth++;
                }
                printhour = (cntHour + pasthour) % aDay;
                printf("����ʱ���ǣ�%d��%d��%d��%dʱ\n", cntYear, printmonth, printday, printhour);
                cntSec = printmonth * aMonth + printday * aDay + printhour * anHour;
                printCurrentGoods(&currentGoods);
                result = goCheckFreshness(&currentGoods);
                if (result)
                    break;
            }
        }
        system("cls");
        printf("����ģʽ���˳�\n");
        cntMon = printmonth;
        cntDay = printday;
        cntHour = printhour;
        printf("����ʱ���ǣ�%d��%d��%d��%dʱ\n", cntYear, cntMon, cntDay, cntHour);
        saveInformation();
        if (result)
            dealWithDecay(&currentGoods);
        printf("���ڿ�ʼ���м۸񲨶�...\n");
        priceFluctuations();
        outMode
    }
}
//--------------------�˵��鿴**
void billMode()
{
    printf("�˵��鿴\n");
}
//--------------------����ʱ���滻
int addFreshTime(char *s)
{
    for (int i = 0; i < 25; i++)
    {
        if (strcmp(s, typeName[i]) == 0)
            return i;
    }
    return 0;
}
//--------------------��ӡ��ǰ���***
void printCurrentGoods(good *L)
{
    good *p = L->nxt;
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("���ʱ��           |���|����    |Ʒ��      |�����۸�        |���������λ |�ȼ� |������ |�Ǽ�  |\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    if (p != NULL)
    {
        while (p != NULL) // ȫ�����ֱ���������
        {
            printf("%d��%02d��%02d��%02dʱ", p->indate.year, p->indate.mon, p->indate.day, p->indate.hour);
            printByPointer(p);
            p = p->nxt;
        }
    }
}
//--------------------��ӡ��Դ�б�***
void printSourceList(good *L)
{
    printf("----------------------------------------------------------------------------------------------------------\n");
    printf("|���\t|����    |Ʒ��     |������     |���ۼۼ���λ   |�ȼ�|�Ǽ�  |������|��Ӧ��                        |\n");
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
//--------------------��ӡ������¼***
void printOrdersRecord(record *L)
{
    record *q = L->nxt;
    printf("----------------------------------------------------------------------------------------------------------------------------\n");
    printf("���\t|ʱ��\t\t   |����    |Ʒ��    |��Ʒ����(��λ) |�ȼ� |�Ǽ�  |���  |��Ӧ��                         |���    |\n");
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
//--------------------��ӡ���ۼ�¼***
void printSalesRecord(record *L)
{
    record *q = L->nxt;
    printf("------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("���\t|ʱ��\t\t   |����    |Ʒ��    |��Ʒ����(��λ) |�ȼ� |�Ǽ�  |���  |���β�� |��Ӧ��                           |��� |\n");
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
//--------------------ͨ��ָ�뵥����ӡ***
void printByPointer(good *p)
{
    int optA[40] = {1, 11, 12, 8, 90, 2, 7, 2, 3, 15, 98, 6, 0};
    printAllTypeGood(p, optA);
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------\n");
}
//--------------------����ͬ���߹�
void seekSameKinds(good *L)
{
    char serName[20];
    int flag = 0;
    system("cls");
    printf("���������ѯ�߹�����<����0���˳�>\n");
    scanf("%s", serName);
    if (!strcmp(serName, "0"))
        return;
    good *g = L->nxt;
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("���ʱ��           |���|����    |Ʒ��      |�����۸�        |���������λ |�ȼ� |������ |�Ǽ�  |\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    while (g != NULL)
    {
        if (!strcmp(serName, g->Name))
        {
            flag = 1;
            printf("%d��%02d��%02d��%02dʱ", g->indate.year, g->indate.mon, g->indate.day, g->indate.hour);
            printByPointer(g);
        }
        g = g->nxt;
    }
    if (!flag)
        printf("δ���ҵ���Ӧ�����߹�!\n");
    printf("��������˳�...\n");
    getch();
    return;
}
//--------------------����ĳλ�˿���Ϣ
void seekCustomers(record *L)
{
    printf("---------------------------------\n");
    printf("|���|�˿�����|��������|�ֻ�β��|\n");
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
    printf("���������ѯ�Ĺ˿�����<����0���˳�>");
    char buyerName[20];
    int buyerPhone;
    int flag = 0;
    scanf("%s", buyerName);
    if (strcmp(buyerName, "0") == 0)
        return;
    printf("���������ѯ�˿͵��ֻ��ź���λ<����0���˳�>");
    buyerPhone = multiInput();
    if (buyerPhone == 0)
        return;
    record *g = L->nxt;
    printf("------------------------------------------------------------------------------------------------------------------\n");
    printf("|ʱ��              |����    |Ʒ��    |��Ʒ����(��λ) |�ȼ�|�Ǽ� |���   |��Ӧ��                          |���   |\n");
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
        printf("δ���ҵ��ù˿�!\n");
    printf("��������˳�...\n");
    getch();
    return;
}
//--------------------����ĳλ��������Ϣ
void seekSuppliers(record *L)
{
    char supName[50];
    int flag = 0;
    record *g = L->nxt;
    system("cls");
    printAllSuppliers(&supplyList);
    printf("-------------------------------------------------------------------------------------------------------------------\n");
    printf("|ʱ��                |����    |Ʒ��    |��Ʒ����(��λ) |�ȼ�|�Ǽ�  |���  |��Ӧ��                       |���     |\n");
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
    printf("��������Ҫ�����鿴�Ĺ�����id<����0���˳�>");
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
                printf("|ʱ��                |����    |Ʒ��    |��Ʒ����(��λ) |�ȼ�|�Ǽ�  |���  |��Ӧ��                       |���     |\n");
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
        printf("δ���ҵ��ù�����!\n");
    printf("��������˳�...\n");
    getch();
    return;
}
//--------------------�޸��߹�����
void fixGoods(good *L)
{
    inMode
    {
        int choice = 0;
        printCurrentGoods(&currentGoods);
        printf("��������Ҫ�޸ĵ���Ʒid<����0�˳�>\n");
        int gid = 0;
        gid = multiInput();
        if (gid == 0) return;
        good *p = idFind(gid, &currentGoods);
        while (!p)
        {
            printf("����Ʒ������,����������\n");
            gid = multiInput();
            p = idFind(gid, &currentGoods);
        }
        printf("-------------------------------------------------------------------------------------------------\n");
        printf("���ʱ��           |���|����    |Ʒ��      |�����۸�        |���������λ |�ȼ� |������ |�Ǽ�  |\n");
        printf("-------------------------------------------------------------------------------------------------\n");
        printf("%d��%02d��%02d��%02dʱ ", p->indate.year, p->indate.mon, p->indate.day, p->indate.hour);
        printByPointer(p);
        printf("|�޸�Ʒ��--1\n");
        printf("|\n");
        printf("|�޸ļ۸�--2\n");
        printf("|\n");
        printf("|�޸Ŀ��--3\n");
        printf("|\n");
        printf("|�޸��Ǽ�--4\n");
        printf("|\n");
        printf("|ɾ������Ʒ--5\n");
        printf("|\n");
        printf("|�˳�--0\n");

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
            printf("ȷ��ɾ����?<YES:1\\NO:0>\n");
            int confirm = 0;
            confirm = multiInput();
            if (confirm)
            {
                delGoodsNode(p);
                printf("ɾ���ɹ�!\n");
            }
            else
            {
                printf("δɾ������Ʒ!\n");
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
//--------------------�޸����ۼ�¼
void fixSalesRecord(record *L)
{
    int choice = 0;
    if (salesRecord.nxt == NULL)
    {
        printf("�������ۼ�¼��\n");
        printf("��������˳�...\n");
        getch();
        return;
    }
    printSalesRecord(&salesRecord);
    printf("��������Ҫ�޸ĵļ�¼id<����0���˳�>");
    int gid = 0;
    gid = multiInput();
    if (gid == 0)
        return;
    record *p = idFindRecord(gid, &salesRecord);
    while (!p)
    {
        printf("�ü�¼������,����������<����0���˳�>\n");
        gid = multiInput();
        if (gid == 0)
            return;
        p = idFindRecord(gid, &salesRecord);
    }
    printf("|�޸�ʱ��--1\n");
    printf("|\n");
    printf("|�޸Ľ��--2\n");
    printf("|\n");
    printf("|�޸��������--3\n");
    printf("|\n");
    printf("|ɾ��������¼(�˻�)--4\n");
    printf("|\n");
    printf("|�˳�--0\n");

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
        printf("ȷ��ɾ����?<YES:1\\NO:0>\n");
        int confirm = 0;
        confirm = multiInput();
        if (confirm)
        {
            totalMoney -= p->money;
            printf("�˻�Ǯ��Ϊ:%.2f", p->money);
            strcpy(p->Supplier, "����Ʒ���˿�");
            // delRecordNode(p);
            saveSalesRecord(&salesRecord);
            saveInformation();
            printf("ɾ���ɹ�!\n");
            printf("��������˳�...\n");
            getch();
        }
        else
        {
            printf("δɾ���ü�¼!\n");
        }
        break;
    }
    case 0:
        system("cls");
        return;
    }
}
//--------------------�޸Ľ�����¼
void fixOrdersRecord(record *L)
{
    int choice = 0;
    if (ordersRecord.nxt == NULL)
    {
        printf("���޽�����¼!\n");
        printf("��������˳�...\n");
        getch();
        return;
    }
    printOrdersRecord(&ordersRecord);
    printf("��������Ҫ�޸ĵļ�¼id<����0���˳�>");
    int gid = 0;
    gid = multiInput();
    if (gid == 0)
        return;
    record *p = idFindRecord(gid, &ordersRecord);
    while (!p)
    {
        printf("�ü�¼������,����������\n");
        gid = multiInput();
        p = idFindRecord(gid, &salesRecord);
    }
    printf("|�޸�ʱ��--1\n");
    printf("|\n");
    printf("|�޸Ľ��--2\n");
    printf("|\n");
    printf("|�޸Ĺ���������--3\n");
    printf("|\n");
    printf("|ɾ��������¼--4\n");
    printf("|\n");
    printf("|�˳�--0\n");

    choice = multiInput();
    switch (choice)
    {
    case 1:
        fixSalesRecordTime(p);
        break; //������salesRecord�ĺ�������Ϊ������һ����
    case 2:
        fixSalesRecordMoney(p);
        break; //������salesRecord�ĺ�������Ϊ������һ����
    case 3:
        fixOrdersRecordSupplier(p);
        break;
    case 4:
    {
        system("cls");
        printf("ȷ��ɾ����?<YES:1\\NO:0>\n");
        int confirm = 0;
        confirm = multiInput();
        if (confirm)
        {
            delRecordNode(p);
            saveOrdersRecord(&ordersRecord);
            printf("ɾ���ɹ�!\n");
            printf("��������˳�...\n");
            getch();
        }
        else
        {
            printf("δɾ���ü�¼!\n");
        }
        break;
    }
    case 0:
        system("cls");
        return;
    }
}

//--------------------��ó�������ʱ��
int getClockTime() { return clock() / CLOCKS_PER_SEC; }
//--------------------�����������ʳ̶�
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
//--------------------��鵥���������ʳ̶�
double howAboutThisOne(good *p)
{
    int nowtime = cntSec;
    double rates = (p->time.freshTime * 1.0 - (nowtime - p->time.addTime) * 1.0) / p->time.freshTime;//��ʽ����
    return rates;
}
//--------------------��������
void dealWithDecay(good *L)
{
    good *g = L->nxt;
    good *g0;
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("���ʱ��           |���|����    |Ʒ��      |�����۸�        |���������λ |�ȼ� |������ |�Ǽ�  |\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    while (g != NULL)
    {
        if (g->Level <= 0)
        {
            printf("%d��%02d��%02d��%02dʱ ", g->indate.year, g->indate.mon, g->indate.day, g->indate.hour);
            printByPointer(g);
            printf("\n");
            g0 = g;
            g = g->nxt;
            delGoodsNode(g0);
        }
        else
            g = g->nxt;
    }
    printf("��ȫ�������ѱ�����\n");
    return;
}
//--------------------ͨ��ָ��ɾ���߹������һ�����
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
//--------------------���浱ǰ��浽�ļ�
void saveCurrentGoods(good *L)
{
    FILE *fpw = fopen("currentGoods.csv", "w");
    if (fpw == NULL)
    {
        printf("���ļ�ʧ��!");
        return;
    }
    fprintf(fpw, "���ʱ�� , ���� , Ʒ�� , �����۸� , ���� , ��λ , ���ʶ� , �Ǽ�\n");
    good *p = L->nxt;
    while (p != NULL)
    {
        fprintf(fpw, "%d %d %d %d , %s , %s , %f , %f , %d , %d , %d\n", p->indate.year, p->indate.mon, p->indate.day, p->indate.hour, p->Name, p->Variety, p->BasePrice, p->Quantity, p->Measure, p->Level, p->StarLevel);
        p = p->nxt;
    }
    fclose(fpw);
    return;
}
//--------------------���������¼���ļ�
void saveOrdersRecord(record *L)
{
    FILE *fpw = fopen("ordersRecord.csv", "w");
    if (fpw == NULL)
    {
        printf("���ļ�ʧ��!");
        return;
    }
    fprintf(fpw, "��� , ʱ�� , ���� , Ʒ�� , ������ , ���ۼ� , ��Ʒ���� , (��λ) , �ȼ� , �Ǽ� , ��Ӧ�� , ���\n");
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
//--------------------�������ۼ�¼���ļ�
void saveSalesRecord(record *L)
{
    FILE *fpw = fopen("salesRecord.csv", "w");
    if (fpw == NULL)
    {
        printf("���ļ�ʧ��!");
        return;
    }
    fprintf(fpw, "��� , ʱ�� , ���� , Ʒ�� , ��Ʒ���� , (��λ) , �ȼ� , �Ǽ� , ��� , �ֻ�β�� , ��Ӧ�� , ���\n");
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
//--------------------����ĳgood�����е�ID
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
//--------------------��������Ʒ��������ﳵ����
good *addGoods2salesGoods(good *gd, float gcnt, int cnt, char buyername[50])
{
    good *q = (good *)malloc(sizeof(good));
    *q = *gd;                                                     //����
    strcpy(q->Buyer, buyername);                                  //������
    strcpy(q->Supplier, "����");                                  //�ṩ��
    q->Quantity = gcnt;                                           //��������
    q->BasePrice = (gd->BasePrice) * freshCoefficient[gd->Level]; //�����۸�*����ϵ��
    if (q->Measure == 1)
    {
        float quan = transWeigh(q->Quantity, q->Name);
        q->Quantity = quan;
        q->Measure = 0;
    }
    q->PackPrice = calcPack(q->Measure, q->Quantity); //��װ����
    q->frm = NULL;
    q->nxt = NULL;
    good *p = findGoodR(&salesGoods);
    q->frm = p;
    p->nxt = q;
    good *cpgd = (good *)malloc(sizeof(good));
    *cpgd = *q;
    return cpgd;
}
//--------------------�ҵ�good�����β���
good *findGoodR(good *L)
{
    good *p = L;
    while (p->nxt != NULL)
        p = p->nxt;
    return p;
}
//--------------------�ϲ�salesGoods������ID��ͬ����Ʒ
void combineTheSame(good *L, int x)
{
    int occupiedSell[10005] = {0};
    good *g = L->nxt;
    good *p = NULL;
    while (g != NULL)
    {
        if (occupiedSell[g->id] == 0) //������Ʒ��û���ֹ������¼���֣�����һ��
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
//--------------------���good������ʣ�±�ͷ
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
//--------------------��ӡ�����γ�����Ʒ
void printWhatWasBought(good *L)
{
    if (L->nxt == NULL)
    {
        printf("�����嵥Ϊ�գ�");
        return;
    }
    good *g = L->nxt;
    printf("�����嵥:\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("���ʱ��           |���|����    |Ʒ��      |�����۸�        |���������λ |�ȼ� |������ |�Ǽ�  |\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    float money = 0;
    float packprice = 0;
    int allcnt = 0;
    int couponLevel;
    while (g != NULL)
    {
        money += (float)(g->Quantity) * (g->BasePrice) + g->PackPrice;
        packprice += g->PackPrice;
        printf("%d��%02d��%02d��%02dʱ ", g->indate.year, g->indate.mon, g->indate.day, g->indate.hour);
        printByPointer(g);
        if (!g->nxt)
            totalMoney += (float)(g->Quantity) * (g->BasePrice) + g->PackPrice;
        allcnt++;
        g = g->nxt;
    }
    printf("|�ܼ�һ���� %.2f|\n", money);
    printf("|���������ۿ� %.2f|\n", couponLevel);
    printf("|������װ��%.2f|\n", packprice);
    printf("\n");
    saveInformation();
    return;
}
//--------------------��ӵ�����Ʒ�����ۼ�¼����
void add2salesRecord(good *L, int gcnt, char buyername[50], int pnumber)
{
    printf("��ӳ�����Ʒ�����ۼ�¼\n");
    record *q = (record *)malloc(sizeof(record));
    strcpy(q->Buyer, buyername);
    strcpy(q->Supplier, "����");
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
//--------------------���ͽ��ת��
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
//--------------------�����װ����
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
//--------------------�ҵ�record�����β���
record *findRecordR(record *L)
{
    record *p = L;
    while (p->nxt)
        p = p->nxt;
    return p;
}
//--------------------���������ʼ��
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
//--------------------��ʱ��
void addTimeInfo(date *D)
{
    D->year = cntYear;
    D->mon = cntMon;
    D->day = cntDay;
    D->hour = cntHour;
}
//--------------------���������������
int printAllTypeGood(good *p, int optA[])
{
    int i = 0;
    while (optA[i])
    {
        switch (optA[i]) // optb:output ����� break;
        {
        case 1:
            printf("   %-4d", p->id);
            optb; // id
        case 2:
            printf("%-4s ", measuringUnit[p->Measure]);
            optb; //��λ
        case 3:
            printf("%-4s ", freshGood[p->Level]);
            optb; //���ʶ�
        case 4:
            printf("%4d ", p->Decay);
            optb; //�Ƿ���
        case 5:
            printf("%8d ", p->SellingWay);
            optb; //���۷�ʽ
        case 6:
            printf("%-6s ", star[p->StarLevel]);
            optb; //Ʒ���Ǽ�
        case 7:
            printf("%8.2f ", p->Quantity);
            optb; //����
        case 8:
            printf("%8.2f ", p->BasePrice);
            optb; //���ۼ�
        case 9:
            printf("%8.2f  ", p->NetPrice);
            optb; //������
        case 10:
            printf("%6.2f ", p->PackPrice);
            optb; //��װ��
        case 11:
            printf("%-8s ", p->Name);
            optb; //����
        case 12:
            printf("%-10s ", p->Variety);
            optb; //Ʒ��
        case 13:
            printf(" %-30s ", p->Supplier);
            optb; //������
        case 14:
            printf("%-10s ", p->Buyer);
            optb; // ������
        case 15:
            printf("%6dh", p->time.freshTime);
            optb; //����ʱ��
        case 16:
            printf("%6d ", p->time.addTime);
            optb; //����ʱ��
        case 17:
            printf("%8.2f ", (p->BasePrice) * (p->Quantity));
            optb; //����*����
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
            printf("Ԫ/");
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
            printf("%d��%02d��%02d��%02dʱ ", q->theDate.year, q->theDate.mon, q->theDate.day, q->theDate.hour);
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
//--------------------��������Ʒ���������������
good *addGoods2ordersGoods(good *gd, float gcnt, int cnt, char *suppliername)
{
    good *q = (good *)malloc(sizeof(good));
    {
        *q = *gd;                                                     //����
        strcpy(q->Supplier, suppliername);                            //������
        strcpy(q->Buyer, "����");                                     //�ṩ��
        q->Quantity = gcnt;                                           //��������
        q->NetPrice = (gd->NetPrice) * (freshCoefficient[gd->Level]); //�����۸�*����ϵ��
        if (q->Measure == 1)
        {
            float quan = transWeigh(q->Quantity, q->Name);
            q->Quantity = quan;
            q->Measure = 0;
        }
        q->PackPrice = calcPack(q->Measure, q->Quantity); //��װ����
        q->frm = NULL;
        q->nxt = NULL;
    }
    good *p = findGoodR(&ordersGoods);
    q->frm = p;
    p->nxt = q;
    return q;
}
//--------------------��ӵ�����Ʒ��������¼����
void add2ordersRecord(good *L, int gcnt, char suppliername[50])
{
    printf("��ӳ�����Ʒ��������¼\n");
    record *q = (record *)malloc(sizeof(record));
    strcpy(q->Buyer, "����");
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
//--------------------��ӡ���ν�����Ʒ
void printWhatWasOrdered(good *L)
{
    if (L->nxt == NULL)
    {
        printf("�����嵥Ϊ�գ�");
        return;
    }
    good *gd = L->nxt;
    printf("�����嵥:\n");
    printf("-------------------------------------------------------------------------------------------------------------\n");
    printf("|���\t|����      |Ʒ��        |������  |�������ۼۼ���λ  |�ȼ�|�Ǽ�  |������|��Ӧ��                      |\n");
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
    printf("|�ܼ�һ���� %.2f|\n", money);
    printf("|������װ��%.2f|\n", packprice);
    printf("\n");
    saveInformation();
    return;
}
//--------------------�ѽ������еĽ����ӵ���ǰ������
void addOrders2Current(good *L)
{
    good *p = L->nxt;
    while (p != NULL)
    {
        good *g = (good *)malloc(sizeof(good)); //��������ռ䣬���ӳ�����
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
//--------------------�޸���ƷƷ����Ϣ
void fixGoodsVariety(good *p)
{
    char name[30];
    printf("����������Ҫ�ĳɵ�Ʒ��:");
    scanf("%s", name);
    strcpy(p->Variety, name);
    printf("�޸ĳɹ�!\n");
    saveCurrentGoods(&currentGoods);
}
//--------------------�޸���Ʒ�����۸�
void fixGoodsBasePrice(good *p)
{
    int price = 0;
    printf("����������Ҫ�ĳɵ��ۼ�:");
    price = multiInput();
    while (price - 0 <= 1e-6)
    {
        printf("������ۼ۲���������������!\n");
        price = multiInput();
    }
    p->BasePrice = price;
    printf("�޸ĳɹ�!\n");
    saveCurrentGoods(&currentGoods);
}
//--------------------�޸���Ʒ����
void fixGoodsQuantity(good *p)
{
    int cnt;
    printf("����������Ҫ�ĳɵ�����:");
    cnt = multiInput();
    while (cnt <= 0)
    {
        printf("�������������������������!\n");
        cnt = multiInput();
    }
    p->Quantity = cnt;
    printf("�޸ĳɹ�!\n");
    saveCurrentGoods(&currentGoods);
}
//--------------------�޸���Ʒ�Ǽ�
void fixGoodsStarLevel(good *p)
{
    int level;
    printf("����������Ҫ�ĳɵ��Ǽ�:\n");
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
        printf("�������������������������!\n");
        level = multiInput();
    }
    p->StarLevel = level;
    printf("�޸ĳɹ�!\n");
    saveCurrentGoods(&currentGoods);
}
//--------------------����ĳrecord�����е�ID
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
//--------------------ɾ��record������
void delRecordNode(record *p)
{
    record *p0 = p->frm;
    p0->nxt = p->nxt;
    if (p->nxt != NULL)
        (p->nxt)->frm = p0;
    free(p);
}
//--------------------�޸����ۼ�¼ʱ��
void fixSalesRecordTime(record *p)
{
    int year, mon, day, hour;
    printf("����������ĳɵ�ʱ��, ��ʽͬ 2022 3 30 10, ��ʾ2022��3��30��10ʱ\n");
    year = multiInput();
    mon = multiInput();
    day = multiInput();
    hour = multiInput();
    date t = p->theDate;
    while (year > t.year)
    {
        printf("�������ݲ��ڷ�Χ�ڣ�����������:\n");
        year = multiInput();
    }
    while (!((year < t.year && mon <= 12 && mon >= 1) || (year == t.year && mon <= t.mon && mon >= 1))) //������
    {
        printf("������·ݲ��ڷ�Χ�ڣ�����������:\n");
        mon = multiInput();
    }
    while (day < 1 || day > monthDayRun[mon - 1])
    {
        printf("��������ڲ��ڷ�Χ�ڣ�����������:\n");
        day = multiInput();
    }
    while (hour > 23 || hour < 0)
    {
        printf("�����ʱ�䲻�ڷ�Χ�ڣ�����������:\n");
        hour = multiInput();
    }
    p->theDate.year = year;
    p->theDate.mon = mon;
    p->theDate.day = day;
    p->theDate.hour = hour;
    printf("�޸ĳɹ�\n");
    saveSalesRecord(&salesRecord);
    saveOrdersRecord(&ordersRecord);
}
//--------------------�޸����ۼ�¼���
void fixSalesRecordMoney(record *p)
{
    double money;
    printf("����������ĳɵĽ��:");
    money = multiInput();
    while (money - 0 < 1e-6)
    {
        printf("����Ľ��ڷ�Χ�ڣ�����������:\n");
        money = multiInput();
    }
    p->money = money;
    printf("�޸ĳɹ�\n");
    saveSalesRecord(&salesRecord);
    saveOrdersRecord(&ordersRecord);
}
//--------------------�޸����ۼ�¼�������
void fixSalesRecordBuyer(record *p)
{
    char name[30];
    printf("����������ĳɵ�����:");
    scanf("%s", name);
    strcpy(p->Buyer, name);
    printf("�޸ĳɹ�\n");
    saveSalesRecord(&salesRecord);
    saveOrdersRecord(&ordersRecord);
}
//--------------------�޸Ľ�����¼����������
void fixOrdersRecordSupplier(record *p)
{
    char name[30];
    printf("����������ĳɵ�����:");
    scanf("%s", name);
    strcpy(p->Supplier, name);
    printf("�޸ĳɹ�\n");
    saveSalesRecord(&salesRecord);
    saveOrdersRecord(&ordersRecord);
}

//--------------------//ͨ���˿������ҹ˿�
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
//--------------------//��ȡ�˿��ļ�
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
        printf("�˿��ļ��������!\n\n");
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
//--------------------����ʱ��ͽ�Ǯ
void saveInformation(void)
{
    FILE *fpw = fopen("information.csv", "w");
    if (fpw == NULL)
    {
        printf("�򿪵�����Ϣ�ļ�ʧ��!\n\n");
        Sleep(2000);
        return;
    }
    fprintf(fpw, "����(��/��/��/ʱ) , ��Ǯ , ���ۼ�¼id , ������¼id\n");
    fprintf(fpw, "%d %d %d %d , %f , %d , %d", cntYear, cntMon, cntDay, cntHour, totalMoney, idsales, idorders);
    fclose(fpw);
    return;
}
//--------------------����˿���Ϣ
void saveCusInformation(customer *L)
{
    FILE *fpw = fopen("members.csv", "w");
    if (fpw == NULL)
    {
        printf("�򿪹˿���Ϣ�ļ�ʧ��!\n\n");
        Sleep(2000);
        return;
    }
    customer *p = L->nxt;
    fprintf(fpw, "�˿����� , �������� , �ֻ�β��\n");
    while (p != NULL)
    {
        fprintf(fpw, "%s , %f , %d\n", p->name, p->points, p->phonenumber);
        p = p->nxt;
    }
    fclose(fpw);
    return;
}
//--------------------��ȡ������¼
void readOrdersRecord(record *L)
{
    FILE *fp = fopen("ordersRecord.csv", "r");
    if (fp == NULL)
    {
        printf("������¼�ļ��������!\n\n");
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
        strcpy(gd.Buyer, "����");
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
//--------------------��ȡ���ۼ�¼
void readSalesRecord(record *L)
{
    FILE *fp = fopen("salesRecord.csv", "r");
    if (fp == NULL)
    {
        printf("���ۼ�¼�ļ��������!\n\n");
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
//--------------------�ж������ȼ�
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
//--------------------�޸������������
void calcCouponMoney(good *L, int allcnt, int cUnit, float tMoney)
{
    good *g = L->nxt;
    printf("��ǰ�����ȼ�: %.1f-%.1f\n", couponUnit[cUnit + 1], couponUnit[cUnit]);
    printf("�����嵥:\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("���ʱ��           |���|����    |Ʒ��      |�����۸�        |���������λ |�ȼ� |������ |�Ǽ�  |\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    float money;
    float packprice = 0;
    while (g != NULL)
    {
        float aveReduce = couponUnit[cUnit] * ((float)(g->Quantity) * (g->BasePrice) + g->PackPrice) / (tMoney);
        money = (float)(g->Quantity) * (g->BasePrice) + g->PackPrice - aveReduce;
        g->BasePrice = g->BasePrice - (aveReduce / g->Quantity);
        packprice += g->PackPrice;
        printf("%d��%02d��%02d��%02dʱ ", g->indate.year, g->indate.mon, g->indate.day, g->indate.hour);
        printByPointer(g);
        totalMoney += money;
        g = g->nxt;
    }
    printf("|�ܼ�һ���� %.2f|\n", tMoney - couponUnit[cUnit]);
    printf("|���������ۿ� %.2f|\n", couponUnit[cUnit]);
    printf("|������װ��%.2f|\n", packprice);
    printf("\n");
    saveInformation();
    return;
}
//--------------------���record������ʣ�±�ͷ
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
//--------------------�ҵ�customer�����β���
customer *findCustomerR(customer *L)
{
    customer *p = L;
    while (p->nxt)
        p = p->nxt;
    return p;
}
//--------------------���customer������ʣ�±�ͷ
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
//--------------------����ϵͳ
void remake()
{
    system("cls");
    idcnt = 1, idsource = 1, idsales = 1, idorders = 1, membercnt = 1;
    clearGoodList(&currentGoods);
    prt("��ǰ��������������!\n");
    clearGoodList(&salesGoods);
    prt("���۵������������!\n");
    clearGoodList(&ordersGoods);
    prt("�����������������!\n");
    clearRecordList(&salesRecord);
    prt("���ۼ�¼�����������!\n");
    clearRecordList(&ordersRecord);
    prt("������¼�����������!\n");
    clearCustomerList(&members);
    prt("��Ա��Ϣ�����������!\n");
    FILE *fpw = fopen("information.csv", "w");
    fprintf(fpw, "����(��/��/��/ʱ) , ��Ǯ , ���ۼ�¼id , ������¼id\n");
    fclose(fpw);
    prt("������Ϣ�������!\n");
    fpw = fopen("currentGoods.csv", "w");
    fprintf(fpw, "���ʱ�� , ���� , Ʒ�� , �����۸� , ���� , ��λ , ���ʶ� , �Ǽ�\n");
    fclose(fpw);
    prt("��ǰ�����Ϣ�������!\n");
    fpw = fopen("salesRecord.csv", "w");
    fprintf(fpw, "��� , ʱ�� , ���� , Ʒ�� , ��Ʒ���� , (��λ) , �ȼ� , �Ǽ� , ��� , �ֻ�β�� , ��Ӧ�� , ���\n");
    fclose(fpw);
    prt("���ۼ�¼��Ϣ�������!\n");
    fpw = fopen("ordersRecord.csv", "w");
    fprintf(fpw, "��� , ʱ�� , ���� , Ʒ�� , ������ , ���ۼ� , ��Ʒ���� , (��λ) , �ȼ� , �Ǽ� , ��Ӧ�� , ���\n");
    fclose(fpw);
    prt("������¼��Ϣ�������!\n");
    fpw = fopen("members.csv", "w");
    fprintf(fpw, "�˿����� , �������� , �ֻ�β��\n");
    prt("��Ա��Ϣ�������!\n\n");
    fclose(fpw);
    printf("���߳��й���ϵͳ����\n\n");
    printf("��������ʼ�ʽ�(Ԫ)\n");
    totalMoney = multiInput();
    printf("��������ʼʱ��,��ʽ��2022 04 09 16\n");
    cntYear = multiInput();
    cntMon = multiInput();
    cntDay = multiInput();
    cntHour = multiInput();
    saveInformation();
    prt("���óɹ�,�ص�������!\n");
}
//--------------------�۸񲨶�
void priceFluctuations()
{
    srand((int)time(0));
    float r = ((rand() % 40) + 80) / 100.0;
    printf("��ǰ�ļ۸�ϵ����:%.2f\n", r);
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
//--------------------������룬��ȡ����
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
//--------------------��ӡʱ����ʾ
void printTimeNote()
{
    if (cntHour >= 22 || cntHour <= 2)
        printf("\n     ��, ����. ���еĹ���û����.\n\n");
    if (cntHour > 2 && cntHour < 7)
        printf("\n     ����֮ҹ.\n\n");
    if (cntHour >= 7 && cntHour < 12)
        printf("\n     û���緹, ȴ���������ʲô.\n\n");
    if (cntHour >= 12 && cntHour < 18)
        printf("\n     emmm...���ϳ�ʲô��?\n\n");
    if (cntHour >= 18 && cntHour < 22)
        printf("\n     When the flowers start to fade.\n\n");
}
//--------------------�ı���ɫ
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
//--------------------������й�����
void printAllSuppliers(sup *L)
{
    sup *p = supplyList.nxt;
    printf("----------------------------------------------------\n");
    printf("��� | ������                                     |\n");
    printf("----------------------------------------------------\n");
    while(p!=NULL)
    {
        printf(" %4d  %-30s\n",p->id,p->name);
        p=p->nxt;
        printf("----------------------------------------------------\n");
    }
    return ;
}
