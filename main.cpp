#define UNICODE
#include <windows.h>
#include <strsafe.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <codecvt>
#include <vector>
#include <set>

extern "C" __declspec(dllexport) const char *obs_get_version_string(void);

int CheckVersionString()
{
	std::cout << obs_get_version_string() << std::endl;

	return 0;
}

int wmain(int argc, WCHAR* argv[])
{
	int exit_code = CheckVersionString();

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
		std::cerr << "CommandLineToArgvW" << std::endl;
	}
	else
	{
		result = wmain(n_args, sz_arglist);

		LocalFree(sz_arglist);
	}
	return result;
}