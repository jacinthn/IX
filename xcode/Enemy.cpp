#include "Enemy.h"

using namespace ci;
using namespace ci::app;

Enemy::Enemy( Vec4f pos, float time, int type, bool invincible, int health): mPos( pos ), mActiveTime( time ), mType( type ), mInvincible( invincible ), mHealth( health )
{
    mVel = 10.0f;
    mDir = Vec3f( 1.0f, 0.0f, 0.0f ).normalized();
    mLive = true;
    mActive = false;
    mBReady = false;
    mDeathTime = 0.0f;
    mDistTrav = 1.0f;
    mMatrix.setToIdentity();
    mVRot = 0.0f;
    mZRot = 0.0f;
}

void Enemy::update( bool hit, bool bulletLive, float time )
{
    if( time >= mActiveTime )
        mActive = true;
    
    if( mActive )
    {
        if(hit && bulletLive && mLive && !mInvincible)
        {
            if(mHealth > 0)
                mHealth -=10;
            else
            {
                mLive = false;
                mVel = 0.0f;
                mDeathTime = getElapsedSeconds() + 0.25f;
            }
        }
        if( mLive )
        {
            switch( mType )
            {
                // test
                case 0:
                    mMatrix.rotate( Vec3f::zAxis().normalized(), 0.033f);
                    if( mDistTrav >= 4500.0f )
                    {
                        mLive = false;
                    }
                    
                    if( mDistTrav < 200.0f )
                    {
                        mDir = Vec3f( 0.0f, 0.0f, 1.0f ).normalized();
                        mVel = 8.0;
                    }
                    else if( mDistTrav >= 2000.0f )
                    {
                        mDir = Vec3f( 0.0f, 0.0f, 1.0f ).normalized();
                        mVel = 8.0;
                    }
                    else
                    {
                        mVel = 0.0f;
                        mDistTrav += 0.0f;
                    }
                    break;
                    
                // Negative log enemy type a
                case 1:
                    mDir = Vec3f( 0.5f/mDistTrav, -pow((5.0f/mDistTrav), 2.0f), 0.0f ).normalized();
                    if( mPos.x >= 500.0f )
                    {
                        if( mVRot < 3.14f/2.0f)
                        {
                            mMatrix.rotate( Vec3f::yAxis().normalized(), -0.0314f);
                            mVRot += 0.0314f;
                        }
                        mVel = 8.0f;
                    }
                    else
                    {
                        mMatrix.rotate( Vec3f::zAxis().normalized(), 0.033f);
                        mVel = 5.0f;
                    }
                    if( mPos.x > 2000.0f )
                        mLive = false;
                    
                    break;
                    
                // Negative log enemy type a right
                case 2:
                    mDir = Vec3f( -0.5f/mDistTrav, -pow((5.0f/mDistTrav), 2.0f), 0.0f ).normalized();
                    if( mPos.x <= -500.0f )
                    {
                        if( mVRot < 3.14f/2.0f)
                        {
                            mMatrix.rotate( Vec3f::yAxis().normalized(), 0.0314f);
                            mVRot += 0.0314f;
                        }
                        mVel = 8.0f;
                    }
                    else
                    {
                        mMatrix.rotate( Vec3f::zAxis().normalized(), -0.033f);
                        mVel = 5.0f;
                    }
                    if( mPos.x < -2000.0f )
                        mLive = false;
                    break;
                
                // Diagonal enemy typa a left
                case 3:
                    mMatrix.rotate( Vec3f::zAxis().normalized(), 0.066f);
                    if( mDistTrav >= 4500.0f )
                    {
                        mLive = false;
                    }
                    
                    if( mDistTrav < 600.0f )
                    {
                        mDir = Vec3f( 7.0f, 1.0f, 0.0f ).normalized();
                        mVel = 10.0;
                    }
                    else if( mDistTrav >= 2000.0f )
                    {
                        mDir = Vec3f( -7.0f, 2.0f, 0.0f ).normalized();
                        mVel = 15.0;
                    }
                    else
                    {
                        mVel = 0.0f;
                        mDistTrav += 5.0f;
                    }
                    break;
                
                // Diagonal enemy type a right
                case 4:
                    mMatrix.rotate( Vec3f::zAxis().normalized(), 0.066f);
                    if( mDistTrav >= 4500.0f )
                    {
                        mLive = false;
                    }
                    
                    if( mDistTrav < 600.0f )
                    {
                        mDir = Vec3f( -7.0f, 1.0f, 0.0f ).normalized();
                        mVel = 10.0;
                    }
                    else if( mDistTrav >= 2000.0f )
                    {
                        mDir = Vec3f( 7.0f, 2.0f, 0.0f ).normalized();
                        mVel = 15.0;
                    }
                    else
                    {
                        mVel = 0.0f;
                        mDistTrav += 5.0f;
                    }
                    break;
                
                // Forward enemy type a w/ pause
                case 5:
                    mMatrix.rotate( Vec3f::zAxis().normalized(), 0.033f);
                    if( mDistTrav >= 4500.0f )
                    {
                        mLive = false;
                    }
                    
                    if( mDistTrav < 200.0f )
                    {
                        mDir = Vec3f( 0.0f, 0.0f, 1.0f ).normalized();
                        mVel = 8.0;
                    }
                    else if( mDistTrav >= 2000.0f )
                    {
                        mDir = Vec3f( 0.0f, 0.0f, 1.0f ).normalized();
                        mVel = 8.0;
                    }
                    else
                    {
                        mVel = 0.0f;
                        mDistTrav += 5.0f;
                    }
                    break;
                
                // Forward enemy type b
                case 6:
                    if(mPos.z >= 350.0f)
                        mLive = false;
                    mDir = Vec3f( 0.0f, 0.0f, 1.0f ).normalized();
                    mVel = 5.0f;
                    mMatrix.rotate( Vec3f::yAxis().normalized(), 0.033f );
                    break;

                
                // Boss Body
                case 7:
                    
                    if( mBReady )
                    {
                        
                        mVel = 0.3f;
                        mDir = Vec3f( cos( 0.5f * getElapsedSeconds()), cos( 1.5f * getElapsedSeconds()), 0.0f ).normalized();
                        
                        if( mDistTrav >= 280.0f )
                        {
                            mDistTrav = 0.0f;
                            mInvincible = true;
                        }
                        else if( mDistTrav >= 240.0f )
                        {
                            if( mVRot > 0.0f )
                                mVRot -= 0.03f;
                            mInvincible = true;
                        }
                        else if( mDistTrav >= 120.0f )
                        {
                            if( mVRot < 1.0f )
                                mVRot += 0.03f;
                            mInvincible = false;
                        }
                        else
                        {
                            mInvincible = true;
                        }
                        
                    }
                    else if( mPos.y >= 0.0f )
                        mBReady = true;
                    else
                        mDir = Vec3f::yAxis();

                    break;
                
//                case 8:
//                    mDir = Vec3f( cos( 0.5f * getElapsedSeconds()), cos( 1.5f * getElapsedSeconds()), 0.0f ).normalized();
//                    mVel = 4.0f;
//                    break;
//                    
//                case 9:
//                    mDir = Vec3f( cos( 0.5f * getElapsedSeconds()), cos( 1.5f * getElapsedSeconds()), 0.0f ).normalized();
//                    mVel = 4.0f;
//                    break;
                   
                // Prop asteroid
                case 10:
                    mMatrix.rotate( Vec3f( 1.0f, 1.0f, 1.0f ).normalized(), 0.033f );
                    if(mPos.z >= 350.0f)
                        mLive = false;
                    mDir = Vec3f( 0.0f, 0.0f, 1.0f ).normalized();
                    mVel = 5.0f;
                    break;
                
                    
                // Forward enemy type b w/ pause
                case 11:
                    mMatrix.rotate( Vec3f::yAxis().normalized(), 0.033f );
                    if( mDistTrav >= 4500.0f )
                    {
                        mLive = false;
                    }
                    
                    if( mDistTrav < 200.0f )
                    {
                        mDir = Vec3f( 0.0f, 0.0f, 1.0f ).normalized();
                        mVel = 8.0;
                    }
                    else if( mDistTrav >= 2000.0f )
                    {
                        mDir = Vec3f( 0.0f, 0.0f, 1.0f ).normalized();
                        mVel = 8.0;
                    }
                    else
                    {
                        mVel = 0.0f;
                        mDistTrav += 5.0f;
                    }
                    break;
                    
                default:
                    mVel = 0.0f;
                    break;
            }
            
            if( mType == 7 )
            {
                if( mBReady )
                {
                    mPos += mDir * 4.0f;
                    mDistTrav += mVel;
                }
                else
                {
                    mPos += mDir * 4.0f;
                }
            }
            else
            {
                mPos += mDir * mVel;
                mDistTrav += mVel;
            }
        }
    }
}

const bool Enemy::isAlive() const
{
    return mLive;
}

const bool Enemy::isActive() const
{
    return mActive;
}

const int Enemy::getType()
{
    return mType;
}

const int Enemy::getHealth()
{
    return mHealth;
}

const float Enemy::getDeathTime()
{
    return mDeathTime;
}

const float Enemy::getVRot()
{
    return mVRot;
}

const Vec4f& Enemy::getPos() const
{
    return mPos;
}

const Matrix44f& Enemy::getMatrix()
{
    return mMatrix;
}

Enemy::~Enemy()
{
    mMatrix.~Matrix44();
    mPos.~Vec4();
    mDir.~Vec3();
}
