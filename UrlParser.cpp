//Implementation of class UrlParser
//Refer to RFC 3986 for implement details: http://tools.ietf.org/html/rfc3986
//
//Author: Tony Zhou

#include <regex>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "UrlParser.h"

using namespace boost;
using namespace HTTPHELPER;
using namespace std;

//Parse URI parts with this regular expression:
//^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?
// 12            3  4          5       6  7        8 9
//
//	scheme 		= $2
//	authority 	= $4
//	path		= $5
//	query		= $7
//	fragment	= $9
regex urlRegExp("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?");

UrlParser::UrlParser(const string & url) :
	schema_("http"),
	host_(),
	port_(80),
	path_(),
	queryParams_(),
	fragment_()
{
	parse(url);
}

void UrlParser::parse(const string & url)
{
	cmatch what;
	//Parse URI parts with regex_match function, regex utility is included in C++11 standard
	//check the definition of urlRegExp for detailed info 
	if(regex_search(url.c_str(), what, urlRegExp))
	{
		valid_ = true;
		
		schema_ = what[2].second;
		parseAuthorityString(what[4].second, host_, &port_);
		path_ = what[5].second;
		query_ = what[7].second;
		parseQueryParams(what[7].second, queryParams_);
		fragment_ = what[9].second;
	}
	else
	{
		valid_ = false;
	}
}

void UrlParser::parseAuthorityString(const string & authorityStr, string & host, unsigned int * portNo)
{
	int positionOfAmp = authorityStr.find("&");
	host = authorityStr.substr(0, positionOfAmp );
	if(positionOfAmp != string::npos)
	{
		string portStr = authorityStr.substr(positionOfAmp + 1);
		*portNo = atoi(portStr.c_str());
	}	
}

void UrlParser::parseQueryParams(const string & queryParams, QueryParamList & paramList)
{
	vector<string> strs;
	boost::algorithm::split(strs, queryParams, boost::is_any_of("&"));
	for(vector<string>::iterator iter = strs.begin(); iter != strs.end(); ++iter)
	{
		string key, value;
		int positionOfEquals = iter->find("=");
		key = iter->substr(0, positionOfEquals );
		if(positionOfEquals != string::npos)
		  value = iter->substr(positionOfEquals + 1);
		  
		paramList.insert(std::pair<string, string>(key, value));
	}
}

string UrlParser::authority() const
{
	stringstream s;
	s<<host_<<":"<<port_;
	return s.str();
}
