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
 * $Id: FeedRepository.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include "FeedRepository.h"

#include "IFeedParser.h"
#include "IDownloader.h"
#include "IDownloadCache.h"
#include "Download.h"
#include "IFeedSources.h"
#include "IDownloadPool.h"

using namespace std;

FeedRepository::FeedRepository(IDownloadCache& downloadCache, IDownloadPool& downloadPool, IDownloader& downloader,
    IFeedParser& feedParser, IFeedSources& feedSources) :
    _downloadCache(downloadCache), _downloadPool(downloadPool), _downloader(downloader), _feedParser(feedParser),
        _feedSources(feedSources)
{
}

Feed FeedRepository::GetFeed(string url)
{
    Feed feed(url);

    if (_downloadCache.IsCached(url))
    {
        _feedParser.Parse(feed);
    }

    if (!_downloadCache.IsCached(url) || _downloadCache.GetAgeInMinutes(url) >= feed.GetTimeToLive())
    {
        RefPtr<Download> download(new Download(url));
        _downloader.PerformDownload(*download);
        _feedParser.Parse(feed);
    }

    return feed;
}

const FeedList FeedRepository::GetRootFeeds() const
{
    FeedList feeds;
    vector<string> urls = _feedSources.GetFeedUrls();

    for (vector<string>::iterator i = urls.begin(); i != urls.end(); i++)
    {
        Feed feed(*i);
        _feedParser.Parse(feed);

        if (!_downloadCache.IsCached(*i) || _downloadCache.GetAgeInMinutes(*i) >= feed.GetTimeToLive())
        {
            _downloadPool.AddDownload(*i);
        }

        feeds.push_back(feed);
    }

    return feeds;
}
