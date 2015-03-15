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

// �޸�LogLevelʱ��Ҫ�����޸�Log.cpp���캯���е��������
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


// ��־�������
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
	// strPath: ·����·����Ϊ����·���������·��
	// strFileNameFormat: �ļ�����ʽ������%B����������%T:ʱ����������գ���%P������ID��%A������������%E����׺
	// nSizeLimit���ļ���С���ƣ��������������ļ���0��ʾ�����ƣ��ļ�������������������������
	// �����0������strFileNameFormat�а���%A�����򲻻ᰴ��С�и�������ļ�
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
	std::string			m_text; // ��������
	WIND::Int32			m_num;  // ��־����
	std::string			m_path; // ·��
	std::string			m_name; // �����ļ���
	std::string			m_strFileNameFormat; // �ļ�����ʽ��
	std::string			m_suffix; // �ļ���׺
	Poco::LogFile*		m_logFile; // д��־����
	WIND::UInt32		m_logSizeLimit; // ��־�ļ���С����
	WIND::UInt32		m_seqID; // �ļ����������
	std::string			m_pid;	// ����id
	WIND::Int32			m_lastDay; // �����������л�
	Timer				m_lastCheckSliceTimer; // �и����ʱ��
	WIND::UInt32		m_sliceMask; // �и����룬Ĭ����ȫ�����
};

enum LogSinkType {
	LogSinkType_RightNow,
	LogSinkType_Delay,
	LogSinkType_Max
};

typedef std::list<LogInfo*> LogBlockList;
typedef std::vector<LogSink*> LogSinkList;
// ��־��
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
	void				AddLogSink (LogSink* sink, LogSinkType sinkType); // ���ӹ���
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
	Poco::FastMutex		m_logLock; // ͬ����
	LogBlockList		m_logList; // ��־����
	volatile bool		m_stop; // ֹͣ����
	WIND::UInt32		m_interval;	// ���¼��������
	Poco::Thread		m_thread; // �̶߳���
	LogSinkList			m_sinkList[LogSinkType_Max]; // ��־��������б�
	IDAllocater<WIND::UInt64> m_idAllocater; // id������
	WIND::UInt64		m_logID; // ��־id
	std::map<int, std::string> m_logLevelStrMap; // ��־�ȼ���Ӧ���ַ���������λ����־ͷ��
};
