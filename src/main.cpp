// a ray tracer in C++

#include <thread>
#include <fstream>
#include <iostream>

#include "config.h"
#include "xml/LoadXML.h"
#include "core/Render.h"
#include "core/ImagePlane.h"

using namespace std;

bool objectIntersection(Ray ray, HitInfo& outHitInfo, const SceneNode& sceneNode);
void rayTracing(int pixel, const ParsedXML& parsedXML, const Render& render);

// ray tracer
int main()
{
    // Latency tracking
    auto begin = std::chrono::system_clock::now();

    // *** Program start ***

    ParsedXML parsedXML { xml, ParsedXML::PRINT };
    Render render;
    render.init(parsedXML.camera.imageWidth, parsedXML.camera.imageHeight);

    // start ray tracing loop (in parallel with threads)
    thread t[numThreads];
    for(int i = 0; i < numThreads; i++)
    {
        t[i] = thread(rayTracing, i, std::ref(parsedXML), std::ref(render));
    }

    // when finished, join all threads back to main
    for(int i = 0; i < numThreads; i++)
    {
        t[i].join();
    }

    // output ray-traced image & z-buffer (if set)
    render.save(output);
    if(zBuffer)
    {
        render.computeZBuffer();
        render.saveZBuffer(zOutput);
    }

    // *** Program End ***

    // App time elapsed
    auto end= std::chrono::system_clock::now();
    std::cout << std:: endl << "Time elapsed: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << " ms" << std::endl;
}

// ray tracing loop (for an individual pixel)
void rayTracing(int pixel, const ParsedXML& parsedXML, const Render& render)
{
    // set variables for generating camera rays
    ImagePlane imgPlane { parsedXML.camera };

    do
    {
        // establish pixel location
        int pX { pixel % render.getWidth() };
        int pY { pixel / render.getWidth() };

        // transform ray into world space
        Point rayDirection { imgPlane.getFirstPixel() +
                             (imgPlane.getDXV() * pX) +
                             (imgPlane.getDYV() * pY) };

        rayDirection.Normalize();

        Ray ray { parsedXML.camera.position,
                  parsedXML.camera.TransformFrom(rayDirection) };

        // traverse through scene DOM
        // transform rays into model space
        // detect ray intersections and get back HitInfo
        HitInfo hitInfo;
        bool hit { objectIntersection(ray, hitInfo, parsedXML.rootNode) };

        // update z-buffer, if necessary
        if(zBuffer)
        {
            // FIXME breaking constness
            render.getZBufferArray()[pixel] = hitInfo.zDistance;
        }

        // shade pixel if it has material
        if (hit)
        {
            // Node exists and has a material assigned
            if (hitInfo.node && hitInfo.node->material_)
            {
                // FIXME breaking constness
                // color the pixel image
                render.getRenderImageArray()[pixel] =
                    Color24(hitInfo.node->material_->shade(ray,
                                                           hitInfo,
                                                           parsedXML.lightsMap));
            }
            else
            {
                // If there is no material or node color it white
                // unlikely to happen
                static constexpr Color24 white { 237, 237, 237 };
                render.getRenderImageArray()[pixel] = white;
            }
        }
        else
        {
            // if we hit nothing just color it black ( background color )
            static constexpr Color24 black { 0, 0, 0 };
            render.getRenderImageArray()[pixel] = black;
        }

        // re-assign next pixel (naive, but works)
        pixel += numThreads;
    } while (pixel < render.getSize());
}

// recursive object intersection through all scene objects for some ray
bool objectIntersection(Ray ray, HitInfo& outHitInfo, const SceneNode& sceneNode)
{
    // transform ray into model space (or local space)
    ray.ToModelSpace(sceneNode);

    // if object gets hit and hit first
    bool hit { false };

    if (sceneNode.mesh_)
    {
        // make hit info for node object (if exists)
        HitInfo tempHitInfo;
        tempHitInfo.node = &sceneNode;

        // check if object is hit
        hit = sceneNode.mesh_->intersectRay(ray, tempHitInfo);

        // check if hit was closer than previous hits and update the output hitInfo
        if (tempHitInfo.zDistance < outHitInfo.zDistance)
        {
            outHitInfo = tempHitInfo;
        }
        else
        {
            // if hit is not closer, don't count as hit
            hit = false;
        }
    }

    // loop on child nodes
    for(size_t i = 0; i < sceneNode.GetChildrenNum(); ++i)
    {
        // grab child node
        const SceneNode& child { *sceneNode.GetChildNodeAt(i) };

        // recursively check this child's descendants for hit info
        bool childHit { objectIntersection(ray, outHitInfo, child) };

        // if child is hit, make sure we pass that on
        if (childHit)
        {
            hit = true;
        }
    }

    // if object (or a descendant) was hit, transform from model space to world space
    if (hit)
    {
        outHitInfo.FromModelSpace(sceneNode);
    }

    // return whether there was a hit on object or its descendants
    return hit;
}