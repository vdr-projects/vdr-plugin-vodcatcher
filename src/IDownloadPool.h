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

#ifndef ___IDOWNLOADPOOL_H
#define ___IDOWNLOADPOOL_H

#include "RefPtr.h"
#include <string>

class Download;

class IDownloadPool
{
public:
    virtual ~IDownloadPool()
    {
    }
    //virtual void AddDownload(RefPtr<Download> download) = 0;
    virtual RefPtr<Download> AddDownload(const std::string url) = 0;
    virtual void RemoveDownload(RefPtr<Download> download) = 0;
    virtual RefPtr<Download> GetNextDownload() = 0;
    virtual RefPtr<Download> GetDownloadByUrl(std::string url) = 0;
};

#endif
