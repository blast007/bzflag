/* bzflag
 * Copyright (c) 1993-2021 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* interface header */
#include "HUDDialog.h"

/* common implementation headers */
#include "FontManager.h"
#include "StateDatabase.h"

// local interface headers
#include "HUDuiControl.h"
#include "HUDuiLabel.h"

HUDDialog::HUDDialog(std::string _title, HUDDialogLayout _layout) : hasSubtitle(false), layout(_layout), focus(NULL)
{
    height = width = 0;
    menuFont = FontManager::instance().getFaceID(BZDB.get("sansSerifFont"));

    HUDuiLabel* label = new HUDuiLabel;
    label->setFontFace(menuFont);
    label->setString(_title);
    list.push_back(label);
}

HUDDialog::~HUDDialog()
{
    // delete all controls left on list
    const int count = list.size();
    for (int i = 0; i < count; i++)
        delete list[i];
}

void            HUDDialog::render()
{
    const int count = list.size();
    for (int i = 0; i < count; i++)
        list[i]->render();
}

void            HUDDialog::resize(int _width, int _height)
{
    width     = _width;
    height    = _height;

    int i;

    // Grab an instance of the font manager
    FontManager &fm = FontManager::instance();

    // use a big font for title, smaller font for the rest
    const float titleFontSize = (float)height / 15.0f;

    // Calculate the title height
    const float titleHeight = fm.getStrHeight(menuFont, titleFontSize, " ");

    // Determine the space we want to fill
    const float usableHeight = ((float)height - (titleHeight * 1.25f)) * 0.6f;

    // Calculate the number of rows we need to fit. Include hidden controls so that we have room if they are shown.
    std::vector<HUDuiControl*>& listHUD = getControls();
    const int numControls = listHUD.size();
    float numRows = 0;
    for (i = 2; i < numControls; ++i)
    {
      if (listHUD[i]->hasPaddingAfter())
        numRows += (layout == HUDDialogSingleColumn)?1.3f:1.75f;
      else
        numRows += 1.0f;
    }

    // Note that a point is equal to 1.333 pixels, and we want the font size in points
    const float fontSize = std::min(titleFontSize * 0.333f, (float)usableHeight / numRows / 1.333f);


    // reposition title
    HUDuiLabel* title = (HUDuiLabel*)listHUD[0];
    title->setFontSize(titleFontSize);
    const float titleWidth = fm.getStrLength(menuFont, titleFontSize, title->getString());
    float x = 0.5f * ((float)_width - titleWidth);
    float y = (float)_height - titleHeight;
    title->setPosition(x, y);

    i = 1;

    if (hasSubtitle)
    {
      HUDuiLabel* subtitle = (HUDuiLabel*)listHUD[i];

      const float subtitleWidth = fm.getStrLength(menuFont, fontSize, subtitle->getString());
      const float subtitleHeight = fm.getStrHeight(menuFont, fontSize, subtitle->getString());
      x = 0.5f * ((float)_width - subtitleWidth);
      y -= 1.75f * subtitleHeight;
      subtitle->setFontSize(fontSize);
      subtitle->setPosition(0.5f * ((float)_width + subtitleHeight), y);
      i++;
    }

    const float h = fm.getStrHeight(menuFont, fontSize, " ");

    // This layout is used for most of our menus
    if (layout == HUDDialogDoubleColumn)
    {
        // reposition options in two columns
        x = 0.5f * (float)_width;
        y -= 0.6f * titleHeight;


        for ( ; i < numControls; i++)
        {
            HUDuiControl *ctl = listHUD[i];
            // Skip hidden controls
            if (ctl->isHidden())
            {
              // If the control would have had padding, still add the padding
              if (ctl->hasPaddingAfter())
                y -= 0.75 * h;
              continue;
            }
            ctl->setFontSize(fontSize);
            ctl->setPosition(x, y);
            if (ctl->hasPaddingAfter())
                y -= 1.75f * h;
            else
                y -= 1.0f * h;
        }
    }
    // This layout is used for our main menu and our help pages with paragraphs
    else if (layout == HUDDialogSingleColumn)
    {
        // reposition menu items (first is centered, rest aligned to the first)
        HUDuiControl* first = (HUDuiLabel*)listHUD[i];
        // TODO: This assumes the first control is a HUDuiLabel. Can we verify the type?
        const float firstWidth = fm.getStrLength(menuFont, fontSize, ((HUDuiLabel*)first)->getString());
        x = 0.5f * ((float)_width - firstWidth);

        for ( ; i < numControls; i++)
        {
            HUDuiControl* ctl = (HUDuiLabel*)listHUD[i];
            if (ctl->isHidden())
              continue;
            ctl->setFontSize(fontSize);
            ctl->setPosition(x, y);
            if (ctl->hasPaddingAfter())
              y -= 1.3f * h;
            else
              y -= 1.0f * h;
        }
      }
}

HUDuiControl*       HUDDialog::getFocus() const
{
    return focus;
}

void            HUDDialog::setFocus(HUDuiControl* _focus)
{
    focus = _focus;
}

void            HUDDialog::setSubtitle(std::string string1, std::string string2)
{
    HUDuiLabel* subtitle = createLabel(string1, string2);
    list.push_back(subtitle);
    hasSubtitle = true;
}

void HUDDialog::initNavigation(std::vector<HUDuiControl*> &listHUD,
                               int start, int end)
{
    int i;
    const int count = listHUD.size();

    for (i = 0; i < start; i++)
    {
        listHUD[i]->setNext(listHUD[i]);
        listHUD[i]->setPrev(listHUD[i]);
    }

    if (start < end)
    {
        listHUD[start]->setNext(listHUD[start+1]);
        listHUD[start]->setPrev(listHUD[end]);
        for (i = start+1; i < end; i++)
        {
            listHUD[i]->setNext(listHUD[i+1]);
            listHUD[i]->setPrev(listHUD[i-1]);
        }
        listHUD[end]->setNext(listHUD[start]);
        listHUD[end]->setPrev(listHUD[end-1]);
    }

    for (i = end+1; i < count; i++)
    {
        listHUD[i]->setNext(listHUD[i]);
        listHUD[i]->setPrev(listHUD[i]);
    }

    setFocus(listHUD[start]);
}

HUDuiLabel* HUDDialog::createLabel(const std::string &string, const std::string &label)
{
    HUDuiLabel* control = new HUDuiLabel;
    control->setFontFace(menuFont);
    if (string.length() > 0)
      control->setString(string);
    if (label.length() > 0)
      control->setLabel(label);
    return control;
}



// Local Variables: ***
// mode: C++ ***
// tab-width: 4 ***
// c-basic-offset: 4 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=4 tabstop=4
