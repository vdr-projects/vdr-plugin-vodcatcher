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
 * $Id: ServiceLocatorStub.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include "ServiceLocatorStub.h"
#include "ThreadAction.h"

using namespace std;

ServiceLocatorStub::ServiceLocatorStub()
{
    feedSources = NULL;
    feedParser = NULL;
    downloadPool = NULL;
    downloadCache = NULL;
    feedMenu = NULL;
    downloadThread = NULL;
    setupMenu = NULL;
}

IFeedSources* ServiceLocatorStub::GetFeedSources()
{
    return feedSources;
}

IFeedParser* ServiceLocatorStub::GetFeedParser()
{
    return feedParser;
}

IDownloadPool* ServiceLocatorStub::GetDownloadPool()
{
    return downloadPool;
}

IDownloadCache* ServiceLocatorStub::GetDownloadCache()
{
    return downloadCache;
}

IThread* ServiceLocatorStub::GetDownloadThread()
{
    return downloadThread;
}

IFeedUpdater* ServiceLocatorStub::GetFeedUpdater()
{
    return feedUpdater;
}

RefPtr<IThreadAction> ServiceLocatorStub::CreateDownloadAction()
{
    return RefPtr<IThreadAction>(NULL);
}

cOsdMenu* ServiceLocatorStub::CreateFeedMenu()
{
    return feedMenu;
}

cOsdMenu* ServiceLocatorStub::CreateItemMenu(const Feed& feed)
{
    return itemMenu;
}

cOsdMenu* ServiceLocatorStub::CreateItemView(Feed feed, Item item)
{
    itemViewWasCreatedFor = item.GetTitle();
    return itemView;
}

IErrorLogger* ServiceLocatorStub::GetErrorLogger()
{
    return errorLogger;
}

cMenuSetupPage* ServiceLocatorStub::CreateSetupMenu()
{
    return setupMenu;
}

IMenuFactory* ServiceLocatorStub::GetMenuFactory()
{
    return this;
}
