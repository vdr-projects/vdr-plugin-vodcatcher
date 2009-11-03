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
#include "DownloadQueue.h"
#include "Download.h"

namespace
{

class DownloadQueueTest : public CxxTest::TestSuite
{
private:
    DownloadQueue* queue;

public:
    void setUp()
    {
        queue = new DownloadQueue();
    }

    void tearDown()
    {
        delete queue;
    }

    void TestEmptyQueue()
    {
        TS_ASSERT(NULL == queue->GetNextDownload().get());
    }

    void TestAddDownload()
    {
        RefPtr<Download> download = queue->AddDownload("url");
        TS_ASSERT(NULL != queue->GetNextDownload().get());
        TS_ASSERT_EQUALS("url", queue->GetNextDownload()->GetUrl());
    }

    void TestRemoveDownloadNotInQueue()
    {
        RefPtr<Download> download1 = queue->AddDownload("url1");
        RefPtr<Download> download2 = queue->AddDownload("url2");

        TS_ASSERT(NULL != queue->GetNextDownload().get());

        queue->RemoveDownload(download2);

        TS_ASSERT(NULL != queue->GetNextDownload().get());
    }

    void TestRemoveDownload()
    {
        RefPtr<Download> download1 = queue->AddDownload("url1");
        RefPtr<Download> download2 = queue->AddDownload("url2");

        queue->RemoveDownload(download1);
        TS_ASSERT_EQUALS("url2", queue->GetNextDownload()->GetUrl());

        queue->RemoveDownload(download2);
        TS_ASSERT(NULL == queue->GetNextDownload().get());
    }

    void TestDontAddDuplicates()
    {
        RefPtr<Download> download1 = queue->AddDownload("url");
        RefPtr<Download> download2 = queue->AddDownload("url");

        queue->RemoveDownload(download1);

        TS_ASSERT(NULL == queue->GetNextDownload().get());
    }

    void TestFifo()
    {
        RefPtr<Download> download1 = queue->AddDownload("url1");
        RefPtr<Download> download2 = queue->AddDownload("url2");

        TS_ASSERT_EQUALS("url1", queue->GetNextDownload()->GetUrl());

        queue->RemoveDownload(download1);

        TS_ASSERT_EQUALS("url2", queue->GetNextDownload()->GetUrl());
    }
};

};
