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
 * File paths definitions for each scene
 */
static constexpr const char* const SCENE_1_IN    = "scenes/scene_1.xml";
static constexpr const char* const SCENE_2_IN    = "scenes/scene_2.xml";

static constexpr const char* const SCENE_1_OUT   = "output/scene_1.ppm";
static constexpr const char* const SCENE_1_OUT_Z = "output/scene_1z.ppm";

static constexpr const char* const SCENE_2_OUT   = "output/scene_2.ppm";
static constexpr const char* const SCENE_2_OUT_Z = "output/scene_2z.ppm";

/****** Configuration ******/

/**
 * Scene to load
 */
static constexpr auto xml = SCENE_2_IN;

/**
 * Output Directories
 */
static constexpr auto output  = SCENE_2_OUT;
static constexpr auto zOutput = SCENE_2_OUT_Z;

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
static const int numThreads = 8;

#endif //RAYTRACING_CONFIG_H
