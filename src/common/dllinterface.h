#pragma once
#ifdef VICORDER_EXPORTS
#define VICORDER_API __declspec(dllexport)
#else
#define VICORDER_API __declspec(dllimport)
#endif