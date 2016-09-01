/**
 * @file   ParseUtils.cpp
 * @author Odysseus Georgoudis
 * @date   31/08/2016
 * @brief  
 *
 */

#include <iostream>
#include "ParseUtils.h"
#include "../../third-party/tinyxml2/tinyxml2.h"

using tinyxml2::XMLElement;

void ReadVector(XMLElement* element, Point& vec)
{
    ReadFloat(element, vec.x, "x");
    ReadFloat(element, vec.y, "y");
    ReadFloat(element, vec.z, "z");
}

void ReadColor(XMLElement* element, Color& col)
{
    // set 3-channel color values
    ReadFloat(element, col.r, "r");
    ReadFloat(element, col.g, "g");
    ReadFloat(element, col.b, "b");

    // read in color scaling factor
    float f { 1.0 };
    ReadFloat(element, f);
    col = col * f;
}

void ReadFloat(tinyxml2::XMLElement* element,
               float& f,
               const char* name /* = "value" */)
{
    element->QueryFloatAttribute(name, &f);
}

void PrintIndent(int level)
{
    for(int i = 0; i < level; i++)
    {
        std::cout << "\t";
    }
}