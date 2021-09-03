#include "reassembler.hpp"

// example
int main()
{
	try {
		std::cout << "Searching for \"input.txt\"...\n";
		std::ifstream file(reassembler::get_path_to_input());
		std::cout << "Reassembling data...\n";
		auto reassembly_result = reassembler::reassemble_data(file);
		if (reassembly_result.has_value()) {
			std::cout << "Reassembly successful, the result is:\n"
					  << *reassembly_result;
		} else {
			std::cout << "Reassembly failed.\n";
		}
	}
	catch (std::exception& e) {
		std::cerr << "Caught a standard exception:\n"
				  << "--------------------\n"
				  << e.what() << "--------------------\n";
	}
	catch (...) {
		std::cerr << "Caught an unknown exception :(";
	}
	return 0;
}
