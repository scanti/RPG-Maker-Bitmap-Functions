#include "stdafx.h"
#include "RPG VX Ace SDK.h"
#include "Your Code.h"

HMODULE ModuleHandle;
int RGSSVersion=0;
bool Initialized=false;

RGSSEVAL RGSSEval;
RGSSGETBOOL RGSSGetBool;
RGSSGETINT RGSSGetInt;
RGSSGETDOUBLE RGSSGetDouble;
// RGSSGETSTRINGUTF16 RGSSGetStringUTF16;
// RGSSSETSTRINGUTF16 RGSSSetStringUTF16;

// This hack will initialize the DLL the first time it is called from RPG Maker VX Ace

class InitSpawn
{
public:
	InitSpawn::InitSpawn()
	{
		Initialize();
	}
};

InitSpawn InitDLL;

void RGSSSetString(char *StringName, char *StringData)
{
	char *buffer=NULL;
	size_t BufferLength=0;

	BufferLength=strlen(StringName)+strlen(StringData)+6;
	buffer = new char[BufferLength];

	strcpy_s(buffer,BufferLength,StringName);
	strcat_s(buffer,BufferLength," = \"");
	strcat_s(buffer,BufferLength,StringData);
	strcat_s(buffer,BufferLength,"\"");
	RGSSEval(buffer);

	delete[] buffer;
	return;
}

void RGSSSetInt(char *VariableName,int Value)
{
	std::ostringstream Buffer;
	Buffer << Value;

	if(Buffer.good())
		RGSSSetString(VariableName,(char *)Buffer.str().c_str());
	else
		RGSSSetString(VariableName,"0");
	return;
}

void RGSSSetDouble(char *VariableName,double Value)
{
	std::ostringstream Buffer;
	Buffer << Value;

	if (Buffer.good())
		RGSSSetString(VariableName,(char *)Buffer.str().c_str());
	else
		RGSSSetString(VariableName,"0.0");
	return;
}

void RGSSSetBool(char *VariableName,bool Value)
{
	if (Value)
		RGSSSetString(VariableName,"true");
	else
		RGSSSetString(VariableName,"false");
	return;
}


RGSSApi bool Initialize()
{
	if(Initialized)
	{
		std::cout << "Already initialized." << std::endl;
		return(true);
	}

	HMODULE RGSSSystemDLL=NULL;
	TCHAR DLLName[MAX_PATH];
	TCHAR RGSSSystemFilePath[MAX_PATH];
	TCHAR IniDir[MAX_PATH];

	GetModuleFileName(ModuleHandle,DLLName,MAX_PATH);

	_tprintf (_T("Initializing %s.\n"),DLLName);

	GetCurrentDirectory(MAX_PATH,IniDir);
	_tcsncat_s(IniDir,MAX_PATH,_T("\\game.ini"),MAX_PATH);

	_tprintf (_T("Game.ini is at %s\n"),IniDir);

	GetPrivateProfileString(_T("Game"),_T("Library"),_T("RGSS301xxx.dll"),RGSSSystemFilePath,MAX_PATH,IniDir);

	/* As the RGSS dll will always be loaded at this point. (The RGSS dll will be the one calling this dll).
	   Use GetModuleHandle instead of LoadLibrary. It's quicker and won't add a reference to the RGSS module
	   that could result in a deadlock when the program exits. */

	RGSSSystemDLL=GetModuleHandle(RGSSSystemFilePath);

	_tprintf(_T("Looking for %s.\n"),RGSSSystemFilePath);

	if (!RGSSSystemDLL)
	{
		std::cerr << "Unable to find RGSS system file (" << RGSSSystemFilePath << ")" << std::endl;
		return false;  // Failed to get module handle for RGSS dll.
	}

	if(GetProcAddress(RGSSSystemDLL,"RGSSInitialize"))
	{
		std::cout << "Running RPG Maker XP." << std::endl;
		RGSSVersion=1;
	}
	else if(GetProcAddress(RGSSSystemDLL,"RGSSInitialize2"))
	{
		std::cout << "Running RPG Maker VX." << std::endl;
		RGSSVersion=2;
	}
	else if(GetProcAddress(RGSSSystemDLL,"RGSSInitialize3"))
	{
		std::cout << "Running RPG Maker VX Ace." << std::endl;
		RGSSVersion=3;
	}

	RGSSEval=(RGSSEVAL)GetProcAddress(RGSSSystemDLL,"RGSSEval");
	if (!RGSSEval)
	{
		std::cerr << "Unable to find RGSSEval" << std::endl;
		return false;
	}

	RGSSGetBool=(RGSSGETBOOL)GetProcAddress(RGSSSystemDLL,"RGSSGetBool");
	if (!RGSSGetBool)
	{
		std::cerr << "Unable to find RGSSGetBool" << std::endl;
		return false;
	}

	RGSSGetInt=(RGSSGETINT)GetProcAddress(RGSSSystemDLL,"RGSSGetInt");
	if (!RGSSGetInt)
	{
		std::cerr << "Unable to find RGSSGetInt" << std::endl;
		return false;
	}

	RGSSGetDouble=(RGSSGETDOUBLE)GetProcAddress(RGSSSystemDLL,"RGSSGetDouble");
	if (!RGSSGetDouble)
	{
		std::cerr << "Unable to find RGSSGetDouble" << std::endl;
		return false;
	}

	/*
	RGSSGetStringUTF16=(RGSSGETSTRINGUTF16)GetProcAddress(RGSSSystemDLL,"RGSSGetStringUTF16");
	if (!RGSSGetStringUTF16)
	{
		std::cerr << "Unable to find RGSSGetStringUTF16" << std::endl;
		return false;
	}

	RGSSSetStringUTF16=(RGSSSETSTRINGUTF16)GetProcAddress(RGSSSystemDLL,"RGSSSetStringUTF16");
	if (!RGSSSetStringUTF16)
	{
		std::cerr << "Unable to find RGSSSetStringUTF16" << std::endl;
		return false;
	}
*/
	RGSSSetInt("$RGSSVERSION",RGSSVersion);

	RGSSYourCustomInitCode();

	std::cout << "All OK." << std::endl;

	Initialized=true;

	return true;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call==DLL_PROCESS_ATTACH)
		ModuleHandle=hModule;
    return TRUE;
}

