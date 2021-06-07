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

#ifndef __HUDDIALOG_H__
#define __HUDDIALOG_H__

/* common */
#include "common.h"

#include "HUDuiLabel.h"

/* system headers */
#include <string>
#include <vector>

/* local interface headers */
class HUDuiControl;
class HUDuiDefaultKey;

enum HUDDialogLayout {
    // Used for main menu and help pages
    HUDDialogSingleColumn,
    // Used for most other menus
    HUDDialogDoubleColumn,
    // For special layouts, such as the video resolution and key mapping menus
    HUDDialogCustom
};

/**
 * HUDDialog:
 *  A dialog of HUDuiControls.
 */
class HUDDialog
{
public:
    HUDDialog(std::string title, HUDDialogLayout layout = HUDDialogDoubleColumn);
    virtual     ~HUDDialog();

    void            render();

    virtual HUDuiDefaultKey*    getDefaultKey() = 0;
    virtual void        show() {}
    virtual void        execute() = 0;
    virtual void        dismiss() {}
    virtual void        resize(int _width, int _height);
    virtual void        setFailedMessage(const char *)
    {
        ;
    };

    HUDuiControl*       getFocus() const;
    void            setFocus(HUDuiControl*);
    void            setSubtitle(std::string string1, std::string string2 = "");

    void            initNavigation(std::vector<HUDuiControl*> &list, int start, int end);

    HUDuiLabel*     createLabel(const std::string &string, const std::string &label = "");



protected:
    const std::vector<HUDuiControl*>&   getControls() const
    {
        return list;
    }
    std::vector<HUDuiControl*>&     getControls()
    {
        return list;
    }

    int             getHeight() const
    {
        return height;
    }
    int             getWidth() const
    {
        return width;
    }

protected:
    int             height, width, menuFont;
    bool            hasSubtitle;
    HUDDialogLayout layout;

private:
    std::vector<HUDuiControl*>  list;
    HUDuiControl*       focus;
};


#endif /* __HUDDIALOG_H__ */

// Local Variables: ***
// mode: C++ ***
// tab-width: 4 ***
// c-basic-offset: 4 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=4 tabstop=4
