#include "stdafx.h"

Config::Config(void)
{
    this->m_codeExePriority = 1;
    this->m_ExeName = NULL;
    this->m_isHighPriority = 1;
    this->m_isNtDll = 1;
    this->m_isLazy = 0;
    this->m_nbTimeOff = 400;
    this->m_nbTimeOn = 600;
    this->m_pid = 0;
}

Config::~Config(void)
{
}

void Config::SetExeName(WCHAR * e)
{
    this->m_ExeName = e;
}

void Config::SetProcessId(DWORD p)
{
    this->m_pid = p;
}

void Config::SetTimeOn(int ton)
{
    this->m_nbTimeOn = ton;
}

void Config::SetTimeOff(int toff)
{
    this->m_nbTimeOff = toff;
}

void Config::SetHighPriority(int p)
{
    this->m_isHighPriority = p;
}

void Config::SetNtDll(int n)
{
    this->m_isNtDll = n;
}

void Config::SetCodeExePriority(int p)
{
    this->m_codeExePriority = p;
}

void Config::SetLazy(int l)
{
    this->m_isLazy = l;
}



WCHAR * Config::GetExeName()
{
    return this->m_ExeName;
}

DWORD Config::GetProcessId()
{
    return this->m_pid;
}

int Config::GetTimeOn()
{
    return this->m_nbTimeOn;
}

int Config::GetTimeOff()
{
    return this->m_nbTimeOff;
}

int Config::GetHighPriority()
{
    return this->m_isHighPriority;
}

int Config::GetNtDll()
{
    return this->m_isNtDll;
}

int Config::GetCodeExePriority()
{
    return this->m_codeExePriority;
}

int Config::GetLazy()
{
    return this->m_isLazy;
}
