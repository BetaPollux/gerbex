/*
 * Polarity.h
 *
 *  Created on: Oct. 27, 2024
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
#ifndef POLARITY_H_
#define POLARITY_H_

#include <string>
#include <stdexcept>

namespace gerbex {

//TODO this class is ridiculous?

class Polarity {
public:
	Polarity() :
			Polarity(true) {
	}
	Polarity(bool isDark) :
			m_isDark { isDark } {
	}
	virtual ~Polarity() = default;
	bool operator==(const Polarity &rhs) const {
		return m_isDark == rhs.m_isDark;
	}
	bool operator!=(const Polarity &rhs) const {
		return m_isDark != rhs.m_isDark;
	}
	static Polarity Dark() {
		return Polarity(true);
	}
	static Polarity Clear() {
		return Polarity(false);
	}
	static Polarity FromCommand(const std::string &str) {
		if (str == "C") {
			return Polarity::Clear();
		} else if (str == "D") {
			return Polarity::Dark();
		} else {
			throw std::invalid_argument("invalid polarity");
		}
	}

private:
	bool m_isDark;
};

} /* namespace gerbex */

#endif /* POLARITY_H_ */
