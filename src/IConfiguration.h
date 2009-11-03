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

#ifndef ___ICONFIGURATION_H
#define ___ICONFIGURATION_H

#include <string>
#include "Item.h"

enum MediaPlayerType
{
	Mplayer,
	Xineliboutput
};

class IConfiguration
{
public:
    virtual ~IConfiguration()
    {
    }
    virtual const std::string GetCacheDirName() const = 0;
    virtual const std::string GetSourcesFileName() const = 0;
    virtual int GetMaxCacheAge() const = 0;
    virtual void SetMaxCacheAge(const int age) = 0;
    virtual void SetPlayBackQuality(const StreamType quality) = 0;
    virtual StreamType GetPlayBackQuality() const = 0;
    virtual void SetMediaPlayerType(MediaPlayerType mediaplayerType) = 0;
    virtual MediaPlayerType GetMediaPlayerType() const = 0;
};

#endif
