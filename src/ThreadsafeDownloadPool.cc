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

#include "ThreadsafeDownloadPool.h"
#include "Thread.h"
#include "Sleeper.h"
#include "CriticalSection.h"
#include "Download.h"

using namespace std;

ThreadSafeDownloadPool::ThreadSafeDownloadPool(RefPtr<IDownloadPool> downloadPool,
    RefPtr<ICriticalSection> criticalSection) :
    downloadPool(downloadPool), criticalSection(criticalSection)
{
}

void ThreadSafeDownloadPool::RemoveDownload(RefPtr<Download> download)
{
    criticalSection->Enter();
    downloadPool->RemoveDownload(download);
    criticalSection->Leave();
}

RefPtr<Download> ThreadSafeDownloadPool::GetNextDownload()
{
    criticalSection->Enter();
    RefPtr<Download> nextDownload = downloadPool->GetNextDownload();
    criticalSection->Leave();
    return nextDownload;
}

RefPtr<Download> ThreadSafeDownloadPool::GetDownloadByUrl(string url)
{
    criticalSection->Enter();
    RefPtr<Download> download = downloadPool->GetDownloadByUrl(url);
    criticalSection->Leave();
    return download;
}

RefPtr<Download> ThreadSafeDownloadPool::AddDownload(const string url)
{
    criticalSection->Enter();
    RefPtr<Download> download = downloadPool->AddDownload(url);
    criticalSection->Leave();
    return download;
}
