/*
 * cgal_demo.cpp
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

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/draw_polygon_set_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes_2;
typedef CGAL::Polygon_set_2<K> Polygon_set_2;
typedef CGAL::Point_2<K> Point_2;

Polygon_2 rectangle(int l, int x = 0, int y = 0) {
	// Create a rectangle with given side length.
	Polygon_2 P;
	P.push_back(Point_2(-l + x, -l + y));
	P.push_back(Point_2(l + x, -l + y));
	P.push_back(Point_2(l + x, l + y));
	P.push_back(Point_2(-l + x, l + y));
	return P;
}

int main() {
	// Make base group
	Polygon_set_2 group;
	group.join(rectangle(3, -1, -1));
	group.join(rectangle(3, 1, 1));

	// Make mask set
	Polygon_set_2 mask;
	mask.join(rectangle(2, -1, -1));
	mask.join(rectangle(2, 1, 1));

	// Apply mask
	group.difference(mask);

	// Add more shapes
	group.join(rectangle(1));

	CGAL::draw(group);
	return 0;
}

