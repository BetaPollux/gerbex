/*
 * FileParser.h
 *
 *  Created on: Mar. 24, 2024
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

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include "DataTypeParser.h"
#include <istream>
#include <memory>
#include <string>
#include <list>

namespace gerbex {

/*
 * Takes an input stream and returns Gerber words
 */
class FileParser {
public:
	FileParser();
	FileParser(std::unique_ptr<std::istream> stream);
	virtual ~FileParser();
	// Get the next command from the stream. Word commands are a single word, whereas extended commands can have multiple words.
	// Commands can span multiple lines in the file.
	// Words always end in *. Returns an empty vector on EOF.
	Fields GetNextCommand();
	void SetIstream(std::unique_ptr<std::istream> istream);
	int GetCurrentLine() const;

private:
	std::unique_ptr<std::istream> m_istream;
	int m_currentLine;
};

} /* namespace gerbex */

#endif /* FILEPARSER_H_ */
