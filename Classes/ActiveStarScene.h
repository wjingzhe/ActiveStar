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

#ifndef __ActiveStarScene_SCENE_H__
#define __ActiveStarScene_SCENE_H__
#include "Astar/AstarItem.h"
#include "Astar/Astar.h"
#include "cocos2d.h"
#include <atomic>

#include "editor-support/spine/spine-cocos2dx.h"

class AstarItem;


class ActiveStarScene : public cocos2d::Layer
{
	enum 
	{
		kTagTileMap = 1,
	};

protected:

	// implement the "static create()" method manually
    CREATE_FUNC(ActiveStarScene);

	ActiveStarScene();

	bool touchBegan(cocos2d::Touch  *touch, cocos2d::Event  *event);

	void alterMapPosi();

private:

	cocos2d::Vector<AstarItem*> * m_pPath;

	std::atomic_bool m_bIsSearching;

	int m_nItemIndex;

	int m_nStepIndex;

	spine::SkeletonAnimation * m_pTamara;

	cocos2d::TMXTiledMap * m_pMap;

	Astar * m_pAstar;



	static float m_fDeltaXCount;

	static float m_fDeltaYCount;

public:
	
	~ActiveStarScene();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	void update(float dt)override;
    
    


};

#endif // __ActiveStarScene_SCENE_H__
