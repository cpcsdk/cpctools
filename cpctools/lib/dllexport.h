#if defined _WINDOWS
	#ifdef BUILDING_DLL
		#define DLL_PUBLIC __declspec(dllexport)
	#else
		#define DLL_PUBLIC __declspec(dllimport)
	#endif
#else
	#define DLL_PUBLIC
#endif
