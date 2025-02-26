/*  MaplyMBTileFetcher.h
 *  WhirlyGlobe-MaplyComponent
 *
 *  Created by Steve Gifford on 9/13/18.
 *  Copyright 2011-2022 mousebird consulting inc
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

#import "loading/MaplyTileSourceNew.h"
#import "loading/MaplySimpleTileFetcher.h"
#import "math/MaplyCoordinateSystem.h"

/**
    MBTiles tile fetcher.
 
    This tile fetcher focuses on a single MBTiles file.  You mate this
    with a QuadImageLoader to do the actual work.

    Will work for image or vector MBTiles files.
  */
@interface MaplyMBTileFetcher : MaplySimpleTileFetcher

/// Initialize with the name of the local MBTiles file
- (nullable instancetype)initWithMBTiles:(NSString *__nonnull)fileName;

/// Initialize with the name of the local MBTiles file and cache size in bytes (rounded up to whole pages)
- (nullable instancetype)initWithMBTiles:(NSString *__nonnull)fileName
                               cacheSize:(int)cacheSize;

// Coordinate system (probably Spherical Mercator)
- (MaplyCoordinateSystem * __nonnull)coordSys;

/// Format directly from the metadata
@property (nonatomic,nullable) NSString *format;

@end
