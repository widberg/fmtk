#ifndef SEH_EXCEPTION_HPP
#define SEH_EXCEPTION_HPP

#include <Windows.h>
#include <string>

class SEHException : public std::exception
{
public:
	SEHException(const EXCEPTION_POINTERS* ep) noexcept;

	const char* what() const noexcept;
private:
	std::string msg;
};

#endif // !SEH_EXCEPTION_HPP
