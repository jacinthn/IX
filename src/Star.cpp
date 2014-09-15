#include "Star.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
using namespace ci;

Star::Star( Vec3f loc, Color col ):mLoc( loc ), mOrigin( loc ), mColor( col )
{
    mVel    = 10.0f;
    mRadius = mLoc.z/100.0f + 0.5f;
}

void Star::update(float xInfluence, float yInfluence)
{
    if( mLoc.z >= 400.0f )
    {
        mLoc.x = randFloat( (-1.0f/3.0f) * app::getWindowWidth(), (1.0f/3.0f) * app::getWindowWidth() );
        mLoc.y = randFloat( (-1.0f/3.0f) * app::getWindowHeight(), (1.0f/3.0f) * app::getWindowHeight() );
        mLoc.z = 0.0f;
        
        if( mLoc.x > 0.0f )
            mLoc.x += 50.0f;
        else
            mLoc.x -= 50.0f;
        
        if( mLoc.y > 0.0f )
            mLoc.y += 50.0f;
        else
            mLoc.y -= 50.0f;
    }
    float xDir = mLoc.x/(app::getWindowWidth()/2.0f);
    float yDir = mLoc.y/(app::getWindowHeight()/2.0f);
    
    Vec3f tempDir = Vec3f( xDir - xInfluence, yDir - yInfluence, 1.0f ).normalized();
    mLoc += tempDir * mVel;
    mRadius = mLoc.z/100.0f + 0.5f;
}

void Star::draw()
{
    gl::color( mColor );
    gl::drawSolidRect( Rectf(
                             mLoc.x - mRadius,
                             mLoc.y + mRadius,
                             mLoc.x + mRadius,
                             mLoc.y - mRadius) );
}

Star::~Star()
{
    mLoc.~Vec3();
    mColor.~ColorT();
}