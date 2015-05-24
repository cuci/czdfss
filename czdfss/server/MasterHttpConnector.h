/*
 * masterhttpconnector.h
 *
 *  Created on: 2015年5月3日
 *      Author: root
 */

#ifndef MASTERHTTPCONNECTOR_H_
#define MASTERHTTPCONNECTOR_H_

#include"AbstractMasterConnector.h"
#include<cstdint>
#include<list>

// @param maxim connection number
#define MAX_CONN_NUM 1024

#define INET_IPV4
#ifdef INET_IPV4
	#define IN_ADDR_TYPE struct in_addr
#else
	#define IN_ADDR_TYPE struct in_addr6
#endif



class MasterHttpConnector : public AbstractMasterConnector{
private:
	//int conn_set[MAX_CONN_NUM];
	std::list<int> conn_set
	//network short
	int http_port;
	IN_ADDR_TYPE netw_addr;
	int conn_num;
public:
	MasterHttpConnector();
	int init();
	void startToListen();
	void stopListening();
	int registerHandle();
};






#endif /* MASTERHTTPCONNECTOR_H_ */
