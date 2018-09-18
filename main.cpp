#define UNICODE
#include <windows.h>
#include <strsafe.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <codecvt>
#include <vector>
#include <set>

void print_error(LPWSTR label)
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(wcslen((LPCWSTR)lpMsgBuf) + wcslen((LPCWSTR)label) + 40) * sizeof(WCHAR));
	StringCchPrintfW((LPWSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(WCHAR),
		L"%s failed with error %d: %s",
		label, dw, lpMsgBuf);

	OutputDebugStringW(GetCommandLineW());
	OutputDebugStringW((LPCWSTR)lpDisplayBuf);
	OutputDebugStringW(L"\n");

	std::cerr << GetCommandLineW() << ":" << (LPCWSTR)lpDisplayBuf << std::endl;

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

int CheckVersionString(std::wstring path)
{
	using convert_type = std::codecvt_utf8<WCHAR>;
	std::wstring_convert<convert_type, WCHAR> converter;

	int result = 255;

	size_t index = path.find_last_of('\\');
	std::wstring folder_path = path.substr(0, index);
	if (!folder_path.size()) {
		folder_path = L".";
	}

	if (SetCurrentDirectoryW(folder_path.c_str())) {
		result = 3;

		HMODULE hModule = LoadLibraryW(path.c_str());
		if (hModule != NULL) {
			result = 4;

			FARPROC pProc = GetProcAddress(hModule, "obs_get_version_string");

			if (pProc != NULL) {
				result = 5;

				typedef char* (*obs_get_version_string_t)();

				char* version_string = ((obs_get_version_string_t)pProc)();

				std::cout << version_string << std::endl;

				result = 0;
			}
			else print_error(L"GetProcAddress");

			FreeLibrary(hModule);
		}
		else print_error(L"LoadLibraryW");
	}
	else print_error(L"SetCurrentDirectoryW");

	return result;
}

int wmain(int argc, WCHAR* argv[])
{
	int exit_code = 1;

	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " path-to.dll" << std::endl;
	}
	else {
		exit_code = 2;

		std::wstring path = argv[1];
		
		int result = CheckVersionString((LPCTSTR)path.c_str());

		if (result == 0) {
			exit_code = 0;
		}
	}

	return exit_code;
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	int result = 255;

	LPWSTR *sz_arglist;
	int n_args;
	sz_arglist = CommandLineToArgvW(GetCommandLineW(), &n_args);
	if (sz_arglist == NULL)
	{
		print_error(L"CommandLineToArgvW");
	}
	else
	{
		result = wmain(n_args, sz_arglist);

		LocalFree(sz_arglist);
	}
	return result;
}