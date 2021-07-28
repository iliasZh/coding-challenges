#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <stdexcept>
#include <cctype>


/*
 *										HACKERRANK CHALLENGE

 * This challenge works with a custom-designed markup language HRML.
 * In HRML, each element consists of a starting and ending tag, and there are attributes associated with each tag. 
 * Only starting tags can have attributes. We can call an attribute by referencing the tag,
 * followed by a tilde, '~' and the name of the attribute. The tags may also be nested.
 * 
 * The opening tags follow the format:
 * <tag-name attribute1-name = "value1" attribute2-name = "value2" ...>
 * 
 * The closing tags follow the format:
 * </tag-name>
 * 
 * The attributes are referenced as:
 * tag1~value  
 * tag1.tag2~name
 * 
 * Given the source code in HRML format consisting of N lines, answer Q queries.
 * For each query, print the value of the attribute specified.
 * Print "Not Found!" if the attribute does not exist.
 */


namespace parser_details
{
	namespace definitions
	{
		using std::string;
		using std::vector;
		using string_list = vector<string> ;
		using string_pair = std::pair<string, string>;

		struct tag_desc
		{
			bool is_opening;
			string name;
			vector<string_pair> attr_list;
		};

		struct query_desc
		{
			string_list tag_path;
			string attr_name;
		};
	}
	using namespace definitions;

	void throw_err(string s) { throw std::logic_error(std::move(s)); }

	bool isalnum_str(string::const_iterator begin, string::const_iterator end) noexcept
	{
		for (auto it = begin; it != end; ++it)
		{
			if (std::isalnum((unsigned char)(*it)) == 0 && (*it != '_')) return false;
		}
		return true;
	}

	void check_name(const string& name)
	{
		if (name.empty()) throw_err("empty name not allowed!");

		const bool is_first_char_alpha = (std::isalpha((unsigned char)name.front()) != 0);
		const bool is_rest_alnum = isalnum_str(name.begin() + 1, name.end());
		bool valid = is_first_char_alpha && is_rest_alnum;
		if (!valid) throw_err("invalid name: " + name);
	}

	void check_value(const string& val)
	{
		if (!isalnum_str(val.begin(), val.end())) throw_err("invalid value: " + val);
	}

	string_list parse_string(string str, char delim = ' ')
	{
		string_list parsed_tokens;
		string token;
		std::istringstream iss{ str };
		while (std::getline(iss, token, delim))
			parsed_tokens.push_back(std::move(token));
		return parsed_tokens;
	}

	void remove_enclosure(string& str, char open, char close)
	{
		if (str.size() < 2u) throw_err("string \"" + str + "\" is too small");
		if (str.front() == open && str.back() == close)
		{
			str.erase(0u, 1u); str.pop_back(); return;
		}
		
		string enc; enc += open; enc += close;
		throw_err("--->   " + str + "   <--- proper enclosure expected: " + enc);
	}

	tag_desc parse_closing_tag(string_list tokens)
	{
		if (tokens.size() != 1) throw_err("closing tag cannot have attributes!");

		tag_desc td; td.is_opening = false;
		string tag_name = tokens.front(); tag_name.erase(0u, 1u); // delete '/'
		check_name(tag_name);
		td.name = tag_name;
		return td;
	}

	tag_desc parse_opening_tag(string_list tokens)
	{
		// valid opening tag: "tag_name", "attr_name" "=" "attr_val", "attr_name" "=" "attr_val" ...
		if (tokens.size() % 3u != 1u) throw_err("opening tag invalid!");

		tag_desc td; td.is_opening = true;
		string tag_name = tokens.front();
		check_name(tag_name);
		td.name = tag_name;

		for (auto it = tokens.begin() + 1; it != tokens.end(); it += 3)
		{
			if (*(it + 1) != "=") throw_err("'=' expected instead of '" + *(it + 1) + "'");

			string attr_name = *it;			// e.g. attrname
			check_name(attr_name);

			string attr_val = *(it + 2);	// e.g. "attrval"
			remove_enclosure(attr_val, '\"', '\"');
			check_value(attr_val);

			td.attr_list.push_back({ attr_name, attr_val });
		}
		return td;
	}
}

namespace parser_implementation
{
	using namespace parser_details::definitions;
	using namespace parser_details;

	tag_desc parse_tag(std::istream& is)
	{
		string line;
		std::getline(is, line);
		remove_enclosure(line, '<', '>');
		
		string_list tokens = parse_string(line, ' ');
		if (tokens.empty()) throw_err("empty brackets <>!");
		if (tokens.front().front() != '/')
			return parse_opening_tag(tokens);
		else
			return parse_closing_tag(tokens);
	}

	query_desc parse_query(std::istream& is)
	{
		query_desc qd; string line;
		std::getline(is, line);
		string_list tmp = parse_string(line, '~');
		if (tmp.size() == 2u)
		{
			qd.tag_path = parse_string(tmp.front(), '.');
			for (auto& str : qd.tag_path)
				check_name(str);
			qd.attr_name = tmp.back();
			check_name(qd.attr_name);
		}
		else throw_err("exactly one access operator '~' permitted/needed!");
		return qd;
	}

	class HRML_listing
	{
	public:
		void read(size_t lines, std::istream& is = std::cin)
		{
			if (lines % 2u == 1u) throw_err("number of lines must be even!");
			for (size_t i = 0u; i < lines; ++i)
				read_line(is);
			if (!tag_stack.empty()) throw_err("unclosed tag: " + tag_stack.back());
		}
		void query(const query_desc& qd, std::ostream& os = std::cout)
		{
			if (listing.empty()) throw_err("listing empty, read() before query()-ing!");

			int queried_tag_line = find_queried_tag(qd, os);
			if (queried_tag_line == -1) return;

			for (const auto& attr : listing[queried_tag_line].attr_list)
				if (attr.first == qd.attr_name)
				{
					os << attr.second << "\n";
					return;
				}
			os << "'" + qd.attr_name + "' attribute not found!\n";
			return;
		}
	private:
		int find_queried_tag(const query_desc& qd, std::ostream& os)
		{
			int curr_tag = -1;
			for (size_t i = 0; i < qd.tag_path.size(); ++i)
			{
				const string& tag_name = qd.tag_path[i];
				int next_tag = find_tag_index(tag_name, curr_tag);
				if (next_tag < curr_tag)
				{
					os << "tag '" + tag_name + "' not found!\n";
					return -1;
				}

				for (int n = curr_tag + 1; n <= next_tag; ++n)
					send_to_stack(listing[n], false);

				if (tag_stack.size() != i + 1)
				{
					os << "correct path to '" + tag_name + "' is: " + get_path_from_stack() + "\n";
					tag_stack = {};
					return -1;
				}

				curr_tag = next_tag;
			}
			tag_stack = {};
			return curr_tag;
		}
		string get_path_from_stack()
		{
			// !stack.empty()
			std::ostringstream path;
			for (const auto& tag : tag_stack)
				path << tag << "->";
			string str = path.str();
			str.erase(str.cend() - 2, str.cend());
			return str;
		}
		void send_to_stack(const tag_desc& td, bool check)
		{
			if (td.is_opening)
				tag_stack.push_back(td.name);
			else
			{
				if (check && tag_stack.empty()) throw_err("loose closing tag: " + td.name);

				if (check && (td.name != tag_stack.back()))
					throw_err("'" + tag_stack.back() + "' closing tag expected instead of '" + td.name + "'");
				else
					tag_stack.pop_back();
			}
		}
		void check_for_copies(const tag_desc& td)
		{
			// check listing for copies of td
			for (const auto& tag : listing)
			{
				if (!(tag.is_opening xor td.is_opening) && tag.name == td.name)
				{
					string type = (tag.is_opening ? "opening" : "closing");
					throw_err("two " + type + " '" + tag.name + "' tags");
				}
			}
		}
		void read_line(std::istream& is)
		{
			tag_desc td = parse_tag(is);
			check_for_copies(td);
			send_to_stack(td, true);
			listing.push_back(td);
		}
		int find_tag_index(const std::string& tag_name, const int start_index)
		{
			for (int i = start_index + 1; i < (int)listing.size(); ++i)
			{
				if (listing[i].name == tag_name && listing[i].is_opening)
					return i;
			}
			return -1000;
		}
	private:
		vector<tag_desc> listing;
		string_list tag_stack;
	};
}

namespace parser
{
	using parser_implementation::HRML_listing;
	using parser_implementation::parse_query;
	using parser_details::definitions::query_desc;
}

int main()
{
	try
	{
		std::ifstream in;
		in.open("input.txt");

		std::istream& is = in;

		size_t n, q;
		is >> n; is >> q;
		is >> std::ws; // discard first line's \n

		parser::HRML_listing listing;
		listing.read(n, is >> std::ws);
		
		for (size_t i = 0u; i < q; ++i)
		{
			parser::query_desc qd = parser::parse_query(is >> std::ws);
			listing.query(qd);
		}
		in.clear();
		in.close();
	}
	catch (std::logic_error& e)
	{
		std::cout << "Error: " << e.what();
	}
	
	std::cout << "\nPress enter to exit\n";
	std::cin.get();
	return 0;
}