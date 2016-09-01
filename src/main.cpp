// a ray tracer in C++

#include <thread>
#include <fstream>
#include <iostream>

#include "config.h"
#include "xml/LoadXML.h"
#include "core/Render.h"
#include "core/ImagePlane.h"

using namespace std;

bool objectIntersection(const Ray& inRay, HitInfo& outHitInfo, const SceneNode& sceneNode);
void rayTracing(int pixel, const ParsedXML& parsedXML, const Render& render);

// ray tracer
int main()
{
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

        Color24 color;
        // shade pixel if it has material

        if (hit)
        {
            // Node exists and has a material assigned
            if (hitInfo.node && hitInfo.node->material_)
            {
                color = Color24(hitInfo.node->material_->shade(ray, hitInfo, parsedXML.lightsMap));
            }
            else
            {
                // if we hit nothing just color it white
                color.Set(237, 237, 237);
            }
        }
        else
        {
            color.Set(0, 0, 0);
        }

        // color the pixel image
        render.getRenderImageArray()[pixel] = color;  // FIXME breaking constness

        // re-assign next pixel (naive, but works)
        pixel += numThreads;
    } while (pixel < render.getSize());
}

// recursive object intersection through all scene objects for some ray
bool objectIntersection(const Ray& inRay, HitInfo& outHitInfo, const SceneNode& sceneNode)
{
    // transform ray into model space (or local space)
    Ray ray { sceneNode.toModelSpace(inRay) };

    // make hit info for node object (if exists)
    HitInfo tempHitInfo;
    // if object gets hit and hit first
    bool hit;

    if(sceneNode.mesh_)
    {
        tempHitInfo.node = &sceneNode;
        // check if object is hit
        hit = sceneNode.mesh_->intersectRay(ray, tempHitInfo);
    }

    if(hit)
    {
        // check if hit was closer than previous hits and update the output hitInfo
        if(tempHitInfo.zDistance < outHitInfo.zDistance)
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
        if(childHit)
        {
            hit = true;
        }
    }

    // if object (or a descendant) was hit, transform from model space (to world space)
    if(hit)
    {
        sceneNode.fromModelSpace(outHitInfo);
    }

    // return whether there was a hit on object or its descendants
    return hit;
}