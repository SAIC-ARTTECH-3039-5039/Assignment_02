#pragma once


#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxGui.h"


class ofApp: public ofBaseApp
{
public:
    // Q. What is this enum and why do we use it?
    enum MeshMode
    {
        MESH_MODE_0,
        MESH_MODE_1,
        MESH_MODE_2
    };
    
    void setup() override;
    void update() override;
    void draw() override;
  
    void keyPressed(int key) override;
    
    MeshMode currentMeshMode = MESH_MODE_0;
    
    // For the Kinect V2!
    ofxKinect kinect;
    ofTexture kinectTexure;
    
    ofParameterGroup filterParmeters;
    ofParameter<float> minDepthThreshold;
    ofParameter<float> maxDepthThreshold;
    ofParameter<int> minBrightnessThreshold;
    ofParameter<int> maxBrightnessThreshold;
    
    ofxPanel gui;
    
    ofEasyCam cam;
    
    ofMesh mesh;
    
    
    
};
