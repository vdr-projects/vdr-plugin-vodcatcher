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

#ifndef ___SERVICELOCATORSTUB_H
#define ___SERVICELOCATORSTUB_H

#include "IServiceLocator.h"
#include "IMenuFactory.h"

class ServiceLocatorStub : public IServiceLocator, IMenuFactory
{
public:
    IFeedSources* feedSources;
    IFeedParser* feedParser;
    IDownloadPool* downloadPool;
    IDownloadCache* downloadCache;
    IThread* downloadThread;
    IFeedUpdater* feedUpdater;
    cOsdMenu* feedMenu;
    cOsdMenu* itemMenu;
    cOsdMenu* itemView;
    std::string itemAssignedToPlayerControl;
    std::string itemViewWasCreatedFor;
    cControl* playerControl;
    IErrorLogger* errorLogger;
    cMenuSetupPage* setupMenu;

public:
    ServiceLocatorStub();
    // <IServiceLocator>
    IFeedSources* GetFeedSources();
    IFeedParser* GetFeedParser();
    IDownloadPool* GetDownloadPool();
    IDownloadCache* GetDownloadCache();
    IThread* GetDownloadThread();
    IFeedUpdater* GetFeedUpdater();
    RefPtr<IThreadAction> CreateDownloadAction();
    cOsdMenu* CreateFeedMenu();
    cOsdMenu* CreateItemView(Feed feed, Item item);
    cControl* CreatePlayerControl(Item item);
    IErrorLogger* GetErrorLogger();
    cMenuSetupPage* CreateSetupMenu();
    IMenuFactory* GetMenuFactory();
    // </IServiceLocator>
    // <IMenuFactory>
    cOsdMenu* CreateItemMenu(const Feed& feed);
    // </IMenuFactory>
};

#endif
