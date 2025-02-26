/*
 *  WGViewTracker.h
 *  WhirlyGlobeComponent
 *
 *  Created by Steve Gifford on 7/26/12.
 *  Copyright 2012-2022 mousebird consulting
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
 *
 */

#import <UIKit/UIKit.h>
#import "math/MaplyCoordinate.h"

/** 
    The View Tracker associates a view with a geographic location.
    
    The Maply View Tracker will move a UIView around to keep track of a geographic location.  This is basically used for popups.  The system will move the view around at the end of the frame render.  It will hide the UIView if needed or make it reappear.  The UIView should be a child of view controller's view.
  */
@interface MaplyViewTracker : NSObject

/** 
    The UIView we want moved around.
    
    This is the UIView we'll tie to the geographic location.  If you want to center it, use offsets within the UIView.
  */
@property (nonatomic,strong) UIView *__nullable view;

/** 
    The geographic location where we want to place the UIView.
    
    This is the location (lon/lat in radians) where we want to stick the UIView.  The location on screen will be updated as the user manipulates the map or globe.
  */
@property (nonatomic,assign) MaplyCoordinate loc;

/** 
    An offset in screen space for the view tracker.
    
    This offset is added to the location after it's projected into screen space.
 */
@property (nonatomic, assign) CGPoint offset;

/** 
    The lowest height at which we'll see the view tracker.
    
    This value is in display coordinates.
  */
@property (nonatomic,assign) float minVis;

/** 
    the maximum height at which we'll see the view being tracked.
    
    This value is in display coordinates.
  */
@property (nonatomic,assign) float maxVis;

@end

typedef MaplyViewTracker WGViewTracker;
