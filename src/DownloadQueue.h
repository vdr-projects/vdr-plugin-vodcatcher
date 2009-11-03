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
 * $Id: DownloadQueue.h 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#ifndef ___DOWNLOADQUEUE_H
#define ___DOWNLOADQUEUE_H

#include <deque>
#include "IDownloadPool.h"

class DownloadQueue : public IDownloadPool
{
    typedef std::deque< RefPtr<Download> > DownloadDeque;

private:
    DownloadDeque downloads;

private:
    DownloadDeque::iterator LocateDownload(std:: string url);

public:
    // <IDownloadPool>
    RefPtr<Download> AddDownload(const std::string url);
    virtual void RemoveDownload(RefPtr<Download> download);
    RefPtr<Download> GetNextDownload();
    RefPtr<Download> GetDownloadByUrl(std::string url);
    // </IDownloadPool>
};

#endif // ___DOWNLOADQUEUE_H
