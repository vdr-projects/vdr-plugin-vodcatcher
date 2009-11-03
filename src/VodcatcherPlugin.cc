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

#include "VodcatcherPlugin.h"
#include <iostream>
#include <getopt.h>
#include "Version.h"
#include "IFeedUpdater.h"
#include "IErrorLogger.h"
#include "IDownloadCache.h"
#include "IServiceLocator.h"
#include "Thread.h"

using namespace std;

VodcatcherPlugin::VodcatcherPlugin() :
    cacheDirName(DEFAULT_CACHE_DIR), _maxCacheAge(30), _playBackQuality(High)
{
}

void VodcatcherPlugin::SetServiceLocator(RefPtr<IServiceLocator> serviceLocator)
{
    this->serviceLocator = serviceLocator;
}

const char* VodcatcherPlugin::Version(void)
{
    return VERSION;
}

const char* VodcatcherPlugin::Description(void)
{
    return tr("Browse and play video podcasts");
}

const char* VodcatcherPlugin::MainMenuEntry(void)
{
    return tr("Vodcatcher");
}

bool VodcatcherPlugin::Initialize(void)
{
    const char* configDir = ConfigDirectory();

    if (configDir)
    {
        sourcesFileName = string(configDir) + "/vodcatchersources.conf";
    }

    if (CacheDirIsAccessible())
    {
        return true;
    }
    else
    {
        serviceLocator->GetErrorLogger()->LogError("Unable to access cache directory `" + GetCacheDirName() + "`");
        return false;
    }
}

const char* VodcatcherPlugin::CommandLineHelp(void)
{
    return "  -c, --cache=CACHE_DIR    specify cache dir, defaults to "
        "/var/cache/vdr-plugin-vodcatcher";
}

bool VodcatcherPlugin::ProcessArgs(int argc, char* argv[])
{
    static struct option longOptions[] =
        {
            { "cache", required_argument, NULL, 'c' },
            { NULL } };

    optind = 0;
    opterr = 0;

    int optionChar;
    int optionIndex = 0;
    while ((optionChar = getopt_long(argc, argv, "c:", longOptions, &optionIndex)) != -1)
    {
        switch (optionChar)
        {
            case 'c':
            {
                cacheDirName = optarg;
                break;
            }
            default:
            {
                cerr << argv[0] << ": " << "invalid option " << argv[optind-1] << endl;
                return false;
            }
        }
    }
    return true;
}

const string VodcatcherPlugin::GetCacheDirName() const
{
    return cacheDirName;
}

const string VodcatcherPlugin::GetSourcesFileName() const
{
    return sourcesFileName;
}

int VodcatcherPlugin::GetMaxCacheAge() const
{
    return _maxCacheAge;
}

void VodcatcherPlugin::SetMaxCacheAge(const int age)
{
    _maxCacheAge = age;
}

void VodcatcherPlugin::SetPlayBackQuality(const StreamType quality)
{
    _playBackQuality = quality;
}

StreamType VodcatcherPlugin::GetPlayBackQuality() const
{
    return _playBackQuality;
}

void VodcatcherPlugin::SetMediaPlayerType(const MediaPlayerType mediaPlayerType)
{
    _mediaPlayerType = mediaPlayerType;
}

MediaPlayerType VodcatcherPlugin::GetMediaPlayerType() const
{
	return _mediaPlayerType;
}

cOsdObject* VodcatcherPlugin::MainMenuAction()
{
    return serviceLocator->CreateFeedMenu();
}

bool VodcatcherPlugin::Start()
{
    serviceLocator->GetDownloadThread()->Start();
    serviceLocator->GetFeedUpdater()->Update();
    return true;
}

void VodcatcherPlugin::Stop()
{
    serviceLocator->GetDownloadThread()->Stop();
}

void VodcatcherPlugin::Housekeeping()
{
    serviceLocator->GetFeedUpdater()->Update();
    serviceLocator->GetDownloadCache()->CleanUp(24 * GetMaxCacheAge());
}

bool VodcatcherPlugin::CacheDirIsAccessible()
{
    if (0 == access((GetCacheDirName()+"/.").c_str(), W_OK))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool VodcatcherPlugin::SetupParse(const char* Name, const char* Value)
{
    if (string(Name) == "MaxCacheAge")
    {
        _maxCacheAge = atoi(Value);
        return true;
    }

    if (string(Name) == "MediaPlayerType")
    {
        _mediaPlayerType = (MediaPlayerType) atoi(Value);
        return true;
    }

    return false;
}

cMenuSetupPage* VodcatcherPlugin::SetupMenu()
{
    return serviceLocator->CreateSetupMenu();
}
