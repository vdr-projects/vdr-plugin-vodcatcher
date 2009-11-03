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
 * $Id: LocalFileCache.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include <sys/stat.h>
#include <fstream>
#include <dirent.h>
#include "LocalFileCache.h"
#include "SdbmHashCalculator.h"
#include <values.h>

using namespace std;

LocalFileCache::LocalFileCache(const string& cacheDirName) :
    _cacheDirName(cacheDirName)
{
}

RefPtr<istream> LocalFileCache::CreateStreamByUrl(const string& url) const
{
    ifstream* localFile = new ifstream();
    localFile->open(UrlToLocalFileName(url).c_str());
    if (!localFile->fail())
    {
        return RefPtr<istream>(localFile);
    }
    else
    {
        delete localFile;
        return RefPtr<istream>(NULL);
    }
}

void LocalFileCache::Put(istream& document, const string& url)
{
    ofstream localFile(UrlToLocalFileName(url).c_str());
    localFile << document.rdbuf();
    localFile.close();
    if (!localFile.good())
    {
        unlink(UrlToLocalFileName(url).c_str());
    }
}

string LocalFileCache::UrlToLocalFileName(const string& url) const
{
    return _cacheDirName + "/" + SdbmHashCalculator::Calculate(url);
}

long LocalFileCache::GetAgeInMinutes(const string& url) const
{
    return GetLocalFileAge(UrlToLocalFileName(url));
}

bool LocalFileCache::IsCached(const string& url) const
{
    struct stat buf;
    if (0 == stat(UrlToLocalFileName(url).c_str(), &buf))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void LocalFileCache::CleanUp(const long maxAgeInDays)
{
    DIR* directory = opendir(_cacheDirName.c_str());

    if (directory)
    {
        struct dirent* entry;
        while ((entry = readdir(directory)))
        {
            string fileName = entry->d_name;
            if ((fileName != ".") & (fileName != ".."))
            {
                fileName = string(_cacheDirName) + "/" + fileName;
                if (GetLocalFileAge(fileName) >= maxAgeInDays * 60 * 24)
                {
                    remove(fileName.c_str());
                }
            }
        }
    }

    closedir(directory);
}

long LocalFileCache::GetLocalFileAge(const string& localFileName) const
{
    struct stat buf;
    time_t tm = time(NULL);
    
    if (0 == stat(localFileName.c_str(), &buf))
    {
        return (tm - buf.st_mtime) / 60 ;
    }

    return MAXLONG;
}
