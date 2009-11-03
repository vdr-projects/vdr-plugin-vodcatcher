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
 * $Id: VodcatcherPluginCommandline_test.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include <string>
#include <iostream>
#include <cxxtest/TestSuite.h>
#include "VodcatcherPlugin.h"
#include "StderrMock.h"

using namespace std;

namespace
{

class VodcatcherPluginCommandLineTest : public CxxTest::TestSuite
{
private:
    VodcatcherPlugin* plugin;
    StdErrMock* stdErrMock;

private:
    void RedirectStdErr()
    {
        stdErrMock = new StdErrMock();
    }

    void RestoreStdErr()
    {
        delete stdErrMock;
    }

public:
    void TestCommandLineHelp()
    {
        VodcatcherPlugin plugin;
        TS_ASSERT_EQUALS(string("  -c, --cache=CACHE_DIR    specify cache "
                "dir, defaults to /var/cache/vdr-plugin-vodcatcher"),
            plugin.CommandLineHelp());
    }

    void TestDefaultCommandLineParameters()
    {
        VodcatcherPlugin plugin;
        char first[] = "dummy";
        char* argv[1] =
            { first };

        TS_ASSERT(plugin.ProcessArgs(0, argv));
        TS_ASSERT_EQUALS("/var/cache/vdr-plugin-vodcatcher", plugin.GetCacheDirName());
    }

    void TestWrongCommandLineParameters()
    {
        VodcatcherPlugin plugin;
        StdErrMock stdErrMock;
        char first[] = "dummy";
        char second[] = "--foo";
        char* argv[2] =
            { first, second };

        stdErrMock.Expect("dummy: invalid option --foo\n");
        TS_ASSERT(!plugin.ProcessArgs(2, argv));
        stdErrMock.Verify();
    }

    void TestShortCacheDirParameter()
    {
        VodcatcherPlugin plugin;
        char first[] = "dummy";
        char second[] = "-c";
        char third[] = "/foo";
        char* argv[3] =
            { first, second, third };

        TS_ASSERT(plugin.ProcessArgs(3, argv));
        TS_ASSERT_EQUALS("/foo", plugin.GetCacheDirName());
    }

    void TestLongCacheDirParameter()
    {
        VodcatcherPlugin plugin;
        char first[] = "dummy";
        char second[] = "--cache=/foo";
        char* argv[2] =
            { first, second };

        TS_ASSERT(plugin.ProcessArgs(2, argv));
        TS_ASSERT_EQUALS("/foo", plugin.GetCacheDirName());
    }
};

}
;
