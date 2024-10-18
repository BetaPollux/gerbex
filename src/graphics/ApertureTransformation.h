/*
 * ApertureTransformation.h
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

#ifndef APERTURETRANSFORMATION_H_
#define APERTURETRANSFORMATION_H_

#include "Point.h"
#include <string>

namespace gerbex {

enum class Polarity {
	Dark,
	Clear
};


enum class Mirroring {
	None,	// None
	X,	// X-axis
	Y,	// Y-axis
	XY	// Both X and Y axes
};


/*
 * Parameters that transform the polarity and shape of the aperture when it is
 * used to create an object.
 * Mirroring is performed before rotation.
 * Rotation is around the aperture origin.
 * Scaling is centered on the aperture origin.
 */
class ApertureTransformation {
public:
	ApertureTransformation();
	ApertureTransformation(Polarity polarity, Mirroring mirroring, double rotation, double scaling);
	virtual ~ApertureTransformation() = default;
	void Stack(const ApertureTransformation &transform);
	bool operator==	(const ApertureTransformation& rhs) const;
	bool operator!=	(const ApertureTransformation& rhs) const;
	Mirroring GetMirroring() const;
	void SetMirroring(Mirroring mirroring);
	Polarity GetPolarity() const;
	void SetPolarity(Polarity polarity);
	double GetRotationDegrees() const;
	void SetRotationDegrees(double rotationDegrees);
	double GetScalingFactor() const;
	void SetScalingFactor(double scalingFactor);
	Point Apply(const Point &point, const Point &reference = Point());
	double ApplyScaling(double value);
	static Polarity PolarityFromCommand(const std::string &str);
	static Mirroring MirroringFromCommand(const std::string &str);

private:
	bool m_isDark;
	bool m_mirrorX, m_mirrorY;
	double m_rotation_degrees;
	double m_scaling_factor;
};

} /* namespace gerbex */

#endif /* APERTURETRANSFORMATION_H_ */
