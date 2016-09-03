/**
 * @file   LoadXml.cpp
 * @author Odysseus Georgoudis
 * @date   31/08/2016
 * @brief  
 *
 */

#include "LoadXml.h"

#include <iostream>

#include "ParseUtils.h"
#include "../../third-party/tinyxml2/tinyxml2.h"
#include "../core/materials/Blinn.h"
#include "../core/materials/Phong.h"
#include "../core/lights/Ambient.h"
#include "../core/lights/Directional.h"
#include "../core/lights/Point.h"

using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XML_SUCCESS;

const Sphere ParsedXML::aSphere;

void ParsedXML::LoadXml(const char* file)
{
    XMLDocument xmlDoc;
    xmlDoc.LoadFile(file);

    if (xmlDoc.Error())
    {
        throw std::invalid_argument("ERROR: Failed to load XML file " +
            std::string(file) + ". Invalid location or syntax.\n" +
            "Check if the file is inside the scenes folder "
                "and that the syntax is correct.\n");
    }

    XMLElement* xml { xmlDoc.FirstChildElement("xml") };
    if (xml == nullptr)
    {
        throw std::invalid_argument("ERROR: Failed to load XML file " +
            std::string(file) + ". No \"xml\" tag found.\n");
    }

    XMLElement* sceneXMLElem { xml->FirstChildElement("scene") };
    if (sceneXMLElem == nullptr)
    {
        throw std::invalid_argument("ERROR: Failed to load XML file " +
            std::string(file) + ". No \"scene\" tag found.\n");
    }

    XMLElement* camXMLElem { xml->FirstChildElement("camera") };
    if (camXMLElem == nullptr)
    {
        throw std::invalid_argument("ERROR: Failed to load XML file " +
            std::string(file) + ". No \"camera\" tag found.\n");
    }

    LoadScene(sceneXMLElem);

    LoadCamera(camXMLElem);
}

void ParsedXML::LoadScene(tinyxml2::XMLElement* sceneXMLElement)
{
    XMLElement* sceneXMLElementChild = sceneXMLElement->FirstChildElement();

    while(sceneXMLElementChild)
    {
        if(strcmp(sceneXMLElementChild->Value(), "object") == 0)
        {
            LoadNode(&rootNode, sceneXMLElementChild);
        }
        else if(strcmp(sceneXMLElementChild->Value(), "material") == 0)
        {
            LoadMaterial(sceneXMLElementChild);
        }
        else if(strcmp(sceneXMLElementChild->Value(), "light") == 0)
        {
            LoadLight(sceneXMLElementChild);
        }

        sceneXMLElementChild = sceneXMLElementChild->NextSiblingElement();
    }

    for (auto& elem : nodeMaterial)
    {
        // node's material = find the name in materials map
        elem.first.material_ = materialsMap[elem.second].get();
    }
}

void ParsedXML::LoadCamera(XMLElement* cameraXMLElement)
{
    XMLElement* cameraXMLElemChild = cameraXMLElement->FirstChildElement();

    // load all camera values
    while(cameraXMLElemChild)
    {
        if(strcmp(cameraXMLElemChild->Value(), "position") == 0)
        {
            ReadVector(cameraXMLElemChild, camera.position);
        }
        else if(strcmp(cameraXMLElemChild->Value(), "target") == 0)
        {
            ReadVector(cameraXMLElemChild, camera.direction);
        }
        else if(strcmp(cameraXMLElemChild->Value(), "up") == 0)
        {
            ReadVector(cameraXMLElemChild, camera.up);
        }
        else if(strcmp(cameraXMLElemChild->Value(), "fov") == 0)
        {
            ReadFloat(cameraXMLElemChild, camera.fov);
        }
        else if(strcmp(cameraXMLElemChild->Value(), "width") == 0)
        {
            cameraXMLElemChild->QueryIntAttribute("value", &camera.imageWidth);
        }
        else if(strcmp(cameraXMLElemChild->Value(), "height") == 0)
        {
            cameraXMLElemChild->QueryIntAttribute("value", &camera.imageHeight);
        }

        cameraXMLElemChild = cameraXMLElemChild->NextSiblingElement();
    }

    // compute remaining camera values
    camera.setup();
}

// add a node's children from the XML structure
void ParsedXML::LoadNode(SceneNode* node, XMLElement* objectXMLElement, int level /* = 0 */)
{

    // assign a child to the parent node

    // get the name of the parent node
    const char* name = objectXMLElement->Attribute("name");

    // print out object name
    if (printXml_ == Print::PRINT)
    {
        PrintIndent(level);
        printf("Object [");
        if(name)
        {
            printf("%s", name);
        }
        printf("]");
    }

    // get the type of the parent node
    const char* type = objectXMLElement->Attribute("type");
    const IMesh* mesh { nullptr };
    if(type)
    {
        // for sphere
        if(strcmp(type, "sphere") == 0)
        {
            mesh = &aSphere;

            // print out object type
            if (printXml_ == Print::PRINT)
                printf(" - Sphere");

            // for unknown object
        }
        else
        {
            // print out object type
            if (printXml_ == Print::PRINT)
                printf(" - UNKNOWN TYPE");
        }
    }

    // add the node
    node->AddChildNode(name, mesh);
    auto& currNode = node->GetLastInsertedNode();

    // get the material type of the parent node
    const char* materialName = objectXMLElement->Attribute("material");
    if(materialName)
    {
        // print out object material
        if (printXml_ == Print::PRINT)
            printf(" <%s>", materialName);

        //FIXME
        std::pair<SceneNode&, std::string> tempPair {currNode, materialName};
        nodeMaterial.push_back(tempPair);
    }
    if (printXml_ == Print::PRINT)
        printf("\n");

    // load the appropriate transformation information
    LoadTransform(&currNode, objectXMLElement, level);

    // recursively loop through remaining objects
    for(XMLElement* child = objectXMLElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        if(strcmp(child->Value(), "object") == 0)
            LoadNode(&currNode, child, level + 1);
    }
}

// load in the material information for each element
void ParsedXML::LoadMaterial(XMLElement* materialXMLElement)
{
    // get & print material name
    const char* name = materialXMLElement->Attribute("name");
    if (printXml_ == Print::PRINT)
    {
        printf("Material [");
        if(name)
            printf("%s", name);
        printf("]");
    }

    // get material's type
    const char* type = materialXMLElement->Attribute("type");
    if(type)
    {
        if(strcmp(type, "blinn") == 0) // blinn material type
        {
            // print out material type
            if (printXml_ == Print::PRINT)
            {
                printf(" - Blinn\n");
            }

            AddMaterial_<BlinnMaterial>(name, materialXMLElement);
        }
        else if(strcmp(type, "phong") == 0) // phong material type
        {
            // print out material type
            if (printXml_ == Print::PRINT)
            {
                printf(" - Phong\n");
            }

            AddMaterial_<PhongMaterial>(name, materialXMLElement);
        }
        else // unknown material type
        {
            if (printXml_ == Print::PRINT)
            {
                printf(" - UNKNOWN MATERIAL\n");
            }
        }
    }
}

// load in the transformation terms for each node
void ParsedXML::LoadTransform(SceneNode* nodeTransformation, XMLElement* objectXMLElement, int level)
{
    // recursively apply transformations to child nodes that have been already set
    for(XMLElement* child = objectXMLElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        // check if child is a scaling term
        if(strcmp(child->Value(), "scale") == 0)
        {
            float v = 1.0;
            Point s(1, 1, 1);
            ReadFloat(child, v);
            ReadVector(child, s);
            s *= v;
            nodeTransformation->Scale(s.x, s.y, s.z);

            // print out scaling term
            if (printXml_ == Print::PRINT)
            {
                PrintIndent(level + 1);
                printf("scale %f %f %f\n", s.x, s.y, s.z);
            }
            // check if child is a rotation term
        }
        else if(strcmp(child->Value(), "rotate") == 0)
        {
            Point r(0, 0, 0);
            ReadVector(child, r);
            r.Normalize();
            float a;
            ReadFloat(child, a, "angle");
            nodeTransformation->Rotate(r, a);

            // print out rotation term
            if (printXml_ == Print::PRINT)
            {
                PrintIndent(level + 1);
                printf("rotate %f degrees around %f %f %f\n", a, r.x, r.y, r.z);
            }

            // check if child is a translation term
        }
        else if(strcmp(child->Value(), "translate") == 0)
        {
            Point p(0, 0, 0);
            ReadVector(child, p);
            nodeTransformation->Translate(p);

            // print out translation term
            if (printXml_ == Print::PRINT)
            {
                PrintIndent(level + 1);
                printf("translate %f %f %f\n", p.x, p.y, p.z);
            }
        }
    }
}

// load in the light information for each element
void ParsedXML::LoadLight(XMLElement* lightXMLElement)
{
    // get & print light name
    const char* name = lightXMLElement->Attribute("name");
    if (printXml_ == Print::PRINT)
    {
        printf("Light [");
        if(name)
            printf("%s", name);
        printf("]");
    }

    // get light type
    const char* type = lightXMLElement->Attribute("type");
    if(type)
    {
        // ambient light type
        if(strcmp(type, "ambient") == 0)
        {
            // print out light type
            if (printXml_ == Print::PRINT)
            {
                printf(" - Ambient\n");
            }

            Color intensity { 1, 1, 1 };
            // check children for light properties
            for(XMLElement *child = lightXMLElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
            {
                // load intensity (color) of light (for all lights)
                if(strcmp(child->Value(), "intensity") == 0 )
                {
                    ReadColor(child, intensity);

                    // print out light intensity color
                    if (printXml_ == Print::PRINT)
                    {
                        printf("  intensity %f %f %f\n",
                               intensity.r,
                               intensity.g,
                               intensity.b);
                    }
                }
            }

            lightsMap[name] = std::make_unique<AmbientLight>(intensity);

            // direct light type
        }
        else if(strcmp(type, "direct") == 0)
        {
            // print out light type
            if (printXml_ == Print::PRINT)
            {
                printf(" - Direct\n");
            }

            Color intensity { 1, 1, 1 };
            Point direction { 1, 1, 1 };
            // check children for light properties
            for(XMLElement *child = lightXMLElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
            {
                // load intensity (color) of light (for all lights)
                if(strcmp(child->Value(), "intensity") == 0)
                {
                    ReadColor(child, intensity);

                    // print out light intensity color
                    if (printXml_ == Print::PRINT)
                    {
                        printf("  intensity %f %f %f\n", intensity.r, intensity.g, intensity.b);
                    }

                    // load direction of light
                }
                else if(strcmp(child->Value(), "direction") == 0)
                {
                    ReadVector(child, direction);

                    // print out light direction
                    if (printXml_ == Print::PRINT)
                    {
                        printf("  direction %f %f %f\n", direction.x, direction.y, direction.z);
                    }
                }
            }
            lightsMap[name] = std::make_unique<DirectionalLight>(intensity, direction);
            // point light type
        }
        else if(strcmp(type, "point") == 0)
        {
            // print out light type
            if (printXml_ == Print::PRINT) {
                printf(" - Point\n");
            }

            Color intensity { 1, 1, 1 };
            Point position { 0, 0, 0 };

            // check children for light properties
            for(XMLElement *child = lightXMLElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement()){

                // load intensity (color) of light (for all lights)
                if(strcmp(child->Value(), "intensity") == 0)
                {
                    ReadColor(child, intensity);

                    // print out light intensity color
                    if (printXml_ == Print::PRINT)
                    {
                        printf("  intensity %f %f %f\n", intensity.r, intensity.g, intensity.b);
                    }

                    // load position of light
                }
                else if(strcmp(child->Value(), "position") == 0)
                {
                    ReadVector(child, position);

                    // print out light intensity color
                    if (printXml_ == Print::PRINT)
                    {
                        printf("  position %f %f %f\n", position.x, position.y, position.z);
                    }
                }
            }
            lightsMap[name] = std::make_unique<PointLight>(intensity, position);
            // unknown light type
        }
        else
        {

            if (printXml_ == Print::PRINT)
            {
                printf(" - UNKNOWN LIGHT\n");
            }
        }
    }
}

template <typename TLight>
void ParsedXML::AddMaterial_(const char* materialName,
                             XMLElement* materialXMLElement)
{
    // initialize values
    Color diffuse   { 1, 1, 1 };
    Color specular  { 1, 1, 1 };
    float shininess { 1.0 };

    // check children for material properties
    for(XMLElement *child = materialXMLElement->FirstChildElement();
        child != nullptr;
        child = child->NextSiblingElement())
    {
        if (strcmp(child->Value(), "diffuse") == 0) // load diffuse color
        {
            ReadColor(child, diffuse);

            if (printXml_ == Print::PRINT)
            {
                printf("  diffuse %f %f %f\n", diffuse.r, diffuse.g, diffuse.b);
            }
        }
        else if (strcmp(child->Value(), "specular") == 0) // load specular color
        {
            ReadColor(child, specular);

            if (printXml_ == Print::PRINT)
            {
                printf("  specular %f %f %f\n", specular.r, specular.g, specular.b);
            }
        }
        else if (strcmp(child->Value(), "glossiness") == 0) // load shininess value
        {
            ReadFloat(child, shininess);

            if (printXml_ == Print::PRINT)
            {
                printf("  glossiness %f\n", shininess);
            }
        }
    }

    materialsMap[materialName] = std::make_unique<TLight>(diffuse,
                                                         specular,
                                                         shininess);
}