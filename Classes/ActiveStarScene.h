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
