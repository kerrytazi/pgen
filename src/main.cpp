#include "pch.hpp"
#include "../libs/pgen-lib/src/pgen.hpp"


int main(int argc, const char **argv)
{
	std::string output_file;
	std::string input_data;
	std::string output_namespace;

	int positional = 0;

	for (int i = 1; i < argc; ++i)
	{
		if (std::string_view(argv[i]) == "--output")
		{
			if (++i >= argc) break;
			output_file = argv[i];
		}
		else
		if (std::string_view(argv[i]) == "--input")
		{
			if (++i >= argc) break;

			std::stringstream buffer;

			{
				std::ifstream t(argv[i]);
				buffer << t.rdbuf();
			}

			input_data = buffer.str();
		}
		else
		if (std::string_view(argv[i]) == "--namespace")
		{
			if (++i >= argc) break;
			output_namespace = argv[i];
		}
		if (std::string_view(argv[i]) == "--")
		{
			std::stringstream buffer;

			{
				std::ifstream t(argv[i]);
				buffer << std::cin.rdbuf();
			}

			input_data = buffer.str();
		}
		else
		{
			switch (positional)
			{
				case 0:
					output_file = argv[i];
					break;
				case 1:
					{
						std::stringstream buffer;

						{
							std::ifstream t(argv[i]);
							buffer << t.rdbuf();
						}

						input_data = buffer.str();
					}
					break;
			}

			++positional;
		}
	}

	if (output_file.empty() || input_data.empty())
	{
		std::cout << "Invalid params\n";
		std::cout << "Example usage:\n";
		std::cout << "\tpgen output.cpp rules.txt\n";
		std::cout << "\tpgen --output output.cpp --input rules.txt --namespace test\n";
		std::cout << "\tcat rules.txt | pgen output.cpp --\n";

		return 1;
	}

	auto rules = pgen::parse(input_data.data(), input_data.size());

	pgen::helpers::GenerateCodeParams params;
	params.custom_namespace = output_namespace;

	std::ofstream(output_file) << pgen::helpers::generate_code(rules, params);

	return 0;
}

