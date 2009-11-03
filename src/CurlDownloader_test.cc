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
 * $Id: CurlDownloader_test.cc 7656 2008-08-09 21:07:45Z svntobi $
 *
 */

#include <cxxtest/TestSuite.h>
#include "CurlDownloader.h"
#include <fstream>
#include <sstream>
#include "StringMessageMock.h"
#include "Download.h"
#include "DownloadCacheMock.h"
#include <stdlib.h>

using namespace std;

namespace
{

class CurlDownloaderTest : public CxxTest::TestSuite
{
public:
    void tearDown()
    {
        unlink("test.download");
    }

    void TestWriteData()
    {
        std::stringstream testStream;
        const char* buffer = "MemBlock1MemBlock2MemBlock3";

        TS_ASSERT_EQUALS(9*2, (int) CurlDownloader::WriteDataToStream(
                buffer, 9, 2, &testStream));

        TS_ASSERT_EQUALS("MemBlock1MemBlock2", testStream.str());
    }

    void TestWriteDataCausingWriteException()
    {
        std::filebuf streamBuf;
        std::ostream testStream(&streamBuf);
        testStream.exceptions(ofstream::badbit);

        const char* buffer = "X";

        TS_ASSERT_EQUALS(0, (int) CurlDownloader::WriteDataToStream(
                buffer, 1, 1, &testStream));
    }

    void TestDownload()
    {
        DownloadCacheMock downloadCache;
        CurlDownloader downloader(downloadCache);
        Download download("file://test.download");

        system("echo -n foo >test.download");

        downloadCache.ExpectMethod("Put", "foo", download.GetUrl());

        TS_ASSERT(downloader.PerformDownload(download));

        downloadCache.Verify();
    }

    void TestDownloadFailure()
    {
        DownloadCacheMock downloadCache;
        CurlDownloader downloader(downloadCache);
        Download download("file://test.download");

        unlink("test.download");

        TS_ASSERT(!downloader.PerformDownload(download));

        downloadCache.Verify();
    }
};

};
