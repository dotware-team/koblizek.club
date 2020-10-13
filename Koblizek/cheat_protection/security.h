#pragma once
#include <cmath>
#include <random>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <urlmon.h>
#include "../instr.h"
#include "../winapi_own.h"
#include "../obfuscation.hpp" //obfuscation file 
#pragma comment(lib, "urlmon.lib")

#define ENCRYPT_START OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN OBF_BEGIN
#define ENCRYPT_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END OBF_END

namespace koblizek_inject
{
	__forceinline int one()
	{
		ENCRYPT_START
		return (int)__TIME__;
		ENCRYPT_END
	}
	__forceinline char* two()
	{
		ENCRYPT_START
		return (char*)__TIME__;
		ENCRYPT_END
	}
	__forceinline float three()
	{
		ENCRYPT_START
		return (int)__TIME__;
		ENCRYPT_END
	}
	__forceinline int four()
	{
		ENCRYPT_START
		return pow((int)__TIME__, (unsigned int)__DATE__);
		ENCRYPT_END
	}
	__forceinline float five()
	{
		ENCRYPT_START
		return sqrt((int)__TIME__);
		ENCRYPT_END
	}
	__forceinline int six()
	{
		ENCRYPT_START
		return (int)__TIME__ + (int)__DATE__;
		ENCRYPT_END
	}
	__forceinline float eight()
	{
		ENCRYPT_START
		return (int)__TIME__ / (int)__DATE__;
		ENCRYPT_END
	}
	__forceinline int nine()
	{
		ENCRYPT_START
		return pow((int)__TIME__, (unsigned int)__DATE__) / (int)__TIME__;
		ENCRYPT_END
	}
	__forceinline float ten()
	{
		ENCRYPT_START
		return sqrt((int)__TIME__ + (int)__DATE__);
		ENCRYPT_END
	}

	__forceinline int confuse1()
	{
		ENCRYPT_START
		switch (((int)__TIME__ + (int)__DATE__ + rand() % 80000))
		{
		case 0:
			return eight();
		case 1:
			return nine();
		case 2:
			return (int)two();
		case 3:
			return five();
		case 4:
			return four();
		case 5:
			return ten();
		case 6:
			return three();
		case 7:
			return one();
		case 8:
			return six();
		}
		ENCRYPT_END
	}

	__forceinline char confuse2()
	{
		ENCRYPT_START
		switch ((confuse1() + rand() % 80000))
		{
		case 0:
			return five();
		case 1:
			return nine();
		case 2:
			return (int)two();
		case 3:
			return eight();
		case 4:
			return four();
		case 5:
			return ten();
		case 6:
			return three();
		case 7:
			return one();
		case 8:
			return six();
		}
		ENCRYPT_END
	}

	__forceinline float confuse3()
	{
		ENCRYPT_START
		switch ((confuse2() + confuse1() + rand() % 80000))
		{
		case 0:
			return (int)two();
		case 1:
			return nine();
		case 2:
			return eight();
		case 3:
			return five();
		case 4:
			return three();
		case 5:
			return ten();
		case 6:
			return four();
		case 7:
			return one();
		case 8:
			return six();
		}
		ENCRYPT_END
	}
	__forceinline int koblizek()
	{
		ENCRYPT_START
		int confuse_koblizek;
		switch (((int)__TIME__ + rand() % 80000))
		{
		case 0:
			confuse_koblizek = confuse1();
		case 1:
			confuse_koblizek += confuse3();
		case 2:
			confuse_koblizek += confuse2();
		case 3:
			confuse_koblizek = confuse3();
		case 4:
			confuse_koblizek += confuse1();
		case 5:
			confuse_koblizek -= confuse2();
		case 6:
			confuse_koblizek -= confuse3();
		case 7:
			confuse_koblizek = confuse3();
		case 8:
			confuse_koblizek = confuse2();
		case 9:
			return confuse_koblizek;
		}
		ENCRYPT_END
	}
}

namespace koblizek_initialize
{
	__forceinline int one()
	{
		ENCRYPT_START
			return (int)__TIME__;
		ENCRYPT_END
	}
	__forceinline char* two()
	{
		ENCRYPT_START
			return (char*)__TIME__;
		ENCRYPT_END
	}
	__forceinline float three()
	{
		ENCRYPT_START
			return (int)__TIME__;
		ENCRYPT_END
	}
	__forceinline int four()
	{
		ENCRYPT_START
			return pow((int)__TIME__, (unsigned int)__DATE__);
		ENCRYPT_END
	}
	__forceinline float five()
	{
		ENCRYPT_START
			return sqrt((int)__TIME__);
		ENCRYPT_END
	}
	__forceinline int six()
	{
		ENCRYPT_START
			return (int)__TIME__ + (int)__DATE__;
		ENCRYPT_END
	}
	__forceinline float eight()
	{
		ENCRYPT_START
			return (int)__TIME__ / (int)__DATE__;
		ENCRYPT_END
	}
	__forceinline int nine()
	{
		ENCRYPT_START
			return pow((int)__TIME__, (unsigned int)__DATE__) / (int)__TIME__;
		ENCRYPT_END
	}
	__forceinline float ten()
	{
		ENCRYPT_START
			return sqrt((int)__TIME__ + (int)__DATE__);
		ENCRYPT_END
	}

	__forceinline int confuse1()
	{
		ENCRYPT_START
			switch (((int)__TIME__ + (int)__DATE__ + rand() % 5000))
			{
			case 0:
				return eight();
			case 1:
				return nine();
			case 2:
				return (int)two();
			case 3:
				return five();
			case 4:
				return four();
			case 5:
				return ten();
			case 6:
				return three();
			case 7:
				return one();
			case 8:
				return six();
			}
		ENCRYPT_END
	}

	__forceinline char confuse2()
	{
		ENCRYPT_START
			switch ((confuse1() + rand() % 5000))
			{
			case 0:
				return five();
			case 1:
				return nine();
			case 2:
				return (int)two();
			case 3:
				return eight();
			case 4:
				return four();
			case 5:
				return ten();
			case 6:
				return three();
			case 7:
				return one();
			case 8:
				return six();
			}
		ENCRYPT_END
	}

	__forceinline float confuse3()
	{
		ENCRYPT_START
			switch ((confuse2() + confuse1() + rand() % 5000))
			{
			case 0:
				return (int)two();
			case 1:
				return nine();
			case 2:
				return eight();
			case 3:
				return five();
			case 4:
				return three();
			case 5:
				return ten();
			case 6:
				return four();
			case 7:
				return one();
			case 8:
				return six();
			}
		ENCRYPT_END
	}
	__forceinline int koblizek()
	{
		ENCRYPT_START
		int initialize_koblizek;
		switch (((int)__TIME__ + rand() % 5000))
		{
		case 0:
			initialize_koblizek = confuse1();
		case 1:
			initialize_koblizek += confuse3();
		case 2:
			initialize_koblizek += confuse2();
		case 3:
			initialize_koblizek = confuse3();
		case 4:
			initialize_koblizek += confuse1();
		case 5:
			initialize_koblizek -= confuse2();
		case 6:
			initialize_koblizek -= confuse3();
		case 7:
			initialize_koblizek = confuse3();
		case 8:
			initialize_koblizek = confuse2();
		case 9:
			return initialize_koblizek;
		}
		ENCRYPT_END
	}
}

namespace koblizek_packer
{
	__forceinline int one()
	{
		ENCRYPT_START
			return (int)__TIME__;
		ENCRYPT_END
	}
	__forceinline char* two()
	{
		ENCRYPT_START
			return (char*)__TIME__;
		ENCRYPT_END
	}
	__forceinline float three()
	{
		ENCRYPT_START
			return (int)__TIME__;
		ENCRYPT_END
	}
	__forceinline int four()
	{
		ENCRYPT_START
			return pow((int)__TIME__, (unsigned int)__DATE__);
		ENCRYPT_END
	}
	__forceinline float five()
	{
		ENCRYPT_START
			return sqrt((int)__TIME__);
		ENCRYPT_END
	}
	__forceinline int six()
	{
		ENCRYPT_START
			return (int)__TIME__ + (int)__DATE__;
		ENCRYPT_END
	}
	__forceinline float eight()
	{
		ENCRYPT_START
			return (int)__TIME__ / (int)__DATE__;
		ENCRYPT_END
	}
	__forceinline int nine()
	{
		ENCRYPT_START
			return pow((int)__TIME__, (unsigned int)__DATE__) / (int)__TIME__;
		ENCRYPT_END
	}
	__forceinline float ten()
	{
		ENCRYPT_START
			return sqrt((int)__TIME__ + (int)__DATE__);
		ENCRYPT_END
	}

	__forceinline int confuse1()
	{
		ENCRYPT_START
			switch (((int)__TIME__ + (int)__DATE__ + rand() % 100000))
			{
			case 0:
				return eight();
			case 1:
				return nine();
			case 2:
				return (int)two();
			case 3:
				return five();
			case 4:
				return four();
			case 5:
				return ten();
			case 6:
				return three();
			case 7:
				return one();
			case 8:
				return six();
			}
		ENCRYPT_END
	}

	__forceinline char confuse2()
	{
		ENCRYPT_START
			switch ((confuse1() + rand() % 100000))
			{
			case 0:
				return five();
			case 1:
				return nine();
			case 2:
				return (int)two();
			case 3:
				return eight();
			case 4:
				return four();
			case 5:
				return ten();
			case 6:
				return three();
			case 7:
				return one();
			case 8:
				return six();
			}
		ENCRYPT_END
	}

	__forceinline float confuse3()
	{
		ENCRYPT_START
			switch ((confuse2() + confuse1() + rand() % 100000))
			{
			case 0:
				return (int)two();
			case 1:
				return nine();
			case 2:
				return eight();
			case 3:
				return five();
			case 4:
				return three();
			case 5:
				return ten();
			case 6:
				return four();
			case 7:
				return one();
			case 8:
				return six();
			}
		ENCRYPT_END
	}
	__forceinline int koblizek()
	{
		ENCRYPT_START
		int packer_koblizek;
		switch (((int)__TIME__ + rand() % 100000))
		{
		case 0:
			packer_koblizek = confuse1();
		case 1:
			packer_koblizek += confuse3();
		case 2:
			packer_koblizek += confuse2();
		case 3:
			packer_koblizek = confuse3();
		case 4:
			packer_koblizek += confuse1();
		case 5:
			packer_koblizek -= confuse2();
		case 6:
			packer_koblizek -= confuse3();
		case 7:
			packer_koblizek = confuse3();
		case 8:
			packer_koblizek = confuse2();
		case 9:
			return packer_koblizek;
		}
		ENCRYPT_END
	}
}