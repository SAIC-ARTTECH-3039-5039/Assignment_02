#include "ofApp.h"


void ofApp::setup()
{
    // Set the background to black.
    ofBackground(0);
    
    // Initialize Kinect V2.
    auto deviceList = ofxKinectV2().getDeviceList();

    // Check to see if there is a KinectV2 attached.
    if (!deviceList.empty())
    {
        // Open the Kinect.
        kinect.open(deviceList[0].serial);
    }
    else
    {
        ofLogError() << "Failure: No Kinect V2 found, closing.";
        ofExit();
    }

    // Set up GUI.
    gui.setup();
    filterParmeters.setName("Filter Parameters");
    filterParmeters.add(minDepthThreshold.set("Min. Depth", 500, 0, 12000));
    filterParmeters.add(maxDepthThreshold.set("Max. Depth", 6000, 0, 12000));
    filterParmeters.add(minBrightnessThreshold.set("Min. Bright", 0, 0, 255));
    filterParmeters.add(maxBrightnessThreshold.set("Max. Bright", 255, 0, 255));
    gui.add(filterParmeters);
}


void ofApp::update()
{
    kinect.update();
    
    if (kinect.isFrameNew())
    {
        ofFloatPixels rawDepthPixels = kinect.getRawDepthPixels();
        ofPixels rgbPixels = kinect.getRgbPixels();
        
        ////////////////////////////////////////////////////////////////////////
        /// YOUR WORK HERE ! ///////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // Create the ofMesh.
        //     - clear the ofMesh
        // for each depth pixel ...
        //     - add a vertex at its x, y, z (depth).
        //     - add a texture coordinate for the corresponding location in the
        //       image.
        ////////////////////////////////////////////////////////////////////////

        
        if (currentMeshMode == MESH_MODE_0)
        {
            ////////////////////////////////////////////////////////////////////////
            /// YOUR WORK HERE ! ///////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////
            //
            //     - set the mesh's primitive type to OF_PRIMITIVE_TRIANGLES.
            //     - create plane (this is essentially the 03_MeshPlane example)
            //       by adding indices that will result in two triangles per
            //       point.
            //
            ////////////////////////////////////////////////////////////////////////
        }
        else if (currentMeshMode == MESH_MODE_1)
        {
            ////////////////////////////////////////////////////////////////////////
            /// YOUR WORK HERE ! ///////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////
            //
            //     - set the mesh's primitive type to OF_PRIMITIVE_TRIANGLE_STRIP.
            //     - create plane by adding indices that will result in a surface
            //       created as a triangle strip.
            //     - this should look very similar (if not exactly the same as
            //       MESH_MODE_0. the goal is to think in another primitive
            //       mode.
            //
            ////////////////////////////////////////////////////////////////////////
        }
        else if (currentMeshMode == MESH_MODE_2)
        {
            ////////////////////////////////////////////////////////////////////////
            /// YOUR WORK HERE ! ///////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////
            //
            //     - set the mesh's primitive type to OF_PRIMITIVE_TRIANGLES.
            //     - create plane (this is essentially the 03_MeshPlane example)
            //       by adding indices that will result in two triangles per
            //       point.
            //     - The DIFFERENCE BETWEEN THIS AN MESH_MODE_0
            //     - DO NOT generate the triangle if ANY of the three indices of
            //       a given triangle include a depth value that is not between
            //       minDepthThreshold and maxDepthThresholdcooresponds or a
            //       pixel brightness value that is not between minBrightnessThreshold
            //       and maxBrightnessThreshold.
            //
            ////////////////////////////////////////////////////////////////////////
        }

        
        // Load the pixels into a texture because we will bind to it when we
        // draw the mesh.
        kinectTexure.loadData(rgbPixels);
    }
}

void ofApp::draw()
{
    // Enable the depth test to make sure our mesh is rendered correctly.
    ofEnableDepthTest();
    // Being rendering from the viewpoint of the camera.
    cam.begin();
    // Bind to the texture, so the mesh knows what Texture our texture
    // coordinates refer to.
    kinectTexure.bind();
    // Draw the mesh and all of its parts.
    mesh.draw();
    // Unbind since we're not using it any more.
    kinectTexure.unbind();
    // End rendering from the viewpoint of the camera.
    cam.end();
    // We must disable depth test when drawing the GUI.
    ofDisableDepthTest();
    // Draw the GUI.
    gui.draw();
}


void ofApp::keyPressed(int key)
{
    // If the spacebar is pressed ...
    if (key == ' ')
    {
        ////////////////////////////////////////////////////////////////////////
        /// YOUR WORK HERE ! ///////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        //
        // Re-write this switch statement as an if / else if / else statement.
        switch (currentMeshMode)
        {
            case MESH_MODE_0:
                currentMeshMode = MESH_MODE_1;
                break;
            case MESH_MODE_1:
                currentMeshMode = MESH_MODE_2;
                break;
            case MESH_MODE_2:
                currentMeshMode = MESH_MODE_0;
                break;
        }
        ////////////////////////////////////////////////////////////////////////
    }
}


