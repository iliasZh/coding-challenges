#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric> // std::accumulate
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace reassembler_impl
{
namespace fsys = std::filesystem;
struct data_segment
{
	int			index		   = -1;
	short		data_type	   = -1;
	short		end_flag	   = -1;
	short		content_length = -1;
	std::string content{};
};

std::vector<data_segment> read_packet(std::istream& input_stream)
{
	size_t num_of_segments = 0U;
	input_stream >> num_of_segments;

	std::vector<data_segment> packet(num_of_segments);
	for (auto& segment : packet) {
		input_stream >> segment.data_type >> segment.index >>
			segment.end_flag >> segment.content_length >> segment.content;
	}
	return packet;
}

void filter_out_unneeded_data_types(std::vector<data_segment>& packet,
									short					   target_data_type)
{
	auto data_type_filter = [target_data_type](const data_segment& ds) {
		return ds.data_type == target_data_type;
	};
	auto packet_new_end =
		std::partition(packet.begin(), packet.end(), data_type_filter);

	packet.erase(packet_new_end, packet.end());
	packet.shrink_to_fit();
}

void stable_sort_by_index(std::vector<data_segment>& packet)
{
	auto comp = [](const data_segment& lhs, const data_segment& rhs) {
		return lhs.index < rhs.index;
	};
	std::stable_sort(packet.begin(), packet.end(), comp);
}

void remove_duplicates(std::vector<data_segment>& packet)
{
	auto equal_indexes = [](const data_segment& lhs, const data_segment& rhs) {
		return lhs.index == rhs.index;
	};
	auto non_unique_begin =
		std::unique(packet.begin(), packet.end(), equal_indexes);
	packet.erase(non_unique_begin, packet.end());
}

bool can_reassemble(const std::vector<data_segment>& p)
{
	for (auto it = p.cbegin(); it != p.cend() - 1; ++it) {
		if (it->end_flag != 0 ||
			(size_t)it->content_length != it->content.length()) {
			return false;
		}
	}
	auto it = p.cend() - 1;
	return it->end_flag == 1 &&
		   (size_t)it->content_length == it->content.length();
}

std::optional<std::string> reassembly(const std::vector<data_segment>& packet)
{
	if (can_reassemble(packet)) {
		auto accumulator = [](const std::string& str, const data_segment& ds) {
			return str + ds.content + ' ';
		};
		std::string result = std::accumulate(packet.begin(), packet.end(),
											 std::string{}, accumulator);
		result.pop_back();
		return result;
	}
	return {};
}

/// reassembles the message from the packet described in input_stream
std::optional<std::string> reassemble_data(std::istream& input_stream)
{
	auto packet = read_packet(input_stream);

	short target_data_type = 0;
	input_stream >> target_data_type;

	filter_out_unneeded_data_types(packet, target_data_type);

	stable_sort_by_index(packet);

	remove_duplicates(packet);

	return reassembly(packet);
}

fsys::path get_path_to_src()
{
	auto path = fsys::current_path();
	while (*(--path.end()) != "reassemble-data") {
		path = path.parent_path();
	}
	return path / "src";
}

/// returns a full path to input.txt in /src or /build directories
/// if there's no input.txt, throws
fsys::path get_path_to_input()
{
	auto src_path	= get_path_to_src() / "input.txt";
	auto build_path = fsys::current_path() / "input.txt";

	// prepare error message
	std::stringstream exception_msg{};
	exception_msg << "fatal: failed to locate \"input.txt\"\n"
				  << "search paths are:\n"
				  << src_path << '\n'
				  << build_path << '\n';

	if (fsys::exists(src_path)) {
		return src_path;
	}
	if (fsys::exists(build_path)) {
		return build_path;
	}
	throw std::runtime_error(exception_msg.str());
}
} // namespace reassembler_impl

namespace reassembler
{
using reassembler_impl::get_path_to_input;
using reassembler_impl::reassemble_data;
} // namespace reassembler