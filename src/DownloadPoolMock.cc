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

#include "DownloadPoolMock.h"
#include "Download.h"

using namespace std;

DownloadPoolMock::DownloadPoolMock() :
    StringMessageMock()
{
}

DownloadPoolMock::DownloadPoolMock(StringMessageMock& parent) :
    StringMessageMock(parent)
{
}

RefPtr<Download> DownloadPoolMock::AddDownload(const std::string url)
{
    AddMethod("AddDownload", url);
    lastAddedDownload = RefPtr<Download>(new Download(url));
    return lastAddedDownload;
}

void DownloadPoolMock::RemoveDownload(RefPtr<Download> download)
{
    AddMethod("RemoveDownload", download->GetUrl());
}

RefPtr<Download> DownloadPoolMock::GetNextDownload()
{
    AddMethod("GetNextDownload");
    if (!returnedDownload.get())
    {
        return RefPtr<Download>();
    }
    else
    {
        return returnedDownload;
    }
}

RefPtr<Download> DownloadPoolMock::GetDownloadByUrl(string url)
{
    AddMethod("GetDownloadByUrl", url);
    return returnedDownload;
}
