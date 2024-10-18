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
		m_isDark { true }, m_mirrorX { false }, m_mirrorY { false }, m_rotation_degrees {
				0.0 }, m_scaling_factor { 1.0 } {
	// Empty
}

ApertureTransformation::ApertureTransformation(Polarity polarity,
		Mirroring mirroring, double rotation, double scaling) :
		m_rotation_degrees { rotation }, m_scaling_factor { scaling } {
	SetPolarity(polarity);
	SetMirroring(mirroring);
}

bool ApertureTransformation::operator ==(
		const ApertureTransformation &rhs) const {
	return (m_isDark == rhs.m_isDark) && (m_mirrorX == rhs.m_mirrorX)
			&& (m_mirrorY == rhs.m_mirrorY)
			&& (m_rotation_degrees == rhs.m_rotation_degrees)
			&& (m_scaling_factor == rhs.m_scaling_factor);
}

bool ApertureTransformation::operator !=(
		const ApertureTransformation &rhs) const {
	return (m_isDark != rhs.m_isDark) || (m_mirrorX != rhs.m_mirrorX)
			|| (m_mirrorY != rhs.m_mirrorY)
			|| (m_rotation_degrees != rhs.m_rotation_degrees)
			|| (m_scaling_factor != rhs.m_scaling_factor);
}

Mirroring ApertureTransformation::GetMirroring() const {
	if (m_mirrorX && m_mirrorY) {
		return Mirroring::XY;
	} else if (m_mirrorX) {
		return Mirroring::X;
	} else if (m_mirrorY) {
		return Mirroring::Y;
	} else {
		return Mirroring::None;
	}
}

void ApertureTransformation::SetMirroring(Mirroring mirroring) {
	m_mirrorX = mirroring == Mirroring::X || mirroring == Mirroring::XY;
	m_mirrorY = mirroring == Mirroring::Y || mirroring == Mirroring::XY;
}

Polarity ApertureTransformation::GetPolarity() const {
	return m_isDark ? Polarity::Dark : Polarity::Clear;
}

void ApertureTransformation::SetPolarity(Polarity polarity) {
	m_isDark = polarity == Polarity::Dark;
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
	if (scalingFactor <= 0.0) {
		throw std::invalid_argument("scaling factor must be > 0");
	}
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

double ApertureTransformation::ApplyScaling(double value) {
	return value * m_scaling_factor;
}

void ApertureTransformation::Stack(const ApertureTransformation &transform) {
	m_scaling_factor *= transform.m_scaling_factor;
	m_rotation_degrees += transform.m_rotation_degrees;
	if (!transform.m_isDark) {
		m_isDark = !m_isDark;
	}
	if (transform.m_mirrorX) {
		m_mirrorX = !m_mirrorX;
	}
	if (transform.m_mirrorY) {
		m_mirrorY = !m_mirrorY;
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

Point ApertureTransformation::Apply(const Point &point,
		const Point &reference) {
	Point result = point - reference;
	if (m_mirrorX) {
		result.SetX(-result.GetX());
	}
	if (m_mirrorY) {
		result.SetY(-result.GetY());
	}
	result *= m_scaling_factor;
	result.Rotate(m_rotation_degrees);
	return result + reference;
}

} /* namespace gerbex */

