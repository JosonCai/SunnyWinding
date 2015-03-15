#pragma once

#include "DataType.h"
#include "Poco/Timestamp.h"
#include <string>

// ���ʱ���ʱ��
class Timer
{
public:
	Timer ();
	Timer (WIND::UInt32 interval);
	~Timer ();

public:
	bool				Update (WIND::UInt32 elapse);
	WIND::UInt32		GetInterval ();
	void				SetInterval (WIND::UInt32 interval);
	WIND::UInt32		GetLeftTime ();
	void				SetLeftTime (WIND::UInt32 ms);
	void				Reset ();

private:
	WIND::UInt32		m_interval;
	WIND::UInt32		m_tick;
};

// �������ڼ�ʱ��
class Ticker
{
public:
	Ticker (const char *title = "Ticker");
	~Ticker ();
	void echo ();

private:
	std::string						m_title;
	Poco::Timestamp::TimeVal		m_timeVal;
	bool							m_echo;
};