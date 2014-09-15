#pragma once

#include "cinder/Vector.h"

class Star
{
  public:
    // Methods
    Star( ci::Vec3f loc, ci::Color color );
    void update( float xInfluence, float yInfluence );
    void draw();
    ~Star();
    
    // Mechanics
    ci::Vec3f           mLoc;
    const ci::Vec3f     mOrigin;
    float               mVel;
    
    // Appearance
    float               mRadius;
    ci::Color           mColor;
};