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
 * $Id: OsdItemView.h 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#ifndef ___OSDITEMVIEW_H
#define ___OSDITEMVIEW_H

#include <vdr/menu.h>
#include "ItemView.h"
#include "RefPtr.h"

class ItemViewPresenter;

class OsdItemView : public cMenuText, public IItemView
{
private:
    RefPtr<ItemViewPresenter> presenter;
    std::string redHelp;
    std::string greenHelp;
    std::string yellowHelp;
    std::string blueHelp;

private:
    void UpdateHelpButtons();
    const char* EmptyToNull(const char* str);

public:
    OsdItemView(RefPtr<ItemViewPresenter> presenter);
    eOSState ProcessKey(eKeys Key);
    // <IItemView>
    void SetDescription(std::string description);
    void SetTitle(std::string title);
    void SetRedHelp(std::string text);
    void SetGreenHelp(std::string text);
    void SetYellowHelp(std::string text);
    void SetBlueHelp(std::string text);
    // </IItemView>
};

#endif
