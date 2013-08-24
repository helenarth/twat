/*
 * Copyright (c) 2013 Christoph Malek
 * See LICENSE for more information.
 */


#include "master.h"

#include "info_decoder.h"
#include "pk_requests.h"

#include <base/system.h>

#include <core/shared/network.h>

#include <cstring>


TWAT::TwTools::CMasterList::CMasterList()
{
	m_chunkSize = 0;
}

void TWAT::TwTools::CMasterList::AddAddr(const std::string &ip)
{
	m_ips.push_back(ip);
}

void TWAT::TwTools::CMasterList::AddChunkSize(int size)
{
	m_chunkSize += size;
}


TWAT::TwTools::CMasterRequest::CMasterRequest()
{
	//	m_sock = System::UdpSock();
	m_sock = System::UdpSock();
}

TWAT::TwTools::CMasterRequest::~CMasterRequest()
{
	System::SockClose(m_sock);
}

void TWAT::TwTools::CMasterRequest::AddServer(const std::string &addr)
{
	System::CIpAddr *tmpAddr = new System::CIpAddr(addr);
	m_addrs.push_back(tmpAddr);
}

void TWAT::TwTools::CMasterRequest::ClearServers()
{
	m_addrs.clear();
}

int TWAT::TwTools::CMasterRequest::PullCount()
{
	int count = 0;
	int gotLen = 0;
	unsigned char *recData = (unsigned char *)std::malloc(1024);

	for(std::vector<System::CIpAddr *>::iterator i = m_addrs.begin(); i != m_addrs.end(); ++i)
	{
		CNetworkPacket *pk = new CNetworkPacket(*i, (unsigned char *)SERVERBROWSE_GETCOUNT, 8);
		pk->MakeConnless();
		CNetworkBase::Send(m_sock, pk);

		if((gotLen = CNetworkBase::RecvRaw(m_sock, recData, 1024)) > 0)
		{
			// TODO: CHECK PK

			int tmpCount = CRawInfoDecoder::DecodeCountInfo(recData, gotLen);

			if(tmpCount != -1)
				count += tmpCount;
			else
				DBG("error while decode recved count data from: %", System::IpAddrToStr(*i));
		}
		else
			DBG("error while connect to master: %", System::IpAddrToStr(*i));
	}

	std::free(recData);
	return count;
}

bool TWAT::TwTools::CMasterRequest::PullList(CMasterList *lst)
{
	int gotLen = 0;
	unsigned char *recData = (unsigned char *)std::malloc(2048); // moa space


	for(std::vector<System::CIpAddr *>::iterator i = m_addrs.begin(); i != m_addrs.end(); ++i)
	{
		CNetworkPacket *pk = new CNetworkPacket(*i, (unsigned char *)SERVERBROWSE_GETLIST, 8);
		pk->MakeConnless();
		CNetworkBase::Send(m_sock, pk);

		while((gotLen = CNetworkBase::RecvRaw(m_sock, recData, 2048)) > 0) // get the info splittet in X pk's
		{
			if(!CRawInfoDecoder::DecodeListInfo(recData, gotLen, lst))
				DBG("error while decode recved list data from: %", System::IpAddrToStr(*i));
		}
	}

	std::free(recData);
	return true;
}
