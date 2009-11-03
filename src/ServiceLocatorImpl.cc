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
 * $Id: ServiceLocatorImpl.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include "ServiceLocatorImpl.h"
#include "FeedsConfigFile.h"
#include "IConfiguration.h"
#include "RssFeedParser.h"
#include "VdrThread.h"
#include "VdrSleeper.h"
#include "VdrCriticalSection.h"
#include "DownloadQueue.h"
#include "ThreadsafeDownloadPool.h"
#include "SynchedDownloadPool.h"
#include "LocalFileCache.h"
#include "CurlDownloader.h"
#include "FeedUpdaterImpl.h"
#include "DownloadAction.h"
#include "OsdListMenu.h"
#include "FeedMenuController.h"
#include "ItemMenuPresenter.h"
#include "OsdItemView.h"
#include "ItemViewPresenter.h"
#include "SyslogErrorLogger.h"
#include "OsdSetupMenu.h"
#include "MplayerPlugin.h"
#include "XineliboutputPlayer.h"
#include "FeedRepository.h"
#include "IMenuFactory.h"
#include "VdrInterface.h"

using namespace std;

ServiceLocatorImpl::ServiceLocatorImpl(IConfiguration& configuration) :
    configuration(configuration)
{
}

IFeedSources* ServiceLocatorImpl::GetFeedSources()
{
    if (!feedSources.get())
    {
        feedSources = RefPtr<IFeedSources>(new FeedsConfigFile(configuration.GetSourcesFileName()));
    }

    return feedSources.get();
}

IFeedParser* ServiceLocatorImpl::GetFeedParser()
{
    if (!feedParser.get())
    {
        feedParser = RefPtr<IFeedParser>(new RssFeedParser(*GetDownloadCache()));
    }

    return feedParser.get();
}

IDownloadPool* ServiceLocatorImpl::GetDownloadPool()
{
    if (!downloadPool.get())
    {
        RefPtr<ISleeper> downloadSleeper(new VdrSleeper());
        RefPtr<ICriticalSection> criticalSection(new VdrCriticalSection());

        // Download pool decorated with thread safety and thread synchronization
        RefPtr<IDownloadPool> pool(new DownloadQueue());
        RefPtr<IDownloadPool> threadSafePool(new ThreadSafeDownloadPool(pool,
            criticalSection));
        RefPtr<IDownloadPool> synchedPool(new SynchedDownloadPool(
            threadSafePool, downloadSleeper));

        downloadPool = RefPtr<IDownloadPool>(synchedPool);
    }

    return downloadPool.get();
}

IDownloadCache* ServiceLocatorImpl::GetDownloadCache()
{
    if (!downloadCache.get())
    {
        downloadCache = RefPtr<IDownloadCache>(new LocalFileCache(configuration.GetCacheDirName()));
    }

    return downloadCache.get();
}

IDownloader* ServiceLocatorImpl::GetDownloader()
{
    if (!downloader.get())
    {
        downloader = RefPtr<IDownloader>(new CurlDownloader(*GetDownloadCache()));
    }

    return downloader.get();
}

IThread* ServiceLocatorImpl::GetDownloadThread()
{
    if (!downloadThread.get())
    {
        downloadThread = RefPtr<IThread>(new VdrThread(CreateDownloadAction()));
    }

    return downloadThread.get();
}

IFeedUpdater* ServiceLocatorImpl::GetFeedUpdater()
{
    if (!feedUpdater.get())
    {
        feedUpdater = RefPtr<IFeedUpdater>(new FeedUpdaterImpl(*GetFeedSources(), *GetDownloadPool(),
            *GetDownloadCache()));
    }

    return feedUpdater.get();
}

RefPtr<IThreadAction> ServiceLocatorImpl::CreateDownloadAction()
{
    return RefPtr<IThreadAction>(new DownloadAction(*GetDownloadPool(),
        *GetDownloader()));
}

IMenuFactory* ServiceLocatorImpl::GetMenuFactory()
{
    return this;
}

cOsdMenu* ServiceLocatorImpl::CreateFeedMenu()
{
    return new OsdListMenu(
        RefPtr<IListMenuPresenter>(new FeedMenuController(*GetMenuFactory(), *CreateFeedRepository())));
}

RefPtr<IFeedRepository> ServiceLocatorImpl::CreateFeedRepository()
{
    RefPtr<IFeedRepository> feedRepository(new FeedRepository(*GetDownloadCache(), *GetDownloadPool(), *GetDownloader(), *GetFeedParser(), *GetFeedSources()));
    return feedRepository;
}

cOsdMenu* ServiceLocatorImpl::CreateItemMenu(const Feed& feed)
{
    RefPtr<IMediaPlayer> mplayerPlugin;
    
    switch(configuration.GetMediaPlayerType())
    {
    case Mplayer:
    	mplayerPlugin = new MplayerPlugin(*GetErrorLogger());
    	break;
    default:
    	mplayerPlugin = new XineliboutputPlayer(*GetErrorLogger());
    	break;
    }
    RefPtr<IVdrInterface> vdrInterface(new VdrInterface());
    return new OsdListMenu(
        RefPtr<IListMenuPresenter>(new ItemMenuPresenter(*this, feed, CreateFeedRepository(), configuration, mplayerPlugin, vdrInterface)), 9);
}

cOsdMenu* ServiceLocatorImpl::CreateItemView(Feed feed, Item item)
{
    RefPtr<ItemViewPresenter> itemViewPresenter(new ItemViewPresenter(feed.GetTitle(), item, configuration));

    return new OsdItemView(itemViewPresenter);
}

IErrorLogger* ServiceLocatorImpl::GetErrorLogger()
{
    if (!errorLogger.get())
    {
        errorLogger = RefPtr<IErrorLogger>(new SyslogErrorLogger());
    }

    return errorLogger.get();
}

cMenuSetupPage* ServiceLocatorImpl::CreateSetupMenu()
{
    return new OsdSetupMenu(configuration);
}
