#ifndef __LoginModel__
#define __LoginModel__
#include "cocos2d.h"
#include "MVCFrame.h"
#include "json/document.h"

class retModel : public mvc::Model
{
public:
	DEFINE_SIGANL_INSTANCE(retModel);
	
	 retModel();
	 ~retModel();
	void  initModel();
private:
public:
    int state;
	std::string token;
	std::string model;
	std::string account;
};

class LoginModel : public mvc::Model
{
public:
	DEFINE_SIGANL_INSTANCE(LoginModel);
	
	 LoginModel();
	 ~LoginModel();
	void  initModel();
	void assignComplete(std::string notice);
private:
public:
	retModel* ret;
	std::string _print_content;
    int is_actived;
	int is_true;
};

class Login_indexProtocol :public mvc::Protocol
{
public:
	Login_indexProtocol();
	~Login_indexProtocol();
	CREATE_PROTOCOL(Login_indexProtocol);
	 virtual bool init();
	void virtual parser(char* response);
private:
	LoginModel* model;
};
class Login_checkActiveProtocol :public mvc::Protocol
{
public:
	Login_checkActiveProtocol();
	~Login_checkActiveProtocol();
	CREATE_PROTOCOL(Login_checkActiveProtocol);
	 virtual bool init();
	void virtual parser(char* response);
private:
	LoginModel* model;
};
class Login_doActiveProtocol :public mvc::Protocol
{
public:
	Login_doActiveProtocol();
	~Login_doActiveProtocol();
	CREATE_PROTOCOL(Login_doActiveProtocol);
	 virtual bool init();
	void virtual parser(char* response);
private:
	LoginModel* model;
};
#endif