/*
 * masterhttpconnector.cpp
 *
 *  Created on: 2015年5月3日
 *      Author: root
 */
#include"abstractmasterconnector.h"
#include"masterhttpconnector.h"
#include"common.h"
#include<fstream>
#include<cstring>
#include<iostream>
#include<net/if.h>
#include<sys/ioctl.h>
#include<unistd.h>

#define INET_IPV4

#ifdef INET_IPV4
#define INET_VERSION AF_INET
#define SOCK_ADDR_VERSION struct sockaddr_in
#define PRESENT_ADDRSTRLEN INET_ADDRSTRLEN
#define IN_ADDR_TYPE struct in_addr
//#define SIN_ADDR sin_addr
#endif

int get_host_address(IN_ADDR_TYPE & dest){
	int s;
	s=socket(INET_VERSION,SOCK_STREAM,0);
	if(s<0){
		std::cerr<<"get_host_address: error, open socket \n";
		return -1;
	}
	// stores the network configure info
	struct ifconf ifconf;
	//stores the configure of each inet card interface
	struct ifreq ifr[50];
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

int MasterHttpConnector::init(){
	http_port=htons(80);

	std::ifstream cfgFile;
	cfgFile.open("/etc/czdfss.configure",std::ios::in);
	if(cfgFile.fail()){
		std::cout<<"init error, fails to open the configure file"<<std::endl;
		return -1;
	}
	// temp buffer to save a configure file line
	const unsigned line_size=1024;
	char buf[line_size+1];
	while(!cfgFile.eof()){
		cfgFile.getline(buf,line_size);
		for(int i=0;i<strlen(buf);i++){
			if(isalpha(buf[i]==0)){
				if(buf[i]=='#') break;
				continue;
			}
		}
	}
}

MasterHttpConnector::MasterHttpConnector(){
	init();
}


