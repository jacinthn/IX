#pragma once

#include "cinder/Vector.h"

class Enemy
{
  public:
    Enemy( ci::Vec4f pos, float time, int type, bool invincible, int health = 100 );
    void update( bool hit, bool bulletLive, float time );
    const bool isAlive() const;
    const bool isActive() const;
    const int  getType();
    const int  getHealth();
    const float getDeathTime();
    const float getVRot();
    const ci::Matrix44f& getMatrix();
    const ci::Vec4f& getPos() const;
    ~Enemy();
  
  private:
    ci::Vec4f       mPos;
    ci::Vec3f       mDir;
    int             mType, mHealth;
    float           mVel, mDeathTime, mActiveTime, mDistTrav, mVRot, mZRot;
    ci::Matrix44f   mMatrix;
    bool            mLive, mActive, mInvincible, mBReady;
};
