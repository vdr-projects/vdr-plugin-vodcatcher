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
 * $Id: plugin.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include <string>
#include <vdr/plugin.h>
#include "pluginhooks.h"

using namespace std;

cPlugin::cPlugin(void)
{
}

cPlugin::~cPlugin()
{
}

const char* cPlugin::CommandLineHelp(void)
{
    return "";
}

bool cPlugin::ProcessArgs(int argCount, char* argValues[])
{
    return false;
}

bool cPlugin::Start(void)
{
    return false;
}

void cPlugin::Stop(void)
{
}

void cPlugin::Housekeeping(void)
{
}

const char* cPlugin::MainMenuEntry(void)
{
    return "";
}

bool cPlugin::Initialize(void)
{
    return false;
}

cOsdObject* cPlugin::MainMenuAction(void)
{
    return NULL;
}

cMenuSetupPage* cPlugin::SetupMenu(void)
{
    return NULL;
}

bool cPlugin::SetupParse(const char* Name, const char* Value)
{
    return false;
}

bool cPlugin::Service(const char* Id, void* Data)
{
    return false;
}

const char** cPlugin::SVDRPHelpPages(void)
{
    return NULL;
}

cString cPlugin::SVDRPCommand(const char* Command, const char* Option, int &ReplyCode)
{
    return "";
}

#if APIVERSNUM >= 10507
void cPlugin::RegisterI18n(const void *)
{
}
#else
void cPlugin::RegisterI18n(const tI18nPhrase * const Phrases)
{
    PluginHooks::Phrases = Phrases;
}
#endif

const char* cPlugin::ConfigDirectory(const char* PluginName)
{
    return "/vdrstub";
}

void cPlugin::MainThreadHook(void)
{
}

cString cPlugin::Active(void)
{
    return NULL;
}

#if APIVERSNUM >= 10501
time_t cPlugin::WakeupTime(void)
{
    return 0;
}
#endif
