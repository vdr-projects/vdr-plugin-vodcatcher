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
 * $Id: IServiceLocator.h 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#ifndef ___ISERVICELOCATOR_H
#define ___ISERVICELOCATOR_H

#include "Feed.h"
#include "Item.h"
#include "RefPtr.h"

class IFeedSources;
class IFeedParser;
class IDownloadPool;
class IDownloadCache;
class IFeedUpdater;
class IThreadAction;
class IPesPlayer;
class IMp3Decoder;
class IThread;
class IErrorLogger;
class cControl;
class cOsdMenu;
class cMenuSetupPage;
class IMenuFactory;

class IServiceLocator
{
public:
    virtual ~IServiceLocator()
    {
    }
    virtual IFeedSources* GetFeedSources() = 0;
    virtual IFeedParser* GetFeedParser() = 0;
    virtual IDownloadPool* GetDownloadPool() = 0;
    virtual IDownloadCache* GetDownloadCache() = 0;
    virtual IThread* GetDownloadThread() = 0;
    virtual IFeedUpdater* GetFeedUpdater() = 0;
    virtual RefPtr<IThreadAction> CreateDownloadAction() = 0;
    virtual cOsdMenu* CreateFeedMenu() = 0;
    virtual cOsdMenu* CreateItemView(Feed feed, Item item) = 0;
    virtual IErrorLogger* GetErrorLogger() = 0;
    virtual cMenuSetupPage* CreateSetupMenu() = 0;
    virtual IMenuFactory* GetMenuFactory() = 0;
};

#endif // ___ISERVICELOCATOR_H
