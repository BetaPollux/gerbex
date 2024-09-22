/*
 * ApertureTransformation.cpp
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

#include "ApertureTransformation.h"
#include <stdexcept>

namespace gerbex {

ApertureTransformation::ApertureTransformation() :
		m_polarity { Polarity::Dark }, m_mirroring { Mirroring::None }, m_rotation_degrees {
				0.0 }, m_scaling_factor { 1.0 }

{
	// Empty

}

ApertureTransformation::ApertureTransformation(Polarity polarity,
		Mirroring mirroring, double rotation, double scaling) :
		m_polarity { polarity }, m_mirroring { mirroring }, m_rotation_degrees {
				rotation }, m_scaling_factor { scaling }

{
	// Empty

}

ApertureTransformation::~ApertureTransformation() {
	// Empty
}

bool ApertureTransformation::operator ==(
		const ApertureTransformation &rhs) const {
	return (m_polarity == rhs.m_polarity) && (m_mirroring == rhs.m_mirroring)
			&& (m_rotation_degrees == rhs.m_rotation_degrees)
			&& (m_scaling_factor == rhs.m_scaling_factor);
}

bool ApertureTransformation::operator !=(
		const ApertureTransformation &rhs) const {
	return (m_polarity != rhs.m_polarity) || (m_mirroring != rhs.m_mirroring)
			|| (m_rotation_degrees != rhs.m_rotation_degrees)
			|| (m_scaling_factor != rhs.m_scaling_factor);
}

Mirroring ApertureTransformation::GetMirroring() const {
	return m_mirroring;
}

void ApertureTransformation::SetMirroring(Mirroring mirroring) {
	m_mirroring = mirroring;
}

Polarity ApertureTransformation::GetPolarity() const {
	return m_polarity;
}

void ApertureTransformation::SetPolarity(Polarity polarity) {
	m_polarity = polarity;
}

double ApertureTransformation::GetRotationDegrees() const {
	return m_rotation_degrees;
}

void ApertureTransformation::SetRotationDegrees(double rotationDegrees) {
	m_rotation_degrees = rotationDegrees;
}

double ApertureTransformation::GetScalingFactor() const {
	return m_scaling_factor;
}

void ApertureTransformation::SetScalingFactor(double scalingFactor) {
	m_scaling_factor = scalingFactor;
}

Polarity ApertureTransformation::PolarityFromCommand(const std::string &str) {
	if (str == "C") {
		return Polarity::Clear;
	} else if (str == "D") {
		return Polarity::Dark;
	} else {
		throw std::invalid_argument("invalid polarity");
	}
}

Mirroring ApertureTransformation::MirroringFromCommand(const std::string &str) {
	if (str == "N") {
		return Mirroring::None;
	} else if (str == "X") {
		return Mirroring::X;
	} else if (str == "Y") {
		return Mirroring::Y;
	} else if (str == "XY") {
		return Mirroring::XY;
	} else {
		throw std::invalid_argument("invalid mirroring");
	}
}

} /* namespace gerbex */

