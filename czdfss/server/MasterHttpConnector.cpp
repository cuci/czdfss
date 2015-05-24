/*
 * masterhttpconnector.cpp
 *
 *  Created on: 2015年5月3日
 *      Author: root
 */
#include"AbstractMasterConnector.h"
#include"MasterHttpConnector.h"
#include"ConfigureParser.h"
#include"common.h"
#include<fstream>
#include<cstring>
#include<iostream>
#include<net/if.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<stack>
using namespace std;


#ifdef INET_IPV4
#define INET_VERSION AF_INET
#define SOCK_ADDR_VERSION struct sockaddr_in
#define PRESENT_ADDRSTRLEN INET_ADDRSTRLEN
#define IN_ADDR_TYPE struct in_addr
//#define SIN_ADDR sin_addr
#endif

#define IF_NUM 50
#define CFG_PATH ("/etc/czdfss.configure")
#define CFG_MASTER_IP ("Master IP")
#define CFG_MASTER_PORT ("Master PORT")
#define CFG_ROOT_DIR ("Root Dir")
#define CFG_CONN_NUM ("Connection #")

static int get_host_address(IN_ADDR_TYPE & dest){
	int s;
	s=socket(INET_VERSION,SOCK_STREAM,0);
	if(s<0){
		std::cerr<<"get_host_address: error, open socket \n";
		return -1;
	}
	// stores the network configure info
	struct ifconf ifconf;
	// stores the configure of each inet card interface
	struct ifreq ifr[IF_NUM];
	ifconf.ifc_buf=(char*) ifr;
	ifconf.ifc_len=sizeof(ifr);
	if(ioctl(s,SIOCGIFCONF,&ifconf)==-1){
		std::cout<<"ioctl: error, fetch the internet interface configuration \n";
		return -1;
	}
	int ifs=ifconf.ifc_len/sizeof(ifr[0]);
	if(!(ifs>1)){
		std::cerr<<ifs<<" interfaces, no valid interface\n";
	}
	// ipv4 address
	SOCK_ADDR_VERSION* s_in=(SOCK_ADDR_VERSION*) &ifr[0].ifr_addr;
	char default_ip[PRESENT_ADDRSTRLEN];
#ifdef INET_IPV4
	if(!inet_ntop(INET_VERSION,&(s_in->sin_addr),default_ip,PRESENT_ADDRSTRLEN)){
		std::cerr<<"inet_ntop: error, fails to get defualt ip addr\n";
		return -1;
	}
#else
	if(!inet_ntop(INET_VERSION,&(s_in->sin_addr6),default_ip,PRESENT_ADDRSTRLEN)){
		std::cerr<<"inet_ntop: error, fails to get defualt ip addr\n";
		return -1;
	}
#endif

	std::cout<<"on network: "<<ifr[0].ifr_name<<", the master ip is: "<<default_ip<<std::endl;
#ifdef INET_IPV4
	dest=s_in->sin_addr;
#else
	dest=s_in->sin_addr6;
#endif
	close(s);
	return 0;
}


/*
	http_port=htons(80);
	get_host_address(netw_addr);
	conn_num=0;
*/
int MasterHttpConnector::init(){
	get_host_address(netw_addr);
	http_port=80;
	conn_num=0;
	ConfigureParser cfgParser(CFG_PATH);
#ifdef INET_IPV4
	netw_addr.s_addr=Convert::string_to_T<uint32_t>(cfgParser[CFG_MASTER_IP]);
#else
   #error "INET_IPV4 not defined"
#endif
	http_port=Convert::string_to_T<uint16_t>(cfgParser[CFG_MASTER_PORT]);
	conn_num=Convert::string_to_T<int>(cfgParser[CFG_CONN_NUM]);
	if(conn_num>MAX_CONN_NUM){
		cerr<<"error, MasterHttpConnector::init, conn_num to large"<<"\n";
		exit(-1);
	}
	return 0;
}

MasterHttpConnector::MasterHttpConnector(){
	init();
}

void MasterHttpConnector::startToListen(){

}


