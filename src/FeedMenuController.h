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
 * $Id: FeedMenuController.h 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#ifndef ___FEEDMENUPRESENTER_H
#define ___FEEDMENUPRESENTER_H

#include <vdr/osdbase.h>
#include "Feed.h"
#include "IListMenuPresenter.h"
#include "Download.h"

class IDownloadPool;
class IMenuFactory;
class IFeedRepository;

class FeedMenuController : public IListMenuPresenter
{
private:
    IMenuFactory& _menuFactory;
    const IFeedRepository& _feedRepository;
    FeedList _feeds;
    IListMenu* _menu;

public:
    FeedMenuController(IMenuFactory& menuFactory, const IFeedRepository& feedRepository);
    // <IListMenuPresenter>
    void Initialize(IListMenu* listMenu);
    const eOSState ProcessKey(const int selectedFeed, const eKeys Key, const eOSState state);
    // </IListMenuPresenter>
};

#endif // ___FEEDMENUPRESENTER_H
