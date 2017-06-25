#pragma once

class Exception
{
public:
	const char* msg;
	Exception(const char* arg)
		: msg(arg)
	{
	}
};

