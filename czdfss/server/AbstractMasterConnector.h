/*
 * abstractserverconnector.h
 *
 *  Created on: 2015年5月3日
 *      Author: root
 */

#ifndef ABSTRACTMASTERCONNECTOR_H_
#define ABSTRACTMASTERCONNECTOR_H_

#define INI_OK 0

class AbstractMasterConnector{
public:
	AbstractMasterConnector(){}
	virtual ~AbstractMasterConnector() {}
	virtual int init() {return INI_OK;};
	virtual void startToListen() =0 ;
	virtual void stopListening() =0 ;
	virtual int registerHandle() =0 ;
};

#endif /* ABSTRACTSERVERCONNECTOR_H_ */
