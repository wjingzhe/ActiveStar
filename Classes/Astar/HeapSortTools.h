/****************************************************************************
Copyright (c) 2014-2015 Jingz @ Recca2 Studio.


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
	//��endIt����Ч���ݷ�Χ����һ���ڱ�λ��,�¸��汾���Ӷ�ԭ��ָ���֧��
	void createNewHeap(cocos2d::Vector<AstarItem*> *vItemList, cocos2d::Vector<AstarItem*>::iterator startIt,
		cocos2d::Vector<AstarItem*>::iterator endIt, AstarGreaterThan greaterFunc, int nodesCount);

	//��λ��ĩλԪ������ΪmaxHeap,ʣ��Ԫ���Կ�ʼ�±�Ϊ�����µ���Ϊ��
	void genarateNextheap(cocos2d::Vector<AstarItem*> * vItemList, cocos2d::Vector<AstarItem*>::iterator startIt,
		cocos2d::Vector<AstarItem*>::iterator endIt, AstarGreaterThan greaterFunc);

	//���������˼�������ں����map�Ĳ��룬���ڲ���ʱ������Ϊ�����޸����нڵ����ݣ�����ʹ��stl������
	//���뵱ǰ�ڵ����ڵ�λ�ã��Կ�ʼ�±�Ϊ�Ѷ������ýڵ��λ��
	void placeElem(cocos2d::Vector<AstarItem*>::iterator startIt, cocos2d::Vector<AstarItem*>::iterator endIt,
		int i, AstarGreaterThan lessFunc);

}

