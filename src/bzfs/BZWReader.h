/* bzflag
 * Copyright (c) 1993-2010 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __BZWREADER_H__
#define __BZWREADER_H__

// bzflag common header
#include "common.h"

#include "net/network.h"

// system headers
#include <iostream>
#include <string>
#include <vector>

/* bzflag common headers */
#include "BZWError.h"
#include "common/cURLManager.h"

class WorldFileObject;
class WorldInfo;

class BZWReader : private cURLManager {
  public:
    BZWReader(const std::string& filename);
    BZWReader(std::istream& in);
    ~BZWReader();

    // external interface
    WorldInfo* defineWorldFromFile();

  private:
    // functions for internal use
    void readToken(char* buffer, int n);
    bool readWorldStream(std::vector<WorldFileObject*>& wlist,
                         class GroupDefinition* groupDef,
                         bool& gotWorld);
    void finalization(char* data, unsigned int length, bool good);

    bool parseNormalObject(const char* token, WorldFileObject** object);
    bool parseCustomObject(const char* token, bool& error, int& lineNum,
                           std::vector<WorldFileObject*>& wlist,
                           GroupDefinition* groupDef, bool& gotWorld);
    bool readRawLines(std::string& args, std::vector<std::string>& lines,
                      const std::string& endToken, int& lineNum);

    // stream to open
    std::string location;
    std::istream* input;

    // data/dependent objects
    BZWError* errorHandler;

    // no default constructor
    BZWReader();

    std::string httpData;

    // states if we read from a blob
    bool fromBlob;

    // mapinfo text lines
    std::vector<std::string> mapInfoLines;
};

#endif

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=2 tabstop=8 expandtab
