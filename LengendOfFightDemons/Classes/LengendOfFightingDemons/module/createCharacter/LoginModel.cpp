#include "LoginModel.h"

CREATE_SIGNAL_INSTANCE(LoginModel,LoginModelInstance);

LoginModel::LoginModel()
{
	initModel();
}

LoginModel::~LoginModel()
{
}

void LoginModel::initModel()
{
	ret = new retModel();
}

void LoginModel::assignComplete(std::string notice)
{
	publishNotice(notice);
}

CREATE_SIGNAL_INSTANCE(retModel,retModelInstance);

retModel::retModel()
{
	initModel();
}

retModel::~retModel()
{
}

void retModel::initModel()
{
}

bool Login_indexProtocol::init()
{
	if(!mvc::Protocol::init())return false;
	return true;
}

Login_indexProtocol::Login_indexProtocol()
{
	model = LoginModel::getInstance();
}

Login_indexProtocol::~Login_indexProtocol()
{

}

void Login_indexProtocol::parser(char* response)
{

	rapidjson::Document doc;
	doc.Parse<0>(response);
	if(doc.HasParseError())
	{
		CCLOG("GetParserError %s\n",doc.GetParseError());
		return;
	}
	model->ret->state = doc["data"]["ret"]["state"].GetInt();
	model->ret->token = doc["data"]["ret"]["token"].GetString();
	model->ret->model = doc["data"]["ret"]["model"].GetString();
	model->ret->account = doc["data"]["ret"]["account"].GetString();
	model->_print_content = doc["data"]["_print_content"].GetString();

	model->assignComplete("Login_indexDataInit");
}

bool Login_checkActiveProtocol::init()
{
	if(!mvc::Protocol::init())return false;
	return true;
}

Login_checkActiveProtocol::Login_checkActiveProtocol()
{
	model = LoginModel::getInstance();
}

Login_checkActiveProtocol::~Login_checkActiveProtocol()
{

}

void Login_checkActiveProtocol::parser(char* response)
{

	rapidjson::Document doc;
	doc.Parse<0>(response);
	if(doc.HasParseError())
	{
		CCLOG("GetParserError %s\n",doc.GetParseError());
		return;
	}
	model->_print_content = doc["data"]["_print_content"].GetString();
	model->is_actived = doc["data"]["is_actived"].GetInt();

	model->assignComplete("Login_checkActiveDataInit");
}

bool Login_doActiveProtocol::init()
{
	if(!mvc::Protocol::init())return false;
	return true;
}

Login_doActiveProtocol::Login_doActiveProtocol()
{
	model = LoginModel::getInstance();
}

Login_doActiveProtocol::~Login_doActiveProtocol()
{

}

void Login_doActiveProtocol::parser(char* response)
{

	rapidjson::Document doc;
	doc.Parse<0>(response);
	if(doc.HasParseError())
	{
		CCLOG("GetParserError %s\n",doc.GetParseError());
		return;
	}
	model->_print_content = doc["data"]["_print_content"].GetString();
	model->is_true = doc["data"]["is_true"].GetInt();

	model->assignComplete("Login_doActiveDataInit");
}

