/*
 * DataTypeParser.h
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

#ifndef DATATYPEPARSER_H_
#define DATATYPEPARSER_H_

#include <string>
#include <unordered_map>
#include <vector>

/*
 *
 */
class DataTypeParser {
public:
	DataTypeParser();
	virtual ~DataTypeParser();
	std::string GetPattern(const std::string& dtype) const;
	uint32_t UnsignedInteger(const std::string &word) const;
	uint32_t PositiveInteger(const std::string &word) const;
	int32_t Integer(const std::string &word) const;
	double UnsignedDecimal(const std::string &word) const;
	double Decimal(const std::string &word) const;
	std::string String(const std::string &word) const;
	std::string Field(const std::string &word) const;
	std::string Name(const std::string &word) const;
	std::string Match(const std::string &word, const std::string &dtype) const;
	static std::vector<std::string> SplitFields(const std::string &word);
	static std::string GetCommandCode(const std::string &word);


private:
	const std::unordered_map<std::string, std::string> m_patterns;

};

#endif /* DATATYPEPARSER_H_ */
