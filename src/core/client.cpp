/*
 * Copyright (c) 2013 Christoph Malek
 * See LICENSE for more information.
 */

#include "client.h"

#include "../base/app_info.h"
#include <iostream>



TWAT::CClient::CClient()
{
	// setup
	m_Config = new CConfig(APP_CONF_PATH);
}

void TWAT::CClient::OnInit()
{
	// check first start and other

}
