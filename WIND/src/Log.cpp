#include "Log.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/Process.h"
#include "Poco/DateTimeFormatter.h"
#include "CommonFunction.h"
#include <sstream>
#include <algorithm>
#include <cstdarg>

#ifdef WIN32 
#define _LINE_END_	"\r\n"
#else
#define _LINE_END_	"\n"
#endif

#define MAX_LOG_SIZE	1024

void LogSinkFile::Write(LogInfo *logInfo)
{
	try {
		++m_num;
		if (m_num >= 100) {
			m_text += logInfo ->logText;
			m_logFile ->write (m_text);
			m_num = 0;
			m_text.clear ();
		}
		else {
			m_text += (logInfo ->logText + _LINE_END_);
		}
	}catch (Poco::Exception &e) {
		printf ("write log exception = %s\n", e.what());
	}
}


void LogSinkFile::Flush()
{
	if (m_logFile != NULL && m_num > 0) {
		m_logFile ->write(m_text.substr(0, m_text.length() - strlen(_LINE_END_)));
		m_num = 0;
		m_text.clear();
	}
}

std::string LogSinkFile::CreateLogFile(bool bCheckSeqID)
{
	Poco::File path (m_path);
	if ( !path.exists() ) {
		path.createDirectories(); // 创建一个目录（包括父目录）
	}
	Poco::LocalDateTime nowdt;
	std::string fullName;
	std::string szTimeBuffer = Poco::DateTimeFormatter::format(nowdt, "%Y%m%d");
	std::string fileName = m_strFileNameFormat;

	ReplaceString (fileName, "%B", m_name);
	ReplaceString(fileName, "%T", szTimeBuffer);
	ReplaceString(fileName, "%P", m_pid);
	ReplaceString(fileName, "%E", m_suffix);

	if ( m_logSizeLimit > 0 ) {
		Poco::File::FileSize size = 0;
		std::string strSeqID;
		std::stringstream ss1;
		ss1 << m_seqID;
		ss1 >> strSeqID;

		if (bCheckSeqID) {
			int i = 1;
			while (true) {
				std::string tmpName = fullName;
				std::stringstream	tss;
				tss << i;
				tss >> strSeqID;
				ReplaceString(tmpName, "%A", strSeqID);
				Poco::File f = Poco::Path(m_path + "/" + tmpName);
				if ( f.exists() ) {
					size = f.getSize();
					m_seqID = i;
					++i;
				}
				else {
					if (size > m_logSizeLimit) {
						m_seqID = i;
					}
					std::stringstream ss3;
					ss3 << i;
					ss3 >> strSeqID;
				}
			}
		}
		ReplaceString(fileName, "%A", strSeqID);
	}

	fullName = m_path + "/" + fileName;
	Poco::File f = Poco::Path (fullName);
	if ( !f.exists() ) {
		f.createFile();
	}

	return fullName;
}

void LogSinkFile::SetSliceMask (WIND::UInt32 mask)
{
	m_sliceMask = mask;
}


void LogSinkFile::SetSuffix(const std::string suffix)
{
	m_suffix = suffix;
}


bool LogSinkFile::Init()
{
	std::stringstream ss;
	ss << Poco::Process::id();
	ss >> m_pid;

	if (m_logSizeLimit > 0) {
		std::string::size_type pos = 0;
		pos = m_strFileNameFormat.find ("%A", pos);
		if (pos == std::string::npos) {
			m_logSizeLimit = 0;
		}
	}

	if ( !m_strFileNameFormat.empty() ) {
		std::string::size_type pos = 0;
		pos = m_strFileNameFormat.find ("%", pos);
		if (pos == std::string::npos) {
			printf("The log file name is invalid.");
			return false;
		}
		std::string fName = CreateLogFile(m_logSizeLimit > 0);
		if (fName.empty()) {
			printf ("Create log file failed.");
			return false;
		}
		m_logFile = new Poco::LogFile(fName);
	}
	Poco::LocalDateTime nowDT;
	m_lastDay = nowDT.day();

	return m_logFile != NULL;
}


void LogSinkFile::Final()
{
	SafeDelete(m_logFile);
}


bool LogSinkFile::SliceByDate()
{
	Poco::LocalDateTime	ldt;
	if (ldt.day() != m_lastDay) {
		Final();
		m_lastDay = ldt.day();
		m_seqID = 1;
		std::string fileName = CreateLogFile();
		m_logFile = new Poco::LogFile(fileName);
		return true;
	}
	return false;
}


bool LogSinkFile::SliceBySize()
{
	if (m_logFile ->size() > m_logSizeLimit) {
		Final();
		++m_seqID;
		std::string fileName = CreateLogFile();
		m_logFile = new Poco::LogFile (fileName);
		return true;
	}
	return false;
}


void LogSinkFile::CheckSlice()
{
	if (m_sliceMask & LogSliceMask_Day) {
		SliceByDate();
	}
	if ( (m_sliceMask & LogSliceMask_Size) && (m_logSizeLimit > 0) ) {
		SliceBySize();
	}
}


void LogSinkFile::PrepareToFlush(WIND::UInt32 elapse, bool exit)
{
	if (m_lastCheckSliceTimer.Update(elapse) || exit) {
		CheckSlice();
	}
}

Log::Log (): m_stop (true), m_logID(1), m_idAllocater(1)
{
	SetInterval(200);
}


Log::~Log ()
{
	Stop();

	for (size_t i = 0; i < LogSinkType_Max; ++i) {
		std::for_each(m_sinkList[i].begin(), m_sinkList[i].end(), [](LogSink*e){delete e;});
	}
	std::for_each(m_logList.begin(), m_logList.end(), [](LogInfo*e){delete e;});
}


void Log::SetInterval(WIND::UInt32 interval)
{
	m_interval = interval;
}


void Log::GetTimePrefix(std::string &time)
{
	Poco::LocalDateTime dt;
	time = "[" + Poco::DateTimeFormatter::format(dt, "%Y-%m-%d %H:%M:%S") + "]";
}


bool Log::Init()
{
	bool allOK = true;
	for (size_t i = 0; i < LogSinkType_Max; ++i) {
		std::for_each(m_sinkList[i].begin(), m_sinkList[i].end(), [&allOK](LogSink* e){allOK = allOK && (e ->Init());});
	}
	if (allOK) {
		SetOrAddLogLevelStr(LogLevel_None, "NONE");
		SetOrAddLogLevelStr(LogLevel_Info, "INF");
		SetOrAddLogLevelStr(LogLevel_Debug, "DBG");
		SetOrAddLogLevelStr(LogLevel_Warning, "WNG");
		SetOrAddLogLevelStr(LogLevel_Error, "ERR");
		SetOrAddLogLevelStr(LogLevel_Important, "IMP");

		m_stop = false;

		if ( !m_sinkList[LogSinkType_Delay].empty()) {
			m_thread.start(*this);
		}
	}
	return allOK;
}


void Log::Stop()
{
	if ( !m_stop ) {
		m_stop = true;
		m_thread.join();
	}
}


WIND::UInt64 Log::GetLogID()
{
	return m_logID;
}


void Log::ReAllocID()
{
	m_logID = m_idAllocater.Alloc();
}


void Log::PushLog(LogInfo* logInfo)
{
	if (!m_stop) {
		Poco::FastMutex::ScopedLock guard (m_logLock);
		std::for_each(m_sinkList[LogSinkType_RightNow].begin(), m_sinkList[LogSinkType_RightNow].end (), [logInfo](LogSink *e) {e ->Write(logInfo);} );
		if (m_thread.isRunning()){
			m_logList.push_back(logInfo);
		}
		else {
			SafeDelete(logInfo);
		}
	}
}


void Log::AddLogSink(LogSink* sink, LogSinkType sinkType)
{
	if (sinkType < LogSinkType_Max) {
		m_sinkList[sinkType].push_back(sink);
	}
}


void Log::Swap(LogBlockList &loglist)
{
	Poco::FastMutex::ScopedLock guard (m_logLock);
	loglist.swap(m_logList);
}


bool Log::Empty()
{
	Poco::FastMutex::ScopedLock guard (m_logLock);
	return m_logList.empty();
}


void Log::Clear()
{
	Poco::FastMutex::ScopedLock guard (m_logLock);
	m_logList.clear();
}


void Log::FlushAll(WIND::UInt32 elapse, bool exit)
{
	std::for_each(m_sinkList[LogSinkType_Delay].begin(), m_sinkList[LogSinkType_Delay].end(), [elapse, exit](LogSink *e) {e ->ReadyToFlush(elapse,exit);});
	if (!this ->Empty()) {
		LogBlockList tmp;
		this ->Swap(tmp);
		for (auto it = tmp.begin(); it != tmp.end(); ++it) {
			LogInfo *logInfo = *it;
			std::for_each(m_sinkList[LogSinkType_Delay].begin(), m_sinkList[LogSinkType_Delay].end(), [logInfo](LogSink *e) {e ->Write(logInfo);});
		}
		std::for_each (m_sinkList[LogSinkType_Delay].begin(), m_sinkList[LogSinkType_Delay].end(), [](LogSink*e) { e ->Flush(); });
		std::for_each(tmp.begin(), tmp.end(), [](LogInfo *e) {SafeDelete (e);});
	}
}


const std::string& Log::GetLogLevelStr(int level)
{
	return m_logLevelStrMap[level];
}


void Log::SetOrAddLogLevelStr(int level, const std::string &str)
{
	m_logLevelStrMap[level] = str;
}


void Log::run ()
{
	if (!m_stop) {
		FlushAll(m_interval, false);
		Poco::Thread::sleep(m_interval);
	}
	FlushAll(0, true);
}


void Log::LogOut(LogLevel level, LogColor color, const char *fmt, ...)
{
	va_list vl;
	va_start (vl, fmt);
	char buf[MAX_LOG_SIZE] = {0};
	vsnprintf(buf, MAX_LOG_SIZE, fmt, vl);
	va_end (vl);

	LogInfo *logInfo = new LogInfo ();
	logInfo ->logLevel = level;
	Log::GetTimePrefix(logInfo ->logText);
	logInfo ->logText = logInfo ->logText + GetLogLevelStr(level) + ":";
	logInfo ->logBodyBegin = (logInfo ->logText).size();
	logInfo ->logText += buf;

	PushLog(logInfo);
}