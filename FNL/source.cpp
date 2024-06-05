#include "includes.hpp"
#include <Shlobj.h>

c_overlay overlay;

#define gameName (_("FortniteClient-Win64-Shipping.exe"))
#define gameWndClass "UnrealWindow"

std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);
const std::string compilation_date = (std::string)_(__DATE__);
const std::string compilation_time = (std::string)_(__TIME__);

using namespace KeyAuth;
std::string name = (_("Lantern - Fortnite")); // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = (_("nHJ20AYoZ0")); // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = (_("ac8126a4c8d20b37adba0052c87842ec3d654a6c7fb5b78b7f8a2992695834da")); // app secret, the blurred text on licenses tab and other tabs
std::string version = (_("1.0")); // leave alone unless you've changed version on website
std::string url = (_("https://keyauth.win/api/1.2/")); // change if you're self-hosting
api KeyAuthApp(name, ownerid, secret, version, url);

#include "WindFinder.hpp"
Windows wind;

int main()
{

	printf(_("Checking Authentication\n"));
	KeyAuthApp.init();
	if (!KeyAuthApp.data.success) {
		printf(_("Failed To Initiate Authentication...\n"));
		Sleep(2000);
		exit(-1);
	}
	Sleep(200);
	system(_("CLS"));
	std::wstring yourAppDataPath;
	wchar_t programDataPath[MAX_PATH];
	SHGetFolderPathW(NULL, CSIDL_COMMON_APPDATA, NULL, 0, programDataPath);
	
	std::wstring filePath = yourAppDataPath + _(L"FN_Key.json");
	std::string s(filePath.begin(), filePath.end());
	
	
	if (!CheckIfJsonKeyExists(s, _("license"))) {
		std::string key = ReadFromJson(s, _("license"));
		KeyAuthApp.license(key);
		if (!KeyAuthApp.data.success)
		{
			std::remove(_("FN_Key.json"));
			printf(_("\n Status: "), KeyAuthApp.data.message);
			Sleep(1500);
			exit(0);
		}
		printf(_("\n\n Successfully Logged In\n"));
	}
	else {
		std::string license;
		printf(_("\n\n Enter your license:"));
		std::cin >> license;
		KeyAuthApp.license(license);
		if (!KeyAuthApp.data.success)
		{
			printf(_("\n Status: "), KeyAuthApp.data.message);
			Sleep(1500);
			exit(0);
		}
	
		printf(_("\n\n Successfully Logged In\n"));
		printf(_("\n\n Creating json File\n"));
	
		if (!WriteToJson(s, _("license"), license, false, "", "")) {
			printf(_("\n\n Failed To Write Json File\n"));
		}
		printf(_("\n\n Successfully Wrote Json File\n"));
	
	}


	Memory.Initialize(gameWndClass);

	printf(_("[+] base_address -> %p\n"), (void*)base_address);
	//std::thread(Cache_thread).detach();
	Sleep(20);
	system(_("CLS"));
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	Input::Init();
	if (!overlay.start_input_handler())
		return 1; // 

	while (overlay.msg_loop() && !overlay.ready) {}

	return 0;
}