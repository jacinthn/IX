#include "Bullet.h"
#include "cinder/gl/gl.h"
#include "cinder/Matrix44.h"

using namespace ci;

Bullet::Bullet()
{
    mPos    = Vec4f( 0.0f, 0.0f, 0.0f, BULLET_RADIUS );
    mDir    = Vec3f( 0.0f, 0.0f, -1.0f );
    mVel    = 100.0f;
    //mVel = 0.5f;
    mLive   = false;
    mxRot   = 0.0f;
    myRot   = 0.0f;
}

void Bullet::update( bool isFiring, float xPos, float yPos,  float xRot, float yRot  )
{
    if( mLive )
    {
        if( mPos.z > -750.0f )
            mPos += Vec4f(mVel * mDir, 0.0f);
        else
        {
            mPos[0] = xPos;
            mPos[1] = yPos;
            mPos[2] = 0.0f;
            mLive   = false;
            mxRot   = 0.0f;
            myRot   = 0.0f;
        }
    }
    else
    {
        mPos[0] = xPos;
        mPos[1] = yPos;
        mPos[2] = 0.0f;
        
        if( isFiring )
        {
            mLive   = true;
            mxRot   = -xRot;
            myRot   = yRot;
            mDir    = Vec3f( 1.15f * -yRot, 0.0f, -1.0f ).normalized();
        }
        else
        {
            mLive   = false;
            mxRot   = 0.0f;
            myRot   = 0.0f;
        }
    }
}

Bullet::~Bullet()
{
    mPos.~Vec4();
    mDir.~Vec3();
}

const float Bullet::getX()
{
    return mPos[0];
}

const float Bullet::getY()
{
    return mPos[1];
}

const float Bullet::getZ()
{
    return mPos[2];
}

const float Bullet::getXRot()
{
    return mxRot;
}

const float Bullet::getYRot()
{
    return myRot;
}

const Vec4f& Bullet::getPos()
{
    return mPos;
}
const bool Bullet::isLive()
{
    return mLive;
}