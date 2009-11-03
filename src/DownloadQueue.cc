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
 * $Id: DownloadQueue.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include "DownloadQueue.h"
#include "Download.h"

using namespace std;

RefPtr<Download> DownloadQueue::GetNextDownload()
{
    if (downloads.empty())
    {
        return RefPtr<Download>();
    }
    else
    {
        return downloads.back();
    }
}

RefPtr<Download> DownloadQueue::GetDownloadByUrl(string url)
{
    DownloadDeque::iterator i = LocateDownload(url);

    if (i != downloads.end())
    {
        return *LocateDownload(url);
    }
    else
    {
        return RefPtr<Download>();
    }
}

void DownloadQueue::RemoveDownload(RefPtr<Download> download)
{
    DownloadDeque::iterator i = LocateDownload(download->GetUrl());

    if (i != downloads.end())
    {
        downloads.erase(i);
    }
}

DownloadQueue::DownloadDeque::iterator DownloadQueue::LocateDownload(string url)
{
    DownloadDeque::iterator i;

    for (i = downloads.begin(); i != downloads.end(); i++)
    {
        if ((*i)->GetUrl() == url)
        {
            break;
        }
    }

    return i;
}

RefPtr<Download> DownloadQueue::AddDownload(const string url)
{
    DownloadDeque::iterator i = LocateDownload(url);

    if (i == downloads.end())
    {
        RefPtr<Download> download(new Download(url));
        downloads.push_front(download);
        return download;
    }

    return *i;
}
