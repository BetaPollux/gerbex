/*
 * Transform.h
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

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Point.h"
#include <string>
#include <vector>

namespace gerbex {

enum class Polarity {
	Dark, Clear
};

enum class Mirroring {
	None,	// None
	X,		// X-axis
	Y,		// Y-axis
	XY		// Both X and Y axes
};

/*
 * Parameters that transform the polarity and shape of the aperture when it is
 * used to create an object.
 * Mirroring is performed before rotation.
 * Rotation is around the aperture origin.
 * Scaling is centered on the aperture origin.
 */
class Transform {
public:
	Transform();
	Transform(Polarity polarity, Mirroring mirroring, double rotation,
			double scaling);
	virtual ~Transform() = default;
	Transform Stack(const Transform &transform) const;
	bool operator==(const Transform &rhs) const;
	bool operator!=(const Transform &rhs) const;
	Mirroring GetMirroring() const;
	void SetMirroring(Mirroring mirroring);
	Polarity GetPolarity() const;
	void SetPolarity(Polarity polarity);
	double GetRotation() const;
	void SetRotation(double degrees);
	double GetScaling() const;
	void SetScaling(double factor);
	Point Apply(const Point &point) const;
	std::vector<Point> ApplyThenTranslate(const std::vector<Point> &points,
			const Point &offset) const;
	double ApplyScaling(double value) const;
	static Polarity PolarityFromCommand(const std::string &str);
	static Mirroring MirroringFromCommand(const std::string &str);

private:
	bool m_isDark;
	bool m_mirrorX, m_mirrorY;
	double m_rotation;
	double m_scaling;
};

} /* namespace gerbex */

#endif /* TRANSFORM_H_ */
