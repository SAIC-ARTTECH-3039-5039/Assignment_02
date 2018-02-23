#pragma once


#include "ofMain.h"
#include "ofxKinectV2.h"
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
    ofxKinectV2 kinect;
    ofTexture kinectTexure;
    
    ofParameterGroup filterParmeters;
    ofParameter<float> minDepthThreshold;
    ofParameter<float> maxDepthThreshold;
    ofParameter<int> minBrightnessThreshold;
    ofParameter<int> maxBrightnessThreshold;
    ofParameter<float> alpha;

    ofxPanel gui;
    
    ofEasyCam cam;
    
    ofMesh mesh;
    
    
    std::size_t nRows = 512;
    std::size_t nCols = 512;
    
    std::vector<glm::vec3> distanceImage;
    
    
};
