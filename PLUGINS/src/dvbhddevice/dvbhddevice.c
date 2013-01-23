/*
 * dvbhddevice.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: dvbhddevice.c 1.19 2013/01/12 14:11:35 kls Exp $
 */

#include <vdr/plugin.h>
#include <vdr/shutdown.h>
#include "dvbhdffdevice.h"
#include "menu.h"
#include "setup.h"

static const char *VERSION        = "0.0.6";
static const char *DESCRIPTION    = trNOOP("HD Full Featured DVB device");
static const char *MAINMENUENTRY  = "dvbhddevice";

class cPluginDvbhddevice : public cPlugin {
private:
  cDvbHdFfDeviceProbe *probe;
  bool mIsUserInactive;
public:
  cPluginDvbhddevice(void);
  virtual ~cPluginDvbhddevice();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return tr(DESCRIPTION); }
  virtual void MainThreadHook(void);
  virtual const char *MainMenuEntry(void);
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
  };

cPluginDvbhddevice::cPluginDvbhddevice(void)
:   mIsUserInactive(true)
{
  probe = new cDvbHdFfDeviceProbe;
}

cPluginDvbhddevice::~cPluginDvbhddevice()
{
  delete probe;
}

void cPluginDvbhddevice::MainThreadHook(void)
{
    bool isUserInactive = ShutdownHandler.IsUserInactive();
    if (isUserInactive != mIsUserInactive)
    {
        mIsUserInactive = isUserInactive;
        if (gHdffSetup.CecEnabled && gHdffSetup.CecTvOn)
        {
            HDFF::cHdffCmdIf * hdffCmdIf = cDvbHdFfDevice::GetHdffCmdHandler();
            if (hdffCmdIf && !mIsUserInactive)
            {
                hdffCmdIf->CmdHdmiSendCecCommand(HDFF_CEC_COMMAND_TV_ON);
            }
        }
    }
}

const char *cPluginDvbhddevice::MainMenuEntry(void)
{
  return gHdffSetup.HideMainMenu ? NULL : MAINMENUENTRY;
}

cOsdObject *cPluginDvbhddevice::MainMenuAction(void)
{
  HDFF::cHdffCmdIf * hdffCmdIf = cDvbHdFfDevice::GetHdffCmdHandler();
  return hdffCmdIf ? new cHdffMenu(hdffCmdIf) : NULL;
}

cMenuSetupPage *cPluginDvbhddevice::SetupMenu(void)
{
  HDFF::cHdffCmdIf * hdffCmdIf = cDvbHdFfDevice::GetHdffCmdHandler();
  return hdffCmdIf ? new cHdffSetupPage(hdffCmdIf) : NULL;
}

bool cPluginDvbhddevice::SetupParse(const char *Name, const char *Value)
{
  return gHdffSetup.SetupParse(Name, Value);
}

VDRPLUGINCREATOR(cPluginDvbhddevice); // Don't touch this!
