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

	std::size_t in_line_number = 0;
	std::size_t out_line_number = 0;

	++out_line_number;
	out << "#line " << 1 << " \"" << argv[1] << "\"" << "\n";

	std::string line;
	while (std::getline(in, line))
	{
		++in_line_number;
		++out_line_number;

		char* c = line.data();
		while (std::isspace(*c)) ++c;

		out << line << "\n";
		if (c[0] != '/' || c[1] != '/' || c[2] != '$')
		{
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

			if (!modules.count(module_name) && module_name[0] != '*' && module_name[1] != '\0')
			{
				modules[module_name] = Module { module_value };
			}
			else
			{
				std::cout << "module already exists" << std::endl;
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

			if (modules.count(module_name))
			{
				if(!modules[module_name].set(variable_name, variable_value))
				{
					std::cout << "bad variable" << "\n";
					return 1;
				}
			}
			else if (module_name[0] == '*' && module_name[1] == '\0')
			{
				for (auto it = modules.begin(); it != modules.end(); ++it)
				{
					if (!it->second.set(variable_name, variable_value))
					{
						std::cout << "bad variable" << "\n";
						return 1;
					}
				}
			}
			else
			{
				std::cout << "bad module" << std::endl;
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

			if (modules.count(module_name))
			{
				if (!modules[module_name].hashes.count(version_name) && version_name[0] != '*' && version_name[1] != '\0')
				{
					modules[module_name].hashes[version_name] = hash_value;
				}
				else
				{
					std::cout << "module already has version by that name" << std::endl;
					return 1;
				}
			}
			else
			{
				std::cout << "bad module" << std::endl;
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

			if (modules.count(module_name))
			{
				if (!modules[module_name].symbols.count(symbol_name))
				{
					modules[module_name].symbols[symbol_name] = Symbol { !strcmp(required_value, "required") };
				}
				else
				{
					std::cout << "module already has symbol by that name" << std::endl;
					return 1;
				}
			}
			else
			{
				std::cout << "bad module" << std::endl;
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

			if (modules.count(module_name))
			{
				if (modules[module_name].symbols.count(symbol_name))
				{
					if (!modules[module_name].symbols[symbol_name].addresses.count(version_name) && modules[module_name].hashes.count(version_name))
					{
						modules[module_name].symbols[symbol_name].addresses[version_name] = address_value;
					}
					else if (version_name[0] == '*' && version_name[1] == '\0')
					{
						for (auto it = modules[module_name].hashes.begin(); it != modules[module_name].hashes.end(); ++it)
						{
							if (!modules[module_name].symbols[symbol_name].addresses.count(it->first))
							{
								modules[module_name].symbols[symbol_name].addresses[it->first] = address_value;
							}
						}
					}
					else
					{
						std::cout << "bad version name" << std::endl;
						return 1;
					}
				}
				else
				{
					std::cout << "module does not have symbol" << std::endl;
					return 1;
				}
			}
			else
			{
				std::cout << "bad module" << std::endl;
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
					std::cout << "module does not have symbol" << std::endl;
					return 1;
				}
			}
			else
			{
				std::cout << "bad module" << std::endl;
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
					std::cout << "module does not have symbol" << std::endl;
					return 1;
				}
			}
			else
			{
				std::cout << "bad module" << std::endl;
				return 1;
			}
		}
		else if (!std::strcmp(command, "emit"))
		{
			const char* emit_section = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* emit_macro_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c != '\0')
			{
				*c++ = '\0';
				while (std::isspace(*c)) ++c;
				if (*c != '\0') return 1;
			}

			if (!std::strcmp(emit_section, "attach"))
			{
				++out_line_number;
				out << "#line " << out_line_number + 1 << " \"" << argv[2] << "\"" << "\n";

				for (auto it = modules.begin(); it != modules.end(); ++it)
				{
					++out_line_number;
					out << "std::unordered_map<std::string, std::string> hashes = { ";
					for (auto jt = it->second.hashes.begin(); jt != it->second.hashes.end(); ++jt)
					{
						out << "{ \"" << jt->second << "\", \"" << jt->first << "\" }, ";
					}
					out << "};\n";


					++out_line_number;
					out << "HINSTANCE hiModule = GetModuleHandleA(" << it->second.value << ");" \
						"CHAR lpPath[MAX_PATH + 1];" \
						"GetModuleFileNameW(" << it->second.value << ", lpPath, MAX_PATH + 1);" \
						"std::string module_hash = md5sum(lpPath);" \
						"std::string version_name = hashes.count(module_hash) ? hashes[module_hash] : \"\";" << "\n";

					std::string export_prefix = it->second.export_prefix;

					for (auto jt = it->second.symbols.begin(); jt != it->second.symbols.end(); ++jt)
					{
						++out_line_number;
						out << "std::unordered_map<std::string, std::string> addresses = { ";
						for (auto kt = jt->second.addresses.begin(); kt != jt->second.addresses.end(); ++kt)
						{
							out << "{ \"" << kt->first << "\", " << kt->second << " }, ";
						}
						out << "};\n";

						++out_line_number;
						out << "std::vector<std::vector<std::uint16_t>> patterns = { ";
						for (auto kt : jt->second.patterns)
						{
							out << "{ ";
							for (std::uint16_t x : kt)
							{
								out << x << ", ";
							}
							out << " }, ";
						}
						out << "};\n";

						++out_line_number;
						out << "void* " << export_prefix << jt->first << " = nullptr; if (addresses.count(version_name)) { " << export_prefix << jt->first << " = addresses[version_name];";
						if (!it->second.base_address.empty())
						{
							out << export_prefix << jt->first << " = " << export_prefix << jt->first << " - " << it->second.base_address << " + hiModule;";
						}
						out << "} else { for (auto pattern : patterns) { if (" << export_prefix << jt->first << " = find_pattern(hiModule, pattern)) { break; } } if (!" << export_prefix << jt->first << ") { do {";
						for (auto kt : jt->second.pattern_reads)
						{
							out << "if (" << export_prefix << jt->first << " = find_pattern({ ";
							for (std::uint16_t x : kt.pattern)
							{
								out << x << ", ";
							}
							out << " }) { " << export_prefix << jt->first << " = *(void**)(" << export_prefix << jt->first << kt.read_offset_value << "); break; }";
						}
						out << "} while(false); } if (!" << export_prefix << jt->first << " && " << jt->second.required << ") { assert(false); }}\n";

						++out_line_number;
						if (!jt->second.required)
						{
							out << "if (" << export_prefix << jt->first << ") {";
						}
						out << it->second.emit_prefix << emit_macro_name << "(" << jt->first << ");";
						if (!jt->second.required)
						{
							out << "}";
						}
						out << "\n";
					}
				}

				++out_line_number;
				out << "#line " << in_line_number + 1 << " \"" << argv[1] << "\"" << "\n";
			}
			else if (!std::strcmp(emit_section, "detach"))
			{
				++out_line_number;
				out << "#line " << out_line_number + 1 << " \"" << argv[2] << "\"" << "\n";

				for (auto it = modules.begin(); it != modules.end(); ++it)
				{
					std::string emit_prefix = it->second.emit_prefix;

					for (auto jt = it->second.symbols.begin(); jt != it->second.symbols.end(); ++jt)
					{
						++out_line_number;
						if (!jt->second.required)
						{
							out << "if (" << it->second.export_prefix << jt->first << ") {";
						}
						out << it->second.emit_prefix << emit_macro_name << "(" << jt->first << ");";
						if (!jt->second.required)
						{
							out << "}";
						}
						out << "\n";
					}
				}

				++out_line_number;
				out << "#line " << in_line_number + 1 << " \"" << argv[1] << "\"" << "\n";
			}
			else
			{
				std::cout << "bad emit section" << std::endl;
				return 1;
			}
		}
		else
		{
			std::cout << "bad command" << std::endl;
			return 1;
		}
	}

	return 0;
}
