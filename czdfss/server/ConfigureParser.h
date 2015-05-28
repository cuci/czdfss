/*
 * configureparser.h
 *
 *  Created on: 2015年5月5日
 *      Author: root
 */

#ifndef CONFIGUREPARSER_H_
#define CONFIGUREPARSER_H_
#include<string>
#include<fstream>
#include<iostream>
#include<map>
#include<sstream>

//using namespace std;
//reference: www.dreamincode.net/forums/topic/183191-create-a-simple-configuration-file-parser

class Convert{
public:
	template<typename T>
	static std::string T_to_string(const T &val){
		std::ostringstream ostr;
		ostr<<val;
		return ostr.str();
	}
	template<typename T>
	static T string_to_T(const std::string& val){
		std::istringstream istr(val);
		T rVal;
		if(!(istr>>rVal)){
			std::cerr<<"string_to_T: error\n";
		}
		return rVal;
	}
};

class ConfigureParser{
private:
	std::map<std::string,std::string> parameters;
	std::string fName;
	std::ifstream inFile;
public:
	ConfigureParser(const std::string cfg_file);
	~ConfigureParser();
	void deleteComment(std::string& line);

	// check whether the line is valid
	bool isValidLine(std::string &line);
	void extractValues(void);

public:
	const std::string& operator[](std::string key);

};



#endif /* CONFIGUREPARSER_H_ */
