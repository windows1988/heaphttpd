/*
	Copyright (c) openheap, uplusware
	uplusware@gmail.com
*/

#ifndef _HTTP_H_
#define _HTTP_H_

#include "httpcomm.h"
#include "formdata.h"
#include "cache.h"
#include "webcgi.h"
#include "wwwauth.h"
#include "serviceobj.h"

enum WebSocket_HandShake
{
    Websocket_None = 0,
    Websocket_Sync,
    Websocket_Ack
};

enum Http_Connection
{
    httpContinue = 0,
    httpKeepAlive,
    httpClose,
	httpWebSocket
};

class CHttp
{
public:
	CHttp(ServiceObjMap* srvobj, int sockfd, const char* servername, unsigned short serverport, const char* clientip, memory_cache* ch,
		const char* work_path, const char* php_mode, const char* fpm_addr, unsigned short fpm_port, const char* phpcgi_path, const char* private_path, unsigned int global_uid, AUTH_SCHEME wwwauth_scheme = asNone,
		BOOL isSSL = FALSE, const char* ca_crt_root = NULL, const char* ca_crt_server = NULL, const char* ca_password = NULL, const char* ca_key_server = NULL, BOOL enableclientcacheck = FALSE);
	virtual ~CHttp();

	virtual Http_Connection LineParse(char* text);
	virtual int ProtRecv(char* buf, int len);
		

	int HttpSend(const char* buf, int len);
	int HttpRecv(char* buf, int len);
	
	void SetMetaVarsToEnv();
	void SetMetaVar(const char* name, const char* val);
	void WriteDataToCGI(int fd);
    void ReadDataFromCGI(int fd, string& strResponse);
	
	int parse_multipart_value(const char* szKey, fbufseg & seg);
	int parse_multipart_filename(const char* szKey, string& filename);
	int parse_multipart_type(const char* szKey, string& type);

	
	const char* GetResource() { return m_resource.c_str(); }
	const char* GetUserName() { return m_username.c_str(); }
	const char* GetPassword() { return m_password.c_str(); }
	
	formdata* GetFormData() { return m_formdata; }

	void SetResource(const char* resource)
	{ 
		m_resource = resource;
	}

	memory_cache* m_cache;

	string m_servername;
	unsigned short m_serverport;
	string m_clientip;
	
	Http_Method GetMethod()
	{
		return m_http_method;
	}
	
	const char* GetCGIPgmName()
	{
		return m_cgi_pgm.c_str();
	}
	
    const char* GetRequestField(const char* name)
    {
        return m_request_hdr.GetField(name);
    }
    
    void GetRequestField(const char* name, int & val)
    {
        m_request_hdr.GetField(name, val);
    }
    
    map<string, string> _POST_VARS_; /* var in post data */
	map<string, string> _GET_VARS_; /* var in query string */
    map<string, string> _COOKIE_VARS_; /* var in cookie */
    
	BOOL IsPassedWWWAuth() { return m_passed_wwwauth; }
	AUTH_SCHEME GetWWWAuthScheme() { return m_wwwauth_scheme; };
    
    WebSocket_HandShake GetWebSocketHandShake() { return m_web_socket_handshake; }
	void SetWebSocketHandShake(WebSocket_HandShake shake) { m_web_socket_handshake = shake;}
	
	int GetSocket() { return m_sockfd; }
    SSL* GetSSL() { return m_isSSL ? m_ssl : NULL; }
    void SetServiceObject(const char * objname, SERVICE_OBJECT_HANDLE objptr);
    void* GetServiceObject(const char* objname);
	WebCGI m_cgi;
protected:
	BOOL m_isSSL;
	SSL* m_ssl;
	SSL_CTX* m_ssl_ctx;
	
	int m_sockfd;
	linesock* m_lsockfd;
	linessl * m_lssl;

	Http_Method m_http_method;
	string m_resource;
	
	string m_querystring;
	string m_postdata;
	
	fbuffer* m_postdata_ex; /* huge size post data */
	formdata* m_formdata; /* For upload files */
	
	unsigned int m_content_length;

	Post_Content_Type m_content_type;
	string m_boundary;

	string m_host_addr;
	unsigned short m_host_port;

	string m_username;
	string m_password;

	string m_cookie;
    
    string m_work_path;
	string m_php_mode;
	string m_fpm_addr;
	unsigned short m_fpm_port;
    string m_phpcgi_path;
    string m_private_path;
    unsigned int m_global_uid;
    
	string m_ca_crt_root;
	string m_ca_crt_server;
	string m_ca_password;
	string m_ca_key_server;
	BOOL m_enableclientcacheck;
    
    CHttpRequestHdr m_request_hdr;
	string m_cgi_pgm;
	
	BOOL m_passed_wwwauth;
	AUTH_SCHEME m_wwwauth_scheme;
    
    BOOL m_keep_alive;
    WebSocket_HandShake m_web_socket_handshake;
    ServiceObjMap* m_srvobj;
};

#endif /* _HTTP_H_ */

