SRCS = \
  src/MplayerPlugin.cc \
  src/OsdItemView.cc \
  src/OsdListMenu.cc \
  src/OsdSetupMenu.cc \
  src/PluginCreator.cc \
  src/ServiceLocatorImpl.cc \
  src/SyslogErrorLogger.cc \
  src/VdrCriticalSection.cc \
  src/VdrInterface.cc \
  src/VdrSleeper.cc \
  src/VdrThread.cc \
  src/XineliboutputPlayer.cc \

SRCS_TESTABLE = \
  src/FeedRepository.cc \
  src/CurlDownloader.cc \
  src/Download.cc \
  src/DownloadAction.cc \
  src/DownloadQueue.cc \
  src/Feed.cc \
  src/FeedMenuController.cc \
  src/FeedsConfigFile.cc \
  src/FeedUpdaterImpl.cc \
  src/HtmlToText.cc \
  src/Item.cc \
  src/ItemMenuPresenter.cc \
  src/ItemViewPresenter.cc \
  src/LocalFileCache.cc \
  src/VodcatcherPlugin.cc \
  src/Rfc822DateTime.cc \
  src/RssFeedParser.cc \
  src/SdbmHashCalculator.cc \
  src/StreamType.cc \
  src/SynchedDownloadPool.cc \
  src/ThreadsafeDownloadPool.cc \
  tinyxml/tinystr.cpp \
  tinyxml/tinyxml.cpp \
  tinyxml/tinyxmlerror.cpp \
  tinyxml/tinyxmlparser.cpp \
  

SRCS_TESTONLY = \
  src/DownloadCacheMock.cc \
  src/DownloadPoolMock.cc \
  src/FeedUpdaterMock.cc \
  src/ListMenuMock.cc \
  src/ServiceLocatorStub.cc \
  src/SleeperMock.cc \
  src/StderrMock.cc \
  src/StringMessageMock.cc \
  src/ThreadMock.cc \
  src/vdr-stub/ccontrolstub.cc \
  src/vdr-stub/i18n.cc \
  src/vdr-stub/menuitems.cc \
  src/vdr-stub/osdbase.cc \
  src/vdr-stub/plugin.cc \
  src/vdr-stub/pluginhooks.cc \
  src/vdr-stub/tools.cc \
