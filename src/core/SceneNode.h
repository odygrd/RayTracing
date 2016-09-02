/**
 * @file   SceneNode.h
 * @author Odysseus Georgoudis
 * @date   31/08/2016
 * @brief  
 *
 */

#ifndef RAYTRACING_SCENENODE_H
#define RAYTRACING_SCENENODE_H

#include <cstddef>
#include <vector>
#include <memory>
#include <string>

#include "Transformation.h"
#include "Ray.h"
#include "materials/IMaterial.h"
#include "meshes/IMesh.h"

// Node definition (pieces of the scene which store objects)

/*
 * Scene node definition.
 * A scene node contains :
 *  i)   a pointer to a geometric shape placed at the origin ( 0,0,0 )
 *  ii)  it's own transformation matrix
 *  iii) a reference to a material ( if any )
 *  iv) any children nodes
 */
class SceneNode : public Transformation
{
public:
    // FIXME :: make private, read materials first, then construct the object
    std::string      name_;                  ///< node's name
    const IMesh*     mesh_ { nullptr };     ///< object reference
    const IMaterial* material_ { nullptr }; ///< node's material

    /**
     * Constructors
     */
    SceneNode() = default;
    SceneNode(const std::string name, const IMesh* mesh)
        : name_(name),
          mesh_(mesh)
    {}

    /**
     * Add an additional child node
     * @param name
     * @param mesh
     */
    void AddChildNode(const std::string name, const IMesh* mesh)
    {
        childrenNodes_.push_back(std::make_unique<SceneNode>(name, mesh));
    }

    /**
     * @return The last inserted child node
     */
    SceneNode& GetLastInsertedNode()
    {
        return *childrenNodes_.back();
    }

    /**
     * @return The number of children
     */
    size_t GetChildrenNum() const { return childrenNodes_.size(); }

    /**
     * @param index Position of child node
     * @return The child node at given position
     */
    const SceneNode* GetChildNodeAt(std::size_t index) const
    {
        return childrenNodes_[index].get();
    }

private:
    std::vector<std::unique_ptr<SceneNode>> childrenNodes_; ///< contains all the
                                                            ///< children of this node
};

#endif //RAYTRACING_SCENENODE_H
