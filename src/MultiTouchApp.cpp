#include "cinder/Cinder.h"
#include "cinder/app/AppNative.h"
#include "cinder/System.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"

using namespace ci;
using namespace ci::app;

#include <vector>
#include <map>
#include <list>
using namespace std;



// We'll create a new Cinder Application by deriving from the BasicApp class
class MultiTouchApp : public AppNative {

public:
	void	prepareSettings( Settings *settings );

	void	mouseDown( MouseEvent event );
	void	mouseDrag( MouseEvent event );

	void	touchesBegan( TouchEvent event );
	void	touchesMoved( TouchEvent event );
	void	touchesEnded( TouchEvent event );

	void	setup();
	void	draw();
    void    update();
	void	keyDown( KeyEvent event );
	
	vector<Vec2f>                points;
    vector<Vec2f>                currPoints;
    
    int     maxLength;
    
    Perlin pNoise;
    float noiseInc;
    float n;
    float minDist;
    float maxDist;
    
    
};

void MultiTouchApp::setup()
{
	console() << "MT: " << System::hasMultiTouch() << " Max points: " << System::getMaxMultiTouchPoints() << std::endl;
    
    maxLength = 700;
   // pNoise.setSeed(0.1);
    
    noiseInc = 0.01;
    
    
}

void MultiTouchApp::prepareSettings( Settings *settings )
{
	
    settings->enableMultiTouch();
    
}

void MultiTouchApp::touchesBegan( TouchEvent event )
{
     
    
    for (vector<TouchEvent::Touch>::const_iterator t = event.getTouches().begin(); t != event.getTouches().end(); t++ ){
        
        currPoints.push_back(t->getPos());
        
      }
}

void MultiTouchApp::touchesMoved( TouchEvent event )
{
    
    
    
    for (vector<TouchEvent::Touch>::const_iterator t = event.getTouches().begin(); t != event.getTouches().end(); t++ ){
        
        currPoints.push_back(t->getPos());
        
    }

}

void MultiTouchApp::touchesEnded( TouchEvent event )
{
    
    for( vector<TouchEvent::Touch>::const_iterator t = event.getTouches().begin(); t !=
        event.getTouches().end(); ++t ) {
        
		points.push_back(t->getPos());
        
    }
    
}

void MultiTouchApp::mouseDown( MouseEvent event )
{
	console() << "Mouse down: " << event.isRight() << " & " << event.isControlDown() << std::endl;	
}

void MultiTouchApp::mouseDrag( MouseEvent event )
{
	console() << "Mouse drag: " << std::endl;	
}

void MultiTouchApp::update(){
    
    if(points.size()> maxLength){
        points.erase(points.begin());
    }
    
    n += noiseInc;
    
    if(n > 1){
        n = 0;
    }
   
    minDist =  maxDist - (maxDist/2)* abs(pNoise.noise(n));
    
    console() << "min Dist" << minDist << std::endl;
    
    
}

void MultiTouchApp::draw()
{
	gl::enableAlphaBlending();
	gl::setMatricesWindow( getWindowSize() );
	gl::clear( Color( 0.1f, 0.1f, 0.1f ));
    gl::color(1, 1, 1, 0.1);
    
    for(int a = 0; a < points.size(); a ++){
        
        for(int b = 0; b < points.size(); b ++){
            float d = points[a].distance(points[b]) ;
            if(d > maxDist){
                maxDist = d;
             }
            
            if( d < minDist){
               gl::drawLine(points[a], points[b]);
            }
            
        }
    }
    
    for (int a = 0; a < currPoints.size(); a ++) {
        
              for(int b = 0; b < points.size(); b ++){
                  
                  //if(points[a].distance(points[b]) < minDist){
                      
                      gl::drawLine(currPoints[a], points[b]);
                  
           // }
        }
    }
    
    
//    for (int a = 0; a < currPoints.size(); a ++) {
//        
//        for(int b = 0; b < currPoints.size(); b ++){
//            
//           gl::drawLine(points[a], points[b]);
//           
//        }
//    }

    
    
     currPoints.clear();
}

void MultiTouchApp::keyDown(cinder::app::KeyEvent event){
  
    if (event.getChar() == 'f') {
         setFullScreen(!isFullScreen());
    }
    if (event.getChar() == 'c') {
        points.clear();
    }
}

CINDER_APP_NATIVE( MultiTouchApp, RendererGl )