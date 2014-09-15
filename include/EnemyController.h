#pragma once
#include "Enemy.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/gl.h"
#include "cinder/Matrix44.h"
#include <list>
#include "Resources.h"

class EnemyController
{
  public:
    EnemyController();
    const std::list<Enemy>& enemyList();
    void update( const ci::Vec4f& bPos, const bool bulletLive, const bool started );
    void draw();
    ~EnemyController();
    
  private:
    // Mechanics
    ci::Matrix44f       mTrans;
    std::list<Enemy>    mEnemies;
    bool                mStarted;
    void                init( float startTime );
    
    
    // Model
    ci::TriMesh         mEnemyATriMesh;
    ci::gl::VboMesh     mEnemyAVbo;
    
    ci::TriMesh         mEnemyBTriMesh;
    ci::gl::VboMesh     mEnemyBVbo;
    
    ci::TriMesh         mAsteroidATriMesh;
    ci::gl::VboMesh     mAsteroidAVbo;
    
    ci::TriMesh         mAsteroidBTriMesh;
    ci::gl::VboMesh     mAsteroidBVbo;
    
    ci::TriMesh         mBossWingTriMesh;
    ci::gl::VboMesh     mBossWingVbo;
    
    ci::TriMesh         mBossBodyTriMesh;
    ci::gl::VboMesh     mBossBodyVbo;
};