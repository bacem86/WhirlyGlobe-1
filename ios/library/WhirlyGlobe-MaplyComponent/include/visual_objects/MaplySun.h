/*  MaplySun.h
 *  WhirlyGlobe-MaplyComponent
 *
 *  Created by Steve Gifford on 6/24/15.
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

#import <UIKit/UIKit.h>
#import "visual_objects/MaplyComponentObject.h"
#import "control/WhirlyGlobeViewController.h"
#import "rendering/MaplyLight.h"

/** 
    Utility for calculating sun position and shading info.
    
    This is a utility class that figures out where the sun is at a given date and provides positional information for lighting calculations.
  */
@interface MaplySun : NSObject

/** 
    Initialize with a date.
    
    Initialize with the given date.  The sun position will correspond to that.
  */
- (nonnull instancetype)initWithDate:(NSDate *__nonnull)date;

/// Return the vector corresponding to the sun location from the earth.
- (MaplyCoordinate3d)getDirection;

/// Makes up a light that corresponds to the sun's location at a given time
- (nonnull MaplyLight *)makeLight;

/// Returns the location above the globe in lon/lat.  Yay geocentrism!
- (MaplyCoordinate3d)asPosition;

@end
