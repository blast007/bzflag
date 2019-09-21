/* bzflag
 * Copyright (c) 1993-2018 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __CUSTOM_WORLD_BOUNDRY_WALL_H__
#define __CUSTOM_WORLD_BOUNDRY_WALL_H__

/* common interface header */
#include "WallObstacle.h"
#include "WorldInfo.h"
#include "vectors.h"

/* system header */
#include <string>
#include <vector>


class CustomWorldBoundryWall
{
public:
    CustomWorldBoundryWall();
    bool read(const char *cmd, std::istream& input);
    void write(WorldInfo* world, std::vector<cfvec2> vertices) const;

private:
    std::vector<int> vertexIndexes;

    float wallHeight;

    bool ricochet;
};


#endif  /* __CUSTOM_WORLD_BOUNDRY_WALL_H__ */

// Local variables: ***
// mode: C++ ***
// tab-width: 4***
// c-basic-offset: 4 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=4 tabstop=4
