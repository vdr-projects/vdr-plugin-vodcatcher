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

#include <sstream>
#include <curl/curl.h>
#include "CurlDownloader.h"
#include "IDownloadCache.h"
#include "Download.h"

using namespace std;

CurlDownloader::CurlDownloader(IDownloadCache& downloadCache) :
    downloadCache(downloadCache)
{
}

bool CurlDownloader::PerformDownload(Download& download)
{
    stringstream stream;
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlDownloader::WriteDataToStream);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &download);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
#ifdef DEBUG
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
#endif
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "vdr-vodcatcher/0.1");
        curl_easy_setopt(curl, CURLOPT_URL, download.GetUrl().c_str());
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (CURLE_OK == res)
        {
            downloadCache.Put(stream, download.GetUrl());
            return true;
        }
    }
    return false;
}

size_t CurlDownloader::WriteDataToStream(const void *buffer, size_t sizeOfMemblock, size_t numberOfMemblocks,
    void *streamPointer)
{
    ostream* stream = (ostream*) streamPointer;

    try
    {
        int bufferSize = sizeOfMemblock * numberOfMemblocks;
        stream->write((const char*) buffer, bufferSize);
        return bufferSize;
    }
    catch(...)
    {
    }

    return 0;
}
