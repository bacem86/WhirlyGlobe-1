/*  MaplyAnimateTranslation.cpp
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 1/20/12.
 *  Copyright 2011-2022 mousebird consulting
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#import "SceneRenderer.h"
#import "MaplyAnimateTranslation.h"

using namespace WhirlyKit;
using namespace Eigen;

namespace Maply {
    
bool MaplyGestureWithinBounds(const Point2dVector &bounds,const Point3d &loc,
                              SceneRenderer *sceneRender,MapView *testMapView,Point3d *newCenter)
{
    if (newCenter)
        *newCenter = loc;
    
    if (bounds.empty())
        return true;
    
    // The corners of the view should be within the bounds
    Point2f frameSize = sceneRender->getFramebufferSize();
    const Point2f corners[4] = {
        { 0, 0 },
        { frameSize.x(), 0.0 },
        frameSize,
        { 0.0, frameSize.y() },
    };

    bool isValid = false;
    Point2d locOffset(0,0);
    for (unsigned tests=0;tests<4;tests++)
    {
        Point3d newLoc = loc+Point3d(locOffset.x(),locOffset.y(),0.0);
        testMapView->setLoc(newLoc,false);
        Eigen::Matrix4d fullMatrix = testMapView->calcFullMatrix();
        
        bool checkOkay = true;
        for (unsigned int ii=0;ii<4;ii++)
        {
            Point3d planePt;
            testMapView->pointOnPlaneFromScreen(corners[ii], &fullMatrix, frameSize, &planePt, false);
            if (!PointInPolygon(Point2d(planePt.x(),planePt.y()), bounds))
            {
                Point2d closePt;
                ClosestPointToPolygon(bounds, Point2d(planePt.x(),planePt.y()), &closePt);
                Point2d thisOffset = 1.001 * (closePt - Point2d(planePt.x(),planePt.y()));
                // Try to move around, inward
                locOffset += thisOffset;
                checkOkay = false;
                
                break;
            }
        }
        
        if (checkOkay)
        {
            isValid = true;
            if (newCenter)
                *newCenter = newLoc;
            break;
        }
    }
    
    return isValid;
}
    
AnimateViewTranslation::AnimateViewTranslation(const MapViewRef &mapView,
                                               WhirlyKit::SceneRenderer *inRenderer,
                                               Point3d &newLoc,float howLong) :
    renderer(inRenderer)
{
    startDate = TimeGetCurrent();
    endDate = startDate + howLong;
    startLoc = mapView->getLoc();
    endLoc = newLoc;
}

void AnimateViewTranslation::setBounds(const Point2d *inBounds)
{
    bounds.clear();
    bounds.reserve(4);
    for (unsigned int ii=0;ii<4;ii++)
        bounds.push_back(inBounds[ii]);
}

// Bounds check on a single point
bool AnimateViewTranslation::withinBounds(const Point3d &loc,MapView * testMapView,Point3d *newCenter)
{
    return MaplyGestureWithinBounds(bounds,loc,renderer,testMapView,newCenter);
}

void AnimateViewTranslation::updateView(WhirlyKit::View *view)
{
    auto mapView = (MapView *)view;
    if (startDate == 0.0)
        return;

    const TimeInterval now = TimeGetCurrent();
    const TimeInterval span = endDate - startDate;
    const TimeInterval remain = endDate - now;
    
    Point3d newLoc;
    if (remain < 0)
    {
        // All done, snap to end
        newLoc = endLoc;
        startDate = 0;
        endDate = 0;
        mapView->cancelAnimation();
    }
    else
    {
        // Interpolate in the middle
        const float t = (span-remain)/span;
        newLoc = startLoc + (endLoc-startLoc)*t;
    }
    
    // Test the prospective point first
    Point3d newCenter;
    MapView testMapView(*mapView);
    if (withinBounds(newLoc, &testMapView, &newCenter))
    {
        mapView->setLoc(newCenter);
    }
}

}
