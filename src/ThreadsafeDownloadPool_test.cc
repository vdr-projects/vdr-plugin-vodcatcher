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
 * $Id: ThreadsafeDownloadPool_test.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include <cxxtest/TestSuite.h>
#include "StringMessageMock.h"
#include "ThreadsafeDownloadPool.h"
#include "DownloadPoolMock.h"
#include "Download.h"
#include "CriticalSection.h"

using namespace std;

namespace
{

class CriticalSectionMock : public StringMessageMock, public ICriticalSection
{
public:
    CriticalSectionMock(StringMessageMock& parent) :
        StringMessageMock(parent)
    {
    }

    // <ICriticalSection>
    void Enter()
    {
        AddMethod("Enter");
    }

    void Leave()
    {
        AddMethod("Leave");
    }
    // </ICriticalSection>
};

class ThreadSafeDownloadPoolTest : public CxxTest::TestSuite
{
private:
    StringMessageMock combinedMock;
    DownloadPoolMock* downloadPool;
    ICriticalSection* criticalSection;
    ThreadSafeDownloadPool* queue;

public:
    void setUp()
    {
        combinedMock.ResetMock();
        downloadPool = new DownloadPoolMock(combinedMock);
        criticalSection = new CriticalSectionMock(combinedMock);
        queue = new ThreadSafeDownloadPool(RefPtr<IDownloadPool>(downloadPool),
            RefPtr<ICriticalSection>(criticalSection));
    }

    void tearDown()
    {
        delete queue;
    }

    void TestAddDownload()
    {
        combinedMock.ExpectMethod("Enter");
        combinedMock.ExpectMethod("AddDownload", "url");
        combinedMock.ExpectMethod("Leave");

        RefPtr<Download> download = queue->AddDownload("url");

        combinedMock.Verify();
    }

    void TestGetNextDownload()
    {
        RefPtr<Download> download(new Download("url"));
        downloadPool->returnedDownload = download;

        combinedMock.ExpectMethod("Enter");
        combinedMock.ExpectMethod("GetNextDownload");
        combinedMock.ExpectMethod("Leave");

        TS_ASSERT_EQUALS("url", queue->GetNextDownload()->GetUrl());

        combinedMock.Verify();
    }

    void TestRemoveDownload()
    {
        RefPtr<Download> download(new Download("foo"));

        combinedMock.ExpectMethod("Enter");
        combinedMock.ExpectMethod("RemoveDownload", "foo");
        combinedMock.ExpectMethod("Leave");

        queue->RemoveDownload(download);

        combinedMock.Verify();
    }

    void TestGetDownloadByUrl()
    {
        RefPtr<Download> download(new Download("url"));
        downloadPool->returnedDownload = download;

        combinedMock.ExpectMethod("Enter");
        combinedMock.ExpectMethod("GetDownloadByUrl", "url");
        combinedMock.ExpectMethod("Leave");

        TS_ASSERT_EQUALS("url",
            queue->GetDownloadByUrl("url")->GetUrl());

        combinedMock.Verify();
    }
};

};
