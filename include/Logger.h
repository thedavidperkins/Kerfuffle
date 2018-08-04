#ifndef KERF_LOGGER_H
#define KERF_LOGGER_H

#include <string>
#include <fstream>

#define LOG Log::getInstance()

class Log {
public:
	static Log& getInstance() {
		static Log inst;
		return inst;
	}

	void openFile(const std::string& name) {
		m_strm.open(name);
	}

	void closeFile() {
		m_strm.close();
	}

	void operator()(const std::string& toLog) {
		if (m_strm.is_open()) m_strm << toLog << std::endl;
	}
private:
	Log() : m_strm() {}

	std::ofstream m_strm;
};

#endif//KERF_LOGGER_H
