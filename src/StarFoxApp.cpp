#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/ObjLoader.h"
#include "cinder/TriMesh.h"
#include "cinder/Camera.h"
#include "cinder/Arcball.h"
#include "cinder/params/Params.h"
#include "cinder/Rand.h"

#include "Resources.h"
#include "StarController.h"
#include "BulletController.h"
#include "EnemyController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class StarFoxApp : public AppNative {
  public:
    void                    prepareSettings( Settings * settings);
	void                    setup();
	void                    update();
	void                    draw();
    void                    keyDown( KeyEvent event);
    void                    keyUp( KeyEvent event );
    void                    shutdown();

  private:
    // Camera
    CameraPersp             mCamera;
    CameraOrtho             mTestCamera;
    params::InterfaceGlRef  mParams;

    // Light
    Vec4f                   mLightPos;
    Vec4f                   mLightAmbient;
    Vec4f                   mLightDiffuse;
    Vec4f                   mLightSpecular;
    
    // Ship Transformation
    Matrix44f               mShipTrans;
    float                   yRot, xRot;
    float                   yRotVel, xRotVel;
    float                   xDist, yDist;
    float                   mDirLeft, mDirRight, mDirUp, mDirDown;
    float                   mRDirLeft, mRDirRight;
    float                   xVel, yVel;
    float                   mHoriWindowBounds;
    float                   mVertWindowBounds;
    
    // Bullet Trans
    Vec3f                   mTarget;
    
    // Shader
    gl::GlslProg            mPhongShader;
    
    // Model
    TriMesh                 mArwingTriMesh;
    gl::VboMesh             mArwingVbo;
    
    // Game State
    float                   mCurrTime;
    bool                    keyPressed;
    bool                    mFiring;
    StarController          *mStarController;
    BulletController        *mBullletController;
    EnemyController         *mEnemyController;
    float                   rInputSum;
    float                   mShakeTime;
    int                     mShipHealth;
    bool                    mAlive;
    bool                    mStarted;
};

void StarFoxApp::prepareSettings( Settings *settings)
{
    settings->setWindowSize( 1366, 768 );
    settings->setFrameRate( 60.0f );
    
}

void StarFoxApp::setup()
{
//---------Set up Camera-------------------//
    mCamera = CameraPersp( 1366.0f, 768.0f, 60.0f);
    mCamera.setPerspective( 60.0f, getWindowAspectRatio(), 0.5f, 2000.0f);
    mCamera.lookAt( Vec3f( 0.0f, 0.0f, 600.0f), Vec3f( 0.0f, 0.0f, 0.0f) );
    

    mTestCamera = CameraOrtho( -638.0f, 638.0f, -384.0f, 384.0f, 0.5f, 1000.0f );
    mTestCamera.lookAt( Vec3f( 0.0f, 0.0f, 600.0f), Vec3f::zero() );
    
//--------Load Arwing model----------------//
    ObjLoader mLoader( (DataSourceRef) loadResource( RES_OBJ_IX), (DataSourceRef) loadResource( RES_MAT_IX ) );
    mLoader.load( &mArwingTriMesh );
    mArwingVbo = gl::VboMesh( mArwingTriMesh );
    
//---------Set up light parameters--------//
    mLightPos =             Vec4f( -40.0f, 400.0f, 300.0f, 1.0f );
    mLightAmbient =         Vec4f( 0.5f, 0.5f, 0.5f, 1.0f );
    mLightDiffuse=          Vec4f( 0.75f, 0.75f, 0.75f, 1.0f );
    mLightSpecular=         Vec4f( 0.5f, 0.5f, 0.5f, 1.0f );
    
//-------Initialize ship orientation------//
    mShipTrans.setToIdentity();
    mShipTrans.scale( Vec3f( 0.5f, 0.5f, 0.5f ) );
    mShipTrans.rotate( Vec3f::yAxis().normalized(), 3.14f );
    
    // Horizontal orientation
    yRot                = 0.0f;
    yRotVel             = 0.016f;
    xDist               = 0.0f;
    xVel                = 20.0f;
    
    // Vertical orientation
    xRot                = 0.0f;
    xRotVel             = 0.016f;
    yDist               = 0.0f;
    yVel                = 20.0f;
    
    // Directional input
    mDirLeft            = 0.0f;
    mDirRight           = 0.0f;
    mDirUp              = 0.0f;
    mDirDown            = 0.0f;
    
    mRDirLeft           = 0.0f;
    mRDirRight          = 0.0f;
    
    mHoriWindowBounds   = 580.0f;
    mVertWindowBounds   = 300.0f;
    
    rInputSum           = 0.0f;
    

//------------Load shaders--------------//
    try
    {
        mPhongShader = gl::GlslProg( loadResource( RES_SHADER_PHONG_VERT ), loadResource( RES_SHADER_PHONG_FRAG ));
    }
    catch( gl::GlslProgCompileExc &ex )
    {
        console() << "Couldn't compile shader." << endl;
        console() << ex.what() << endl;
        quit();
    }
    catch( ... )
    {
        console() << "Couldn't load shader. " << endl;
        quit();
    }
    
//--------Initialize Game state--------//
    mCurrTime           = getElapsedSeconds();
    keyPressed          = false;
    mFiring             = false;
    mStarController     = new StarController( getWindowWidth(), getWindowHeight(), 100.0f );
    mBullletController  = new BulletController();
    mEnemyController    = new EnemyController();
    mShipHealth         = 200;
    mShakeTime          = 0.0f;
    mAlive              = true;
    mStarted            = false;
    
//--------Set up Params for debugging-----//
    mParams = params::InterfaceGl::create( "Star Fox App ", Vec2i( 200, 200 ) );
    mParams->addParam( "Time", &mCurrTime );
    mParams->addParam( "Y-Rot", &yRot );
    mParams->addParam( "R-left", &mRDirLeft );
    mParams->addParam( "R-right", &mRDirRight );
    mParams->addParam( "Ship Health", &mShipHealth );
    mParams->addParam( "Started", &mStarted );
}

void StarFoxApp::update()
{
    mCurrTime = getElapsedSeconds();

    mStarController->update( 0.0f, 0.0f );
    
//----------- Manage ship movement-----//
    // Bind X and Y coordinates to screen
    if( xDist <= -mHoriWindowBounds )
        mDirLeft = 0.0f;
    
    if( xDist >= mHoriWindowBounds )
        mDirRight = 0.0f;
    
    if( yDist >= mVertWindowBounds )
        mDirUp = 0.0f;
    
    if( yDist <= -mVertWindowBounds )
        mDirDown = 0.0f;
    
    if( yRot >= 3.14f/4.0f )
        mRDirLeft = 0.0f;
    
    if( yRot <= -3.14f/4.0f )
        mRDirRight = 0.0f;
    
    
    // Horizon movement/rotation
    xDist += ( (mDirLeft + mDirRight) * xVel );
    yRot -= ( ( 3.0f * ( mRDirLeft + mRDirRight ) ) * yRotVel );
    
    // Vertical movement/rotation
    yDist += ( (mDirUp + mDirDown) * xVel );
    xRot -= ( (mDirUp + mDirDown) * xRotVel );
    

    mShipTrans.setToIdentity();
    mShipTrans.translate( Vec3f( xDist, yDist, 0.0f ) );
    mShipTrans.scale( Vec3f( 20.0f, 20.0f, 20.0f ) );
    mShipTrans.rotate( Vec3f::yAxis().normalized(), 3.14f + yRot);
    mShipTrans.rotate( Vec3f::xAxis().normalized(), xRot);
    
//---------------Orient le bullet---//
    mBullletController->update( mFiring, xDist, yDist, xRot, yRot );

//--------------Update le enemies--//
    mEnemyController->update( mBullletController->getBulletPos(), mBullletController->getBulletStatus(), mStarted );

//-------------Hit detection for ship------------------------//
    for( std::list<Enemy>::const_iterator e = mEnemyController->enemyList().begin();
        e != mEnemyController->enemyList().end(); e++)
    {
        if( e->isActive() && e->isAlive() )
        {
            float distance  = sqrt(
                                   pow( xDist - e->getPos().x, 2.0f) +
                                   pow( yDist - e->getPos().y, 2.0f) +
                                   pow( 0.0f - e->getPos().z, 2.0f) );
            
            float radii = 100.0f + e->getPos().w;
            
            if( distance <= radii )
            {
                mShipHealth -= 1;
                mShakeTime = getElapsedSeconds() + 0.25f;
            }
        }
        else
            continue;
    }

//---------------Shakey Cam--------------//
    if(getElapsedSeconds() < mShakeTime )
    {
        mCamera.lookAt( Vec3f( 0.0f, 0.0f, 600.0f), randVec3f() * 5.0f );
        mShipTrans.rotate( Vec3f::yAxis().normalized(), randFloat( -0.12f, 0.12f));
        mShipTrans.rotate( Vec3f::xAxis().normalized(), randFloat( -0.12f, 0.12f) );
    }
    else
    {
        mCamera.lookAt( Vec3f( 0.0f, 0.0f, 600.0f), Vec3f::zero() );
    }
}

void StarFoxApp::draw()
{
	gl::clear( Color( 0.1216f, 0.1137f, 0.1647f ), true );
    gl::enable( GL_DEPTH_TEST );

//-------Begin Draw-----------------//
    gl::pushMatrices();
    //gl::setViewport( Area( Rectf( 0.0f, 0.0f, 2366.0f, 1768.0f) ) );
    gl::setMatrices( mCamera );
    
    mStarController->draw();
    
    
//--------Draw Bullets---------------//
    mBullletController->draw();
    
//--------Draw Ship---------------//
    mPhongShader.bind();
    mPhongShader.uniform( "lightPos", mLightPos );
    mPhongShader.uniform( "lightAmbt", mLightAmbient );
    mPhongShader.uniform( "lightDiff", mLightDiffuse );
    mPhongShader.uniform( "lightSpec", mLightSpecular );
    mEnemyController->draw();
    
    gl::pushMatrices();
    gl::color( 1.0f, 1.0f, 1.0f );
    gl::translate( xDist, yDist, 0.0f );
    gl::rotate( Quatf( Vec3f::yAxis().normalized(), yRot) );
    gl::translate( 0.0f, 0.0f, -350.0f );
    gl::drawStrokedRect( Rectf(-30.0f, -30.0f, 30.0f, 30.0f) );
    gl::popMatrices();
    
    if( mShipHealth > 0 )
    {
        gl::pushMatrices();
        gl::multModelView( mShipTrans );
        gl::draw( mArwingVbo );
        gl::popMatrices();
    }
    else
    {
        gl::drawSolidCircle( Vec2f::zero(), 20.0f );
    }
    
    
    mPhongShader.unbind();

    
    gl::popMatrices();
    
    if( mShipHealth > 0 )
        gl::drawSolidRect( Rectf( 40.0f, 700.0f, 40.0f + (mShipHealth/100.0f) * 200.0f, 705.0f  ) );
//--------End Draw------------//
    //mParams->draw();
}

void StarFoxApp::keyDown( KeyEvent event )
{
    keyPressed = true;
    
    switch( event.getChar() ) {
        case 'd':
            mDirRight   =  1.0f;
            break;
        
        case 'a':
            mDirLeft    = -1.0f;
            break;
        
        case 'w':
            mDirUp      =  1.0f;
            break;
            
        case 's':
            mDirDown    = -1.0f;
            break;
        
        case 'k':
            mFiring     = true;
            break;
        
        case 'j':
            mRDirLeft    = -1.0f;
            break;
            
        case 'l':
            mRDirRight =  1.0f;
            break;
            
        case ' ':
            mStarted = true;
    }

}

void StarFoxApp::keyUp( KeyEvent event )
{
    
    switch( event.getChar() ) {
        case 'd':
            mDirRight =  0.0f;
            break;
            
        case 'a':
            mDirLeft  =  0.0f;
            break;
            
        case 'w':
            mDirUp    = 0.0f;
            break;
            
        case 's':
            mDirDown    = 0.0f;
            break;
        
        case 'k':
            mFiring     = false;
            break;

        case 'j':
            mRDirLeft    = 0.0f;
            break;
            
        case 'l':
            mRDirRight =  0.0f;
            break;
    }
}

void StarFoxApp::shutdown()
{
    mArwingTriMesh.clear();
    mArwingVbo.reset();
    mCamera.~CameraPersp();
    mParams.reset();
    mStarController->~StarController();
    mBullletController->~BulletController();
    mEnemyController->~EnemyController();

    
    mLightAmbient.~Vec4();
    mLightDiffuse.~Vec4();
    mLightSpecular.~Vec4();
    mLightPos.~Vec4();
    
    mShipTrans.~Matrix44();
}

CINDER_APP_NATIVE( StarFoxApp, RendererGl )
