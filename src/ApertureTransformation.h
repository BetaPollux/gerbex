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


enum class LoadPolarity {
	Dark,
	Clear
};


enum class LoadMirroring {
	N,	// None
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
	virtual ~ApertureTransformation();
	bool operator==	(const ApertureTransformation& rhs) const;
	bool operator!=	(const ApertureTransformation& rhs) const;
	LoadMirroring GetMirroring() const;
	void SetMirroring(LoadMirroring mirroring);
	LoadPolarity GetPolarity() const;
	void SetPolarity(LoadPolarity polarity);
	double GetRotationDegrees() const;
	void SetRotationDegrees(double rotationDegrees);
	double GetScalingFactor() const;
	void SetScalingFactor(double scalingFactor);

private:
	LoadPolarity m_polarity;
	LoadMirroring m_mirroring;
	double m_rotation_degrees;
	double m_scaling_factor;
};

#endif /* APERTURETRANSFORMATION_H_ */
