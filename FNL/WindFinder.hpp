#pragma once
#include "Includes.hpp"

class Windows {
public:

	void Init();

	DWORD ProcIdFinder(std::string ProcID);
	bool Find_Title();

private:
	std::string reason;

};

static std::vector<std::string> BadNames = {
	_("IDA: Quick start"),
	_("Memory Viewer"),
	_("Process List"),
	_("KsDumper"),
};

static std::vector<std::string> EXENames = {
	_("cheatengine-x86_64-SSE4-AVX2.exe"),
	_("ollydbg.exe"),
	_("ReClass.NET.exe"),
	_("KsDumper.exe"),
	_("vboxtray.exe"),
	_("111"),
	_("111"),
	_("111"),
	_("111"),
	_("111"),
	_("111"),
	_("111"),

};