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
    filterParmeters.add(alpha.set("Alpha", 0, 0, 1));
    gui.add(filterParmeters);
}


void ofApp::update()
{
    kinect.update();
    
    if (kinect.isFrameNew())
    {
        // Get a copy of the RGB pixels that are registered to the depth image.
        // This means that they have the same width / height.
        ofPixels rgbPixels = kinect.getRegisteredPixels();
        
        nRows = rgbPixels.getHeight();
        nCols = rgbPixels.getWidth();

        if (distanceImage.size() != nRows * nCols)
        {
            distanceImage.resize(nRows * nCols);
        }
        
        ////////////////////////////////////////////////////////////////////////
        /// YOUR WORK HERE ! ///////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // Create the ofMesh.
        //     - clear the ofMesh
        
        mesh.clear();
        
        // for each depth pixel ...
        //     - add a vertex at its x, y, z (depth).
        //     - add a texture coordinate for the corresponding location in the
        //       image.
        //
        //  e.g.
        //  for all x ...
        //     for all y ...
        //         mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
        //         mesh.addTexCoord(glm::vec2(x, y));
        //
        ////////////////////////////////////////////////////////////////////////

        float w = rgbPixels.getWidth();
        float h = rgbPixels.getHeight();

        // Divide up the rectangle into rows / columns.
        for (std::size_t iy = 1; iy < (nRows - 1); iy++)
        {
            for (std::size_t ix = 0; ix < (nCols - 1);  ix++)
            {
                // Calculate the x and y coordinate based on the current row / col
                // and the w / h of the desired plane.
                float x = w * ix / (nCols - 1);
                float y = h * iy / (nRows - 1);
                
                auto world = kinect.getWorldCoordinateAt(x, y);

                mesh.addVertex(world);
                mesh.addTexCoord(glm::vec2(x, y));
                
            }
        }

        if (currentMeshMode == MESH_MODE_0)
        {
            ////////////////////////////////////////////////////////////////////////
            /// YOUR WORK HERE ! ///////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////
            //
            //     - Set the mesh's primitive type to OF_PRIMITIVE_TRIANGLES.
            //     - Create a plane by adding indices that will result in two 
            //       triangles per point. This is essentially the 03_MeshPlane 
            //       example.
            //
            ////////////////////////////////////////////////////////////////////////
            
            mesh.setMode(OF_PRIMITIVE_TRIANGLES);
            
            for (int y = 0; y < nRows - 1; y++)
            {
                for (int x = 0; x < nCols - 1; x++)
                {
                    mesh.addIndex((y + 0) * nCols + (x + 0));
                    mesh.addIndex((y + 0) * nCols + (x + 1));
                    mesh.addIndex((y + 1) * nCols + (x + 0));

                    mesh.addIndex((y + 0) * nCols + (x + 1));
                    mesh.addIndex((y + 1) * nCols + (x + 1));
                    mesh.addIndex((y + 1) * nCols + (x + 0));
                }
            }

        }
        else if (currentMeshMode == MESH_MODE_1)
        {
            ////////////////////////////////////////////////////////////////////////
            /// YOUR WORK HERE ! ///////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////
            //
            //     - Set the mesh's primitive type to OF_PRIMITIVE_TRIANGLE_STRIP.
            //     - Create a plane by adding indices that will result in a surface
            //       created as a triangle strip.
            //     - This should look like MESH_MODE_0.  The goal is to think in
            //       terms of another primitive type.
            //
            ////////////////////////////////////////////////////////////////////////
            
            
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            
            for (std::size_t y = 0; y < (nRows - 1); y++)
            {
                // The even rows.
                if ((y & 1) == 0)
                {
                    for(std::size_t x = 0; x < nCols; x++)
                    {
                        mesh.addIndex( (y + 0) * nCols + (x + 0) );
                        mesh.addIndex( (y + 1) * nCols + (x + 0));
                    }
                }
                else
                {
                    for(std::size_t x = nCols-1; x >0; x--)
                    {
                        mesh.addIndex( (y + 1) * nCols + (x + 0) );
                        mesh.addIndex( (y + 0) * nCols + (x - 1) );
                    }
                }
            }
            
            if (nRows % 2 != 0) mesh.addIndex(mesh.getNumVertices() - nCols);

        }
        else if (currentMeshMode == MESH_MODE_2)
        {
            ////////////////////////////////////////////////////////////////////////
            /// YOUR WORK HERE ! ///////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////
            //
            //     - Set the mesh's primitive type to OF_PRIMITIVE_TRIANGLES.
            //     - Create a like in MESH_MODE_0 above, but ...
            //     - DO NOT generate the triangle if ANY of the three indices of
            //       a given triangle include a depth value that is not between
            //       minDepthThreshold and maxDepthThresholdcooresponds or a
            //       pixel brightness value that is not between minBrightnessThreshold
            //       and maxBrightnessThreshold.
            //
            ////////////////////////////////////////////////////////////////////////
            
            mesh.setMode(OF_PRIMITIVE_TRIANGLES);

            auto depthImage = kinect.getRawDepthPixels();
            
            for (int y = 0; y < nRows - 1; y++)
            {
                for (int x = 0; x < nCols - 1; x++)
                {
                    std::size_t index0 = (y + 0) * nCols + (x + 0);
                    std::size_t index1 = (y + 0) * nCols + (x + 1);
                    std::size_t index2 = (y + 1) * nCols + (x + 0);
                    std::size_t index3 = (y + 1) * nCols + (x + 1);

                    float depth0 = depthImage[index0];//.getColor(index0).getBrightness();
                    float depth1 = depthImage[index1];//.getColor(index1).getBrightness();
                    float depth2 = depthImage[index2];//.getColor(index2).getBrightness();
                    float depth3 = depthImage[index3];//.getColor(index3).getBrightness();

                    int bright0 = rgbPixels.getColor(index0).getBrightness();
                    int bright1 = rgbPixels.getColor(index1).getBrightness();
                    int bright2 = rgbPixels.getColor(index2).getBrightness();
                    int bright3 = rgbPixels.getColor(index3).getBrightness();

                    bool valid0 = (depth0 > minDepthThreshold) && (depth0 < maxDepthThreshold);// && (bright0 > minBrightnessThreshold) && (bright0 < maxBrightnessThreshold);
                    bool valid1 = (depth1 > minDepthThreshold) && (depth1 < maxDepthThreshold);// && (bright1 > minBrightnessThreshold) && (bright1 < maxBrightnessThreshold);
                    bool valid2 = (depth2 > minDepthThreshold) && (depth2 < maxDepthThreshold);// && (bright2 > minBrightnessThreshold) && (bright2 < maxBrightnessThreshold);
                    bool valid3 = (depth3 > minDepthThreshold) && (depth3 < maxDepthThreshold);// && (bright3 > minBrightnessThreshold) && (bright3 < maxBrightnessThreshold);

                    if (valid0 && valid1 && valid2)
                    {
                        mesh.addIndex(index0);
                        mesh.addIndex(index1);
                        mesh.addIndex(index2);
                    }
                    
                    if (valid1 && valid3 && valid2)
                    {
                        mesh.addIndex(index1);
                        mesh.addIndex(index3);
                        mesh.addIndex(index2);
                    }
                    
                }
            }


        }

        
        // Load the pixels into a texture because we will bind to it when we
        // draw the mesh.
        kinectTexure.loadData(rgbPixels);
    }
}

void ofApp::draw()
{
    if (!kinectTexure.isAllocated()) return;
    
    // Enable the depth test to make sure our mesh is rendered correctly.
    ofEnableDepthTest();
    // Being rendering from the viewpoint of the camera.
    cam.begin();
    ofPushMatrix();
//    ofTranslate(0, 0, .5);
    ofScale(200, -200, 200);
    // Bind to the texture, so the mesh knows what Texture our texture
    // coordinates refer to.
    kinectTexure.bind();
    // Draw the mesh and all of its parts.
    mesh.draw();
    // Unbind since we're not using it any more.
    kinectTexure.unbind();
    // End rendering from the viewpoint of the camera.
    ofPopMatrix();
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


