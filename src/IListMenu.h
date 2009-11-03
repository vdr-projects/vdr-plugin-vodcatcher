/*
 * vdr-vodcatcher - A plugin for the Linux Video Disk Recorder
 * Copyright (c) 2007 - 2008 Tobias Grimm <vdr@e-tobi.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * $Id: IListMenu.h 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#ifndef ___ILISTMENU_H
#define ___ILISTMENU_H

#include <string>
#include <vdr/osdbase.h>
#include "IMenu.h"

class IListMenu : public IMenu
{
public:
    virtual ~IListMenu()
    {
    }
    virtual void SetTitle(const std::string title) = 0;
    virtual void AddItem(const std::string item) = 0;
    virtual eOSState ShowSubMenu(cOsdMenu* menu) = 0;
    virtual void Clear() = 0;
    virtual void Refresh() = 0;
    virtual bool IsActiveMenu() = 0;
};

#endif
