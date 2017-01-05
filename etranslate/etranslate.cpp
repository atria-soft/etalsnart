/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etranslate/debug.hpp>
#include <etranslate/etranslate.hpp>
#include <map>
#include <etk/os/FSNode.hpp>
#include <ejson/ejson.hpp>

class LocalInstanceTranslation {
	private:
		std::map<std::string,std::string> m_listPath;
		std::string m_major;
		std::string m_languageDefault;
		std::string m_language;
		bool m_translateLoadad;
		std::map<std::string,std::string> m_translate;
	public:
		LocalInstanceTranslation() :
		  m_major("etranslate"),
		  m_languageDefault("EN"),
		  m_language(""),
		  m_translateLoadad(false) {
			// nothing to do ...
		}
	public:
		void addPath(const std::string& _lib, const std::string& _path, bool _major) {
			auto it = m_listPath.find(_lib);
			if (it == m_listPath.end()) {
				m_listPath.insert(make_pair(_lib, _path));
			} else {
				it->second = _path;
			}
			if (_major == true) {
				m_major = _lib;
				ETRANSLATE_INFO("Change major translation : '" << m_major << "'");
			}
			m_translateLoadad = false;
			m_translate.clear();
		};
		
		const std::string& getPaths(const std::string& _lib) {
			auto it = m_listPath.find(_lib);
			if (it == m_listPath.end()) {
				static const std::string g_error("");
				return g_error;
			}
			return it->second;
		};
		
		void setLanguageDefault(const std::string& _lang) {
			if (m_languageDefault == _lang) {
				return;
			}
			ETRANSLATE_INFO("Change default language translation : '" << _lang << "'");
			m_languageDefault = _lang;
			m_translateLoadad = false;
			m_translate.clear();
		};
		
		const std::string& getLanguageDefault() {
			return m_languageDefault;
		};
		
		void setLanguage(const std::string& _lang) {
			if (m_language == _lang) {
				return;
			}
			m_language = _lang;
			m_translateLoadad = false;
			m_translate.clear();
			if (_lang == "EN") {
				ETRANSLATE_INFO("Change language translation: '" << _lang << "'=English");
			} else if (_lang == "FR") {
				ETRANSLATE_INFO("Change language translation: '" << _lang << "'=French");
			} else if (_lang == "DE") {
				ETRANSLATE_INFO("Change language translation: '" << _lang << "'=German");
			} else if (_lang == "SP") {
				ETRANSLATE_INFO("Change language translation: '" << _lang << "'=Spanish");
			} else if (_lang == "JA") {
				ETRANSLATE_INFO("Change language translation: '" << _lang << "'=Japanese");
			} else if (_lang == "IT") {
				ETRANSLATE_INFO("Change language translation: '" << _lang << "'=Italian");
			} else if (_lang == "KO") {
				ETRANSLATE_INFO("Change language translation: '" << _lang << "'=Korean");
			} else if (_lang == "RU") {
				ETRANSLATE_INFO("Change language translation: '" << _lang << "'=Russian");
			} else if (_lang == "PT") {
				ETRANSLATE_INFO("Change language translation: '" << _lang << "'=Portuguese, Brazilian");
			} else if (_lang == "ZH") {
				ETRANSLATE_INFO("Change language translation: '" << _lang << "'=Chinese");
			} else {
				ETRANSLATE_INFO("Change language translation: '" << _lang << "'=Unknow");
			}
		};
		
		const std::string& getLanguage() {
			return m_language;
		};
		
		std::string get(const std::string& _instance) {
			loadTranslation();
			ETRANSLATE_VERBOSE("Request translate: '" << _instance << "'");
			// find all iterance of '_T{' ... '}'
			std::string out;
			auto itOld = _instance.begin();
			size_t pos = _instance.find("_T{");
			while (pos != std::string::npos) {
				out.append(itOld, _instance.begin() + pos);
				auto it = _instance.begin() + pos + 3;
				itOld = it;
				pos = _instance.find("}", pos);
				if (pos == std::string::npos) {
					ETRANSLATE_WARNING("missing end translation '}' in: '" << _instance << "'");
					it = _instance.end();
				} else {
					it = _instance.begin() + pos;
				}
				std::string basicEmptyValue = std::string(itOld, it);
				auto itTranslate = m_translate.find(basicEmptyValue);
				if (itTranslate == m_translate.end()) {
					ETRANSLATE_DEBUG("Can not find tranlation : '" << _instance << "'");
					out += basicEmptyValue;
				} else {
					out += itTranslate->second;
				}
				if (it != _instance.end()) {
					itOld = it+1;
				} else {
					itOld = it;
				}
				pos = _instance.find("_T{", pos);
			}
			if (itOld != _instance.end()) {
				out.append(itOld, _instance.end());
			}
			ETRANSLATE_VERBOSE("      translation: '" << out << "'");
			return out;
		};
	private:
		void loadTranslation() {
			if (m_translateLoadad == true) {
				return;
			}
			ETRANSLATE_VERBOSE("Load Translation MAJOR='" << m_major << "' LANG='" << m_language << "' default=" << m_languageDefault );
			// start parse language for Major:
			auto itMajor = m_listPath.find(m_major);
			if (itMajor != m_listPath.end()) {
				std::string filename(itMajor->second + "/" + m_language + ".json");
				ejson::Document doc;
				doc.load(filename);
				for (auto element : doc.getKeys()) {
					std::string val = doc[element].toString().get();
					m_translate.insert(make_pair(element, val));
				}
				filename = itMajor->second + "/" + m_languageDefault + ".json";
				doc.load(filename);
				for (auto element : doc.getKeys()) {
					std::string val = doc[element].toString().get();
					auto itTrans = m_translate.find(element);
					if (itTrans == m_translate.end()) {
						m_translate.insert(make_pair(element, val));
					}
				}
			}
			// start parse language:
			for (auto &it : m_listPath) {
				if (it.first == m_major) {
					continue;
				}
				std::string filename(it.second + "/" + m_languageDefault + ".json");
				if (etk::FSNodeExist(filename) == false) {
					continue;
				}
				ejson::Document doc;
				doc.load(filename);
				for (auto element : doc.getKeys()) {
					std::string val = doc[element].toString().get();
					auto itTrans = m_translate.find(element);
					if (itTrans == m_translate.end()) {
						m_translate.insert(make_pair(element, val));
					}
				}
			}
			// start parse default language:
			for (auto &it : m_listPath) {
				if (it.first == m_major) {
					continue;
				}
				std::string filename(it.second + "/" + m_languageDefault + ".json");
				if (etk::FSNodeExist(filename) == false) {
					continue;
				}
				ejson::Document doc;
				doc.load(filename);
				for (auto element : doc.getKeys()) {
					std::string val = doc[element].toString().get();
					auto itTrans = m_translate.find(element);
					if (itTrans == m_translate.end()) {
						m_translate.insert(make_pair(element, val));
					}
				}
			}
		}
		
};

static LocalInstanceTranslation& getInstanceTranslation() {
	static LocalInstanceTranslation g_val;
	return g_val;
}
static int32_t g_isInit = 0;

void etranslate::init(int _argc, const char** _argv) {
	if (g_isInit > 0) {
		g_isInit++;
		return;
	}
	ETRANSLATE_INFO("E-translate system init");
	for (int32_t iii=0; iii<_argc ; ++iii) {
		std::string data = _argv[iii];
		if (    data == "-h"
		     || data == "--help") {
			ETRANSLATE_PRINT("e-translate - help : ");
			ETRANSLATE_PRINT("        -h/--help:               Dispplay this help");
		} else if (etk::start_with(data, "--etranslate") == true) {
			ETRANSLATE_ERROR("Can not parse the argument : '" << data << "'");
		}
	}
	g_isInit++;
}

void etranslate::unInit() {
	if (g_isInit >= 1) {
		g_isInit--;
		return;
	}
	if (g_isInit == 1) {
		ETRANSLATE_INFO("E-translate system un-init");
		g_isInit--;
		return;
	}
	ETRANSLATE_ERROR("E-translate system un-init (already done before)");
}

void etranslate::addPath(const std::string& _lib, const std::string& _path, bool _major) {
	if (g_isInit <= 0) {
		ETRANSLATE_ERROR("E-translate system has not been init");
	}
	getInstanceTranslation().addPath(_lib, _path, _major);
}

const std::string& etranslate::getPaths(const std::string& _lib) {
	if (g_isInit <= 0) {
		ETRANSLATE_ERROR("E-translate system has not been init");
	}
	return getInstanceTranslation().getPaths(_lib);
}

void etranslate::setLanguageDefault(const std::string& _lang) {
	if (g_isInit <= 0) {
		ETRANSLATE_ERROR("E-translate system has not been init");
	}
	getInstanceTranslation().setLanguageDefault(_lang);
}

const std::string& etranslate::getLanguageDefault() {
	if (g_isInit <= 0) {
		ETRANSLATE_ERROR("E-translate system has not been init");
	}
	return getInstanceTranslation().getLanguageDefault();
}

void etranslate::setLanguage(const std::string& _lang) {
	if (g_isInit <= 0) {
		ETRANSLATE_ERROR("E-translate system has not been init");
	}
	getInstanceTranslation().setLanguage(_lang);
}

const std::string& etranslate::getLanguage() {
	if (g_isInit <= 0) {
		ETRANSLATE_ERROR("E-translate system has not been init");
	}
	return getInstanceTranslation().getLanguage();
}

void etranslate::autoDetectLanguage() {
	if (g_isInit <= 0) {
		ETRANSLATE_ERROR("E-translate system has not been init");
	}
	ETRANSLATE_VERBOSE("Auto-detect language of system");
	std::string nonameLocalName;
	std::string userLocalName;
	std::string globalLocalName;
	try {
		nonameLocalName = std::locale(std::locale(), new std::ctype<char>).name();
		userLocalName = std::locale("").name();
		globalLocalName = std::locale().name();
		ETRANSLATE_VERBOSE("    The default locale is '" << globalLocalName << "'");
		ETRANSLATE_VERBOSE("    The user's locale is '" << userLocalName << "'");
		ETRANSLATE_VERBOSE("    A nameless locale is '" << nonameLocalName << "'");
	} catch (std::runtime_error e) {
		ETRANSLATE_ERROR("Can not get Locals ==> set English ...");
		nonameLocalName = "EN";
		userLocalName = "EN";
		globalLocalName = "EN";
	}
	std::string lang = nonameLocalName;
	if (    lang == "*"
	     || lang == "") {
		lang = userLocalName;
	}
	if (    lang == "*"
	     || lang == "") {
		lang = globalLocalName;
	}
	if (    lang == "C"
	     || lang == ""
	     || lang.size() < 2) {
		lang = "EN";
	}
	lang = std::string(lang.begin(), lang.begin()+2);
	lang = etk::toupper(lang);
	ETRANSLATE_INFO("Select Language : '" << lang << "'");
	getInstanceTranslation().setLanguage(lang);
	return;
	// dead code ...
	#if defined(__TARGET_OS__Linux)
		char *s = getenv("LANG");
		if (s == nullptr || strlen(s) < 2) {
			ETRANSLATE_INFO("Try to determine system language FAIL ...");
		} else {
			std::string lang;
			lang += s[0];
			lang += s[1];
			lang = etk::toupper(lang);
			getInstanceTranslation().setLanguage(lang);
		}
	#else
		ETRANSLATE_INFO("Can not auto-detect language ...");
		getInstanceTranslation().setLanguage("EN");
	#endif
}

std::string etranslate::get(const std::string& _instance) {
	return getInstanceTranslation().get(_instance);
}


