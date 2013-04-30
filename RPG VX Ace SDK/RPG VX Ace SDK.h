#pragma once

#define RGSSApi extern "C" __declspec(dllexport)

// Stores handle of DLL file. May come in useful at some point.
extern HMODULE ModuleHandle;
extern int RGSSVersion;

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved );

typedef int (*RGSSEVAL)(char *);
extern RGSSEVAL RGSSEval;

typedef bool (*RGSSGETBOOL)(char *);
extern RGSSGETBOOL RGSSGetBool;

typedef int (*RGSSGETINT)(char *);
extern RGSSGETINT RGSSGetInt;

typedef double (*RGSSGETDOUBLE)(char *);
extern RGSSGETDOUBLE RGSSGetDouble;

/*
typedef wchar_t*(*RGSSGETSTRINGUTF16)(char *);
extern RGSSGETSTRINGUTF16 RGSSGetStringUTF16;

typedef void (*RGSSSETSTRINGUTF16)(char *, wchar_t*);
extern RGSSSETSTRINGUTF16 RGSSSetStringUTF16;
*/

void RGSSSetString(char *StringName, char *StringData);
void RGSSSetInt(char *VariableName,int Value);
void RGSSSetDouble(char *VariableName,double Value);
void RGSSSetBool(char *VariableName,bool Value);

RGSSApi bool Initialize();