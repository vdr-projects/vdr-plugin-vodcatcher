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
 * $Id: OsdListMenu.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include "OsdListMenu.h"
#include "IListMenuPresenter.h"

using namespace std;

OsdListMenu::OsdListMenu(RefPtr<IListMenuPresenter> presenter, int tabStop1)
:cOsdMenu("", tabStop1), presenter(presenter)
{
    this->presenter->Initialize(this);
}

void OsdListMenu::SetTitle(const string title)
{
    cOsdMenu::SetTitle(title.c_str());
}

eOSState OsdListMenu::ProcessKey(eKeys Key)
{
    eOSState state = cOsdMenu::ProcessKey(Key);

    return presenter->ProcessKey(Current(), Key, state);
}

void OsdListMenu::AddItem(const std::string item)
{
    Add(new cOsdItem(item.c_str()));
}

eOSState OsdListMenu::ShowSubMenu(cOsdMenu* menu)
{
    return AddSubMenu(menu);
}

void OsdListMenu::Clear()
{
    cOsdMenu::Clear();
}

void OsdListMenu::Refresh()
{
    cOsdMenu::Display();
}

void OsdListMenu::SetRedHelp(const string text)
{
    redHelp = text;
    UpdateHelpButtons();
}

void OsdListMenu::SetGreenHelp(const string text)
{
    greenHelp = text;
    UpdateHelpButtons();
}

void OsdListMenu::SetYellowHelp(string text)
{
    yellowHelp = text;
    UpdateHelpButtons();
}

void OsdListMenu::SetBlueHelp(const string text)
{
    blueHelp = text;
    UpdateHelpButtons();
}

void OsdListMenu::UpdateHelpButtons()
{
    cOsdMenu::SetHelp(EmptyToNull(redHelp.c_str()),
      EmptyToNull(greenHelp.c_str()), EmptyToNull(yellowHelp.c_str()),
      EmptyToNull(blueHelp.c_str()));
}

const char* OsdListMenu::EmptyToNull(const char* str) const
{
    if (*str)
    {
        return str;
    }
    else
    {
        return NULL;
    }
}

bool OsdListMenu::IsActiveMenu()
{
    return !HasSubMenu();
}
