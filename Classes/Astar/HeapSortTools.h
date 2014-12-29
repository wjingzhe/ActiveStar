/****************************************************************************
Copyright (c) 2014-2015 Jingz @ ReccaStudio.


Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#pragma once
#include "cocos2d.h"
#include "AstarItem.h"

namespace HeapSort
{
	//usage is similar stl algorithm,endIt is outSide of the vector
	//即endIt是有效数据范围的下一个哨兵位置,下个版本增加对原生指针的支持
	void createNewHeap(cocos2d::Vector<AstarItem*> *vItemList,cocos2d::Vector<AstarItem*>::iterator & const startIt ,cocos2d::Vector<AstarItem*>::iterator endIt,AstarLessThan lessFunc,int nodesCount);

	//在位将末位元素生成为maxHeap,剩余元素以开始下标为顶重新调整为堆
	void genarateNextheap(cocos2d::Vector<AstarItem*> * vItemList,cocos2d::Vector<AstarItem*>::iterator & const startIt,cocos2d::Vector<AstarItem*>::iterator endIt,AstarLessThan lessFunc);

	//排序的整体思想类似于红黑树map的插入，已在插入时有序，因为可能修改已有节点数据，不能使用stl的容器
	//传入当前节点所在的位置，以开始下标为堆顶调整该节点的位置
	void placeElem(cocos2d::Vector<AstarItem*>::iterator & const startIt,int n,int i,AstarLessThan lessFunc);

}

