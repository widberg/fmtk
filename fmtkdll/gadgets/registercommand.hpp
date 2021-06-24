BF(void __usercall Real_RegisterCommand)(LPCSTR name AT edi, LPCVOID pThis, LPVOID callback)
	(0x0069a400);

FF(void __usercall FMTK_RegisterCommand)(LPCSTR name AT edi, LPCVOID pThis, LPVOID callback)
(
	LOG(trace, FMTK, "Registering command: {}", name);

	Real_RegisterCommand_trampoline(name, pThis, callback);

	RETURN;
)

struct CommandName
{
	std::string longName;
	std::string shortName;

	CommandName(const char* name)
	{
		for (const char* c = name; *c != '\0'; ++c)
		{
			longName.push_back(std::toupper(*c));
			if (std::isupper(*c))
			{
				shortName.push_back(*c);
			}
		}
	}

	bool operator==(const char* str) const
	{
		std::string name = str;
		std::transform(name.begin(), name.end(), name.begin(), std::toupper);
		return (name == longName || name == shortName);
	}

	bool operator==(const std::string& str) const
	{
		std::transform(str.begin(), str.end(), str.begin(), std::toupper);
		return (str == longName || str == shortName);
	}

	bool operator==(const CommandName& c) const
	{
		return (c.longName == longName && c.shortName == shortName);
	}
};

// TODO: It's late and I'm tired. There is probably a better way to store these.
std::list<std::pair<CommandName, bool(*)(int, const char**)>> commandCallbacks;

bool GenericCommandCallback()
{
	const char** pArg0 = (const char**)((char*)*pGlobalCommandState + 0xa3b0);
	std::string name = *pArg0;
	int argc = *(int*)((char*)*pGlobalCommandState + 0x23ac);

	static const char* argv[32];

	for (int i = 0; i < argc; ++i)
	{
		argv[i] = *(pArg0 + i);
	}

	auto commandPair = std::find_if(commandCallbacks.begin(), commandCallbacks.end(), [&](const std::pair<CommandName, bool(*)(int, const char**)>& x) -> bool { return x.first == name; });
	if (commandPair != commandCallbacks.end())
	{
		return commandPair->second(argc, argv);
	}

	LOG(error, FMTK, "Call to unrecognized command {}", name);

	return false;
}

void RegisterCommand(const char* name, bool(*callback)(int argc, const char** argv))
{
	commandCallbacks.push_back({ name, callback});
	Real_RegisterCommand_trampoline(name, *pGlobalCommandState, (void*)GenericCommandCallback);
}

void UnregisterCommand(const char* name)
{
	std::string capsName = name;
	for (char& c : capsName) c = std::toupper(c);
	auto newEnd = std::remove_if(commandCallbacks.begin(), commandCallbacks.end(), [&](const std::pair<CommandName, bool(*)(int, const char**)>& x) -> bool { return x.first == capsName; });
	commandCallbacks.erase(newEnd, commandCallbacks.end());
}
