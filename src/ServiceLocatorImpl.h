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

#ifndef ___SERVICELOCATORIMPL_H
#define ___SERVICELOCATORIMPL_H

#include "IServiceLocator.h"
#include "RefPtr.h"
#include "IMenuFactory.h"

class IDownloader;
class IPesPacketBuffer;
class IConfiguration;
class IFeedRepository;

class ServiceLocatorImpl : public IServiceLocator, IMenuFactory
{
private:
    IConfiguration& configuration;
    RefPtr<IFeedSources> feedSources;
    RefPtr<IFeedParser> feedParser;
    RefPtr<IDownloadPool> downloadPool;
    RefPtr<IDownloadCache> downloadCache;
    RefPtr<IThread> downloadThread;
    RefPtr<IDownloader> downloader;
    RefPtr<IFeedUpdater> feedUpdater;
    RefPtr<IErrorLogger> errorLogger;
    RefPtr<IMenuFactory> menuFactory;

private:
    IDownloader* GetDownloader();
    RefPtr<IPesPacketBuffer> CreateSynchedPcmBuffer();

public:
    ServiceLocatorImpl(IConfiguration& configuration);
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
    IErrorLogger* GetErrorLogger();
    cMenuSetupPage* CreateSetupMenu();
    // </IServiceLocator>
    IMenuFactory* GetMenuFactory();
    RefPtr<IFeedRepository> CreateFeedRepository();
    // <IMenuFactory>
    cOsdMenu* CreateItemMenu(const Feed& feed);
    // </IMenuFactory>
};

#endif // ___SERVICELOCATORIMPL_H
