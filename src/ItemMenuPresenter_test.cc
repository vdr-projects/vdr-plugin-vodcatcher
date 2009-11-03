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
 * $Id: ItemMenuPresenter_test.cc 7656 2008-08-09 21:07:45Z svntobi $
 *
 */

#include <cxxtest/TestSuite.h>
#include "ItemMenuPresenter.h"
#include "ServiceLocatorStub.h"
#include "ListMenuMock.h"
#include "DownloadPoolMock.h"
#include "DownloadCacheMock.h"
#include "vdr-stub/ccontrolstub.h"
#include "IMediaPlayer.h"
#include "IFeedRepository.h"
#include "ConfigurationStub.h"
#include "IVdrInterface.h"

using namespace std;

namespace
{

class An_ItemMenuPresenter_for_an_empty_feed: public CxxTest::TestSuite
{
private:
    ServiceLocatorStub _serviceLocator;
    DownloadCacheMock _downloadCache;
    DownloadPoolMock _downloadPool;
    RefPtr<IMediaPlayer> _mplayerPlugin;
    RefPtr<ListMenuMock> _menu;
    RefPtr<Feed> _feed;
    RefPtr<ItemMenuPresenter> _presenter;
    RefPtr<IFeedRepository> _feedRepository;
    RefPtr<IConfiguration> _configuration;
    RefPtr<IVdrInterface> _vdrInterface;

public:
	void setUp()
	{
		_menu = new ListMenuMock();
		_feed = new Feed("");
        _feed->SetTitle("dummy title");
        _presenter = new ItemMenuPresenter(_serviceLocator, *_feed, _feedRepository, *_configuration, _mplayerPlugin, _vdrInterface);
        _serviceLocator.downloadPool = &_downloadPool;
	}

	void Should_do_nothing()
    {
        _presenter->Initialize(_menu.get());
        _menu->isActiveMenu = true;

        TS_ASSERT_EQUALS(osUnknown, _presenter->ProcessKey(-1, kOk, osUnknown));
    }

    void Should_display_the_feed_title_as_the_menu_title()
    {
        _menu->ExpectMethod("SetTitle", "dummy title");

        _presenter->Initialize(_menu.get());

        VERIFY_EXPECTATIONS(*_menu);
    }

    void Should_display_a_no_entries_message_as_the_first_menu_entry()
    {
        _menu->ExpectMethod("AddItem", "i18n:No entries!");

        _presenter->Initialize(_menu.get());

        VERIFY_EXPECTATIONS(*_menu);
    }
};

class FeedRepositoryMock: public IFeedRepository
{
public:
    Feed GetFeed(std::string url) { return Feed(""); }
    const FeedList GetRootFeeds() const { return FeedList(); }
};

class MplayerPluginMock: public IMediaPlayer, public StringMessageMock
{
public:
    bool PlayResult;

    MplayerPluginMock() :
        PlayResult(true)
    {
    }

    bool Play(string url)
    {
        AddMethod("Play", url);
        return PlayResult;
    }
};

class VdrInterfaceMock: public IVdrInterface, public StringMessageMock
{
public:
    void ShowMessage(const string& message)
    {
        AddMethod("ShowMessage", message);
    }

    void ShowErrorMessage(const string& message)
    {
        AddMethod("ShowErrorMessage", message);
    }
};

class An_ItemMenuPresenter_for_a_nonempty_feed : public CxxTest::TestSuite
{
private:
	ServiceLocatorStub _serviceLocator;
	DownloadCacheMock _downloadCache;
	DownloadPoolMock _downloadPool;
	RefPtr<MplayerPluginMock> _mplayerPlugin;
	RefPtr<ListMenuMock> _menu;
	RefPtr<Feed> _feed;
	RefPtr<ItemMenuPresenter> _presenter;
	RefPtr<IFeedRepository> _feedRepository;
	RefPtr<ConfigurationStub> _configuration;
	RefPtr<VdrInterfaceMock> _vdrInterface;

public:
	void setUp()
    {
        _menu = new ListMenuMock();
        _menu->isActiveMenu = true;
        _feed = new Feed("");
        Item item1("item1", Rfc822DateTime("Tue, 10 Jun 2003 04:00:00 GMT"), "");
        Item item2("item2", Rfc822DateTime("Tue, 11 Jun 2003 04:00:00 GMT"), "");
        Item item3("item2", Rfc822DateTime("Tue, 11 Jun 2003 04:00:00 GMT"), "");
        item2.SetVideoCastStream(High, "streamUrlHigh");
        item2.SetVideoCastStream(Medium, "streamUrlMedium");
        item2.SetVideoCastStream(Low, "streamUrlLow");
        item2.SetStreamLength("12 min");
        item3.SetSubFeedUrl("linkUrl");
        _feed->AddItem(item1);
        _feed->AddItem(item2);
        _feed->AddItem(item3);
        _mplayerPlugin = new MplayerPluginMock();
        _configuration = new ConfigurationStub();
        _configuration->SetPlayBackQuality(High);
        _vdrInterface = new VdrInterfaceMock();
        _presenter = new ItemMenuPresenter(_serviceLocator, *_feed, _feedRepository, *_configuration, _mplayerPlugin, _vdrInterface);
        _feedRepository = new FeedRepositoryMock();
        _serviceLocator.downloadPool = &_downloadPool;
    }

	void Should_display_the_item_dates_titels_and_lengths_as_menu_entries()
	{
		_menu->ExpectMethod("AddItem", "10.06.03\titem1");
		_menu->ExpectMethod("AddItem", "11.06.03\titem2 (12 min)");

		_presenter->Initialize(_menu.get());

        VERIFY_EXPECTATIONS(*_menu);
	}

	void Should_set_the_green_help_button_to_play_when_moving_the_selection_to_a_videocast_item()
	{
		_menu->ExpectMethod("SetGreenHelp", "i18n:Play");

		_presenter->Initialize(_menu.get());
		_presenter->ProcessKey(1, kDown, osContinue);

        VERIFY_EXPECTATIONS(*_menu);
	}

    void Should_set_the_red_help_button_to_record_when_moving_the_selection_to_a_videocast_item()
    {
        _menu->ExpectMethod("SetRedHelp", "i18n:Record");

        _presenter->Initialize(_menu.get());
        _presenter->ProcessKey(1, kDown, osContinue);

        VERIFY_EXPECTATIONS(*_menu);
    }

    void Should_set_the_yellow_help_button_to_the_last_selected_quality_when_moving_the_selection_to_a_videocast_item()
    {
        _configuration->SetPlayBackQuality(Medium);

        _menu->ExpectMethod("SetYellowHelp", "i18n:Medium");

        _presenter->Initialize(_menu.get());
        _presenter->ProcessKey(1, kDown, osContinue);

        VERIFY_EXPECTATIONS(*_menu);
    }

    void Should_open_a_submenu_when_selecting_a_category_item()
	{
		cOsdMenu itemMenu("");
		_serviceLocator.itemMenu = &itemMenu;

		_menu->ExpectMethod("ShowSubMenu", _menu->MakeString(&itemMenu));

		_presenter->Initialize(_menu.get());
		TS_ASSERT_EQUALS(osContinue, _presenter->ProcessKey(2, kOk, osUnknown));

        VERIFY_EXPECTATIONS(*_menu);
	}

	void Should_play_a_stream_in_the_selected_quality_when_pressing_green()
	{
        cOsdMenu itemMenu("");
        _serviceLocator.itemMenu = &itemMenu;
        _configuration->SetPlayBackQuality(Medium);

        _mplayerPlugin->ExpectMethod("Play", "streamUrlMedium");

        _presenter->Initialize(_menu.get());
        _presenter->ProcessKey(1, kGreen, osUnknown);

        VERIFY_EXPECTATIONS(*_mplayerPlugin);
	}

    void Should_leave_the_osd_menu_when_playing_a_stream()
    {
        cOsdMenu itemMenu("");
        _serviceLocator.itemMenu = &itemMenu;
        _configuration->SetPlayBackQuality(Medium);

        _presenter->Initialize(_menu.get());
        TS_ASSERT_EQUALS(osEnd, _presenter->ProcessKey(1, kGreen, osUnknown));
    }

    void Should_display_a_message_when_playing_a_stream()
    {
        cOsdMenu itemMenu("");
        _serviceLocator.itemMenu = &itemMenu;
        _configuration->SetPlayBackQuality(Medium);
        _presenter->Initialize(_menu.get());

        _vdrInterface->ExpectMethod("ShowMessage", "i18n:Starting playback, please wait...");

        _presenter->ProcessKey(1, kGreen, osUnknown);

        VERIFY(*_vdrInterface);
    }

    void Should_display_a_message_when_playing_a_stream_failed()
    {
        cOsdMenu itemMenu("");
        _serviceLocator.itemMenu = &itemMenu;
        _configuration->SetPlayBackQuality(Medium);
        _presenter->Initialize(_menu.get());
        _mplayerPlugin->PlayResult = false;

        _vdrInterface->ExpectMethod("ShowErrorMessage", "i18n:Playback failed!");

        _presenter->ProcessKey(1, kGreen, osUnknown);

        VERIFY(*_vdrInterface);
    }

    void Should_not_leave_the_menu_when_playing_a_stream_failed()
    {
        cOsdMenu itemMenu("");
        _serviceLocator.itemMenu = &itemMenu;
        _configuration->SetPlayBackQuality(Medium);
        _presenter->Initialize(_menu.get());
        _mplayerPlugin->PlayResult = false;

        TS_ASSERT_EQUALS(osContinue, _presenter->ProcessKey(1, kGreen, osUnknown));
    }

    void Should_toggle_the_quality_when_pressing_the_yellow_key()
	{
        cOsdMenu itemMenu("");
        _serviceLocator.itemMenu = &itemMenu;
        _configuration->SetPlayBackQuality(Low);
        _presenter->Initialize(_menu.get());

        _menu->ResetMock();
        _menu->ExpectMethod("SetYellowHelp", "i18n:High");
        _menu->ExpectMethod("SetYellowHelp", "i18n:Medium");
        _menu->ExpectMethod("SetYellowHelp", "i18n:Low");

        TS_ASSERT_EQUALS(osContinue, _presenter->ProcessKey(1, kYellow, osUnknown));
        TS_ASSERT_EQUALS(High, _configuration->GetPlayBackQuality());

        TS_ASSERT_EQUALS(osContinue, _presenter->ProcessKey(1, kYellow, osUnknown));
        TS_ASSERT_EQUALS(Medium, _configuration->GetPlayBackQuality());

        TS_ASSERT_EQUALS(osContinue, _presenter->ProcessKey(1, kYellow, osUnknown));
        TS_ASSERT_EQUALS(Low, _configuration->GetPlayBackQuality());

        VERIFY_EXPECTATIONS(*_menu);
	}
};
};
