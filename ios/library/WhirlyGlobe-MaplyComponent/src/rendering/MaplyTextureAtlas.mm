/*
 *  MaplyTextureAtlas_private.h
 *  WhirlyGlobe-MaplyComponent
 *
 *  Created by Steve Gifford on 9/11/14.
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
 *
 */

#import "MaplyTextureAtlas_private.h"

using namespace WhirlyKit;

typedef std::set<DynamicTextureAtlas *> DynamicTextureAtlasSet;

class SubTexToAtlas
{
public:
    SubTexToAtlas() : atlas(NULL) { }
    SubTexToAtlas(SimpleIdentity subTexID) : subTex(subTexID), atlas(NULL) { }
    SubTexToAtlas(SimpleIdentity subTexID, DynamicTextureAtlas *atlas) : subTex(subTexID), atlas(atlas) { }
    
    // Comparison operator
    bool operator < (const SubTexToAtlas &that) const
    {
        return subTex.getId() < that.subTex.getId();
    }
    
    // Which sub texture
    SubTexture subTex;
    // Is in which atlas
    DynamicTextureAtlas *atlas;
};

typedef std::set<SubTexToAtlas> SubTexToAtlasSet;

@implementation MaplyTextureAtlasGroup
{
    SceneRenderer *sceneRender;
    Scene *scene;
    DynamicTextureAtlasSet atlases;
    SubTexToAtlasSet subTexMap;
    int atlasSize;
}

- (instancetype)initWithScene:(WhirlyKit::Scene *)inScene sceneRender:(SceneRenderer *)inSceneRender
{
    self = [super init];
    if (!self)
        return nil;
    
    sceneRender = inSceneRender;
    scene = inScene;
    atlasSize = 1024;
    
    return self;
}

- (void)dealloc
{
    for (DynamicTextureAtlasSet::iterator it = atlases.begin();
         it != atlases.end(); ++it)
    {
        DynamicTextureAtlas *atlas = *it;
        delete atlas;
    }
}

- (void)setSize:(int)size
{
    atlasSize = size;
}

- (bool)addTexture:(Texture *)tex subTex:(WhirlyKit::SubTexture &)outSubTex changes:(WhirlyKit::ChangeSet &)changes
{
    std::vector<Texture *> texs;
    texs.push_back(tex);

    DynamicTextureAtlas *foundAtlas = NULL;
    SubTexture subTex;

    @synchronized(self)
    {
        // Look for a match
        for (DynamicTextureAtlasSet::iterator it = atlases.begin();
             it != atlases.end(); ++it)
        {
            DynamicTextureAtlas *atlas = *it;
            if (tex->getFormat() == atlas->getFormat())
                if (atlas->addTexture(sceneRender,texs,-1,NULL,NULL,subTex,changes,0))
                {
                    scene->addSubTexture(subTex);
                    foundAtlas = atlas;
                    break;
                }
        }
        
        // Make up a new texture atlas
        if (!foundAtlas)
        {
            foundAtlas = new DynamicTextureAtlas("Maply Texture Atlas",atlasSize,16,tex->getFormat());
            atlases.insert(foundAtlas);
            if (foundAtlas->addTexture(sceneRender,texs, -1, NULL, NULL, subTex, changes, 0))
                scene->addSubTexture(subTex);
        }
        
        // If we put the texture somewhere, return true
        if (subTex.texId != EmptyIdentity)
        {
            subTexMap.insert(SubTexToAtlas(subTex.getId(), foundAtlas));
            outSubTex = subTex;
            return true;
        }
    }
    
    return false;
}

- (void)removeTexture:(WhirlyKit::SimpleIdentity)subTexID changes:(WhirlyKit::ChangeSet &)changes when:(TimeInterval)when
{
    @synchronized(self)
    {
        SubTexToAtlasSet::iterator it = subTexMap.find(SubTexToAtlas(subTexID));
        if (it != subTexMap.end())
        {
            // Clear out the
            const SubTexToAtlas &entry = *it;
            entry.atlas->removeTexture(entry.subTex, changes, when);
            
            // May need to remove the texture atlas
            entry.atlas->cleanup(changes,when);
            if (entry.atlas->empty())
            {
                entry.atlas->teardown(changes);
                delete entry.atlas;
                atlases.erase(entry.atlas);
            }
            
            subTexMap.erase(it);
        } else {
            NSLog(@"SubTex: Asked to remove sub texture that isn't present.");
        }
    }
}

- (void)clear:(WhirlyKit::ChangeSet &)changes
{
    @synchronized(self)
    {
        for (DynamicTextureAtlasSet::iterator it = atlases.begin();
             it != atlases.end(); ++it)
        {
            DynamicTextureAtlas *atlas = *it;
            atlas->cleanup(changes,0.0);
            delete atlas;
        }
        
        atlases.clear();
    }
}

- (void)dumpStats
{
    int numRegions=0,numDynamicTextures=0;
    
    @synchronized(self)
    {
        for (auto &it : atlases)
        {
            int nr,ndt;
            it->getUsage(nr, ndt);
            numRegions += nr;
            numDynamicTextures += ndt;
        }
    }
    
    NSLog(@"Texture Atlas: %d regions, %d dynamic textures",numRegions,numDynamicTextures);
}

@end
