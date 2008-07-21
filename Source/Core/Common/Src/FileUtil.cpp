#include "Common.h"
#include "FileUtil.h"
#ifdef _WIN32
#include <shellapi.h>
#endif

bool File::Exists(const std::string &filename)
{
#ifdef _WIN32
	return GetFileAttributes(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
#else
	return true; //TODO
#endif
}

bool File::IsDirectory(const std::string &filename) {
#ifdef _WIN32
	return (GetFileAttributes(filename.c_str()) & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
	return false; //TODO
#endif
}

std::string SanitizePath(const std::string &filename) {
	std::string copy = filename;
	for (int i = 0; i < copy.size(); i++)
		if (copy[i] == '/') copy[i] = '\\';
	return copy;
}

void File::Launch(const std::string &filename)
{
#ifdef _WIN32
	std::string win_filename = SanitizePath(filename);
	SHELLEXECUTEINFO shex = { sizeof(shex) };
	shex.fMask = SEE_MASK_NO_CONSOLE; // | SEE_MASK_ASYNC_OK;
	shex.lpVerb = "open";
	shex.lpFile = win_filename.c_str();
	shex.nShow = SW_SHOWNORMAL;
	ShellExecuteEx(&shex);
#endif
}

void File::Explore(const std::string &path)
{
#ifdef _WIN32
	std::string win_path = SanitizePath(path);
	SHELLEXECUTEINFO shex = { sizeof(shex) };
	shex.fMask = SEE_MASK_NO_CONSOLE; // | SEE_MASK_ASYNC_OK;
	shex.lpVerb = "explore";
	shex.lpFile = win_path.c_str();
	shex.nShow = SW_SHOWNORMAL;
	ShellExecuteEx(&shex);
#endif
}

// Returns true if successful, or path already exists.
bool File::CreateDir(const std::string &path)
{
#ifdef _WIN32
	if (::CreateDirectory(path.c_str(), NULL))
	{
		return true;
	}
	else
	{
		DWORD error = GetLastError();
		if (error == ERROR_ALREADY_EXISTS)
		{
			PanicAlert("%s already exists", path.c_str());
			return true;
		}
		PanicAlert("Error creating directory: %i", error);
		return false;
	}
#endif
}