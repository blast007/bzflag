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
#include "CustomWorldBoundryWall.h"

/* common implementation headers */
#include "StateDatabase.h"

/* system headers */
#include <sstream>
#include <iostream>
#include <string>
#include <string.h>


CustomWorldBoundryWall::CustomWorldBoundryWall()
{
    ricochet = false;
    wallHeight = BZDB.eval("_wallHeight");
    return;
}


static void getIntList (std::istream& input, std::vector<int>& list)
{
    std::string args;
    int value;

    list.clear();
    std::getline(input, args);
    std::istringstream parms(args);
    input.putback('\n');

    while (parms >> value)
        list.push_back(value);

    return;
}


bool CustomWorldBoundryWall::read(const char *cmd, std::istream& input)
{
    if (strcasecmp(cmd, "vertices") == 0)
    {
        getIntList (input, vertexIndexes);
        if (vertexIndexes.size() < 3)
        {
            std::cout << "world boundry walls need at least 3 vertices" << std::endl;
            return false;
        }
    }
    else if (strcasecmp(cmd, "ricochet") == 0)
        ricochet = true;
    else if (strcasecmp(cmd, "height") == 0)
    {
        if (!(input >> wallHeight))
            return false;
    }
    else
    {
        std::cout << "unknown world boundry wall property: " << cmd << std::endl;
        return false;
    }

    return true;
}


void CustomWorldBoundryWall::write(WorldInfo *world, std::vector<cfvec2> vertices) const
{
    // Make sure we have at leat 3 vertex indexes
    if (vertexIndexes.size() < 3)
    {
        std::cout << "world boundry walls need at least 3 vertices" << std::endl;
        return;
    }

    // Validate the vertex indexes
    int vertexCount = vertices.size();
    for (auto vert_it = vertexIndexes.begin(); vert_it != vertexIndexes.end(); ++vert_it)
    {
        if (*vert_it < 0 || *vert_it >= vertexCount)
        {
            std::cout << "world boundry wall requested vertex " << *vert_it << " but only 0 through " << vertexCount-1 <<
                      " are available" << std::endl;
            return;
        }
    }

    double firstVertice[2] = { 0.0, 0.0 };
    double lastVertice[2] = { 0.0, 0.0 };
    for (auto vert_it = vertexIndexes.begin(); vert_it != vertexIndexes.end(); ++vert_it)
    {
        auto vertice = vertices.at(*vert_it);

        // Store the first vertice for later
        if (vert_it == vertexIndexes.begin())
        {
            firstVertice[0] = lastVertice[0] = vertice[0];
            firstVertice[1] = lastVertice[1] = vertice[1];
        }
        else
        {
            world->addWall((float)lastVertice[0], (float)lastVertice[1], (float)vertice[0], (float)vertice[1], wallHeight,
                           ricochet);
            lastVertice[0] = vertice[0];
            lastVertice[1] = vertice[1];
        }
    }
    world->addWall((float)lastVertice[0], (float)lastVertice[1], (float)firstVertice[0], (float)firstVertice[1],
                   wallHeight, ricochet);

    return;
}


// Local variables: ***
// mode: C++ ***
// tab-width: 4***
// c-basic-offset: 4 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=4 tabstop=4
