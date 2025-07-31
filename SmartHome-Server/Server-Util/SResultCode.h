#ifndef SRESULTCODE_H_
#define SRESULTCODE_H_

#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonParseError>
#include<QJsonArray>
#include<QJsonValue>


class SResultCode
{
	struct Code
	{
		int  code;
		const char* codeStr;
		const char* message;

		Code(int code, const char* codeStr, const char* message)
			:code(code), codeStr(codeStr), message(message)
		{

		}
		void addCodeToObj(QJsonObject&obj)const
		{
			obj.insert("code", code);
			obj.insert("message", message);
		}
		operator QJsonValue()const
		{
			return code;
		}
	};
public:
#define CODE(name,value,message) inline static Code name{value,#name,message}


	CODE(Success, 0, "成功");
	CODE(SuccessButNotData, 1, "成功但没有操作数据");
	//参数错误 1000-1999
	CODE(ParamJsonInvalid, 1000, "请求参数Json无效");
	CODE(ParamLoss, 1001, "缺少必要参数");
	CODE(ParamParseError, 1002, "参数解析错误");
	//服务器错误  2000-2999
	CODE(ServerSqlQueryError, 2000, "Sql执行错误");
	CODE(ServerSqlInnerError, 2001, "服务器内部错误");
	CODE(ServerResourceNotFound, 2002, "资源未找到");
	//用户错误
	CODE(UserAccountDisable, 3000, "用户已被禁用");
	CODE(UserLoginError, 3001, "帐号或密码错误");
	CODE(UserAccountExists, 3002, "用户已存在");
	CODE(UserUnanthorized, 3003, "未认证的用户，请先登录");
	CODE(UserAuthFormatError, 3004, "认证格式错误,仅支持Bearer方法");
	CODE(UserAuthTokenInvaild, 3005, "token无效");
	CODE(UserAuthTokenExpired, 3006, "token已过期");
	CODE(UserConfidentialError, 3007, "用户账号或密保错误");
	//其他错误


	friend class SResult;
};

class SResult
{
public:
	static void addSuccessCode(QJsonObject& obj, const SResultCode::Code& code)
	{
		obj.insert("code",code.code);
		obj.insert("message", code.message);
	}

	static void addFailuresCode(QJsonObject& obj, const SResultCode::Code& code)
	{
		obj.insert("code", code.code);
		obj.insert("message", code.message);
	}

};

#endif // !SRESULTCODE_H_
