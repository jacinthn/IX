#include "BulletController.h"
#include "cinder/app/AppNative.h"
#include "cinder/ObjLoader.h"


using namespace ci;
using namespace ci::app;

BulletController::BulletController()
{
    ObjLoader mLoader = ObjLoader( (DataSourceRef) loadResource( RES_OBJ_LASER), (DataSourceRef) loadResource( RES_MAT_LASER ) );
    mLoader.load( &mBulletTriMesh );
    mBulletVbo = gl:: VboMesh( mBulletTriMesh );
    
    mBullet = Bullet();
    
    mTrans1 = Matrix44f();
    mTrans2 = Matrix44f();
}

void BulletController::update( bool firing, float xPos, float yPos, float xRot, float yRot  )
{
    mBullet.update( firing, xPos, yPos, xRot, yRot );
    
    mTrans1.setToIdentity();
    mTrans1.translate( mBullet.getPos().xyz() );
    mTrans1.rotate( Vec3f::xAxis().normalized(), mBullet.getXRot() );
    mTrans1.rotate( Vec3f::yAxis().normalized(), mBullet.getYRot() + 3.14f/2.0f);
    mTrans1.scale( Vec3f( 10.0f, 10.0f, 10.0f ) );
}

void BulletController::draw()
{
    if( mBullet.isLive() )
    {
        gl::pushMatrices();
//        gl::drawString( "" + std::to_string(mBullet.getX()) + ", " + std::to_string(mBullet.getY()) + ", " + std::to_string(mBullet.getZ()), Vec2f(0.0f, 30.0f ) );
        gl::multModelView( mTrans1 );
        
        gl::draw( mBulletVbo );
        gl::popMatrices();
    }
}

const bool BulletController::getBulletStatus()
{
    return mBullet.isLive();
}

const Vec4f& BulletController::getBulletPos()
{
    return mBullet.getPos();
}

BulletController::~BulletController()
{
    mTrans1.~Matrix44();
    mTrans2.~Matrix44();
    mBullet.~Bullet();
    mBulletTriMesh.clear();
    mBulletVbo.reset();
}
