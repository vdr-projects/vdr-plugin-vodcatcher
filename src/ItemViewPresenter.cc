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
 * $Id: ItemViewPresenter.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include "ItemViewPresenter.h"
#include <vdr/i18n.h>
#include "ItemView.h"
#include "IConfiguration.h"

using namespace std;

ItemViewPresenter::ItemViewPresenter(const string feedTitle, const Item item, IConfiguration& configuration) :
    feedTitle(feedTitle), item(item), _configuration(configuration)
{
}

void ItemViewPresenter::Initialize(IItemView* view)
{
    this->view = view;
    view->SetTitle(feedTitle);

    view->SetDescription(item.GetDate().ToLongString() + "\n\n" + item.GetTitle() + "\n\n" + item.GetDescription());

    if (item.IsVideocast())
    {
        view->SetRedHelp(tr("Record"));
        view->SetGreenHelp(tr("Play"));
        UpdateYellowHelp();
    }
    else
    {
        view->SetRedHelp("");
        view->SetGreenHelp("");
        view->SetYellowHelp("");
    }
}

eOSState ItemViewPresenter::ProcessKey(eOSState state, eKeys key)
{
    switch (key)
    {
        case kYellow:
            switch (_configuration.GetPlayBackQuality())
            {
                case High:
                    _configuration.SetPlayBackQuality(Medium);
                    break;
                case Medium:
                    _configuration.SetPlayBackQuality(Low);
                    break;
                case Low:
                    _configuration.SetPlayBackQuality(High);
                    break;
            }
            UpdateYellowHelp();
            return osContinue;
        case kRed:
        case kGreen:
            // indicates the parent, that it can process these keys
            return osUnknown;
        default:
            return state;
    }
}

void ItemViewPresenter::UpdateYellowHelp()
{
    view->SetYellowHelp(StreamTypeToString(_configuration.GetPlayBackQuality()));
}
