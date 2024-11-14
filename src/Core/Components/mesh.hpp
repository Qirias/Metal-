//
//  TextureArray.hpp
//  Metal-Tutorial
//

#pragma once

#include <simd/simd.h>
using namespace simd;

#include <vector>
#include <string>

#include <tinyobjloader/tiny_obj_loader.h>
#include "vertexData.hpp"
#include "textureArray.hpp"

inline bool operator==(const Vertex& lhs, const Vertex& rhs) {
    return lhs.position.x == rhs.position.x &&
           lhs.position.y == rhs.position.y &&
           lhs.position.z == rhs.position.z &&
           lhs.normal.x == rhs.normal.x &&
           lhs.normal.y == rhs.normal.y &&
           lhs.normal.z == rhs.normal.z &&
           lhs.textureCoordinate.x == rhs.textureCoordinate.x &&
           lhs.textureCoordinate.y == rhs.textureCoordinate.y &&
    lhs.diffuseTextureIndex == rhs.diffuseTextureIndex;
}

namespace std {
    template<> struct hash<simd::float3> {
        size_t operator()(simd::float3 const& vector) const {
            size_t h1 = hash<float>{}(vector.x);
            size_t h2 = hash<float>{}(vector.y);
            size_t h3 = hash<float>{}(vector.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };

    template<> struct hash<simd::float2> {
        size_t operator()(simd::float2 const& vector) const {
            size_t h1 = hash<float>{}(vector.x);
            size_t h2 = hash<float>{}(vector.y);
            return h1 ^ (h2 << 1);
        }
    };

    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            size_t h1 = hash<float3>{}(vertex.position.xyz);
            size_t h2 = hash<float3>{}(vertex.normal.xyz);
            size_t h3 = hash<float2>{}(vertex.textureCoordinate);
            size_t h4 = hash<int>{}(vertex.diffuseTextureIndex);
            
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
        }
    };
}

struct Mesh {
//    Mesh(std::string filePath, MTL::Device* metalDevice);
	Mesh(std::string filePath, MTL::Device* metalDevice, MTL::VertexDescriptor* vertexDescriptor);
    Mesh(MTL::Device* device, const Vertex* vertexData, size_t vertexCount, const uint32_t* indexData, size_t indexCount);

    ~Mesh();

private:
    void loadObj(std::string filePath);
	void calculateTangentSpace(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    void createBuffers(MTL::VertexDescriptor* vertexDescriptor);
    
    std::vector<Vertex>                     vertices;
    std::vector<uint32_t>                   vertexIndices;
    TextureArray*                           diffuseTexturesArray;
	TextureArray*                           normalTexturesArray;
    std::unordered_map<Vertex, uint32_t>    vertexMap;
    
public:
    MTL::Device*    device;
    MTL::Buffer*    vertexBuffer;
    MTL::Buffer*    indexBuffer;
    unsigned long   indexCount;
    MTL::Texture*   diffuseTextures;
	MTL::Texture*   normalTextures;
    MTL::Buffer*    diffuseTextureInfos;
	MTL::Buffer*    normalTextureInfos;
};
