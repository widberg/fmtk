#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>

struct FunctionParameter
{
	std::string parameter_type;
	std::string parameter_name;
	std::string parameter_register;
};

struct FunctionSignature
{
	FunctionSignature() {}
	FunctionSignature(const std::string& data)
	{
		const char* c = data.c_str();

		while (std::isspace(*c)) ++c;
		if (*c == '\0') return;

		const char* return_type_begin = c;
		while (*c != '\0' && *c != '(' && !std::isspace(*c)) ++c;
		if (*c == '\0') return;
		return_type = std::string(return_type_begin, c - return_type_begin);
		while (std::isspace(*c)) ++c;
		if (*c == '\0') return;

		if (*c == '(')
		{
			function_pointer = true;
			++c;
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return;
		}

		const char* calling_convention_begin = c;
		while (*c != '\0' && !std::isspace(*c)) ++c;
		if (*c == '\0') return;
		calling_convention = std::string(calling_convention_begin, c - calling_convention_begin);
		if (calling_convention == "__usercall")
		{
			calling_convention == "__stdcall";
		}
		else if (calling_convention == "__userpurge")
		{
			calling_convention == "__cdecl";
		}
		while (std::isspace(*c)) ++c;
		if (*c == '\0') return;

		if (function_pointer)
		{
			if (*c != '*') return;
			++c;
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return;
		}

		const char* function_name_begin = c;

		if (function_pointer)
		{
			if (*c != ')') return;
			++c;
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return;
		}
		else
		{
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return;
			function_name = std::string(function_name_begin, c - function_name_begin);
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return;
		}

		if (*c == '@')
		{
			++c;
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return;

			const char* return_register_begin = c;
			while (*c != '\0' && *c != '(' && !std::isspace(*c)) ++c;
			if (*c == '\0') return;
			return_register = std::string(return_register_begin, c - return_register_begin);
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return;
		}

		if (*c != '(') return;
		++c;
		while (std::isspace(*c)) ++c;
		if (*c == '\0') return;

		while (*c != '\0')
		{
			if (*c == ')') break;

			FunctionParameter parameter;

			const char* parameter_type = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return;
			parameter.parameter_type = std::string(parameter_type, c - parameter_type);
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return;

			const char* parameter_name = c;
			while (*c != '\0' && !std::isspace(*c)) ++c;
			if (*c == '\0') return;
			parameter.parameter_name = std::string(parameter_name, c - parameter_name);
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return;

			if (*c == '@')
			{
				++c;
				while (std::isspace(*c)) ++c;
				if (*c == '\0') return;

				const char* parameter_register = c;
				while (*c != '\0' && *c != ')' && !std::isspace(*c)) ++c;
				if (*c == '\0') return;
				parameter.parameter_register = std::string(parameter_register, c - parameter_register);
				while (std::isspace(*c)) ++c;
				if (*c == '\0') return;
			}

			parameters.push_back(parameter);

			if (*c == ')') break;
			if (*c != ',') return;
			++c;
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return;
		}

		if (*c != ')') return;
		++c;
		while (std::isspace(*c)) ++c;
		if (*c != '\0') return;

		valid = true;
	}

	bool function_pointer = false;
	bool valid = false;
	std::string return_type;
	std::string calling_convention;
	std::string function_name;
	std::string return_register;
	std::vector<FunctionParameter> parameters;
};

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
	std::size_t usercall_function_depth = 0;
	FunctionSignature usercall_function_signature;

	std::size_t in_line_number = 0;
	std::size_t out_line_number = 0;

	++out_line_number;
	out << "#line " << 1 << " \"" << argv[1] << "\"" << "\n";

	std::string line;
	while (std::getline(in, line))
	{
		++in_line_number;

		char* c = line.data();
		while (std::isspace(*c)) ++c;

		if (c[0] != '/' || c[1] != '/' || c[2] != '$')
		{
			if (usercall_function_depth)
			{
				usercall_function_depth = usercall_function_depth + std::count(line.begin(), line.end(), '{') - std::count(line.begin(), line.end(), '}');

				if (line.find("return") != std::string::npos)
				{
					++out_line_number;
					while (true)
					{
						std::size_t pos = line.find("return");
						if (pos == std::string::npos) break;
						if ((pos == 0 || std::isspace(line[pos - 1])) && (pos == line.length() - 6 || (std::isspace(line[pos + 6]) || line[pos + 6] == ';')))
						{
							out << line.substr(0, pos);
							line = line.substr(pos);
							std::size_t semicolon_pos = line.find(';');
							if (semicolon_pos == std::string::npos) return 1;
							std::string return_statement = line.substr(6, semicolon_pos - 6);

							out << "__asm { pop esi };__asm { pop edi };__asm { pop edx };__asm { pop ecx };__asm { pop ebx };__asm { pop eax };";
							if (!return_statement.empty() && !std::all_of(return_statement.begin(), return_statement.end(), std::isspace))
							{
								out << usercall_function_signature.return_type << " _usercall_internal_return = " << return_statement << ";__asm {mov " << usercall_function_signature.return_register << ", _usercall_internal_return};";
							}

							out << "return;";

							line = line.substr(semicolon_pos + 1);
						}
					}
					out << line << "\n";
				}
				else
				{
					++out_line_number;
					out << line << "\n";
				}
			}
			else
			{
				++out_line_number;
				out << line << "\n";
			}

			continue;
		}

		++out_line_number;
		out << line << "\n";
		line.push_back('\0');

		c += 3;
		while (std::isspace(*c)) ++c;
		if (*c == '\0') return 1;

		const char* command = c;
		while (*c != '\0' && !std::isspace(*c)) ++c;
		*c++ = '\0';
		while (std::isspace(*c)) ++c;

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
			if (*c == '\0') return 1;
			*c++ = '\0';
			while (std::isspace(*c)) ++c;
			if (*c == '\0') return 1;

			const char* type_value = c;

			if (modules.count(module_name))
			{
				if (!modules[module_name].symbols.count(symbol_name))
				{
					modules[module_name].symbols[symbol_name] = Symbol { !strcmp(required_value, "required"), type_value };
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
						if (!modules[module_name].symbols[symbol_name].addresses.count(""))
						{
							modules[module_name].symbols[symbol_name].addresses[""] = address_value;
						}
						else
						{
							std::cout << "This symbol already has a wildcard" << std::endl;
							return 1;
						}

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
		else if (!std::strcmp(command, "usercall"))
		{
			if (*(c - 1) != '\0') return 1;

			std::string signature;
			std::string line;
			++in_line_number;
			while (std::getline(in, line) && line.find('{') == std::string::npos)
			{
				++in_line_number;
				signature += " " + line;
			}

			signature += line.substr(0, line.find('{'));
			line = line.substr(line.find('{') + 1);

			++out_line_number;
			out << "#line " << out_line_number + 1 << " \"" << argv[2] << "\"" << "\n";

			FunctionSignature function_signature(signature);
			if (!function_signature.valid || function_signature.function_pointer) return 1;

			++out_line_number;
			out << function_signature.return_type << " " << function_signature.calling_convention << " " << function_signature.function_name << "(";
			bool first = true;
			for (auto parameter : function_signature.parameters)
			{
				if (parameter.parameter_register.empty())
				{
					if (!first)
					{
						out << ", ";
					}
					else
					{
						first = false;
					}

					out << parameter.parameter_type << " " << parameter.parameter_name;
				}
			}
			out << "){__asm { push eax };__asm { push ebx };__asm { push ecx };__asm { push edx };__asm { push edi };__asm { push esi };";

			for (auto parameter : function_signature.parameters)
			{
				if (!parameter.parameter_register.empty())
				{
					out << parameter.parameter_type << " " << parameter.parameter_name << "; __asm {mov " << parameter.parameter_name << ", " << parameter.parameter_register << "};";
				}
			}
			out << "\n";

			++out_line_number;
			out << "#line " << in_line_number + 1 << " \"" << argv[1] << "\"" << "\n";
			in.seekg(-line.length(), std::ios::cur);

			usercall_function_depth = 1;
			usercall_function_signature = function_signature;
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
						"std::string module_hash = MD5(lpPath);" \
						"std::string version_name = hashes.count(module_hash) ? hashes[module_hash] : \"\";" << "\n";

					std::string export_prefix = it->second.export_prefix;

					for (auto jt = it->second.symbols.begin(); jt != it->second.symbols.end(); ++jt)
					{
						++out_line_number;
						out << "std::unordered_map<std::string, " << jt->second.type << "> addresses = { ";
						for (auto kt = jt->second.addresses.begin(); kt != jt->second.addresses.end(); ++kt)
						{
							out << "{ \"" << kt->first << "\", static_cast<" << jt->second.type << ">(" << kt->second << ") }, ";
						}
						out << "};\n";

						++out_line_number;
						out << "std::vector<std::vector<std::uint16_t>> patterns = { ";
						for (auto kt : jt->second.patterns)
						{
							out << "{ ";
							for (std::uint16_t x : kt)
							{
								out << "0x" << std::hex << x << ", ";
							}
							out << " }, ";
						}
						out << "};\n";

						++out_line_number;
						out << "auto " << export_prefix << jt->first << " = static_cast<" << jt->second.type << ">(nullptr); if (addresses.count(version_name)) { " << export_prefix << jt->first << " = addresses[version_name];";
						if (!it->second.base_address.empty())
						{
							out << export_prefix << jt->first << " = " << export_prefix << jt->first << " - " << it->second.base_address << " + hiModule;";
						}
						out << "} else { for (auto pattern : patterns) { if (" << export_prefix << jt->first << " = FIND_PATTERN(hiModule, pattern)) { break; } } if (!" << export_prefix << jt->first << ") { do {";
						for (auto kt : jt->second.pattern_reads)
						{
							out << "if (" << export_prefix << jt->first << " = static_cast<" << jt->second.type << ">(FIND_PATTERN({ ";
							for (std::uint16_t x : kt.pattern)
							{
								out << "0x" << std::hex << x << ", ";
							}
							out << " }))) { " << export_prefix << jt->first << " = *(void**)(" << export_prefix << jt->first << kt.read_offset_value << "); break; }";
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
