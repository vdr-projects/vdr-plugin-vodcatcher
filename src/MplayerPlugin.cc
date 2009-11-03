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
 * $Id: MplayerPlugin.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include "MplayerPlugin.h"
#include <vdr/plugin.h>
#include <iostream>
#include <fstream>
#include "IErrorLogger.h"

using namespace std;

const char* MplayerPlugin::ServiceId = "MPlayer-Play-v1";

MplayerPlugin::MplayerPlugin(IErrorLogger& errorLogger) :
    _errorLogger(errorLogger)
{

}

bool MplayerPlugin::Play(string url)
{
    _errorLogger.LogDebug("Starting to play " + url);
    
    MPlayerServiceData data;
    const char* const tmpPlayListFileName = "/tmp/vodcatcher.pls";
    
    ofstream tmpPlayList(tmpPlayListFileName);
    tmpPlayList << url;
    tmpPlayList.close();
    
    data.data.filename = tmpPlayListFileName;
    
    if (!cPluginManager::CallFirstService(ServiceId, &data))
    {
        _errorLogger.LogDebug((string)"Failed to locate Mplayer service '" + ServiceId + "'");
        return false;
    }
    
    if (!data.result)
    {
        _errorLogger.LogDebug("Mplayer service failed");
        return false;
    }
    
    return true;
}
