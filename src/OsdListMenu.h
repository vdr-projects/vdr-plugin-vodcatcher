/*
 * vdr-vodcatcher - A plugin for the Linux Video Disk Recorder
 * Copyright (c) 2007 - 2009 Tobias Grimm <vdr@e-tobi.net>
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
 */

#ifndef ___OSDLISTMENU_H
#define ___OSDLISTMENU_H

#include <string>
#include <vdr/osdbase.h>
#include "IListMenu.h"
#include "RefPtr.h"

class IListMenuPresenter;

class OsdListMenu : public cOsdMenu, public IListMenu
{
private:
    RefPtr<IListMenuPresenter> presenter;
    std::string redHelp;
    std::string greenHelp;
    std::string yellowHelp;
    std::string blueHelp;

private:
    void UpdateHelpButtons();
    const char* EmptyToNull(const char* str) const;

public:
    OsdListMenu(RefPtr<IListMenuPresenter> presenter, int tabStop1 = 0);
    // <cOsdMenu>
    eOSState ProcessKey(eKeys Key);
    // <cOsdMenu>

    // <IListMenu>
    void SetTitle(const std::string title);
    void SetRedHelp(const std::string text);
    void SetGreenHelp(const std::string text);
    void SetYellowHelp(const std::string text);
    void SetBlueHelp(const std::string text);
    void AddItem(const std::string item);
    eOSState ShowSubMenu(cOsdMenu* menu);
    void Clear();
    void Refresh();
    bool IsActiveMenu();
    // </IListMenu>
};

#endif // ___OSDLISTMENU_H
