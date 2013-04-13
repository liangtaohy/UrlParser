//UrlParser : a url parsing class
//Function : break a complete url into scheme, host, port, path, query params and fragment
//Refer to RFC 3986 for implementation details: http://tools.ietf.org/html/rfc3986
//
//Author: Tony Zhou

#ifndef _H_HTTPHELPER_URLPARSER_H_
#define _H_HTTPHELPER_URLPARSER_H_

#include <iostream>
#include <string>
#include <map>
#include <boost/noncopyable.hpp>

namespace HTTPHELPER
{

using std::string;
using std::map;

//
//UrlParser: A URL parsing class
//
class UrlParser : boost::noncopyable
{
public:
	UrlParser(const string & url);
	~UrlParser()
	{  
	}
	
	bool valid() const { return valid_; }
	string schema() const { return schema_; }
	string host() const{ return host_; }
	unsigned int port() const { return port_; }
	string authority() const;
	string path() const { return path_; }
	string query() const { return query_; }
	string getParamValue(const string & field) const { return queryParams_.at(field); }
	string fragment() const { return fragment_; };
protected:
	
private:
	typedef map<string, string> QueryParamList;

	void parse(const string & url);
	void parseAuthorityString(const string & authorityStr, string & host, unsigned int * portNo);
	void parseQueryParams(const string & queryParams, QueryParamList & paramList);

	bool valid_;
	string schema_;
	string host_;
	unsigned int port_;
	string path_;
	QueryParamList queryParams_;
	string query_;
	string fragment_;
};

}

#endif