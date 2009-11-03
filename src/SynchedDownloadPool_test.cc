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

#include <cxxtest/TestSuite.h>
#include "SynchedDownloadPool.h"
#include "DownloadPoolMock.h"
#include "SleeperMock.h"
#include "Download.h"

using namespace std;

namespace
{

class SynchedDownloadPoolTest : public CxxTest::TestSuite
{
public:
    void TestWrappedDownloadPool()
    {
        DownloadPoolMock* wrappedDownloadPool = new DownloadPoolMock();
        RefPtr<IDownloadPool> wrappedDownloadPoolPtr(wrappedDownloadPool);
        SynchedDownloadPool downloadPool(wrappedDownloadPoolPtr, RefPtr<ISleeper>(new SleeperMock()));

        wrappedDownloadPool->ExpectMethod("AddDownload", "foo");
        wrappedDownloadPool->ExpectMethod("GetNextDownload");
        wrappedDownloadPool->ExpectMethod("RemoveDownload", "foo");

        RefPtr<Download> download = downloadPool.AddDownload("foo");
        wrappedDownloadPool->returnedDownload = download;
        TS_ASSERT(NULL != downloadPool.GetNextDownload().get());
        downloadPool.RemoveDownload(download);
        wrappedDownloadPool->Verify();
    }

    void TestSleepIfPoolIsEmpty()
    {
        StringMessageMock mock;
        DownloadPoolMock* wrappedDownloadPool = new DownloadPoolMock(mock);
        RefPtr<IDownloadPool> wrappedDownloadPoolPtr(wrappedDownloadPool);
        SynchedDownloadPool downloadPool(wrappedDownloadPoolPtr, RefPtr<ISleeper>(new SleeperMock(mock)));

        mock.ExpectMethod("GetNextDownload");
        mock.ExpectMethod("Sleep");

        TS_ASSERT(NULL == downloadPool.GetNextDownload().get());

        mock.Verify();
    }

    void TestDoNotSleepIfPoolContainsDownloads()
    {
        StringMessageMock mock;
        DownloadPoolMock* wrappedDownloadPool = new DownloadPoolMock(mock);
        RefPtr<IDownloadPool> wrappedDownloadPoolPtr(wrappedDownloadPool);
        SynchedDownloadPool downloadPool(wrappedDownloadPoolPtr, RefPtr<ISleeper>(new SleeperMock(mock)));
        RefPtr<Download> download(new Download("foo"));
        wrappedDownloadPool->returnedDownload = download;

        mock.ExpectMethod("GetNextDownload");

        TS_ASSERT(NULL != downloadPool.GetNextDownload().get());

        mock.Verify();
    }

    void TestWakeUpWhenAddingNewDownload()
    {
        StringMessageMock mock;
        DownloadPoolMock* wrappedDownloadPool = new DownloadPoolMock(mock);
        RefPtr<IDownloadPool> wrappedDownloadPoolPtr(wrappedDownloadPool);
        SynchedDownloadPool downloadPool(wrappedDownloadPoolPtr, RefPtr<ISleeper>(new SleeperMock(mock)));

        mock.ExpectMethod("AddDownload", "foo");
        mock.ExpectMethod("WakeUp");

        RefPtr<Download> download = downloadPool.AddDownload("foo");

        mock.Verify();
    }
};

};
