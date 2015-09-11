/*
	Copyright (c) openheap, uplusware
	uplusware@gmail.com
*/
#include <stdio.h>
#include "sample.h"

//API sample
void ApiSample::Response()
{
    CHttpResponseHdr header;
	string strResp;
	string abc, def;
    abc = m_session->_POST_VARS_["abc"];
    def = m_session->_POST_VARS_["def"];
	if( abc == "" || def == "" )
	{
       
        header.SetStatusCode(SC200);
		header.SetField("Content-Type", "text/html");
		strResp = "<html></head><title>API Sample</title></head><body>API Sample: couldn't get the corresponding POST data</body></html>";
        header.SetField("Content-Length", strResp.length());
	}
	else
	{    
		int a = atoi(abc.c_str());
		int d = atoi(def.c_str());
		int sum = a + d;
		
		char szTmp[64];
		sprintf(szTmp, "%d", sum);
        
		header.SetStatusCode(SC200);
		header.SetField("Content-Type", "text/html");
		header.SetField("Set-Cookie", m_session->m_clientip.c_str());
	
		header.Text();
        
		strResp = "<html></head><title>API Sample</title></head><body><h1>niuhttpd web server/0.3</h1>API Sample: ";
		strResp += abc;
		strResp += " + ";
		strResp += def;
		strResp += " = ";
		strResp += szTmp;
		strResp += "</body></html>";
        header.SetField("Content-Length", strResp.length());
	}
    m_session->HttpSend(header.Text(), header.Length());
	m_session->HttpSend(strResp.c_str(), strResp.length());

}

void* api_sample_response(CHttp* session, const char* html_path)
{
	ApiSample *pDoc = new ApiSample(session, html_path);
	pDoc->Response();
	delete pDoc;
}
