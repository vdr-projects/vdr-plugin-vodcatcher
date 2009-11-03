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
 * $Id: ItemViewPresenter_test.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include <cxxtest/TestSuite.h>
#include <string>
#include "Item.h"
#include "ItemView.h"
#include "ItemViewPresenter.h"
#include "ConfigurationStub.h"
#include "RefPtr.h"

using namespace std;

namespace
{

class ItemViewStub : public IItemView
{
public:
    string description;
    string title;
    string redHelp;
    string greenHelp;
    string yellowHelp;

public:
    // <IItemView>
    void SetDescription(string description)
    {
        this->description = description;
    }

    void SetTitle(string title)
    {
        this->title = title;
    }

    void SetRedHelp(string text)
    {
        redHelp = text;
    }

    void SetGreenHelp(string text)
    {
        greenHelp = text;
    }

    void SetYellowHelp(string text)
    {
        yellowHelp = text;
    }
    void SetBlueHelp(string text)
    {
    }
    // </IItemView>
};

class An_ItemViewPresenter_for_any_item : public CxxTest::TestSuite
{
private:
    ItemViewStub view;
    RefPtr<ItemViewPresenter> _presenter;
    RefPtr<ConfigurationStub> _configuration;

public:
    void setUp()
    {
        view.title = "";
        view.description = "";
        Item item("title", Rfc822DateTime("long_date"), "description");
        _configuration = new ConfigurationStub();
        _presenter = new ItemViewPresenter("FeedTitle", item, *_configuration);
    }
    
    void Should_show_the_item_description()
    {
        _presenter->Initialize(&view);

        TS_ASSERT_EQUALS("long_date\n\ntitle\n\ndescription", view.description);
    }
    
    void Should_display_the_feed_title_as_menu_title()
    {
        _presenter->Initialize(&view);

        TS_ASSERT_EQUALS("FeedTitle", view.title);
    }

    void Should_ignore_already_processed_keys()
    {
        _presenter->Initialize(&view);

        TS_ASSERT_EQUALS(osBack, _presenter->ProcessKey(osBack, kOk));
    }
};

class An_ItemViewPresenter_for_a_non_videocast_item : public CxxTest::TestSuite
{
private:
    ItemViewStub view;
    RefPtr<ItemViewPresenter> _presenter;
    RefPtr<ConfigurationStub> _configuration;

public:
    void setUp()
    {
        view.title = "";
        view.description = "";
        Item item("title", Rfc822DateTime("long_date"), "description");
        _configuration = new ConfigurationStub();
        _configuration->SetPlayBackQuality(High);
        _presenter = new ItemViewPresenter("", item, *_configuration);
    }

    void Should_not_display_the_Play_and_Record_help_for_a_non_videocast_item()
    {
        view.redHelp = view.greenHelp = view.yellowHelp = "dummy";

        _presenter->Initialize(&view);

        TS_ASSERT_EQUALS("", view.greenHelp);
        TS_ASSERT_EQUALS("", view.redHelp);
        TS_ASSERT_EQUALS("", view.yellowHelp);
    }
};

class An_ItemViewPresenter_for_a_videocast_item : public CxxTest::TestSuite
{
private:
    ItemViewStub view;
    RefPtr<ItemViewPresenter> _presenter;
    RefPtr<ConfigurationStub> _configuration;

public:
    void setUp()
    {
        view.title = "";
        view.description = "";
        Item item("title", Rfc822DateTime("long_date"), "description");
        item.SetVideoCastStream(High, "streamUrl");
        _configuration = new ConfigurationStub();
        _configuration->SetPlayBackQuality(High);
        _presenter = new ItemViewPresenter("", item, *_configuration);
    }

    void Should_display_the_Play_Record_and_quality_help_for_a_videocast_item()
    {
        view.redHelp = view.greenHelp = view.yellowHelp = "dummy";
        _configuration->SetPlayBackQuality(High);

        _presenter->Initialize(&view);

        TS_ASSERT_EQUALS("i18n:Play", view.greenHelp);
        TS_ASSERT_EQUALS("i18n:Record", view.redHelp);
        TS_ASSERT_EQUALS("i18n:High", view.yellowHelp);
    }

    void Should_let_the_parent_menu_handle_play_and_record()
    {
        _presenter->Initialize(&view);

        TS_ASSERT_EQUALS(osUnknown, _presenter->ProcessKey(osContinue, kRed));
        TS_ASSERT_EQUALS(osUnknown, _presenter->ProcessKey(osContinue, kGreen));
    }

    void Should_toggle_the_quality_when_pressing_the_yellow_key()
    {
        _presenter->Initialize(&view);

        TS_ASSERT_EQUALS("i18n:High", view.yellowHelp);

        _presenter->ProcessKey(osContinue, kYellow);
        TS_ASSERT_EQUALS("i18n:Medium", view.yellowHelp);

        _presenter->ProcessKey(osContinue, kYellow);
        TS_ASSERT_EQUALS("i18n:Low", view.yellowHelp);

        _presenter->ProcessKey(osContinue, kYellow);
        TS_ASSERT_EQUALS("i18n:High", view.yellowHelp);
    }
};

};
