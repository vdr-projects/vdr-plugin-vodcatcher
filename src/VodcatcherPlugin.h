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

#ifndef ___VODCATCHERPLUGIN_H
#define ___VODCATCHERPLUGIN_H

#include <vdr/plugin.h>
#include "RefPtr.h"
#include "IConfiguration.h"

class IServiceLocator;

class VodcatcherPlugin : public cPlugin, public IConfiguration
{
private:
    std::string cacheDirName;
    std::string sourcesFileName;
    RefPtr<IServiceLocator> serviceLocator;
    unsigned int _maxCacheAge;
    StreamType _playBackQuality;
    MediaPlayerType _mediaPlayerType;

private:
    bool CacheDirIsAccessible();

public:
    VodcatcherPlugin();
    void SetServiceLocator(RefPtr<IServiceLocator> serviceLocator);
    // <cPlugin>
    const char* Version();
    const char* Description();
    const char* MainMenuEntry();
    bool Initialize();
    bool Start();
    void Stop();
    void Housekeeping();
    const char* CommandLineHelp();
    bool ProcessArgs(int argc, char* argv[]);
    cOsdObject* MainMenuAction();
    cMenuSetupPage* SetupMenu();
    bool SetupParse(const char* Name, const char* Value);
    // </cPlugin>
    // <IConfiguration>
    const std::string GetCacheDirName() const;
    const std::string GetSourcesFileName() const;
    int GetMaxCacheAge() const;
    void SetMaxCacheAge(const int age);
    void SetPlayBackQuality(const StreamType quality);
    StreamType GetPlayBackQuality() const;
    void SetMediaPlayerType(MediaPlayerType mediaplayerType);
    MediaPlayerType GetMediaPlayerType() const;
    // </IConfiguration>
};

extern "C" void* VDRPluginCreator();

#endif // ___VODCATCHERPLUGIN_H
