#pragma once

#define LT_LIB_EXP __declspec(dllexport)
#define LT_LIB_IMP __declspec(dllimport)

#if defined(LT_LIB_EXPORTS)
	#define LT_LIB_API LT_LIB_EXP
	#define LT_LIB_CLASS LT_LIB_EXP
#else
	#if defined(LT_LIB_IMPORTS)
		#define LT_LIB_API LT_LIB_IMP
		#define LT_LIB_CLASS LT_LIB_IMP
	#else
		#define LT_LIB_API
		#define LT_LIB_CLASS
	#endif
#endif
