/**
 * @file   ParseUtils.h
 * @author Odysseus Georgoudis
 * @date   31/08/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_PARSEUTILS_H
#define RAYTRACING_PARSEUTILS_H

#include "../math/cyMath.h"

namespace tinyxml2 { class XMLElement; }
/**
 * Read in a vector from an XML element
 * @param element
 * @param vec
 */
void ReadVector(tinyxml2::XMLElement* element, Point& vec);

/**
 * Read in a color from an XML element
 * @param element
 * @param col
 */
void ReadColor(tinyxml2::XMLElement* element, Color& col);

/**
 * Read in a float from an XML element
 * @param element
 * @param f
 * @param name
 */
void ReadFloat(tinyxml2::XMLElement* element,
               float& f,
               const char* name = "value");

/**
 * Coordinate print level for debugging scene input
 * @param level
 */
void PrintIndent(int level);

#endif //RAYTRACING_PARSEUTILS_H
