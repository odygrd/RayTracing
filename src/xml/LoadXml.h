/**
 * @file   LoadXml.h
 * @author Odysseus Georgoudis
 * @date   31/08/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_LOADXML_H
#define RAYTRACING_LOADXML_H

#include "../cameras/Camera.h"
#include "../core/SceneNode.h"
#include "../core/lights/ILight.h"
#include "../core/meshes/Sphere.h"
#include <vector>

/**
 * Forward declarations
 */
namespace tinyxml2 { class XMLElement; }

class ParsedXML
{
public:
    Camera       camera;
    SceneNode    rootNode;
    MaterialsMap materialsMap;
    LightsMap    lightsMap;

    std::vector<std::pair<SceneNode&, std::string>> nodeMaterial;

    enum Print
    {
        DONT_PRINT,
        PRINT
    };


    Print printXml_;

    /**
     * Register static meshes
     */
    static const Sphere aSphere;

    ParsedXML(const char* file, Print printXML = Print::DONT_PRINT)
        : printXml_(printXML)
    {
        LoadXml(file);
    }

    /**
     * Initial XML parsing.
     * @param file
     * @param p
     */
    void LoadXml(const char* file);

    /**
     * Continue loading the scene from an XML element. Recursively load nodes,
     * materials, or lights)
     * @param sceneXMLElement
     */
    void LoadScene(tinyxml2::XMLElement* sceneXMLElement);

    /**
     * Load camera values from XML file.
     * @param cameraXMLElement
     * @param camera
     */
    void LoadCamera(tinyxml2::XMLElement* cameraXMLElement);

    /**
     * Add a node's children from the XML structure
     * @param node
     * @param objectXMLElement
     * @param level
     */
    void LoadNode(SceneNode* node,
                  tinyxml2::XMLElement* objectXMLElement,
                  int level = 0);

    /**
     * Load in the material information for each element
     * @param materialXMLElement
     */
    void LoadMaterial(tinyxml2::XMLElement* materialXMLElement);

    /**
     * Load in the transformation terms for each node
     * @param nodeTransformation
     * @param objectXMLElement
     * @param level
     */
    void LoadTransform(SceneNode* nodeTransformation, tinyxml2::XMLElement *objectXMLElement, int level);

    /**
     * Load in the light information for each element
     * @param lightXMLElement
     */
    void LoadLight(tinyxml2::XMLElement* lightXMLElement);
};
#endif //RAYTRACING_LOADXML_H
