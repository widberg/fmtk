#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>

struct PaternRead
{
	std::string read_offset_value;
	std::vector<std::uint16_t> pattern;
};

struct Symbol
{
	bool required;
	std::string type;
	std::unordered_map<std::string, std::string> addresses;
	std::vector<std::vector<std::uint16_t>> patterns;
	std::vector<PaternRead> pattern_reads;
};

struct Module
{
	std::string value;
	std::string export_prefix;
	std::string emit_prefix;
	std::string base_address;
	std::unordered_map<std::string, std::string> hashes;
	std::unordered_map<std::string, Symbol> symbols;

	bool set(const std::string& variable_name, const std::string& variable_value)
	{
		if (variable_name == "export_prefix")
		{
			export_prefix = variable_value;
		}
		else if (variable_name == "emit_prefix")
		{
			emit_prefix = variable_value;
		}
		else if (variable_name == "base_address")
		{
			base_address = variable_value;
		}
		else
		{
			return false;
		}
		return true;
	}
};

struct Context
{
	std::unordered_map<std::string, Module> modules;
};

const char* consumeWhitespace(const char* c)
{
	while (std::isspace(*c)) ++c;
	return c;
}

const char* consumeIdentifier(const char* c, std::string& identifier)
{
	c = consumeWhitespace(c);

	const char* start = c;
	while (std::isalnum(*c) || *c == '_') ++c;

	identifier = std::string(start, c - start);

	return c;
}

bool commandModule(const std::vector<std::string>& argv, Context& context)
{
	if (argv.size() != 2)
	{
		std::cout << "commandModule Wrong number of arguments " << argv.size() << std::endl;
		return false;
	}
	std::cout << "module " << argv[0] << " " << argv[1] << "\n";

	if (context.modules.count(argv[0]))
	{
		std::cout << "module " << argv[0] << " already exists\n";
		return false;
	}

	context.modules[argv[0]] = Module{ argv[1] };

	return true;
}

bool commandSet(const std::vector<std::string>& argv, Context& context)
{
	if (argv.size() != 3)
	{
		std::cout << "commandSet Wrong number of arguments " << argv.size() << std::endl;
		return false;
	}

	if (!context.modules.count(argv[0]))
	{
		std::cout << "module " << argv[0] << " does not exists\n";
		return false;
	}

	if (!context.modules[argv[0]].set(argv[1], argv[2]))
	{
		std::cout << "variable does not exist" << std::endl;
		return false;
	}

	return true;
}

bool commandHash(const std::vector<std::string>& argv, Context& context)
{
	if (argv.size() != 3)
	{
		std::cout << "commandHash Wrong number of arguments " << argv.size() << std::endl;
		return false;
	}

	if (!context.modules.count(argv[0]))
	{
		std::cout << "module " << argv[0] << " does not exists\n";
		return false;
	}

	if (context.modules[argv[0]].hashes.count(argv[1]))
	{
		std::cout << "module already has version by that name" << std::endl;
		return false;
	}

	context.modules[argv[0]].hashes[argv[1]] = argv[2];

	return true;
}

bool commandSymbol(const std::vector<std::string>& argv, Context& context)
{
	if (argv.size() != 4)
	{
		std::cout << "commandSymbol Wrong number of arguments " << argv.size() << std::endl;
		return false;
	}

	if (!context.modules.count(argv[0]))
	{
		std::cout << "module " << argv[0] << " does not exists\n";
		return false;
	}

	if (context.modules[argv[0]].symbols.count(argv[1]))
	{
		std::cout << "module already has symbols by that name" << std::endl;
		return false;
	}

	context.modules[argv[0]].symbols[argv[1]] = Symbol{ argv[2] == "required", argv[3] };

	return true;
}

bool commandAddress(const std::vector<std::string>& argv, Context& context)
{
	if (argv.size() != 4)
	{
		std::cout << "commandAddress Wrong number of arguments " << argv.size() << std::endl;
		return false;
	}

	if (!context.modules.count(argv[0]))
	{
		std::cout << "module " << argv[0] << " does not exists\n";
		return false;
	}

	if (!context.modules[argv[0]].symbols.count(argv[1]))
	{
		std::cout << "module does not have a symbol by that name" << std::endl;
		return false;
	}

	if (!context.modules[argv[0]].hashes.count(argv[2]))
	{
		std::cout << "module does not have a version by that name" << std::endl;
		return false;
	}

	if (context.modules[argv[0]].symbols[argv[1]].addresses.count(argv[2]))
	{
		std::cout << "module does not has an address for this version" << std::endl;
		return false;
	}

	context.modules[argv[0]].symbols[argv[1]].addresses[argv[2]] = argv[3];

	return true;
}

bool parseLine(const std::string& line, Context& context)
{
	std::cout << "line: " << line << "\n";

	const char* c = line.c_str();

	c = consumeWhitespace(c);

	if (c[0] != '/' || c[1] != '/' || c[2] != '$')
	{
		return true;
	}

	c += 3;

	std::string commandName;
	c = consumeIdentifier(c, commandName);

	std::unordered_map<std::string, bool (*)(const std::vector<std::string>&, Context&)> commands =
	{
		{"module", commandModule},
		{"set", commandSet},
		{"hash", commandHash},
		{"symbol", commandSymbol},
		{"address", commandAddress},
	};

	if (!commands.count(commandName)) {
		std::cout << "command not found" << std::endl;
		return false;
	}

	std::cout << "c " << (int) * c << "\n";
	std::vector<std::string> argv;
	while (*c != '\0')
	{
		std::string arg;
		c = consumeIdentifier(c, arg);
		std::cout << "ident " << arg << "\n";
		argv.push_back(arg);
	}

	if (!commands[commandName](argv, context))
	{
		return false;
	}

	return true;
}

int main(int argc, const char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Wrong number of arguments " << argc << std::endl;
		return 1;
	}

	std::ifstream in(argv[1]);
	if (!in.good())
	{
		std::cout << "Could not open for reading" << std::endl;
		return 1;
	}

	std::ofstream out(argv[2]);
	if (!out.good())
	{
		std::cout << "Could not open for writing" << std::endl;
		return 1;
	}

	Context context;

	std::string line;
	while (std::getline(in, line))
	{
		if (!parseLine(line, context))
		{
			std::cout << "parseLine failed" << std::endl;
			return 1;
		}
	}

	return 0;
}