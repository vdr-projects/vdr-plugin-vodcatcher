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

#include "SynchedDownloadPool.h"
#include "Sleeper.h"
#include "Download.h"

using namespace std;

SynchedDownloadPool::SynchedDownloadPool(RefPtr<IDownloadPool> downloadPool, RefPtr<ISleeper> sleeper) :
    downloadPool(downloadPool), sleeper(sleeper)
{
}

void SynchedDownloadPool::RemoveDownload(RefPtr<Download> download)
{
    downloadPool->RemoveDownload(download);
}

RefPtr <Download> SynchedDownloadPool::GetNextDownload()
{
    RefPtr<Download> download = downloadPool->GetNextDownload();

    if (!download.get())
    {
        sleeper->Sleep();
    }

    return download;
}

RefPtr <Download> SynchedDownloadPool::GetDownloadByUrl(string url)
{
    return downloadPool->GetDownloadByUrl(url);
}

RefPtr<Download> SynchedDownloadPool::AddDownload(const string url)
{
    RefPtr<Download> download = downloadPool->AddDownload(url);
    sleeper->WakeUp();
    return download;
}
