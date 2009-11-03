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

#ifndef CONFIGURATIONSTUB_H_
#define CONFIGURATIONSTUB_H_

#include "IConfiguration.h"

class ConfigurationStub : public IConfiguration
{
private:
    StreamType _playBackQuality;

public:
    const std::string GetCacheDirName() const
    {
        return "";
    }
    const std::string GetSourcesFileName() const
    {
        return "";
    }
    int GetMaxCacheAge() const
    {
        return 0;
    }
    void SetMaxCacheAge(const int age)
    {
    }
    void SetPlayBackQuality(const StreamType quality)
    {
        _playBackQuality = quality;
    }
    StreamType GetPlayBackQuality() const
    {
        return _playBackQuality;
    }

    MediaPlayerType GetMediaPlayerType() const
    {
    	return Mplayer;
    }

    void SetMediaPlayerType(MediaPlayerType)
    {
    }
};

#endif
