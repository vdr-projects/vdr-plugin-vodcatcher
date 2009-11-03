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
 * $Id: LocalFileCache.h 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#ifndef ___LOCALFILECACHE_H
#define ___LOCALFILECACHE_H

#include <string>
#include <iostream>
#include "IDownloadCache.h"

class LocalFileCache : public IDownloadCache
{
private:
    std::string _cacheDirName;

private:
    std::string UrlToLocalFileName(const std::string& url) const;
    long GetLocalFileAge(const std::string& localFileName) const;

public:
    LocalFileCache(const std::string& cacheDirName);
    // <IDownloadCache>
    RefPtr<std::istream> CreateStreamByUrl(const std::string& url) const;
    void Put(std::istream& document, const std::string& url);
    long GetAgeInMinutes(const std::string& url) const;
    bool IsCached(const std::string& url) const;
    void CleanUp(const long maxAgeInDays);
    // </IDownloadCache>
};

#endif // ___LOCALFILECACHE_H
