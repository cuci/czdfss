/*
 * ConfigureParser.cpp
 *
 *  Created on: 2015年5月5日
 *      Author: root
 */

#include"ConfigureParser.h"
#include<string>
#include<fstream>
#include<sys/types.h>
#include<regexp.h>

#define MAX_PARAM_LEN 1024
#define RESET_REGEXP_RES(r) do{r.rm_eo=-1; r.rm_so=-1;} while(0)

using namespace std;

inline void DELETE_SPACE(string& line){
	int t;
	if((t=line.find_first_not_of("\t "))==line.npos){
		return;
	}
	line.erase(0,t);
}

// default configure file path: /etc/dfss_configure/.init
ConfigureParser::ConfigureParser(const string cfg_file)
{
	inFile.open(cfg_file.c_str(),ios::in);
	if(inFile.fail()){
		cerr<<"ConfigureParser: error, fails to open configure file\n";
		exit(-1);
	}
	parameters["Master IP"]="default";
	parameters["Master PORT"]="80";
	parameters["Root Dir"]="/usr/share/czdfss";
	parameters["Connection #"]="100";
	extractValues();
}

void ConfigureParser::deleteComment(string& line){
	int pos=line.find('#');
	if(pos!=line.npos)
		line.erase(pos);
}
// after deleteComment and empty_check
bool ConfigureParser::isValidLine(string &line){
	DELETE_SPACE(line);
	// empty line
	if(line.empty()) return true;
	regex_t reg;
	regmatch_t res[1];
	RESET_REGEXP_RES(res[0]);
	// check parameter name
	regcomp(&reg,"^[a-zA-Z0-9]+ [a-zA-Z0-9]+",REG_EXTENDED);
	regexec(&reg,line.c_str(),1,res,0);
	if(res[0].rm_eo==-1){
		return false;
	}
	int explen=res[0].rm_eo-res[0].rm_so;
	if(explen>MAX_PARAM_LEN){
		return false;
	}
	char buf[MAX_PARAM_LEN+1];
	strncmp(line.substr(res[0].rm_so,explen).c_str(),buf,explen);
	buf[explen]='\0';
	if(parameters.find(buf)==parameters.end()){
		cerr<<"isValidLine: error, unkown parameter name (check whether you enter too many spaces or tables characters)---"<<buf<<"\n";
		return false;
	}
	line.erase(0,explen);
	if(line.empty()){
		cerr<<"isValidLine: error, no '=' and value"<<"\n";
		return false;
	}
	// check operator '='
	RESET_REGEXP_RES(res[0]);
	regcomp(&reg,"[ \t]*=",REG_EXTENDED);
	regexec(&reg,line.c_str(),1,res,0);
	if(res[0].rm_eo==-1){
		cerr<<"isValidLine: error, find no '='\n";
		return false;
	}
	explen=res[0].rm_eo-res[0].rm_so;
	line.erase(res[0].rm_so,explen);
	DELETE_SPACE(line);
	if(line.empty()){
		cerr<<"isValidLine: error, find no value\n";
		return false;
	}
	RESET_REGEXP_RES(res[0]);
	// check values
	regcomp(&reg,"^[a-zA-Z0-9/~]+",REG_EXTENDED);
	regexec(&reg,line.c_str(),1,res,0);
	if(res[0].rm_eo==-1){
		cerr<<"isValidLine: error, find no value\n";
		return false;
	}
	explen=res[0].rm_eo-res[0].rm_so;
	strncmp(line.substr(res[0].rm_so,explen).c_str(),buf,explen);
	buf[explen]='\0';
	if(res[0].rm_eo==line.length()){
		parameters[buf]=line.substr(res[0].rm_so,explen);
		return true;
	}
	if(line.find_first_not_of("\t ",res[0].rm_eo,line.length()-res[0].rm_eo)!=line.npos){
		cerr<<"isValidLine: error, find no suitable value\n";
		return false;
	} else{
		parameters[buf]=line.substr(res[0].rm_so,explen);
	}
	return true;
}

void ConfigureParser::extractValues(void){
	string line;
	while(!std::getline(inFile,line) || !inFile.eof()){
		deleteComment(line);
		if(!isValidLine(line)){
			inFile.close();
			exit(-1);
		}
	}
}

const string& ConfigureParser::operator[](std::string key){
		return parameters[key];
}

ConfigureParser::~ConfigureParser(){}





