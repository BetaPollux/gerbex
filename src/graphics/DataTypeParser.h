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
#include <vector>

namespace gerbex {

/*
 *
 */
class DataTypeParser {
public:
	static std::string Match(const std::string &word, const std::string &pattern);
	static std::vector<std::string> SplitFields(const std::string &word);
	static std::vector<double> SplitParams(const std::string &field, char delim);
	static std::string GetCommandCode(const std::string &word);
	static const std::string GetNumberPattern();
	static const std::string GetNamePattern();
	static const std::string GetFieldPattern();

};

} /* namespace gerbex */

#endif /* DATATYPEPARSER_H_ */
