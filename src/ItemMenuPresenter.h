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

#ifndef ___ITEMMENUPRESENTER_H
#define ___ITEMMENUPRESENTER_H

#include <vdr/osdbase.h>
#include "IListMenuPresenter.h"
#include "IListMenu.h"
#include "Feed.h"
#include "RefPtr.h"

class IServiceLocator;
class IItemMenu;
class IMediaPlayer;
class IFeedRepository;
class IConfiguration;
class IVdrInterface;

class ItemMenuPresenter : public IListMenuPresenter
{
private:
    IServiceLocator& serviceLocator;
    Feed feed;
    ItemList displayedItems;
    IListMenu* menu;
    bool _downloading;
    RefPtr<IFeedRepository> _feedRepository;
    IConfiguration& _configuration;
    RefPtr<IMediaPlayer> _mplayerPlugin;
    RefPtr<IVdrInterface> _vdrInterface;

private:
    void UpdateMenu();
    void UpdateDownloadAndPlayButton(int selectedItem);
    void UpdateItemList();
    void ShowItemView(Item item);
    void ShowSubCategory(Item item);
    bool PlayItem(Item item);
    bool IsValidItemSelection(int selectedItem);
    eOSState ProcessItemAction(eKeys key, Item item);
    void UpdateYellowButton() const;

public:
    ItemMenuPresenter(IServiceLocator& serviceLocator, Feed feed, RefPtr<IFeedRepository> feedRepository,
        IConfiguration& configuration, RefPtr<IMediaPlayer> mplayerPlugin, RefPtr<IVdrInterface> vdrInterface);
    // <IListMenuPresenter>
    void Initialize(IListMenu* listMenu);
    const eOSState ProcessKey(const int selectedItem, const eKeys Key, const eOSState state);
    // </IListMenuPresenter>
};

#endif // ___ILISTMENUPRESENTER_H
