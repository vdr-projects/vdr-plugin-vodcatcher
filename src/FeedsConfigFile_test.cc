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

#include <fstream>
#include <cxxtest/TestSuite.h>
#include "FeedsConfigFile.h"

using namespace std;

namespace
{

class FeedsConfigFileTest : public CxxTest::TestSuite
{
private:
    void WriteConfigData(const char* data)
    {
        ofstream configFile("test.config", ios_base::app);
        configFile << data << endl;
        configFile.close();
    }

public:
    void setUp()
    {
        unlink("test.config");
    }

    void tearDown()
    {
        unlink("test.config");
    }

    void TestEmptyConfigFile()
    {
        FeedsConfigFile feedSources("test.config");
        FeedList feeds;
        feedSources.GetFeeds(feeds);
        TS_ASSERT_EQUALS(0, (int) feeds.size());
    }

    void TestSingelFeed()
    {
        WriteConfigData("FEED_URL=foo\n");

        FeedsConfigFile feedSources("test.config");
        FeedList feeds;
        feedSources.GetFeeds(feeds);
        TS_ASSERT_EQUALS(1, (int) feeds.size());
        TS_ASSERT_EQUALS("foo", feeds[0].GetUrl());
    }

    void TestMultipleFeedsWithComments()
    {
        WriteConfigData("FEED_URL=foo\n");
        WriteConfigData("# Comment\n");
        WriteConfigData("# FEED_URL=abc\n");
        WriteConfigData("FEED_URL=bar\n");

        FeedsConfigFile feedSources("test.config");
        FeedList feeds;
        feedSources.GetFeeds(feeds);
        TS_ASSERT_EQUALS(2, (int) feeds.size());
        TS_ASSERT_EQUALS("foo", feeds[0].GetUrl());
        TS_ASSERT_EQUALS("bar", feeds[1].GetUrl());
    }
};

};
