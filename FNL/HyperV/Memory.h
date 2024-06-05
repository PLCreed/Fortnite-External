#pragma once


uintptr_t base_address;
#define STR_BUFFER_SIZE 64
#define WSTR_BUFFER_SIZE 1024

class CMemory
{

private:

	DWORD GetProcessID()
	{
		DWORD dwRet = 0;
		DWORD dwThreadCountMax = 0;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		Process32First(hSnapshot, &pe32);
		do
		{
			// Convert narrow character string to wide character string
			wchar_t szExeFile[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, pe32.szExeFile, -1, szExeFile, MAX_PATH);

			if (wcscmp(szExeFile, L"FortniteClient-Win64-Shipping.exe") == 0)
			{
				DWORD dwTmpThreadCount = GetProcessThreadNumByID(pe32.th32ProcessID);

				if (dwTmpThreadCount > dwThreadCountMax)
				{
					dwThreadCountMax = dwTmpThreadCount;
					dwRet = pe32.th32ProcessID;
				}
			}
		} while (Process32Next(hSnapshot, &pe32));
		CloseHandle(hSnapshot);
		return dwRet;
	}

public:

	
	HWND Window;
	DWORD GamePID;
	HyperV* _HyperV = new HyperV();

	template<typename type>
	inline type Read(uintptr_t address)
	{
		return _HyperV->ReadValue64<type>(address);
	}

	void Read(uintptr_t address, void* buffer, size_t size)
	{

		_HyperV->ReadMem((void*)address, buffer, size);

	}

	template<typename type>
	inline type Read2(uintptr_t PhysAddr)
	{
		return _HyperV->ReadGayPhys64<type>(PhysAddr);
	}

	template<typename type>
	inline bool Write(uintptr_t address, type value)
	{
		return _HyperV->WriteValue64<type>(address, value);
	}

	template<typename type>
	inline type ReadChain(uintptr_t base, std::vector<uintptr_t> chain)
	{
		return _HyperV->ReadChain<type>(base, chain);
	}

	uintptr_t get_module(std::string module_name)
	{
		return _HyperV->GetProcessModule(std::wstring(module_name.begin(), module_name.end()).c_str());
	}


	std::wstring ReadWstr(uintptr_t address)
	{

		wchar_t buffer[WSTR_BUFFER_SIZE * sizeof(wchar_t)];
		Read(address, &buffer, WSTR_BUFFER_SIZE * sizeof(wchar_t));
		return std::wstring(buffer);

	}

	std::string ReadStr(uintptr_t address, int size = STR_BUFFER_SIZE)
	{
		std::unique_ptr<char[]> buffer(new char[size]);
		Read(address, buffer.get(), size);

		if (!buffer.get())
			return "invalid";

		return std::string(buffer.get());
	}


	bool Initialize(std::string wndClassname)
	{

		while (!Window)
		{
			Window = FindWindowA(wndClassname.c_str(), NULL);
			printf(_("Waiting for Game..."));
			Sleep(100);
			system(_("CLS"));
		}


		while (!GetProcessID()){ }

		GamePID = GetProcessID();

		if (!GamePID)
		{
			Sleep(2500);
			return false; // failed to get the process id
		}

		if (!_HyperV->GetExtendProcCr3(GamePID))
		{
			return false;
		}

		base_address = _HyperV->GetProccessBase();

		if (!base_address)
		{
			Sleep(2500);
			return false;
		}

		Sleep(2500);

		return true;

	}

	int GetProcessThreadNumByID(DWORD dwPID)
	{
		HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
			return 0;

		PROCESSENTRY32 pe32 = { 0 };
		pe32.dwSize = sizeof(pe32);
		BOOL bRet = ::Process32First(hProcessSnap, &pe32);;
		while (bRet)
		{
			if (pe32.th32ProcessID == dwPID)
			{
				::CloseHandle(hProcessSnap);
				return pe32.cntThreads;
			}
			bRet = ::Process32Next(hProcessSnap, &pe32);
		}
		return 0;
	}


	

}; inline CMemory Memory;