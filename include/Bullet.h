#pragma once
#include "cinder/Vector.h"

static const float BULLET_RADIUS = 8.0f;
class Bullet
{
  public:
    // Methods
    Bullet();
    void update( bool isFiring, float xPos, float yPos, float xRot, float yRot  );
    ~Bullet();
    const float         getX();
    const float         getY();
    const float         getZ();
    const float         getXRot();
    const float         getYRot();
    const ci::Vec4f&    getPos();
    const bool          isLive();
 private:
    // Mechanics
    ci::Vec4f       mPos;
    ci::Vec3f       mDir;
    float           mVel;
    float           mxRot;
    float           myRot;
    bool            mLive;
    
    // Appearance
    float           mScale;
};