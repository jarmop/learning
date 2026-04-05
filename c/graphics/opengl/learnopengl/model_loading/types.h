#include <cglm/cglm.h>

#define MAX_BONE_INFLUENCE 4

typedef struct {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
} Vertex;

typedef struct {
    unsigned int id;
    char *type;
    char *path;
} Texture;

typedef struct {
    Texture *data;
    unsigned int size;
    unsigned int capacity;
} TextureArray;

typedef struct {
    TextureArray textures;
} Mesh;