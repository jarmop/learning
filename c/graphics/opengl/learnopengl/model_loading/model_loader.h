#include <stdio.h>
#include <stdbool.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cglm/cglm.h>

#include "../glad/glad.h"
#include "../lib/util.h"
#include "../lib/stb_image.h"
#include "types.h"

void texture_array_init(TextureArray *arr) {
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

void texture_array_push(TextureArray *arr, Texture tex) {
    if (arr->size == arr->capacity) {
        arr->capacity = arr->capacity == 0 ? 4 : arr->capacity * 2;
        arr->data = (Texture *)realloc(arr->data, arr->capacity * sizeof(Texture));
    }
    arr->data[arr->size++] = tex;
}

unsigned int texture_from_file(const char *path, const char *directory) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s/%s", directory, path);

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;

    unsigned char *data = stbi_load(filename, &width, &height, &nrComponents, 0);

    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        fprintf(stderr, "Texture failed to load at path: %s \n", path);
        stbi_image_free(data);
    }

    return textureID;
}

TextureArray texturesLoaded = {0, 0, 0};

TextureArray load_material_textures(
    struct aiMaterial *mat,
    enum aiTextureType type,
    const char *typeName,
    const char *directory
) {
    TextureArray textures;
    texture_array_init(&textures);

    unsigned int count = aiGetMaterialTextureCount(mat, type);
    for (unsigned int i = 0; i < count; i++) {
        struct aiString path;
        aiGetMaterialTexture(mat, type, i, &path, NULL, NULL, NULL, NULL, NULL, NULL);

        int isTextureLoaded = 0;
        for (unsigned int j = 0; j < texturesLoaded.size; j++) {
            if (strcmp(texturesLoaded.data[j].path, path.data) == 0) {
                texture_array_push(&textures, texturesLoaded.data[j]);
                isTextureLoaded = 1;
                break;
            }
        }

        if (!isTextureLoaded) {
            Texture texture;
            texture.id = texture_from_file(path.data, directory);
            texture.type = strdup(typeName);
            texture.path = strdup(path.data);
            texture_array_push(&textures, texture);
            texture_array_push(&texturesLoaded, texture);
        }
    }

    return textures;
}

int meshesProcessed = 0;

void process_mesh(struct aiMesh *mesh, const struct aiScene *scene, const char *directory) {
    meshesProcessed++;
    Vertex vertices[mesh->mNumVertices];
    int numberOfIndices = 0;
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        numberOfIndices += mesh->mFaces[i].mNumIndices;
    }
    // Texture textures[256 * 1024];
    unsigned int indices[numberOfIndices];
    // fprintf(stderr, "- process_mesh\n");

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.Position[0] = mesh->mVertices[i].x;
        vertex.Position[1] = mesh->mVertices[i].y;
        vertex.Position[2] = mesh->mVertices[i].z;

        if (mesh->mNormals) {
            vertex.Normal[0] = mesh->mNormals[i].x;
            vertex.Normal[1] = mesh->mNormals[i].y;
            vertex.Normal[2] = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords[0] = mesh->mTextureCoords[0][i].x; 
            vertex.TexCoords[1] = mesh->mTextureCoords[0][i].y;

            vertex.Tangent[0] = mesh->mTangents[i].x;
            vertex.Tangent[1] = mesh->mTangents[i].y;
            vertex.Tangent[2] = mesh->mTangents[i].z;

            vertex.Bitangent[0] = mesh->mBitangents[i].x;
            vertex.Bitangent[1] = mesh->mBitangents[i].y;
            vertex.Bitangent[2] = mesh->mBitangents[i].z;
        } else {
            vertex.TexCoords[0] = 0.0;
            vertex.TexCoords[1] = 0.0;
        }

        vertices[i] = vertex;
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        struct aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices[i] = face.mIndices[j];
        }
    }

    struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    TextureArray diffuseMaps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
    TextureArray specularMaps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular", directory);
    TextureArray normalMaps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal", directory);
    TextureArray heightMaps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height", directory);

    // fprintf(stderr, "diffuseMaps – size: %d, capacity: %d\n", diffuseMaps.size, diffuseMaps.capacity);
    // fprintf(stderr, "specularMaps – size: %d, capacity: %d\n", specularMaps.size, specularMaps.capacity);
    // fprintf(stderr, "normalMaps – size: %d, capacity: %d\n", normalMaps.size, normalMaps.capacity);
    // fprintf(stderr, "heightMaps – size: %d, capacity: %d\n", heightMaps.size, heightMaps.capacity);
    // fprintf(stderr, "textures_loaded – size: %d, capacity: %d\n", textures_loaded.size, textures_loaded.capacity);
    // print_vec3(vertices[0].Position);

}

void process_node(struct aiNode *node, const struct aiScene *scene, const char *directory) {
    // fprintf(stderr, "process_node\n");
    // fprintf(stderr, "process_node: %s\n", node->mName.data);

    // struct aiNode *childNode = node->mChildren[0];

    // fprintf(stderr, "child node: %s\n", childNode->mName.data);


    // struct aiMesh* mesh = scene->mMeshes[childNode->mMeshes[0]];
    // process_mesh(mesh, scene, directory);

    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        process_mesh(mesh, scene, directory);
        // meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene, directory);
    }
}

bool load_obj(const char *directory, const char *filename) {
    // fprintf(stderr, "load_obj\n");

    char model_path[100];
    snprintf(model_path, sizeof(model_path), "%s/%s", directory, filename);

    const struct aiScene* scene = aiImportFile( 
        model_path,
        aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs
    );

    // Create a mesh array of size mNumMeshes
    fprintf(stderr, "Number of meshes: %d\n", scene->mNumMeshes);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        fprintf(stderr, "Failed to load model\n");
        return false;
    }

    process_node(scene->mRootNode, scene, directory);

    fprintf(stderr, "Meshes processed: %d\n", meshesProcessed);

    // Release all resources associated with this import
    aiReleaseImport( scene);
    return true;
}