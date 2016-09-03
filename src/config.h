/**
 * @file   config.h
 * @author Odysseus Georgoudis
 * @date   30/08/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_CONFIG_H
#define RAYTRACING_CONFIG_H

/**
 * Output File paths definitions for each scene
 */

static constexpr auto outputPath { "output/" };

/****** Configuration ******/

/**
 * Print out XML file
 */
static constexpr bool printXML = true;

/**
 * Calculate Z buffer image
 */
static constexpr bool zBuffer = true;

/**
 * Number of threads
 */
static const int numThreads = 1;

#endif //RAYTRACING_CONFIG_H
