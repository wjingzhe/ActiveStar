
临时封档以及讨论相关：http://blog.csdn.net/jingzhewangzi/article/details/42267357


=====================================================
v0.1.4版本： 

计划ID：15022001
Title主题：项目管理
TimeBegan制定时间：2015.02.20
Detail说明：1、完成Mac环境编译支持 2.优化win32等配置流程
Charger负责人：蓝玉
TimeRemain任务周期：一份业余时间
TimeFinished完成日期：2015.02.20
Production成果说明：
1、在APPDelegate中增加资源路径搜索，删除win32、Android编译文件的同名路径支持
2、Mac工程添入代码文件和修改编译SDK为10.10（可自己降回），删除配置信息ios和mac中prefixed head中路径，成功编译并在模拟器中运行


=====================================================
v0.1.3版本： 

计划ID：14121801-02
Title主题：heapSort优化
TimeBegan制定时间：2014.12.30
Detail说明：细节处理1为支持ndk编译声明修改 2对插入时i对应插入元素当前在vector的下标
Charger负责人：蓝玉
TimeRemain任务周期：一份业余时间
TimeFinished完成日期：2014.12.30
Production成果说明：
1、注释掉主循环中调用HeapInit（对应createNewHeap），直接使用genarateNextheap即可。


计划ID：14121801
Title主题：Debug每次open表排序改为HeapSort堆排序或类似增长级别的排序
TimeBegan制定时间：2014.12.18
Detail说明：要求改为支持heapInsert的，可以考虑使用排序的set实现。之前实现为单次冒泡选择，在进行N次迭代时效率变为N(N-1)/2（从1到N的累加和）趋于N*2，在大范围的寻路中效率急剧变低。
Charger负责人：蓝玉
TimeRemain任务周期：2天业余时间
TimeFinished完成日期：2014.12.27
Production成果说明：
1、排序算法已更改为HeapSort，并提取为一个单独的模块。因每次插入节点有范围，改写为非传统堆排序。
2、每次探索记录新增加的节点，在排序期实行堆插入二向定位。
补记2：之前把向上和向下的item定位混合，现在进行分离。之后每探索出新item都执行HeapInsert（对应函数为：placeElem）。出现已在open中item更新路径，调用placeElem即可实现再次堆有序。
下个版本：放弃主循环中调用HeapInit（对应createNewHeap），可以使用genarateNextheap。
3、若是更新已在open表中的节点信息则直接重新定位（类似如插入），防止出现堆错乱
4、工作量出现了错误估计。实际工作接近于一个工作日以上


计划ID：14121802
Title主题：地图移动累加性BUG修正
TimeBegan制定时间：2014.12.18
Detail说明：1、将hero和map的依赖性分离；2、在每次寻路开始和执行结束增加位置修正或状态初始化
Charger负责人：待定（搓衣板、水影、雪茄、蓝玉）
TimeRemain任务周期：7天业余时间（已搁置）
Production成果说明：（由负责人添加）

计划ID：14121803
Title主题：孤岛路径处理
TimeBegan制定时间：2014.12.18
Detail说明：失败查找的情况下会有bool返回，根据位置预测值出掉开始增加的路径
Charger负责人：蓝玉
TimeRemain预计周期：2天业余时间
TimeFInished完成时间：20141230
Production成果说明：
1、孤岛地图的预处理留待下个版本
2、已支持同图孤岛的最接近点路径寻路。先删除了无效路径点再建立路径分支线。详情看generatePath代码


===================================================

*v0.1.2及早前版本： 
计划ID：14120101
Title主题：实现A*框架
TimeBegan制定时间：2014.12.01
Detail说明：1、实现可运行的A*寻路demo 2、采用cocos2d-x3.2及以上的引擎版本 3、发布安卓apk
Charger负责人：蓝玉
TimeRemain任务周期：一周业余时间
Production成果说明：（由负责人添加）
1、3.0的代码转化为3.2的可读性代码，代码风格和实现全是本人的，差别很大：比如斜角的探测、数据处理等，未来对3D的支持都是有可能的 
2、思想都是变成我的了，注释全是我的。这么说，版权是我的。过几天把mit的协议补上。
3、地图是本人编辑出来的
4、安卓版本用cocos命令可以发布


*：为不可靠计划