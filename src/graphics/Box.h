/*
 * Box.h
 *
 *  Created on: Oct 19, 2024
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
#ifndef BOX_H_
#define BOX_H_

namespace gerbex {

class Box {
public:
	Box();
	Box(double width, double height, double left, double bottom);
	virtual ~Box() = default;
	double GetHeight() const;
	double GetWidth() const;
	double GetBottom() const;
	double GetLeft() const;
	Box Extend(const Box &other);

private:
	double m_width, m_height;
	double m_left, m_bottom;
};

} /* namespace gerbex */

#endif /* BOX_H_ */
