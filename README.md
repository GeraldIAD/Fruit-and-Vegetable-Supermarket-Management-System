# Fruit-and-Vegetable-Supermarket-Management-System
Programming Fundamentals Course Design, Fruit and Vegetable Supermarket Management System
## 项目说明
- 《程序设计基础》的课程设计大作业。

- 该程序运行在Windows环境下，人机交互界面为控制台界面，完成对果蔬超市的货物、顾客进行管理，且将数据保存到文件中。系统以菜单的方式进行工作，可选择不同的功能，进入不同界面进行不同操作。
- 该程序采用C语言，编译器为GNU-GCC，程序可执行文件使用.exe格式，读取、保存数据的可使用excel表格提供的.csv格式或.txt格式。

## 主要功能
1. 店铺基本信息，对其进行查看、修改和重置；
1. 货物作为库存，对其进行库存查看操作；
2. 货物作为库存，对其进行信息修改操作，并且保存到文件；
3. 货物作为货源，对其进行买入操作，同时保存进货记录到文件；
4. 货物作为商品，对其进行售出操作，同时保存销售记录到文件；
5. 顾客作为会员，在购买时享有不同程度的优惠，并且有对应信息和记录的查询；
6. 进货商信息和记录，对其进行查询和修改操作；
7. 进货、销售记录，对其进行查询和修改操作；
8. 对于时间，有挂起操作，果蔬会随时间腐烂变质，进行降价和清理操作；价格会随时间波动，进行价格的改动操作。

## 主要数据结构
对于货物: good结构体。有货源、库存、进货单、购物车四个链表。
1. id—存储货物的序号,逐次累加不会重复， int ;
2. Measure—货物使用的单位(斤0、个1、箱2、篮3), int ;
3. Level—货物新鲜等级(全腐0、略腐1、一般2、新鲜3，不同新鲜度的果蔬价格不同) , int;
4. Decay—用于判断货物是否腐烂, int ;
5. Sellingway—销售方式(零售、批发、促销)，int ;
6. Starlevel—品质星级(1-5颗星，对应果蔬的价格也不同), int ;
7. Quantity—货物数量, float , 一般保留两位;
8. BasePrice—零售价, float, 一般保留两位;
9. NetPrice—批发价, float, 一般保留两位;
10.PackPrice—包装费, float, 一般保留两位;
11. Name—种类名称, char数组, 长度为30;
12. Variety—品种名称, char 数组, 长度为30;
13. Supplier—供应商名称, char 数组, 长度为50;
14. Buyer—购买人, char 数组, 长度为30;
15. time--时间结构体，包括：
freshTime, 保质期, int;
addTime, 进货时间, int;
16. nxt,frm--指向下一个和上一个结构体, good 指针型;
17. indate--进货日期, date结构体型;

对于记录: record结构体。有进货、销售记录两个链表。
1.	id--存储记录的序号，逐次累加不会重复.int；
2.	nxt,frm--指向下一个和上一个结构体，record指针型；
3.	Buyer--购买者名称，为“本店”或顾客名称,char数组；
4.	buyerPhonenumber--只有销售给顾客时才有记录，为顾客手机尾号，int；
5.	Supplier--供应者名称，为“本店”或供应商名称，若有退款情况则会在此写明char数组；
6.	money--本次记录中涉及商品的交易金额，float，一般保留两位；
7.	theThing，本次交易涉及的商品信息记录，good型
8.	theDate，本次交易发生的时间，精确到小时，date型

对于顾客: customer结构体
1.	id--顾客编号，int;
2.	name--顾客姓名，char数组，长度为50;
3.	point--顾客积分，int；
4.	phonenumber--顾客电话尾号（后四位），int；
5.	nxt，frm--指向下一个和上一个结构体, Buyer指针型；

对于供货商: supplier结构体
1.	id--供货商编号，int；
2.	name--供货商的名称,char数组；
3.	nxt,frm--指向下一个和上一个结构体，sup指针型；
