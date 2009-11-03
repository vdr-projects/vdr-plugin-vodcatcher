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

#include "FeedMenuController.h"
#include "IServiceLocator.h"
#include "IListMenu.h"
#include "IMenuFactory.h"
#include "IFeedRepository.h"

FeedMenuController::FeedMenuController(IMenuFactory& menuFactory, const IFeedRepository& feedRepository) :
    _menuFactory(menuFactory), _feedRepository(feedRepository)
{
}

void FeedMenuController::Initialize(IListMenu* listMenu)
{
    _menu = listMenu;

    _menu->SetTitle(tr("Video Podcasts"));

    _feeds = _feedRepository.GetRootFeeds();

    for (FeedList::iterator i = _feeds.begin(); i != _feeds.end(); i++)
    {
        _menu->AddItem(i->GetTitle());
    }
}

const eOSState FeedMenuController::ProcessKey(const int selectedFeed, const eKeys key, const eOSState state)
{
    if (!state == osUnknown)
    {
        return state;
    }

    switch (key)
    {
        case kOk:
        {
            if ((selectedFeed >= 0) && ((unsigned int) selectedFeed < _feeds.size()))
            {
                cOsdMenu* itemMenu = _menuFactory.CreateItemMenu(_feeds[selectedFeed]);
                _menu->ShowSubMenu(itemMenu);
            }
            return osContinue;
        }

        default:
        {
            return state;
        }
    }
}
