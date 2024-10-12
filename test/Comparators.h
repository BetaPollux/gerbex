/*
 * Comparators.h
 *
 *  Created on: Sep. 22, 2024
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

#ifndef COMPARATORS_H_
#define COMPARATORS_H_

#include "DataTypeParser.h"
#include "Point.h"
#include "MacroTemplate.h"
#include <memory>
#include "GraphicsStringFrom.h"
#include "CppUTestExt/MockSupport.h"

using namespace gerbex;

class PointComparator : public MockNamedValueComparator
{
public:
    virtual bool isEqual(const void* object1, const void* object2)
    {
    	Point point1 = *(Point*)object1;
    	Point point2 = *(Point*)object2;
        return point1 == point2;
    }
    virtual SimpleString valueToString(const void* object)
    {
    	Point pt = *(Point*)object;
        return StringFrom(pt);
    }
};

class ParamsComparator : public MockNamedValueComparator
{
public:
    virtual bool isEqual(const void* object1, const void* object2)
    {
    	Parameters params1 = *(Parameters*)object1;
    	Parameters params2 = *(Parameters*)object2;
        return params1 == params2;
    }
    virtual SimpleString valueToString(const void* object)
    {
    	Parameters params = *(Parameters*)object;
    	SimpleString str;
    	for (double p: params) {
    		str += StringFrom(p) + " ";
    	}
        return str;
    }
};


class MacroTemplateComparator : public MockNamedValueComparator
{
public:
    virtual bool isEqual(const void* object1, const void* object2)
    {
    	MacroTemplate* macro1 = (MacroTemplate*)object1;
    	MacroTemplate* macro2 = (MacroTemplate*)object2;
        return macro1->GetBody() == macro2->GetBody();
    }
    virtual SimpleString valueToString(const void* object)
    {
    	MacroTemplate* macro = (MacroTemplate*)object;
    	SimpleString str;
    	for (const std::string &w: macro->GetBody()) {
    		str += StringFrom(w) + "*";
    	}
        return str;
    }
};


#endif /* COMPARATORS_H_ */
