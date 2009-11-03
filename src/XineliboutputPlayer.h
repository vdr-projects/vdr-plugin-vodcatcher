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
 * $Id: XineliboutputPlayer.h 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#ifndef XINELIBOUTPUTPLAYER_H_
#define XINELIBOUTPUTPLAYER_H_

#include "IMediaPlayer.h"
#include "IErrorLogger.h"
#include <vdr/plugin.h>

class IErrorLogger;

class XineliboutputPlayer : public IMediaPlayer
{
private:
    static const char* ServiceId;
    IErrorLogger& _errorLogger;

public:
    XineliboutputPlayer(IErrorLogger& errorLogger);
    virtual bool Play(std::string url);
};

#endif /*XINELIBOUTPUTPLAYER_H_*/
