#pragma once

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif






using PsLookupProcessByProcessId = NTSTATUS(__fastcall*)(ULONG64, PVOID*);
using PsGetProcessSectionBaseAddress = PVOID(__fastcall*)(ULONG64);
using PsGetProcessPeb = PVOID(__fastcall*)(ULONG64);
using ObfReferenceObject = LONG_PTR(*)(PVOID);

#pragma warning(push)
#pragma warning(disable: 4214)
#pragma pack(push,2)

typedef struct _FAR_JMP_16
{
	UCHAR  OpCode;  // = 0xe9
	USHORT Offset;
} FAR_JMP_16;

typedef struct _FAR_TARGET_32
{
	ULONG Offset;
	USHORT Selector;
} FAR_TARGET_32;

typedef struct _PSEUDO_DESCRIPTOR_32 {
	USHORT Limit;
	ULONG Base;
} PSEUDO_DESCRIPTOR_32;

#pragma pack(pop)
typedef union _KGDTENTRY64
{
	struct
	{
		USHORT  LimitLow;
		USHORT  BaseLow;
		union
		{
			struct
			{
				UCHAR   BaseMiddle;
				UCHAR   Flags1;
				UCHAR   Flags2;
				UCHAR   BaseHigh;
			} Bytes;

			struct
			{
				ULONG   BaseMiddle : 8;
				ULONG   Type : 5;
				ULONG   Dpl : 2;
				ULONG   Present : 1;
				ULONG   LimitHigh : 4;
				ULONG   System : 1;
				ULONG   LongMode : 1;
				ULONG   DefaultBig : 1;
				ULONG   Granularity : 1;
				ULONG   BaseHigh : 8;
			} Bits;
		};
		ULONG BaseUpper;
		ULONG MustBeZero;
	};
	ULONG64 Alignment;
} KGDTENTRY64, * PKGDTENTRY64;

typedef union _KIDTENTRY64
{
	struct
	{
		USHORT OffsetLow;
		USHORT Selector;
		USHORT IstIndex : 3;
		USHORT Reserved0 : 5;
		USHORT Type : 5;
		USHORT Dpl : 2;
		USHORT Present : 1;
		USHORT OffsetMiddle;
		ULONG OffsetHigh;
		ULONG Reserved1;
	};
	ULONG64 Alignment;
} KIDTENTRY64, * PKIDTENTRY64;

typedef union _KGDT_BASE
{
	struct
	{
		USHORT BaseLow;
		UCHAR BaseMiddle;
		UCHAR BaseHigh;
		ULONG BaseUpper;
	};
	ULONG64 Base;
} KGDT_BASE, * PKGDT_BASE;

typedef union _KGDT_LIMIT
{
	struct
	{
		USHORT LimitLow;
		USHORT LimitHigh : 4;
		USHORT MustBeZero : 12;
	};
	ULONG Limit;
} KGDT_LIMIT, * PKGDT_LIMIT;

#define PSB_GDT32_MAX       3

typedef struct _KDESCRIPTOR
{
	USHORT Pad[3];
	USHORT Limit;
	PVOID Base;
} KDESCRIPTOR, * PKDESCRIPTOR;

typedef struct _KDESCRIPTOR32
{
	USHORT Pad[3];
	USHORT Limit;
	ULONG Base;
} KDESCRIPTOR32, * PKDESCRIPTOR32;

typedef struct _KSPECIAL_REGISTERS
{
	ULONG64 Cr0;
	ULONG64 Cr2;
	ULONG64 Cr3;
	ULONG64 Cr4;
	ULONG64 KernelDr0;
	ULONG64 KernelDr1;
	ULONG64 KernelDr2;
	ULONG64 KernelDr3;
	ULONG64 KernelDr6;
	ULONG64 KernelDr7;
	KDESCRIPTOR Gdtr;
	KDESCRIPTOR Idtr;
	USHORT Tr;
	USHORT Ldtr;
	ULONG MxCsr;
	ULONG64 DebugControl;
	ULONG64 LastBranchToRip;
	ULONG64 LastBranchFromRip;
	ULONG64 LastExceptionToRip;
	ULONG64 LastExceptionFromRip;
	ULONG64 Cr8;
	ULONG64 MsrGsBase;
	ULONG64 MsrGsSwap;
	ULONG64 MsrStar;
	ULONG64 MsrLStar;
	ULONG64 MsrCStar;
	ULONG64 MsrSyscallMask;
} KSPECIAL_REGISTERS, * PKSPECIAL_REGISTERS;

typedef struct _KPROCESSOR_STATE
{
	KSPECIAL_REGISTERS SpecialRegisters;
	CONTEXT ContextFrame;
} KPROCESSOR_STATE, * PKPROCESSOR_STATE;

typedef struct _PROCESSOR_START_BLOCK* PPROCESSOR_START_BLOCK;

typedef struct _PROCESSOR_START_BLOCK
{
	FAR_JMP_16 Jmp;
	ULONG CompletionFlag;
	PSEUDO_DESCRIPTOR_32 Gdt32;
	PSEUDO_DESCRIPTOR_32 Idt32;
	KGDTENTRY64 Gdt[PSB_GDT32_MAX + 1];
	ULONG64 TiledCr3;
	FAR_TARGET_32 PmTarget;
	FAR_TARGET_32 LmIdentityTarget;
	PVOID LmTarget;
	PPROCESSOR_START_BLOCK SelfMap;
	ULONG64 MsrPat;
	ULONG64 MsrEFER;
	KPROCESSOR_STATE ProcessorState;
} PROCESSOR_START_BLOCK;
#pragma warning(pop)

typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
	ULONG NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	WCHAR* Buffer;
} UNICODE_STRING;

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB_LDR_DATA
{
	ULONG Length;
	UCHAR Initialized;
	PVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB
{
	UCHAR InheritedAddressSpace;
	UCHAR ReadImageFileExecOptions;
	UCHAR BeingDebugged;
	UCHAR BitField;
	PVOID Mutant;
	PVOID ImageBaseAddress;
	PPEB_LDR_DATA Ldr;
} PEB, * PPEB;

typedef struct _SYSTEM_BIGPOOL_ENTRY
{
	union
	{
		PVOID VirtualAddress;
		ULONG_PTR NonPaged : 1;
	}
	;
	ULONG_PTR SizeInBytes;
	union
	{
		UCHAR Tag[4];
		ULONG TagUlong;
	};
} SYSTEM_BIGPOOL_ENTRY, * PSYSTEM_BIGPOOL_ENTRY;

typedef struct _SYSTEM_BIGPOOL_INFORMATION
{
	ULONG Count;
	SYSTEM_BIGPOOL_ENTRY AllocatedInfo[ANYSIZE_ARRAY];
} SYSTEM_BIGPOOL_INFORMATION, * PSYSTEM_BIGPOOL_INFORMATION;



struct CupArgs
{
	ULONG64 Argd[5];
};

EXTERN_C ULONG HyperCall(ULONG64 key, ULONG);
EXTERN_C ULONG CupCallEx(ULONG64 key, ULONG, CupArgs* command);
EXTERN_C ULONG64 __fastcall VmCallEx(ULONG64 Rcx, ULONG64 Rdx, ULONG64 R8, ULONG64 R9);
EXTERN_C NTSTATUS NtQuerySystemInformationEx(ULONG SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
PVOID GetKeExport(const char* module_name, const char* export_name, bool rva = false);



typedef struct _EPROCESS* PEPROCESS;


class HyperV
{
public:
	HyperV()
	{

		///printf("[+] MemCopy: %p\n", MemCopy);
		MemCopy = (ULONG64)GetKeExport("ntoskrnl.exe", "memcpy", 0);
		///printf("[+] MemCopy: %p\n", MemCopy);
		PsLookupPeproc = (ULONG64)GetKeExport("ntoskrnl.exe", "PsLookupProcessByProcessId", 0);
		///printf("[+] PsLookupPeproc: %p\n", PsLookupPeproc);
		ObDereferenceObject = (ULONG64)GetKeExport("ntoskrnl.exe", "ObfDereferenceObject", 0);
		///printf("[+] ObDereferenceObject: %p\n", ObDereferenceObject);
		NtShutdownSystemVa = (ULONG64)GetKeExport("ntoskrnl.exe", "NtShutdownSystem", 0);
		///printf("[+] NtShutdownSystemVa: %p\n", NtShutdownSystemVa);
		ProcBaseProt = (ULONG64)GetKeExport("ntoskrnl.exe", "PsGetProcessSectionBaseAddress", 0);
		///printf("[+] ObDereferenceObject: %p\n", ObDereferenceObject);
		ProcPebProt = (ULONG64)GetKeExport("ntoskrnl.exe", "PsGetProcessPeb", 0);
		///printf("[+] ProcPebProt: %p\n", ProcPebProt);



		RandomKey = (0 >= ULLONG_MAX ? 0 : 0 + (ULONG64)((ULLONG_MAX - 0) * ReadTimeStampCounter() * 2));



		CupArgs Args{ RandomKey };
		CupCallEx(0x12C66B554E4775B, 0x618312, &Args);
		//RandomKey = 0xBACFB55FDEAD180;// (0 >= ULLONG_MAX ? 0 : 0 + (ULONG64)((ULLONG_MAX - 0) * ReadTimeStampCounter() * 2));
		//VmCallEx(0x12C66B554E4775B, 825, RandomKey, 0);
	}

	DWORD GetProcessIdByName(const std::wstring& processName) {
		DWORD processIds[1024], cbNeeded, processesCount;
		if (!EnumProcesses(processIds, sizeof(processIds), &cbNeeded)) {
			std::cerr << "EnumProcesses failed. Error code: " << GetLastError() << std::endl;
			return 0;
		}

		processesCount = cbNeeded / sizeof(DWORD);

		for (DWORD i = 0; i < processesCount; i++) {
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);
			if (hProcess) {
				wchar_t szProcessName[MAX_PATH] = L"";
				if (GetModuleBaseNameW(hProcess, NULL, szProcessName, sizeof(szProcessName) / sizeof(wchar_t))) {
					if (processName.compare(szProcessName) == 0) {
						CloseHandle(hProcess);
						return processIds[i];
					}
				}
				CloseHandle(hProcess);
			}
		}

		return 0;
	}

	ULONG InitHyperV() {
		GROUP_AFFINITY OrigGroupAffinity{};
		GetThreadGroupAffinity(GetCurrentThread(), &OrigGroupAffinity);
		const WORD GroupCount = GetActiveProcessorGroupCount();
		for (UINT32 GroupNumber = 0u; GroupNumber < GroupCount; ++GroupNumber)
		{
			const DWORD ProcessorCount = GetActiveProcessorCount(GroupNumber);
			for (UINT32 ProcessorNumber = 0u; ProcessorNumber < ProcessorCount; ++ProcessorNumber)
			{
				GROUP_AFFINITY GroupAffinity{};
				GroupAffinity.Mask = (KAFFINITY)(1) << ProcessorNumber;
				GroupAffinity.Group = GroupNumber;
				SetThreadGroupAffinity(GetCurrentThread(), &GroupAffinity, NULL);
				ULONG Result = HyperCall(RandomKey, 5);
				if (Result != 0)
					return Result;
			}
		}
		SetThreadGroupAffinity(GetCurrentThread(), &OrigGroupAffinity, NULL);
		return 0;
	}

	bool GetStatus() {
		ULONG result = HyperCall(RandomKey, 11);
		if (result != 0)
			return false;

		return true;
	}
	bool InitPageTableBase() {
		BOOL bRet = false;
		PUCHAR Data = (PUCHAR)malloc(0x1000);
		if (Data == NULL)
			return false;

		ULONG Cr3Offset = ULONG(FIELD_OFFSET(PROCESSOR_START_BLOCK, ProcessorState) + FIELD_OFFSET(KSPECIAL_REGISTERS, Cr3));
		for (DWORD_PTR Addr = 0; Addr < 0x100000; Addr += 0x1000)
		{
			if (ReadPhys((ULONG64)Addr, (ULONG64)(void*)Data, 0x1000) == 0)
			{
				if (0x00000001000600E9 != (0xFFFFFFFFFFFF00FF & *(UINT64*)(Data)))
					continue;

				if (0xFFFFF80000000000 != (0xFFFFF80000000003 & *(UINT64*)(Data + FIELD_OFFSET(PROCESSOR_START_BLOCK, LmTarget))))
					continue;

				if (0xFFFFFF0000000FFF & *(UINT64*)(Data + Cr3Offset))
					continue;

				KernelCr3 = *(UINT64*)(Data + Cr3Offset);
				//printf("[+] m_pPML4Base -> 0x%p\n", KernelCr3);
				bRet = true;
				break;
			}
		}
		if (Data)
			free(Data);

		return bRet;
	}
	bool GetExtendProcCr3(ULONG pid) {
		if (InitHyperV() != 0)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // Red text color
			std::cout << "[-] HyperV not running!" << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset text color
			Sleep(2555);
			return false;
		}

		CurentCr3 = GetCurrentProcCr3();
		if (!InitPageTableBase())
			return false;

		NtShutdownSystemPa = Translate(KernelCr3, NtShutdownSystemVa);
		if (!NtShutdownSystemPa)
			return false;

		PVOID Process = NULL;
		Syscall<PsLookupProcessByProcessId>((PVOID)PsLookupPeproc, static_cast<ULONG64>(pid), &Process);
		if (!Process)
			return false;

		BaseAddress = (ULONG64)Syscall<PsGetProcessSectionBaseAddress>((PVOID)ProcBaseProt, (ULONG64)Process);
		ProcessPeb = (ULONG64)Syscall<PsGetProcessPeb>((PVOID)ProcPebProt, (ULONG64)Process);

		void* MyProc;
		Syscall<PsLookupProcessByProcessId>((PVOID)PsLookupPeproc, static_cast<ULONG64>(GetCurrentProcessId()), &MyProc);

		std::vector<U8> shellcode = { 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x83, 0xEC, 0x78, 0x48, 0x8B, 0x84, 0x24, 0x80, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x40, 0x08, 0x48, 0x89, 0x44, 0x24, 0x20, 0x48, 0x8D, 0x54, 0x24, 0x38, 0x48, 0x8B, 0x84, 0x24, 0x80, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x20, 0xFF, 0xD0, 0x0F, 0x20, 0xD8, 0x48, 0x89, 0x44, 0x24, 0x30, 0x48, 0x8B, 0x84, 0x24, 0x80, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x40, 0x10, 0x48, 0x89, 0x44, 0x24, 0x28, 0x48, 0x8D, 0x4C, 0x24, 0x38, 0x48, 0x8B, 0x44, 0x24, 0x28, 0xFF, 0xD0, 0x48, 0x8B, 0x44, 0x24, 0x30, 0x48, 0x83, 0xC4, 0x78, 0xC3 };
		typedef struct CR3_COMMAND
		{
		public:
			U64 process;
			U64 stack_attach;
			U64 stack_detach;
		};
		CR3_COMMAND cmd;
		cmd.process = (uint64_t)Process;
		cmd.stack_attach = (uint64_t)GetKeExport("ntoskrnl.exe", "KeStackAttachProcess", 0);
		cmd.stack_detach = (uint64_t)GetKeExport("ntoskrnl.exe", "KeUnstackDetachProcess", 0);

		using EX_ALLOCATE_POOL = uint64_t(__stdcall*) (int, SIZE_T);
		static const auto ex_allocate_pool = GetKeExport("ntoskrnl.exe", "ExAllocatePool", 0);
		auto block_kernel = Syscall<EX_ALLOCATE_POOL>(ex_allocate_pool, 0, sizeof(cmd) + shellcode.size());

		WriteMemKe((void*)block_kernel, &cmd, sizeof(CR3_COMMAND));
		WriteMemKe(reinterpret_cast<void*>(reinterpret_cast<char*>(block_kernel) + sizeof(CR3_COMMAND)), shellcode.data(), shellcode.size());
		auto cr3 = Syscall<U64(__fastcall*)(U64)>(reinterpret_cast<void*>(reinterpret_cast<char*>(block_kernel) + sizeof(CR3_COMMAND)), block_kernel);

		using EX_FREE_POOL = uint64_t(__stdcall*)(uint64_t);
		const auto ex_free_pool = GetKeExport("ntoskrnl.exe", "ExFreePool",0);
		Syscall<EX_FREE_POOL>(ex_free_pool, block_kernel);
		ExtendCr3 = cr3;
		Syscall<ObfReferenceObject>((PVOID)ObDereferenceObject, Process);
		if (CurentCr3 == 0)
		{
			Process = NULL;
			Syscall<PsLookupProcessByProcessId>((PVOID)PsLookupPeproc, static_cast<ULONG64>(GetCurrentProcessId()), &Process);
			if (!Process)
				return false;
			ReadMemKe(&CurentCr3, (PVOID)((ULONG64)Process + 0x28), 8);
			Syscall<ObfReferenceObject>((PVOID)ObDereferenceObject, Process);
		}
		return true;
	}


	PVOID GetKeExport(const char* module_name, const char* export_name, bool rva)
	{
		void* buffer = nullptr;
		DWORD buffer_size = NULL;

		NTSTATUS status = NtQuerySystemInformationEx(11, buffer, buffer_size, &buffer_size);
		while (status == 0xC0000004L)
		{
			VirtualFree(buffer, 0, MEM_RELEASE);
			buffer = VirtualAlloc(nullptr, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			status = NtQuerySystemInformationEx(11, buffer, buffer_size, &buffer_size);
		}

		if (!NT_SUCCESS(status))
		{
			VirtualFree(buffer, 0, MEM_RELEASE);
			return nullptr;
		}

		const auto modules = static_cast<PRTL_PROCESS_MODULES>(buffer);
		for (auto idx = 0u; idx < modules->NumberOfModules; ++idx)
		{
			const std::string current_module_name = std::string(reinterpret_cast<char*>(modules->Modules[idx].FullPathName) + modules->Modules[idx].OffsetToFileName);
			if (!_stricmp(current_module_name.c_str(), module_name))
			{
				std::string full_path = reinterpret_cast<char*>(modules->Modules[idx].FullPathName);
				full_path.replace(full_path.find("\\SystemRoot\\"), sizeof("\\SystemRoot\\") - 1, std::string(getenv("SYSTEMROOT")).append("\\"));
				const auto module_base = LoadLibraryExA(full_path.c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES);

				PIMAGE_DOS_HEADER p_idh;
				PIMAGE_NT_HEADERS p_inh;
				PIMAGE_EXPORT_DIRECTORY p_ied;

				PDWORD addr, name;
				PWORD ordinal;

				p_idh = (PIMAGE_DOS_HEADER)module_base;
				if (p_idh->e_magic != IMAGE_DOS_SIGNATURE)
					return NULL;

				p_inh = (PIMAGE_NT_HEADERS)((LPBYTE)module_base + p_idh->e_lfanew);
				if (p_inh->Signature != IMAGE_NT_SIGNATURE)
					return NULL;

				if (p_inh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == 0)
					return NULL;

				p_ied = (PIMAGE_EXPORT_DIRECTORY)((LPBYTE)module_base + p_inh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
				addr = (PDWORD)((LPBYTE)module_base + p_ied->AddressOfFunctions);
				name = (PDWORD)((LPBYTE)module_base + p_ied->AddressOfNames);
				ordinal = (PWORD)((LPBYTE)module_base + p_ied->AddressOfNameOrdinals);

				for (DWORD i = 0; i < p_ied->AddressOfFunctions; i++)
				{
					if (!strcmp(export_name, (char*)module_base + name[i]))
					{
						if (!rva)
						{
							auto result = (void*)((std::uintptr_t)modules->Modules[idx].ImageBase + addr[ordinal[i]]);
							VirtualFree(buffer, NULL, MEM_RELEASE);
							FreeLibrary(module_base);
							return (PVOID)result;
						}
						else
						{
							auto result = (ULONG64)(addr[ordinal[i]]);
							VirtualFree(buffer, NULL, MEM_RELEASE);
							FreeLibrary(module_base);
							return (PVOID)result;
						}
					}
				}
			}
		}
		VirtualFree(buffer, NULL, MEM_RELEASE);
		return nullptr;
	}


	template <class T, class ... Ts>
	__forceinline std::invoke_result_t<T, Ts...> Syscall(void* addr, Ts ... args)
	{
		static const auto proc = GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtShutdownSystem");
		std::uint8_t jmp_code[] =
		{
			0xff, 0x25, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00
		};

		std::uint8_t orig_bytes[sizeof jmp_code];
		*reinterpret_cast<void**>(jmp_code + 6) = addr;
		ReadPhys(NtShutdownSystemPa, (ULONG64)orig_bytes, sizeof orig_bytes);
		WritePhys(NtShutdownSystemPa, (ULONG64)jmp_code, sizeof jmp_code);
		auto result = reinterpret_cast<T>(proc)(args ...);
		WritePhys(NtShutdownSystemPa, (ULONG64)orig_bytes, sizeof orig_bytes);
		return result;
	}

	void MMCopyVirtual(U64 proc1, U64 addr1, U64 proc2, U64 addr2, U64 size)
	{
		std::vector<U8> shellcode = {
			0x50, 0x51, 0x52, 0x41, 0x50, 0x41, 0x51, 0x41, 0x57, 0x49, 0xBF, 0xEF, 0xBE, 0xAD, 0xDE, 0x00, 0x00, 0x00, 0x00, 0x49, 0x8B, 0x0F, 0x49, 0x8B, 0x57, 0x08, 0x4D, 0x8B, 0x47, 0x10, 0x4D, 0x8B, 0x4F, 0x18, 0x49, 0x8D, 0x47, 0x28, 0x50, 0x48, 0x31, 0xC0, 0x50, 0x41, 0xFF, 0x77, 0x20, 0x41, 0xFF, 0x57, 0x30, 0x49, 0x89, 0x47, 0x38, 0x41, 0x5F, 0x41, 0x59, 0x41, 0x58, 0x5A, 0x59, 0x58
		};
		struct {
			U64 proc1; //0x0
			U64 addr1; //0x8
			U64 proc2; //0x10
			U64 addr2; //0x18
			U64 size; //0x20
			U64 outptr; //0x28
			U64 mmcopy; //0x30
			U64 status; //0x38
		}data_block;

		data_block.proc1 = proc1;
		data_block.addr1 = addr1;
		data_block.proc2 = proc2;
		data_block.addr2 = addr2;
		data_block.size = size;
		data_block.mmcopy = (U64)GetKeExport("ntoskrnl.exe", "MmCopyVirtualMemory", 0);


		using EX_ALLOCATE_POOL = uint64_t(__stdcall*) (int, SIZE_T);
		 const auto ex_allocate_pool = GetKeExport("ntoskrnl.exe", "ExAllocatePool", 0);
		auto block_kernel = Syscall<EX_ALLOCATE_POOL>(ex_allocate_pool, 0, sizeof(data_block) + shellcode.size());

		WriteMemKe((void*)block_kernel, &data_block, sizeof(data_block));

		*(U64*)(shellcode.data() + 12) = block_kernel;

		WriteMemKe((void*)((U64)block_kernel + sizeof(data_block)), shellcode.data(), shellcode.size());

		Syscall<U64(__fastcall*)()>((void*)((uint64_t)block_kernel + sizeof(data_block)));

		ReadMemKe(&data_block, (void*)block_kernel, sizeof(data_block));

		using EX_FREE_POOL = uint64_t(__stdcall*)(uint64_t);
		static const auto ex_free_pool = GetKeExport("ntoskrnl.exe", "ExFreePool", 0);
		Syscall<EX_FREE_POOL>(ex_free_pool, block_kernel);
	}

	//void ExFreePool(uint64_t addr);
	//uint64_t ExAllocatePool(int pool_type, uint64_t size);


	ULONG64 GetCurrentProcCr3() {
		CupArgs Args{};
		if (CupCallEx(RandomKey, 9, &Args) != 0)
			return 0;

		return Args.Argd[0];
	}

	ULONG64 Translate(ULONG64 Cr3, ULONG64 VirtAddr) {
		CupArgs Args{ Cr3, VirtAddr };
		if (CupCallEx(RandomKey, 10, &Args) != 0)
			return 0;

		return Args.Argd[2];
	}

	ULONG ReadPhys(ULONG64 PhysAddr, ULONG64 Buffer, ULONG64 Size) {
		CupArgs Args{ PhysAddr, Buffer, Size };
		return CupCallEx(RandomKey, 6, &Args);
	}
	ULONG WritePhys(ULONG64 PhysAddr, ULONG64 Buffer, ULONG64 Size) {
		CupArgs Args{ PhysAddr, Buffer, Size };
		return CupCallEx(RandomKey, 7, &Args);
	}

	ULONG ExCopyVirt(ULONG64 SrcCr3, ULONG64 VirtSrc, ULONG64 DestCr3, ULONG64 VirtDest, ULONG64 Size) {
		CupArgs Args{ SrcCr3, VirtSrc, DestCr3, VirtDest, Size };
		return CupCallEx(RandomKey, 8, &Args);
	}

	bool writePhysical(ULONG64 cr3, ULONG64 addr, ULONG64 buffer,size_t size) {
		auto addy = Translate(ExtendCr3, addr);
		WritePhys(addy, buffer, size);
	}
	bool ReadMem(PVOID base, PVOID buffer, SIZE_T size) {
		ULONG Value = ExCopyVirt(ExtendCr3, (ULONG64)(base), CurentCr3, (ULONG64)buffer, size);
		if (0 == Value)
			return true;

		return false;
	}
	bool WriteMem(PVOID base, PVOID buffer, SIZE_T size) {
		ULONG Value = ExCopyVirt(CurentCr3, (ULONG64)(buffer), ExtendCr3, (ULONG64)(base), size);
		if (0 == Value)
			return true;

		return false;
	}

	ULONG64 GetProcessModule(const wchar_t* Name) {
		PEB PebEx{};
		if (!ReadMem((PVOID)ProcessPeb, &PebEx, sizeof(PEB)))
			return 0;

		PEB_LDR_DATA LdrList{};
		if (!ReadMem((PVOID)(PebEx.Ldr), &LdrList, sizeof(PEB_LDR_DATA)))
			return 0;

		ULONG64 first_link = ULONG64(LdrList.InLoadOrderModuleList.Flink);
		ULONG64 forward_link = first_link;

		do
		{
			LDR_DATA_TABLE_ENTRY entry{};
			if (!ReadMem((PVOID)forward_link, &entry, sizeof(LDR_DATA_TABLE_ENTRY)))
				continue;

			std::wstring Buffer(entry.BaseDllName.Length, 0);
			if (!ReadMem((PVOID)entry.BaseDllName.Buffer, (PVOID)Buffer.data(), entry.BaseDllName.Length))
				continue;

			forward_link = ULONG64(entry.InLoadOrderLinks.Flink);
			if (!entry.DllBase)
				continue;

			if (wcsstr(Buffer.c_str(), Name))
				return (ULONG64)entry.DllBase;

		} while (forward_link && forward_link != first_link);
		return 0;
	}

	ULONG64 GetValGuardRegion() {
		void* buffer = nullptr;
		DWORD buffer_size = NULL;

		NTSTATUS status = NtQuerySystemInformationEx(0x42, buffer, buffer_size, &buffer_size);
		while (status == 0xC0000004L)
		{
			VirtualFree(buffer, 0, MEM_RELEASE);
			buffer = VirtualAlloc(nullptr, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			status = NtQuerySystemInformationEx(0x42, buffer, buffer_size, &buffer_size);
		}

		if (!NT_SUCCESS(status))
		{
			if (buffer)
				VirtualFree(buffer, 0, MEM_RELEASE);

			return 0;
		}

		ULONG64 TempAddress = 0;
		const PSYSTEM_BIGPOOL_INFORMATION BigPoolData = static_cast<PSYSTEM_BIGPOOL_INFORMATION>(buffer);
		if (BigPoolData != NULL)
		{
			for (ULONG i = 0; i < BigPoolData->Count; i++)
			{
				PSYSTEM_BIGPOOL_ENTRY AllocationEntry = &BigPoolData->AllocatedInfo[i];
				ULONG64 VirtualAddress = (ULONG64)AllocationEntry->VirtualAddress & ~1ull;
				if (AllocationEntry->NonPaged && AllocationEntry->SizeInBytes == 0x200000)
				{
					if (TempAddress == 0 && AllocationEntry->TagUlong == 'TnoC')
						TempAddress = VirtualAddress;
				}
			}
			VirtualFree(BigPoolData, 0, MEM_RELEASE);
		}
		return TempAddress;
	}

	std::wstring ReadWideString(ULONG64 address) {

		std::wstring buffer;
		do
		{
			buffer.push_back(ReadValue64<wchar_t>(address++));
		} while (ReadValue64<wchar_t>(address) != '\0');
		return buffer;

	}

	std::string ReadString(ULONG64 address) {
		std::string buffer;

		do
		{
			buffer.push_back(ReadValue64<char>(address++));
		} while (ReadValue64<char>(address) != '\0');
		return buffer;
	}


	uintptr_t get_process_cr3(PEPROCESS pprocess);

	ULONG64 GetProccessBase()
	{
		return BaseAddress;
	}

	void SetGuardRegion(uintptr_t Va)
	{
		GuardReg = Va;
	}

	template <class T>
	T ReadClass64(uintptr_t Va)
	{
		T ret = {};
		if (ReadMem((PVOID)(Va), (PVOID)&ret, sizeof(T)))
			return ret;

		return {};
	}

	template <typename T>
	T ReadValue64(uintptr_t Va)
	{
		T ret{};
		if (ReadMem((PVOID)(Va), (PVOID)&ret, sizeof(T)))
			return ret;

		return ret;
	}

	template <typename T>
	T ReadGayPhys64(uintptr_t Va)
	{
		T ret{};
		if (ReadPhys((ULONG64)(Va), (ULONG64)&ret, sizeof(T)))
			return ret;

		return ret;
	}

	template <typename T>
	std::vector<T> ReadVector(uintptr_t Va, int size)
	{
		std::vector<T> TempData{};
		TempData.resize(sizeof(T) * size);
		if (ReadMem((PVOID)(Va), &TempData[0], sizeof(T) * size))
			return TempData;

		return TempData;
	}

	template<typename T>
	T ReadChain(uintptr_t Va, std::vector<uint64_t> chain)
	{
		uint64_t current = Va;
		for (int i = 0; i < chain.size() - 1; i++)
		{
			current = ReadValue64<uint64_t>(current + chain[i]);
		}
		return ReadValue64<T>(current + chain[chain.size() - 1]);
	}

	template <typename T>
	bool WriteValue64(uintptr_t Va, T Buffer)
	{
		return WriteMem((LPVOID)Va, (PVOID)&Buffer, sizeof(T));
	}

	void ReadMemKe(void* dst, void* src, std::size_t size)
	{
		Syscall<decltype(&memcpy)>((PVOID)MemCopy, dst, src, size);
	}

	void WriteMemKe(void* dst, void* src, std::size_t size)
	{
		Syscall<decltype(&memcpy)>((PVOID)MemCopy, dst, src, size);
	}



	template <typename T>
	T ValReadGuarded(uintptr_t Src)
	{
		T ret = {};
		T buffer;
		if (!ReadMem((PVOID)(Src), &buffer, sizeof(T)))
			return ret;

		uintptr_t Val = GuardReg + (*(uintptr_t*)&buffer & 0xFFFFFF);
		return *(T*)&Val;
	}

	bool IsGuarded(uintptr_t Va)
	{
		uintptr_t result = Va & 0xFFFFFFF000000000;
		return result == 0x8000000000 || result == 0x10000000000;
	}

	uintptr_t GetWorldVol(uintptr_t Va)
	{
		uintptr_t UWorldOffset = 0;
		uintptr_t UWorldAddr = ReadValue64<uintptr_t>(Va + 0x60);
		if (UWorldAddr > 0x10000000000)
			UWorldOffset = UWorldAddr - 0x10000000000;
		else
			UWorldOffset = UWorldAddr - 0x8000000000;

		return uintptr_t(Va + UWorldOffset);
	}

	template <typename T>
	T ReadVal(uintptr_t Va)
	{
		T buffer = ReadValue64< uintptr_t >(Va);
		if (IsGuarded((uintptr_t)buffer))
		{
			return ValReadGuarded<uintptr_t>(Va);
		}
		return buffer;
	}


	PPEB GetProcessPeb(DWORD processId) {

		using PS_LOOKUP_PROC = uint64_t(__stdcall*)(DWORD, PEPROCESS*);
		using PS_GET_PROC_PEB = uint64_t(__stdcall*)(PEPROCESS);
		const auto PsGetProcessPebAddr = GetKeExport("ntoskrnl.exe", "PsGetProcessPeb", 0);
		const auto PsLookUpProcAddr = GetKeExport("ntoskrnl.exe", "PsLookupProcessByProcessId", 0);

		if (!PsGetProcessPebAddr == NULL || !PsLookUpProcAddr == NULL)
			return NULL;

		PEPROCESS Process;
		Syscall<PS_LOOKUP_PROC>(PsLookUpProcAddr, processId, &Process);

		
		return PPEB(Syscall<PS_GET_PROC_PEB>(PsGetProcessPebAddr, Process));
	}


private:
	ULONG64 RandomKey;
	ULONG64 KernelCr3;
	ULONG64 CurentCr3;
	ULONG64 ExtendCr3;
	ULONG64 NtShutdownSystemPa;
	ULONG64 NtShutdownSystemVa;
	ULONG64 BaseAddress;
	ULONG64 ProcessPeb;

	ULONG64 Program_Process;

	ULONG64 MemCopy;
	ULONG64 PsLookupPeproc;
	ULONG64 ObDereferenceObject;
	ULONG64 ProcBaseProt;
	ULONG64 ProcPebProt;
	ULONG64 GuardReg;
};


#ifdef IS_HYPER_V
extern HyperV* m_pHyperV;
#endif