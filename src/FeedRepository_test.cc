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
 * $Id: FeedRepository_test.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include <cxxtest/TestSuite.h>

#include "RefPtr.h"
#include "FeedRepository.h"
#include "DownloadCacheMock.h"
#include "DownloadPoolMock.h"
#include "IFeedSources.h"
#include "IDownloader.h"
#include "IFeedParser.h"
#include "Download.h"

using namespace std;

namespace
{

class FeedSourcesStub : public IFeedSources
{
    void GetFeeds(FeedList& feeds) const
    {
    }

    const vector<string> GetFeedUrls() const
    {
        vector<string> urls;
        urls.push_back("feedUrl1");
        urls.push_back("feedUrl2");
        return urls;
    }
};

class FeedParserStub : public IFeedParser
{
    bool Parse(Feed& feed) const
    {
        feed.SetTitle("Parsed");
        feed.SetTimeToLive(60);
        return true;
    }
};

class DownloaderMock : public IDownloader, public StringMessageMock
{
public:
    bool PerformDownload(Download& download)
    {
        AddMethod("PerformDownload", download.GetUrl());
        return true;
    }
};

class A_FeedRepository_with_an_empty_cache : public CxxTest::TestSuite
{
private:
    RefPtr<FeedRepository> _feedRepository;
    RefPtr<DownloadCacheMock> _downloadCache;
    RefPtr<DownloaderMock> _downloader;
    RefPtr<DownloadPoolMock> _downloadPool;
    RefPtr<FeedParserStub> _feedParser;
    RefPtr<IFeedSources> _feedSources;

public:
    void setUp()
    {
        _downloadCache = new DownloadCacheMock();
        _downloadCache->filesAreCached = false;
        _downloadPool = new DownloadPoolMock();
        _feedSources = new FeedSourcesStub();
        _feedParser = new FeedParserStub();
        _downloader = new DownloaderMock();

        _feedRepository = new FeedRepository(*_downloadCache, *_downloadPool, *_downloader, *_feedParser, *_feedSources);
    }

    void Should_add_the_root_feeds_to_the_download_queue()
    {
        _downloadPool->ExpectMethod("AddDownload", "feedUrl1");
        _downloadPool->ExpectMethod("AddDownload", "feedUrl2");

        _feedRepository->GetRootFeeds();

        VERIFY_EXPECTATIONS(*_downloadPool);
    }

    void Should_return_the_root_feeds()
    {
        FeedList feeds = _feedRepository->GetRootFeeds();
        TS_ASSERT_EQUALS((unsigned int) 2, feeds.size());
        TS_ASSERT_EQUALS("feedUrl1", feeds[0].GetUrl());
        TS_ASSERT_EQUALS("feedUrl2", feeds[1].GetUrl());
    }

    void Should_immediately_download_and_parse_a_feed_requested_by_url()
    {
        _downloader->ExpectMethod("PerformDownload", "url");

        Feed feed = _feedRepository->GetFeed("url");

        VERIFY_EXPECTATIONS(*_downloader);
        TS_ASSERT_EQUALS("Parsed", feed.GetTitle());
    }
};

class A_FeedRepository_with_uptodate_feeds_in_the_cache : public CxxTest::TestSuite
{
private:
    RefPtr<FeedRepository> _feedRepository;
    RefPtr<DownloadCacheMock> _downloadCache;
    RefPtr<DownloaderMock> _downloader;
    RefPtr<DownloadPoolMock> _downloadPool;
    RefPtr<FeedParserStub> _feedParser;
    RefPtr<IFeedSources> _feedSources;

public:
    void setUp()
    {
        _downloadCache = new DownloadCacheMock();
        _downloadCache->filesAreCached = true;
        _downloadCache->fileAge = 10;
        _downloadPool = new DownloadPoolMock();
        _feedSources = new FeedSourcesStub();
        _feedParser = new FeedParserStub();
        _downloader = new DownloaderMock();

        _feedRepository = new FeedRepository(*_downloadCache, *_downloadPool, *_downloader, *_feedParser, *_feedSources);
    }

    void Should_not_download_but_only_parse_the_root_feeds()
    {
        FeedList feeds = _feedRepository->GetRootFeeds();

        VERIFY(*_downloadPool);
        VERIFY(*_downloader);
        TS_ASSERT_EQUALS("Parsed", feeds[0].GetTitle());
        TS_ASSERT_EQUALS("Parsed", feeds[1].GetTitle());
    }

    void Should_not_download_but_only_parse_the_feed_requested_by_url()
    {
        Feed feed = _feedRepository->GetFeed("feedUrl");

        VERIFY(*_downloadPool);
        VERIFY(*_downloader);
        TS_ASSERT_EQUALS("Parsed", feed.GetTitle());
    }
};

class A_FeedRepository_with_expired_feeds_in_the_cache : public CxxTest::TestSuite
{
private:
    RefPtr<FeedRepository> _feedRepository;
    RefPtr<DownloadCacheMock> _downloadCache;
    RefPtr<DownloaderMock> _downloader;
    RefPtr<DownloadPoolMock> _downloadPool;
    RefPtr<FeedParserStub> _feedParser;
    RefPtr<IFeedSources> _feedSources;

public:
    void setUp()
    {
        _downloadCache = new DownloadCacheMock();
        _downloadCache->filesAreCached = true;
        _downloadCache->fileAge = 100;
        _downloadPool = new DownloadPoolMock();
        _feedSources = new FeedSourcesStub();
        _feedParser = new FeedParserStub();
        _downloader = new DownloaderMock();

        _feedRepository = new FeedRepository(*_downloadCache, *_downloadPool, *_downloader, *_feedParser, *_feedSources);
    }

    void Should_add_the_root_feeds_to_the_download_queue()
    {
        _downloadPool->ExpectMethod("AddDownload", "feedUrl1");
        _downloadPool->ExpectMethod("AddDownload", "feedUrl2");

        _feedRepository->GetRootFeeds();

        VERIFY_EXPECTATIONS(*_downloadPool);
    }

    void Should_return_the_parsed_root_feeds()
    {
        FeedList feeds = _feedRepository->GetRootFeeds();
        TS_ASSERT_EQUALS((unsigned int) 2, feeds.size());
        TS_ASSERT_EQUALS("Parsed", feeds[0].GetTitle());
        TS_ASSERT_EQUALS("Parsed", feeds[1].GetTitle());
    }
};

};
