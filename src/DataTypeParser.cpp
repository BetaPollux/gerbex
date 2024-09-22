/*
 * DataTypeParser.cpp
 *
 *  Created on: Sep. 17, 2024
 *	Copyright (C) 2024 BetaPollux
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "DataTypeParser.h"
#include <regex>
#include <stdexcept>

DataTypeParser::DataTypeParser() : m_patterns{{"unsigned_integer", "[0-9]+"},
                                              {"positive_integer", "[0-9]*[1-9][0-9]*"},
                                              {"integer", "[+-]?[0-9]+"},
                                              {"decimal", "[+-]?((([0-9]+)(\\.[0-9]*)?)|(\\.[0-9]+))"},
                                              {"unsigned_decimal", "((([0-9]+)(\\.[0-9]*)?)|(\\.[0-9]+))"},
                                              {"string", "[^%*]*"},
                                              {"field", "[^%*,]*"},
                                              {"name", "[._$a-zA-Z][._$a-zA-Z0-9]{0,126}"}}
{
    // Empty
}

DataTypeParser::~DataTypeParser()
{
    // Empty
}

std::string DataTypeParser::GetPattern(const std::string &dtype) const
{
    auto pattern = m_patterns.find(dtype);
    if (pattern == m_patterns.end())
    {
        throw std::invalid_argument("unknown data type");
    }
    return pattern->second;
}

uint32_t DataTypeParser::UnsignedInteger(const std::string &word) const
{
    return std::stoi(Match(word, "unsigned_integer"));
}

uint32_t DataTypeParser::PositiveInteger(const std::string &word) const
{
    return std::stoi(Match(word, "positive_integer"));
}

int32_t DataTypeParser::Integer(const std::string &word) const
{
    return std::stoi(Match(word, "integer"));
}

double DataTypeParser::UnsignedDecimal(const std::string &word) const
{
    return std::stod(Match(word, "unsigned_decimal"));
}

double DataTypeParser::Decimal(const std::string &word) const
{
    return std::stod(Match(word, "decimal"));
}

std::string DataTypeParser::String(const std::string &word) const
{
    return Match(word, "string");
}

std::string DataTypeParser::Field(const std::string &word) const
{
    return Match(word, "field");
}

std::string DataTypeParser::Name(const std::string &word) const
{
    return Match(word, "name");
}

std::string DataTypeParser::Match(const std::string &word, const std::string &dtype) const
{
    auto pattern = m_patterns.find(dtype);
    if (pattern == m_patterns.end())
    {
        throw std::invalid_argument("unknown data type");
    }
    std::smatch match;
    std::regex regex(pattern->second);
    if (std::regex_match(word, match, regex))
    {
        return match[0].str();
    }
    throw std::invalid_argument("invalid " + dtype);
}


std::string DataTypeParser::GetCommandCode(const std::string &word) {
	std::smatch match;
	if (std::regex_search(word, match, std::regex("^([A-Z]{2}|[GM][0-9]{2})"))) {
			return match[0].str();
	} else if (std::regex_search(word, match, std::regex("D([0-9]+)$"))) {
		int ident = std::stoi(match[1].str());
		if (ident < 10) {
			return match[0].str();	// D0n
		} else {
			return "Dnn";
		}
	}
	throw std::invalid_argument("unrecognized word");
}

std::vector<std::string> DataTypeParser::SplitFields(const std::string &word) {
	std::vector<std::string> fields;
    std::istringstream istr(word);

	while (!istr.eof()) {
        std::string field;
        std::getline(istr, field, ',');
		fields.push_back(field);
	}

	return fields;
}

std::vector<double> DataTypeParser::SplitParams(const std::string &field) {
	std::vector<double> params;
    std::istringstream istr(field);

	while (!istr.eof()) {
        std::string param_str;
        std::getline(istr, param_str, 'X');
        try {
        	params.push_back(std::stod(param_str));
        } catch (const std::invalid_argument &ex) {
        	throw std::invalid_argument("invalid non-numeric parameter: " + param_str);
        }
	}

	return params;
}

