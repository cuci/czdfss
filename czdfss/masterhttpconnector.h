/*
 * masterhttpconnector.h
 *
 *  Created on: 2015年5月3日
 *      Author: root
 */

#ifndef MASTERHTTPCONNECTOR_H_
#define MASTERHTTPCONNECTOR_H_

#include"abstractmasterconnector.h"
#include<cstdint>

// @param maxim connection number
#define MAX_CONN_NUM 1024

class MasterHttpConnector : public AbstractMasterConnector{
private:
	int conn_set[MAX_CONN_NUM];
	//network short
	int http_port;
	int netw_addr;
	int conn_num;
public:
	MasterHttpConnector();
	int init();
};






#endif /* MASTERHTTPCONNECTOR_H_ */
