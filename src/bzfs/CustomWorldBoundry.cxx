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

#include "common.h"

/* interface header */
#include "CustomWorldBoundry.h"

/* bzfs implementation headers */
#include "CustomWorldBoundryWall.h"


CustomWorldBoundry::CustomWorldBoundry()
{
    wall = NULL;

    return;
}


CustomWorldBoundry::~CustomWorldBoundry()
{
    if (wall != NULL)
    {
        std::cout << "discarded incomplete world boundry wall" << std::endl;
        delete wall;
    }

    for (auto wall_it = walls.begin(); wall_it != walls.end(); ++wall_it)
    {
        CustomWorldBoundryWall* _wall = *wall_it;
        delete _wall;
    }

    return;
}


bool CustomWorldBoundry::read(const char *cmd, std::istream& input)
{
    if (strcasecmp(cmd, "endwall") == 0)
    {
        if (wall == NULL)
            std::cout << "extra 'endwall' keyword found" << std::endl;
        else
        {
            walls.push_back(wall);
            wall = NULL;
        }
    }
    else if (wall)
    {
        // currently processing a wall
        return wall->read(cmd, input);
    }
    else if (strcasecmp(cmd, "wall") == 0)
    {
        if (wall != NULL)
        {
            std::cout << "discarding incomplete world boundry wall" << std::endl;
            delete wall;
        }
        wall = new CustomWorldBoundryWall ();
    }
    else if (strcasecmp(cmd, "vertex") == 0)
    {
        cfvec2 vertex;
        if (!(input >> vertex[0] >> vertex[1]))
            return false;
        vertices.push_back(vertex);
    }
    else
    {
        // NOTE: the position, size, and rotation
        //       parameters are currently ignored
        return WorldFileObstacle::read(cmd, input);
    }

    return true;
}

void CustomWorldBoundry::writeToWorld(WorldInfo* world) const
{
    //std::vector<CustomWorldBoundryWall*>::iterator wall_it;
    for (auto wall_it = walls.begin(); wall_it != walls.end(); ++wall_it)
    {
        const CustomWorldBoundryWall* curWall = *wall_it;
        curWall->write(world, vertices);
    }
}


// Local variables: ***
// mode: C++ ***
// tab-width: 4***
// c-basic-offset: 4 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=4 tabstop=4
