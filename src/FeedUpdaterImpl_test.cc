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
 * $Id: FeedUpdaterImpl_test.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include <cxxtest/TestSuite.h>
#include "FeedUpdaterImpl.h"
#include "DownloadPoolMock.h"
#include "IFeedSources.h"
#include "Download.h"
#include "DownloadCacheMock.h"

using namespace std;

namespace
{

class FeedSourcesStub : public IFeedSources
{
public:
    // <IFeedSources>
    void GetFeeds(FeedList& feeds) const
    {
        Feed feed1("foo");
        Feed feed2("bar");
        feeds.push_back(feed1);
        feeds.push_back(feed2);
    }

    const vector<string> GetFeedUrls() const
    {
        return vector<string>();
    }
    // </IFeedSources>
};

class FeedUpdaterImplTest : public CxxTest::TestSuite
{
private:
    FeedSourcesStub feedSources;
    DownloadPoolMock downloadPool;
    DownloadCacheMock downloadCache;
    FeedUpdaterImpl* feedUpdater;

public:
    void setUp()
    {
        downloadPool.ResetMock();
        feedUpdater = new FeedUpdaterImpl(feedSources, downloadPool,
            downloadCache);
    }

    void tearDown()
    {
        delete feedUpdater;
    }

    void TestDownloadNewFeeds()
    {
        downloadCache.filesAreCached = false;
        downloadCache.fileAge = 99;
        downloadPool.ExpectMethod("AddDownload", "foo");
        downloadPool.ExpectMethod("AddDownload", "bar");

        feedUpdater->Update();

        downloadPool.Verify();
    }

    void TestDontDownloadUptodateFeeds()
    {
        downloadCache.filesAreCached = true;
        downloadCache.fileAge = 1;

        feedUpdater->Update();

        downloadPool.Verify();
    }

    void TestRefreshOldFeeds()
    {
        downloadCache.filesAreCached = true;
        downloadCache.fileAge = 3;
        downloadPool.ExpectMethod("AddDownload", "foo");
        downloadPool.ExpectMethod("AddDownload", "bar");

        feedUpdater->Update();

        downloadPool.Verify();
    }
};

};
