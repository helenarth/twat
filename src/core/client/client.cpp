/*
 * Copyright (c) 2013 Christoph Malek
 * See LICENSE for more information.
 */

#include "client.h"

#include <base/app_info.h>

#include <core/shared/config.h>
#include <core/client/components/serverbrowser.h>
#include <core/tools/tw/net/server.h>

#include <iostream>
#include <QDir>


TWAT::CClient::CClient()
{

}

void TWAT::CClient::Init()
{
	// start log
	DBG("TWAT version % started. Timestamp: %", APP_VERSION, System::TimeStr());

	// setup config(-path)
	QDir confDir(APP_WORK_PATH.c_str());
	if(!confDir.exists())
		confDir.mkdir(APP_WORK_PATH.c_str());

	// setup main-components
	m_config = new CConfig(APP_CONF_PATH);
	m_config->Init();

	m_twSrvBrowser = new CTwServerBrowser();
}