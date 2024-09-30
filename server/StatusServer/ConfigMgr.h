#pragma once
#include "Singleton.h"
#include "const.h"
struct SectionInfo {
	SectionInfo(){}
	~SectionInfo() {
		_section_datas.clear();
	}
	SectionInfo(const SectionInfo& src) {
		this->_section_datas = src._section_datas;
	}

	SectionInfo& operator=(const SectionInfo& src) {
		if (this == &src) {
			return *this;
		}
		this->_section_datas = src._section_datas;
		return *this;
	}

	std::map<std::string, std::string> _section_datas;

	std::string operator[](const std::string& key) const{
		auto it = _section_datas.find(key);
		if (it != _section_datas.end()) {
			return it->second;
		}
		else {
			return "";
		}
	}
};

class ConfigMgr
{
public:
	~ConfigMgr() {
		_config_map.clear();
	}
	SectionInfo operator[](const std::string& section) {
		if (_config_map.find(section) == _config_map.end()) {
			return SectionInfo();
		}
		return _config_map[section];
	}
	static ConfigMgr& GetInstance() {
		static ConfigMgr cfg;
		return cfg;
	}
	
private:
	ConfigMgr();
	// ´æ´¢sectionºÍkey-value¶ÔµÄmap  
	std::map<std::string, SectionInfo> _config_map;
};
