#pragma once

#include <map>
#include <list>
#include <vector>
#include <string>
#include "DataType.h"
#include "Poco/LogFile.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Thread.h"
#include "IDAllocater.h"
#include "Timer.h"

enum LogColor {
	BLACK = 30,
	RED = 31,
	GREEN = 32,
	BROWN = 33,
	BLUE = 34,
	MAGENTA = 35,
	CYAN = 36,
	GREY = 37,

	LRED = 41,
	LGREEN = 42,
	YELLOW = 43,
	LBLUE = 44,
	LMAGENTA = 45,
	LCYAN = 46,
	WHITE = 47
};

// 修改LogLevel时不要忘记修改Log.cpp构造函数中的相关内容
enum LogLevel
{
	LogLevel_None,
	LogLevel_Info,
	LogLevel_Debug,
	LogLevel_Warning,
	LogLevel_Error,
	LogLevel_Important,
	LogLevel_Max
};

struct LogInfo
{
	LogLevel	logLevel;
	std::string	logText;
	size_t		logBodyBegin;

	LogInfo(): logLevel (LogLevel_Info) {}
	LogInfo (LogLevel logLevel_, const std::string &logText_ ) :
		logLevel (logLevel_), logText (logText_), logBodyBegin (0) {}
};


// 日志输出钩子
class LogSink
{
public:
	LogSink () {}
	virtual ~LogSink () {}

public:
	virtual bool Init () { return true;}
	virtual void Write (LogInfo *logInfo) {}
	virtual void ReadyToFlush (WIND::UInt32 elapse, bool exit) {}
	virtual void Flush () {}
};

enum LogSliceMask
{
	LogSliceMask_Day = 1,
	LogSliceMask_Size = 2
};

class LogSinkFile : public LogSink
{
public:
	LogSinkFile() : m_num (0), m_strFileNameFormat ("%B_%T.%E"), m_logSizeLimit(0), m_logFile(0), m_suffix("log"), m_seqID(1), m_sliceMask (0x8fffffff) {}
	// strPath: 路径，路径可为绝对路径或者相对路径
	// strFileNameFormat: 文件名格式串，（%B：基础名；%T:时间戳（年月日）；%P：进程ID；%A自增长计数；%E：后缀
	// nSizeLimit：文件大小限制，超过将产生新文件，0表示不限制（文件名将不包含自增长计数），
	// 如果非0则需在strFileNameFormat中包含%A，否则不会按大小切割产生新文件
	LogSinkFile (const std::string &strPath, const std::string &strBaseName,
		const std::string &strFileNameFormat = "%B_%T.%E", int nSizeLimit = 50 * 1024 * 1024, int checkInterval = 2000) :
	m_path (strPath), m_name (strBaseName), m_strFileNameFormat (strFileNameFormat), m_logSizeLimit (nSizeLimit),
		m_num(0), m_logFile (0), m_suffix("log"), m_seqID(1), m_sliceMask (0x8fffffff)
	{
		m_lastCheckSliceTimer.SetInterval (checkInterval);
	}
	~LogSinkFile() {}
public:
	virtual bool		Init ();
	virtual void		Write (LogInfo *logInfo);
	virtual void		Flush ();

private:
	bool				SliceByDate ();
	bool				SliceBySize ();
	void				CheckSlice ();
	std::string			CreateLogFile (bool bCheckSeqId = false);
	Poco::LogFile*		GetLogFile () { return m_logFile; }
	void				SetSliceMask (WIND::UInt32 mask);
	void				SetSuffix (const std::string suffix);
	void				PrepareToFlush (WIND::UInt32 elapse, bool exit);
	void				Final ();

private:
	std::string			m_text; // 辅助变量
	WIND::Int32			m_num;  // 日志基数
	std::string			m_path; // 路径
	std::string			m_name; // 基础文件名
	std::string			m_strFileNameFormat; // 文件名格式串
	std::string			m_suffix; // 文件后缀
	Poco::LogFile*		m_logFile; // 写日志对象
	WIND::UInt32		m_logSizeLimit; // 日志文件大小限制
	WIND::UInt32		m_seqID; // 文件编号自增量
	std::string			m_pid;	// 进程id
	WIND::Int32			m_lastDay; // 辅助按天数切换
	Timer				m_lastCheckSliceTimer; // 切割检测计时器
	WIND::UInt32		m_sliceMask; // 切割掩码，默认是全部标记
};

enum LogSinkType {
	LogSinkType_RightNow,
	LogSinkType_Delay,
	LogSinkType_Max
};

typedef std::list<LogInfo*> LogBlockList;
typedef std::vector<LogSink*> LogSinkList;
// 日志类
class Log: public Poco::Runnable
{
public: // c & d
	Log ();
	~Log();

public: // virtual overrides
	void				run ();

public:
	bool				Init ();
	void				Stop ();
	WIND::UInt64		GetLogID();
	void				ReAllocID ();
	void				PushLog (LogInfo* logInfo);
	void				AddLogSink (LogSink* sink, LogSinkType sinkType); // 增加钩子
	void				Clear ();
	bool				Empty ();
	void				SetInterval (WIND::UInt32 interval);
	void				GetTimePrefix (std::string &time);
	void				LogOut (LogLevel level, LogColor color, const char *fmt, ...);
	const std::string&	GetLogLevelStr (int level);
	void				SetOrAddLogLevelStr (int level, const std::string &str);
private:
	void				Swap (LogBlockList &loglist);
	void				FlushAll (WIND::UInt32 elapse, bool exit);
private:
	Poco::FastMutex		m_logLock; // 同步锁
	LogBlockList		m_logList; // 日志队列
	volatile bool		m_stop; // 停止服务
	WIND::UInt32		m_interval;	// 更新间隔，毫秒
	Poco::Thread		m_thread; // 线程对象
	LogSinkList			m_sinkList[LogSinkType_Max]; // 日志输出钩子列表
	IDAllocater<WIND::UInt64> m_idAllocater; // id分配器
	WIND::UInt64		m_logID; // 日志id
	std::map<int, std::string> m_logLevelStrMap; // 日志等级对应的字符串描述，位于日志头部
};
