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
#include "vdr-stub/pluginhooks.h"
#include "VodcatcherPlugin.h"
#include "ThreadMock.h"
#include "ServiceLocatorStub.h"
#include "FeedUpdaterMock.h"
#include "IErrorLogger.h"
#include "DownloadCacheMock.h"
#include "vdr-stub/menusetuppagestub.h"

using namespace std;

namespace
{

class ErrorLoggerStub : public IErrorLogger
{
public:
    string lastLogMessage;
    string lastDebugLogMessage;

public:
    // <IErrorLogger>
    void LogError(const string& Message)
    {
        lastLogMessage = Message;
    }

    void LogDebug(const string& Message)
    {
        lastDebugLogMessage = Message;
    }
    // </IErrorLogger>
};

class VodcatcherPluginTest : public CxxTest::TestSuite
{
private:
    VodcatcherPlugin* plugin;
    ServiceLocatorStub* serviceLocator;
    ErrorLoggerStub errorLoggerStub;

public:
    void setUp()
    {
        serviceLocator = new ServiceLocatorStub();
        plugin = new VodcatcherPlugin;
        plugin->SetServiceLocator(RefPtr<IServiceLocator>(serviceLocator));
        serviceLocator->errorLogger = &errorLoggerStub;
    }

    void tearDown()
    {
        system("rmdir ./notWritable 2>/dev/null");
        delete plugin;
    }

    void TestMainMenuEntry()
    {
        TS_ASSERT_EQUALS(string("i18n:Vodcatcher"), plugin->MainMenuEntry());
    }

    void TestVersion()
    {
        TS_ASSERT_EQUALS(string("0.2.2"), plugin->Version());
    }

    void TestDescription()
    {
        TS_ASSERT_EQUALS(string("i18n:Browse and play video podcasts"),
            plugin->Description());
    }

    void TestSourcesFileName()
    {
        char first[] = "dummy";
        char second[] = "--cache=.";
        char* argv[2] =
            { first, second };
        TS_ASSERT(plugin->ProcessArgs(2, argv));
        TS_ASSERT(plugin->Initialize());
        TS_ASSERT_EQUALS("/vdrstub/vodcatchersources.conf",
            plugin->GetSourcesFileName());
    }

    void TestMainMenuAction()
    {
        cOsdMenu menu("");
        serviceLocator->feedMenu = &menu;
        TS_ASSERT_EQUALS(&menu, plugin->MainMenuAction());
    }

    void TestSetupMenuAction()
    {
        MenuSetupPageStub setupMenu;
        serviceLocator->setupMenu = &setupMenu;
        TS_ASSERT_EQUALS(&setupMenu, plugin->SetupMenu());
    }

    void TestStartRunsDownloadThreadAndUpdatesFeeds()
    {
        ThreadMock downloadThread;
        FeedUpdaterMock feedUpdater;

        serviceLocator->downloadThread = &downloadThread;
        serviceLocator->feedUpdater = &feedUpdater;

        downloadThread.ExpectMethod("Start");
        feedUpdater.ExpectMethod("Update");

        TS_ASSERT(plugin->Start());

        downloadThread.Verify();
        feedUpdater.Verify();
    }

    void TestStopStopsDownloadThread()
    {
        ThreadMock downloadThread;
        serviceLocator->downloadThread = &downloadThread;

        downloadThread.ExpectMethod("Stop");

        plugin->Stop();

        downloadThread.Verify();
    }

    void TestHousekeepingUpdatesFeeds()
    {
        FeedUpdaterMock feedUpdater;
        serviceLocator->feedUpdater = &feedUpdater;
        DownloadCacheMock downloadCache;
        serviceLocator->downloadCache = &downloadCache;

        feedUpdater.ExpectMethod("Update");
        plugin->Housekeeping();
        feedUpdater.Verify();
    }

    void TestHousekeepingCleansCache()
    {
        FeedUpdaterMock feedUpdater;
        serviceLocator->feedUpdater = &feedUpdater;
        DownloadCacheMock downloadCache;
        serviceLocator->downloadCache = &downloadCache;
        plugin->SetupParse("MaxCacheAge", "10");

        downloadCache.ExpectMethod("CleanUp", downloadCache.MakeString(10 * 24));
        plugin->Housekeeping();
        downloadCache.Verify();
    }

    void TestInitializeFailsWithErrorMessageIfCacheDirNotExisting()
    {
        char first[] = "dummy";
        char second[] = "--cache=/foo/notExisting";
        char* argv[2] =
            { first, second };
        errorLoggerStub.lastLogMessage = "";

        TS_ASSERT(plugin->ProcessArgs(2, argv));
        TS_ASSERT(!plugin->Initialize());
        TS_ASSERT_EQUALS("Unable to access cache directory "
            "`/foo/notExisting`", errorLoggerStub.lastLogMessage);
    }

    void TestInitializeFailsWithErrorMessageIfCacheDirNotWritable()
    {
        char first[] = "dummy";
        char second[] = "--cache=./notWritable";
        char* argv[2] =
            { first, second };
        system("mkdir ./notWritable");
        system("chmod a-w ./notWritable");
        errorLoggerStub.lastLogMessage = "";

        TS_ASSERT(plugin->ProcessArgs(2, argv));
        TS_ASSERT(!plugin->Initialize());
        TS_ASSERT_EQUALS("Unable to access cache directory "
            "`./notWritable`", errorLoggerStub.lastLogMessage);
    }

    void TestSetupParseFailsForUnknownSetting()
    {
        TS_ASSERT(!plugin->SetupParse("unknown setting", "value"));
    }

    void TestDefaultMaxCacheAge()
    {
        TS_ASSERT_EQUALS(30, plugin->GetMaxCacheAge());
    }

    void TestSetupParseMaxCacheAge()
    {
        TS_ASSERT(plugin->SetupParse("MaxCacheAge", "10"));
        TS_ASSERT_EQUALS(10, plugin->GetMaxCacheAge());
    }

    void TestSetMaxCacheAge()
    {
        plugin->SetMaxCacheAge(22);
        TS_ASSERT_EQUALS(22, plugin->GetMaxCacheAge());
    }

    void TestDefaultPlayBackQuality()
    {
        TS_ASSERT_EQUALS(High, plugin->GetPlayBackQuality());
    }
};

}
;
