#pragma once
#include <string>
#include <vector>
#include <json/json.h>

#include "ApiPoolError.h"
#include "ApiPoolResponse.h"

namespace ApiPool
{

class Connector;
class Request;

class ApiPool
{
public:
    ApiPool();
    ~ApiPool();

private:
    ApiPool(const ApiPool&);
    ApiPool& operator = (const ApiPool&);

public:
    /**
    @brief 
    @fn
    @param request_src ������Դ
    @param service_name ��������
    @param api_name ����Api�ӿ���
    @param param_list �����б�
    @param json_response ���󷵻� json��ʽ
    @return ������
    */
    Response request(const std::string& method_name,
		             const Json::Value& params,
                     unsigned int time_out = UINT_MAX);

    /**
    @brief 
    @fn 
    @param request_src ������Դ
    @param service_name ��������
    @param api_name ����Api�ӿ���
    @param param_list �����б�
    @return ������
    */
    void notify(const std::string& method_name,
                const Json::Value& params,
                unsigned int time_out = UINT_MAX);

    /**
    */
    bool keepAlive();

    Response request2(const std::string& method_name,
        const Json::Value& params,
        unsigned int time_out = UINT_MAX,
        Request* pReq = NULL);

    Response rawRequest(const std::string& req, 
                        std::string& res, 
                        unsigned int time_out);

    void rawNotify(const std::string& req, 
                   unsigned int time_out);

public:
    void setServer(const std::string& server, short port, bool long_connect = false);

    void setNofify(bool bTrue);

public:
    static std::string getRequestSrc();

    static void setRequestSrc(const std::string& request_src);

private:
    Connector* connector_;

private:
    static std::string request_src_;
};

}; //namespace ApiPool
