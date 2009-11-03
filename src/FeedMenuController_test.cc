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
 * $Id: FeedMenuController_test.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include <vector>
#include <sstream>
#include <cxxtest/TestSuite.h>
#include "FeedMenuController.h"
#include "ServiceLocatorStub.h"
#include "DownloadPoolMock.h"
#include "IListMenu.h"
#include "ListMenuMock.h"
#include "IMenuFactory.h"
#include "IFeedRepository.h"

using namespace std;

namespace
{

class FeedRepositoryStub: public IFeedRepository
{
public:
    FeedList Feeds;
    
public:
    // <IFeedRepository>
    Feed GetFeed(std::string url)
    {
        for (FeedList::iterator i = Feeds.begin(); i < Feeds.end(); i++)
        {
            if ((*i).GetUrl() == url)
            {
                return *i;
            }
        }
        return Feed(url);
    }
    
    const FeedList GetRootFeeds() const
    {
        return Feeds;
    }
    // </IFeedRepository>
};

class MenuFactoryStub: public IMenuFactory
{
public:
    RefPtr<cOsdMenu> Menu;
    
public:
    cOsdMenu* CreateItemMenu(const Feed& feed)
    {
        return Menu.get();
    }
};

class A_FeedMenuController: public CxxTest::TestSuite
{
private:
    RefPtr<FeedMenuController> _feedMenuController;
    RefPtr<MenuFactoryStub> _menuFactory;
    RefPtr<FeedRepositoryStub> _feedRepository;

private:
    void setUp()
    {
        _feedRepository = new FeedRepositoryStub();
        _menuFactory = new MenuFactoryStub();
        _feedMenuController = new FeedMenuController(*_menuFactory, *_feedRepository);
    }

public:
    void Should_set_the_menu_title_on_initialization()
    {
        ListMenuMock menu;
        menu.ExpectMethod("SetTitle", "i18n:Video Podcasts");
        _feedMenuController->Initialize(&menu);
        VERIFY_EXPECTATIONS(menu);
    }

    void Should_add_the_feed_titles_to_the_menu()
    {
        Feed feed1("feed1Url"); feed1.SetTitle("feed1");
        Feed feed2("feed2Url"); feed2.SetTitle("feed2");
        _feedRepository->Feeds.push_back(feed1);
        _feedRepository->Feeds.push_back(feed2);
        ListMenuMock menu;

        menu.ExpectMethod("AddItem", "feed1");
        menu.ExpectMethod("AddItem", "feed2");
        
        _feedMenuController->Initialize(&menu);
        
        VERIFY_EXPECTATIONS(menu);
    }
    
    void Should_do_nothing_when_pressing_ok_while_no_feeds_are_present()
    {
        _menuFactory->Menu = new cOsdMenu("");
        ListMenuMock menu;
        _feedMenuController->Initialize(&menu);
        menu.ResetMock();
        TS_ASSERT_EQUALS(osContinue, _feedMenuController->ProcessKey(0, kOk, osUnknown));
        VERIFY(menu);
    }

    void Should_open_a_sub_menu_when_selecting_a_feed_with_OK()
    {
        Feed feed1("feed1Url"); feed1.SetTitle("feed1");
        Feed feed2("feed2Url"); feed2.SetTitle("feed2");
        _feedRepository->Feeds.push_back(feed1);
        _feedRepository->Feeds.push_back(feed2);
        _menuFactory->Menu = new cOsdMenu("");
        ListMenuMock menu;

        menu.ExpectMethod("ShowSubMenu", menu.MakeString(_menuFactory->Menu.get()));

        _feedMenuController->Initialize(&menu);
        TS_ASSERT_EQUALS(osContinue, _feedMenuController->ProcessKey(1, kOk, osUnknown));
        
        VERIFY_EXPECTATIONS(menu);
    }
};

};
