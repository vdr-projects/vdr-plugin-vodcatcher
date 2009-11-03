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
 * $Id: DownloadAction_test.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include <cxxtest/TestSuite.h>
#include "DownloadAction.h"
#include "StringMessageMock.h"
#include "DownloadQueue.h"
#include "IDownloader.h"
#include "Download.h"

using namespace std;

namespace
{

class MockDownloader : public StringMessageMock, public IDownloader
{
public:
    bool PerformDownload(Download& download)
    {
        AddMethod("PerformDownload", download.GetUrl());
        return true;
    }
};

class DownloadActionTest : public CxxTest::TestSuite
{
private:
    MockDownloader downloader;
    DownloadQueue* downloadQueue;
    DownloadAction* downloadAction;

public:
    void setUp()
    {
        downloader.ResetMock();
        downloadQueue = new DownloadQueue();
        downloadAction = new DownloadAction(*downloadQueue, downloader);
    }

    void tearDown()
    {
        delete downloadAction;
        delete downloadQueue;
    }

    void TestEmptyQueue()
    {
        TS_ASSERT(downloadAction->Action());
        downloader.Verify();
    }

    void TestOneItemInQueue()
    {
        RefPtr<Download> download = downloadQueue->AddDownload("url");

        downloader.ExpectMethod("PerformDownload", "url");
        TS_ASSERT(downloadAction->Action());

        downloader.Verify();
        TS_ASSERT(NULL == downloadQueue->GetNextDownload().get());
    }

    void TestTwoItemsInQueue()
    {
        RefPtr<Download> download1 = downloadQueue->AddDownload("url1");
        RefPtr<Download> download2 = downloadQueue->AddDownload("url2");

        downloader.ExpectMethod("PerformDownload", "url1");
        TS_ASSERT(downloadAction->Action());
        downloader.Verify();

        downloader.ExpectMethod("PerformDownload", "url2");
        TS_ASSERT(downloadAction->Action());
        downloader.Verify();
    }
};

};
