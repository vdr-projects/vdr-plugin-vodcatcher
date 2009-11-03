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

#include <vdr/player.h>
#include "ItemMenuPresenter.h"
#include "IServiceLocator.h"
#include "IDownloadPool.h"
#include "IDownloadCache.h"
#include "Download.h"
#include <sstream>
#include "MplayerPlugin.h"
#include "RssFeedParser.h"
#include "IFeedRepository.h"
#include "IMenuFactory.h"
#include "IConfiguration.h"
#include "IVdrInterface.h"

using namespace std;

ItemMenuPresenter::ItemMenuPresenter(IServiceLocator& serviceLocator, Feed feed,
    RefPtr<IFeedRepository> feedRepository, IConfiguration& configuration, RefPtr<IMediaPlayer> mplayerPlugin,
    RefPtr<IVdrInterface> vdrInterface) :
    serviceLocator(serviceLocator), feed(feed), _feedRepository(feedRepository), _configuration(configuration),
        _mplayerPlugin(mplayerPlugin), _vdrInterface(vdrInterface)
{
}

void ItemMenuPresenter::Initialize(IListMenu* listMenu)
{
    menu = listMenu;
    menu->SetTitle(feed.GetTitle());
    UpdateMenu();
}

const eOSState ItemMenuPresenter::ProcessKey(const int selectedItem, const eKeys key, const eOSState state)
{
    if (state == osUnknown)
    {
        if (IsValidItemSelection(selectedItem))
        {
            return ProcessItemAction(key, displayedItems[selectedItem]);
        }
    }

    if (menu->IsActiveMenu())
    {
        if ((key == kDown) || (key == kUp) || (key == kRight) || (key == kLeft) || (key == kNone))
        {
            UpdateDownloadAndPlayButton(selectedItem);
        }
    }

    return state;
}

bool ItemMenuPresenter::IsValidItemSelection(int selectedItem)
{
    int maxItems = displayedItems.size();
    return ((selectedItem < maxItems) && (selectedItem >= 0));
}

eOSState ItemMenuPresenter::ProcessItemAction(eKeys key, Item item)
{
    switch (key)
    {
        case kOk:
            if (item.IsCategory())
            {
                ShowSubCategory(item);
            }
            else
            {
                ShowItemView(item);
            }
            break;

        case kGreen:
            if (PlayItem(item))
            {
                return osEnd;
            }
            else
            {
                return osContinue;
            }

        case kYellow:
            switch (_configuration.GetPlayBackQuality())
            {
                case Low:
                    _configuration.SetPlayBackQuality(High);
                    break;
                case Medium:
                    _configuration.SetPlayBackQuality(Low);
                    break;
                case High:
                    _configuration.SetPlayBackQuality(Medium);
                    break;
            }
            UpdateYellowButton();
            break;
        default:
            break;
    }
    return osContinue;
}

void ItemMenuPresenter::UpdateYellowButton() const
{
    menu->SetYellowHelp(StreamTypeToString(_configuration.GetPlayBackQuality()));
}

void ItemMenuPresenter::ShowItemView(Item item)
{
    cOsdMenu* itemView = serviceLocator.CreateItemView(feed, item);
    menu->ShowSubMenu(itemView);
}

void ItemMenuPresenter::ShowSubCategory(Item item)
{
    Feed feed = _feedRepository->GetFeed(item.GetSubFeedUrl());
    cOsdMenu* itemMenu = serviceLocator.GetMenuFactory()->CreateItemMenu(feed);
    menu->ShowSubMenu(itemMenu);
}

bool ItemMenuPresenter::PlayItem(Item item)
{
    if (item.IsVideocast())
    {
        if (_mplayerPlugin->Play(item.GetVideoCastStream(_configuration.GetPlayBackQuality())))
        {
            _vdrInterface->ShowMessage(tr("Starting playback, please wait..."));
            return true;
        }
        else
        {
            _vdrInterface->ShowErrorMessage(tr("Playback failed!"));
        }
    }
    return false;
}

void ItemMenuPresenter::UpdateMenu()
{
    UpdateItemList();
    UpdateDownloadAndPlayButton(0);

    menu->Refresh();
}

void ItemMenuPresenter::UpdateItemList()
{
    displayedItems.clear();
    menu->Clear();
    
    RefPtr<Download> download=serviceLocator.GetDownloadPool()->GetDownloadByUrl(feed.GetUrl());
    if (download.get())
    {
        menu->AddItem(tr("Downloading...Please wait!"));
    }
    else
    {
        ItemList items = feed.GetItems();

        if (items.empty())
        {
            menu->AddItem(tr("No entries!"));

        }
        else
        {
            for (ItemList::iterator i = items.begin(); i != items.end(); i++)
            {
                string menuEntry = (*i).GetDate().ToShortString();
                menuEntry += "\t" + (*i).GetTitle();
                if ((*i).GetStreamLength() != "")
                {
                    menuEntry += " (" + (*i).GetStreamLength() + ")";
                }
                menu->AddItem(menuEntry);
                displayedItems.push_back(*i);
            }
        }
    }
}

void ItemMenuPresenter::UpdateDownloadAndPlayButton(int selectedItem)
{
    if (IsValidItemSelection(selectedItem))
    {
        Item item = displayedItems[selectedItem];
        if (item.IsVideocast())
        {
            menu->SetGreenHelp(tr("Play"));
            menu->SetRedHelp(tr("Record"));
            UpdateYellowButton();
        }
    }
}
