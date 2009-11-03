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

#include "OsdItemView.h"
#include "ItemViewPresenter.h"

using namespace std;

OsdItemView::OsdItemView(RefPtr<ItemViewPresenter> presenter)
:cMenuText("", ""), presenter(presenter)
{
    this->presenter->Initialize(this);
}

void OsdItemView::SetTitle(string title)
{
    cMenuText::SetTitle(title.c_str());
}

void OsdItemView::SetDescription(string description)
{
    SetText(description.c_str());
}

eOSState OsdItemView::ProcessKey(eKeys Key)
{
    eOSState state = cMenuText::ProcessKey(Key);

    return presenter->ProcessKey(state, Key);
}

void OsdItemView::SetRedHelp(string text)
{
    redHelp = text;
    UpdateHelpButtons();
}

void OsdItemView::SetGreenHelp(string text)
{
    greenHelp = text;
    UpdateHelpButtons();
}

void OsdItemView::SetYellowHelp(string text)
{
    yellowHelp = text;
    UpdateHelpButtons();
}

void OsdItemView::SetBlueHelp(string text)
{
    blueHelp = text;
    UpdateHelpButtons();
}

void OsdItemView::UpdateHelpButtons()
{
    cMenuText::SetHelp(EmptyToNull(redHelp.c_str()),
      EmptyToNull(greenHelp.c_str()), EmptyToNull(yellowHelp.c_str()),
      EmptyToNull(blueHelp.c_str()));
}

const char* OsdItemView::EmptyToNull(const char* str)
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
