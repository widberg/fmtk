#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

struct PaternRead
{
	std::string read_offset_value;
	std::vector<std::uint16_t> pattern;
};

struct Symbol
{
	bool required;
	std::unordered_map<std::string, std::string> addresses;
	std::vector<std::vector<std::uint16_t>> patterns;
	std::vector<PaternRead> pattern_reads;
};

struct Module
{
	std::string value;
	std::string export_prefix;
	std::string base_address;
	std::unordered_map<std::string, std::string> hashes;
	std::unordered_map<std::string, Symbol> symbols;
};

int main(int argc, const char* argv[])
{
	if (argc != 3) return 1;

	std::ifstream in(argv[1]);
	if (!in.good())
	{
		std::cout << "Could not open for reading" << std::endl;
		return 1;
	}

	std::ofstream out(argv[2]);
	if (!out.good())
	{
		std::cout << "Could not open for writting" << std::endl;
		return 1;
	}

	std::unordered_map<std::string, Module> modules;

	std::string line;
	while (std::getline(in, line))
	{
		char* c = line.data();
		while (std::isspace(*c)) ++c;

		if (c[0] != '/' || c[1] != '/' || c[2] != '$')
		{
			out << line << "\n";
			continue;
		}

		c += 3;
		while (std::isspace(*c)) ++c;
		if (*c == '\0') return 1;

		const char* command = c;
		while (*c != '\0' && !std::isspace(*c)) ++c;
		if (*c == '\0') return 1;
		*c++ = '\0';
		while (std::isspace(*c)) ++c;
		if (*c == '\0') return 1;

		if (!std::strcmp(command, "module"))
		{
			const char* module_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* module_value = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c != '\0')
			{
				*c++ = '\0';
				while (std::isspace(*c)) ++c;
				if (*c != '\0') return 1;
			}

			out << "MODULE " << module_name << " " << module_value << "\n";

			if (!modules.count(module_name))
			{
				modules[module_name] = Module { module_value };
			}
			else
			{
				return 1;
			}
		}
		else if(!std::strcmp(command, "set"))
		{
			const char* module_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* variable_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* variable_value = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c != '\0')
			{
				*c++ = '\0';
				while (std::isspace(*c)) ++c;
				if (*c != '\0') return 1;
			}

			out << "SET " << module_name << " " << variable_name << " " << variable_value << "\n";

			if (modules.count(module_name))
			{
				if (!std::strcmp(variable_name, "export_prefix"))
				{
					modules[module_name].export_prefix = variable_value;
				}
				else if (!std::strcmp(variable_name, "base_address"))
				{
					modules[module_name].base_address = variable_value;
				}
				else
				{
					std::cout << "bad variable" << "\n";
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		else if (!std::strcmp(command, "hash"))
		{
			const char* module_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* version_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* hash_value = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c != '\0')
			{
				*c++ = '\0';
				while (std::isspace(*c)) ++c;
				if (*c != '\0') return 1;
			}

			out << "HASH " << module_name << " " << version_name << " " << hash_value << "\n";

			if (modules.count(module_name))
			{
				if (!modules[module_name].hashes.count(version_name))
				{
					modules[module_name].hashes[version_name] = hash_value;
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		else if (!std::strcmp(command, "symbol"))
		{
			const char* module_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* symbol_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* required_value = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c != '\0')
			{
				*c++ = '\0';
				while (std::isspace(*c)) ++c;
				if (*c != '\0') return 1;
			}

			out << "SYMBOL " << module_name << " " << symbol_name << " " << required_value << "\n";

			if (modules.count(module_name))
			{
				if (!modules[module_name].symbols.count(symbol_name))
				{
					modules[module_name].symbols[symbol_name] = Symbol { !strcmp(required_value, "required") };
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		else if (!std::strcmp(command, "address"))
		{
			const char* module_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* symbol_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* version_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* address_value = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c != '\0')
			{
				*c++ = '\0';
				while (std::isspace(*c)) ++c;
				if (*c != '\0') return 1;
			}

			out << "ADDRESS " << module_name << " " << symbol_name << " " << version_name << " " << address_value << "\n";

			if (modules.count(module_name))
			{
				if (modules[module_name].symbols.count(symbol_name))
				{
					if (!modules[module_name].symbols[symbol_name].addresses.count(version_name))
					{
						modules[module_name].symbols[symbol_name].addresses[version_name] = address_value;
					}
					else
					{
						return 1;
					}
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		else if (!std::strcmp(command, "pattern"))
		{
			const char* module_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* symbol_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			if (*c++ != '\"') return 1;

			char* pattern_value = c;
			while (*c != '\"') ++c;
			if (*c != '\0')
			{
				*c++ = '\0';
				while (std::isspace(*c)) ++c;
				if (*c != '\0') return 1;
			}

			out << "PATTERN " << module_name << " " << symbol_name << " " << pattern_value << "\n";

			if (modules.count(module_name))
			{
				if (modules[module_name].symbols.count(symbol_name))
				{
					std::vector<std::uint16_t> pattern;

					c = pattern_value;
					while (true)
					{
						const char* byte_string = c;
						while (*c != '\0' && !std::isspace(*c)) ++c;
						if (*c != '\0')
						{
							*c++ = '\0';
							while (std::isspace(*c)) ++c;
							if (*c != '\0') break;
						}

						try
						{
							std::uint32_t x = std::stoul(byte_string, nullptr, 16);
							if (x > 0xFF) return 1;
							pattern.push_back(x);
						}
						catch (const std::exception& e)
						{
							pattern.push_back(0xFFFF);
						}
					}

					modules[module_name].symbols[symbol_name].patterns.push_back(pattern);
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		else if (!std::strcmp(command, "pattern_read"))
		{
			const char* module_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* symbol_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* read_offset_value = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			if (*c++ != '\"') return 1;

			char* pattern_value = c;
			while (*c != '\"') ++c;
			if (*c != '\0')
			{
				*c++ = '\0';
				while (std::isspace(*c)) ++c;
				if (*c != '\0') return 1;
			}

			out << "PATTERN_READ " << module_name << " " << symbol_name << " " << read_offset_value << " " << pattern_value << "\n";

			if (modules.count(module_name))
			{
				if (modules[module_name].symbols.count(symbol_name))
				{
					std::vector<std::uint16_t> pattern;

					c = pattern_value;
					while (true)
					{
						const char* byte_string = c;
						while (*c != '\0' && !std::isspace(*c)) ++c;
						if (*c != '\0')
						{
							*c++ = '\0';
							while (std::isspace(*c)) ++c;
							if (*c != '\0') break;
						}

						try
						{
							std::uint32_t x = std::stoul(byte_string, nullptr, 16);
							if (x > 0xFF) return 1;
							pattern.push_back(x);
						}
						catch (const std::exception& e)
						{
							pattern.push_back(0xFFFF);
						}
					}

					modules[module_name].symbols[symbol_name].pattern_reads.push_back(PaternRead { read_offset_value, pattern });
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		else if (!std::strcmp(command, "emit"))
		{
			const char* emit_section = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c != '\0')
			{
				*c++ = '\0';
				while (std::isspace(*c)) ++c;
				if (*c != '\0') return 1;
			}

			if (!std::strcmp(emit_section, "attach"))
			{

			}
			else if (!std::strcmp(emit_section, "detach"))
			{

			}
			else
			{
				return 1;
			}

			out << "EMIT " << emit_section << "\n";
		}
		else
		{
			std::cout << "bad command" << std::endl;
			return 1;
		}
	}

	return 0;
}
