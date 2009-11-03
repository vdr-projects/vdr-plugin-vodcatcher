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

#include "ListMenuMock.h"

using namespace std;

ListMenuMock::ListMenuMock()
{
    red = "";
    green = "";
    yellow = "";
    blue = "";
    isActiveMenu = true;
}


void ListMenuMock::SetTitle(const string title)
{
    AddMethod("SetTitle", title);
}

void ListMenuMock::AddItem(const string item)
{
    AddMethod("AddItem", item);
}

eOSState ListMenuMock::ShowSubMenu(cOsdMenu* menu)
{
    AddMethod("ShowSubMenu", MakeString(menu));
    return osContinue;
}

void ListMenuMock::Clear()
{
    AddMethod("Clear");
}

void ListMenuMock::Refresh()
{
    AddMethod("Refresh");
}

void ListMenuMock::SetRedHelp(const string text)
{
	AddMethod("SetRedHelp", text);
    red = text;
}

void ListMenuMock::SetGreenHelp(const string text)
{
	AddMethod("SetGreenHelp", text);
    green = text;
}

void ListMenuMock::SetYellowHelp(const string text)
{
	AddMethod("SetYellowHelp", text);
    yellow = text;
}

void ListMenuMock::SetBlueHelp(const string text)
{
	AddMethod("SetBlueHelp", text);
    blue = text;
}

bool ListMenuMock::IsActiveMenu()
{
    return isActiveMenu;
}
