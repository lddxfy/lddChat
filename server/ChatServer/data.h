#pragma once

struct Userinfo {
	Userinfo() :name(""), pwd(""), id(0), email(""), nick(""), desc(""), sex(0), icon(""), back("") {}
	Userinfo(const Userinfo&) = default;
	Userinfo& operator=(const Userinfo&) = default;
	std::string name;
	std::string pwd;
	int id;
	std::string email;
	std::string nick;
	std::string desc;
	int sex;
	std::string icon;
	std::string back;
};

struct ApplyInfo {
	ApplyInfo(int uid, std::string name, std::string desc,
		std::string icon, std::string nick, int sex, int status)
		:_uid(uid), _name(name), _desc(desc),
		_icon(icon), _nick(nick), _sex(sex), _status(status) {}

	int _uid;
	std::string _name;
	std::string _desc;
	std::string _icon;
	std::string _nick;
	int _sex;
	int _status;
};