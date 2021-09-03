#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <cctype>
#include <cmath>
#include <stdexcept>

/*
 *										STEPIK.ORG CHALLENGE

 * Задача простая - найти производную многочлена.
 * Многочлен может быть большой, но только с неотрицательными целыми степенями и целыми коэффициентами.
 * Подумайте, каким образом его лучше представить в памяти?
 * Вывод должен быть без пробелов и в порядке убывания степеней, хотя ввод этому правилу может не удовлетворять.
 * Также на ввод может быть подан многочлен с неприведенными слагаемыми.
 */

using power_t = uint32_t;
using coeff_t = int32_t;

std::pair<power_t, coeff_t> parse_term(std::istringstream& iss, bool leading)
{
	std::string coeff;
	bool hasX;
	std::string power;
	char ch = iss.get();
	auto skip_spaces = [&ch, &iss]() -> void { while (ch == ' ') ch = iss.get(); };
	auto make_ret = [&coeff, &power, &hasX]()
	{
		power_t power_ = hasX ? 1u : 0u;
		coeff_t coeff_ = 1;

		if (!power.empty()) power_ = (power_t)std::stoull(power);
		if (!coeff.empty() && !(coeff == "+" || coeff == "-")) coeff_ = (coeff_t)std::stoll(coeff);
		if (coeff == "+") coeff_ = 1;
		if (coeff == "-") coeff_ = -1;
		return std::make_pair(power_, coeff_);
	};
	auto read_unsigned_num = [&ch, &iss](std::string& to) -> void
	{
		while (std::isdigit((unsigned char)ch) != 0)
		{
			to.push_back(ch);
			ch = iss.get();
		}
	};

	skip_spaces();

	if (ch == '+' || ch == '-')
	{
		coeff.push_back(ch);
		ch = iss.get();
	}
	else if (!leading)
		throw std::logic_error("'+' or '-' sign missing");

	skip_spaces();

	read_unsigned_num(coeff);

	skip_spaces();

	if (!coeff.empty() && coeff != "+" && coeff != "-")	// there is a non-empty coeff
	{
		if (ch == '*')	// expect x{^(some power)} next
		{
			ch = iss.get();
			hasX = true;
		}
		else			// expect ch == +/-/EOF
		{
			if (ch != EOF && ch != '+' && ch != '-')
			{
				std::string msg = "'+' or '-' expected instead of "; msg += ch;
				throw std::logic_error(msg);
			}
			iss.unget();
			hasX = false;
			return make_ret();
		}
	}

	skip_spaces();

	if (ch != 'x') { throw std::logic_error(std::string("'x' expected instead of ") + ch); }
	else ch = iss.get();

	skip_spaces();

	if (ch != '^')
	{
		iss.unget();
		return make_ret();
	}
	else ch = iss.get();

	skip_spaces();

	if (std::isdigit((unsigned char)ch) == 0) throw std::logic_error("number expected!");

	read_unsigned_num(power);

	iss.unget();

	return make_ret();
}

std::map<power_t, coeff_t> parse_polynomial(std::string poly)
{
	std::istringstream iss(poly);
	std::map<power_t, coeff_t> poly_terms;
	poly_terms.insert(parse_term(iss, true));
	while (iss.get() != EOF)
	{
		iss.unget();
		auto term = parse_term(iss, false);
		auto it = poly_terms.find(term.first);
		if (it == poly_terms.end())
		{
			poly_terms.insert(term);
		}
		else
		{
			it->second += term.second;
		}
	}

	for (auto it = poly_terms.begin(); it != poly_terms.end();)
	{
		if (it->second != 0) ++it;
		else it = poly_terms.erase(it);
	}

	if (poly_terms.empty()) poly_terms.insert(std::make_pair(0u, 0));

	return poly_terms;
}



std::string derivative(std::string poly)
{
	auto poly_terms = parse_polynomial(poly);
	std::map<power_t,coeff_t> deriv_terms;
	for (auto p_it = poly_terms.cbegin(); p_it != poly_terms.cend(); ++p_it)
	{
		const power_t& power = p_it->first;
		const coeff_t& coeff = p_it->second;
		if (power != 0u)
		{
			deriv_terms.insert(std::make_pair(power - 1, coeff * coeff_t(power) ));
		}
	}

	if (deriv_terms.empty())
		deriv_terms.insert(std::make_pair(0u, 0));

	std::ostringstream deriv_poly;
	for (auto d_it = deriv_terms.crbegin(); d_it != deriv_terms.crend(); ++d_it)
	{
		const power_t& power = d_it->first;
		const coeff_t& coeff = d_it->second;
		std::string leading_sign = (coeff < 0) ? "-" : ((d_it != deriv_terms.crbegin()) ? "+" : "");
		deriv_poly << leading_sign;

		if ((power == 0u) || (std::abs(coeff) != 1))
			deriv_poly << std::abs(coeff);

		if (power > 0u)
		{
			if (power == 1u) deriv_poly << "*x";
			else deriv_poly << "*x^" << power;
		}
	}

	return deriv_poly.str();
}

int main()
{
	std::ifstream is;
	std::string poly;
	is.open("input.txt");
	while (std::getline(is, poly))
	{
		std::cout << "Polynomial: " << poly << std::endl;
		try
		{
			std::cout << "Derivative: " << derivative(poly) << std::endl;
		}
		catch (std::logic_error& e)
		{
			std::cout << "Error: " << e.what() << "\n";
		}
	}

	is.clear();
	is.close();
	std::cout << "\nPress Enter to quit\n";
	std::cin.get();
	return 0;
}