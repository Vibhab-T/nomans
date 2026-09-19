#include "Icosphere.h"
#include "raylib/raymath.h"

#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

namespace {

struct Triangle {
  Vector3 a, b, c;
};

// Base icosahedron
constexpr std::array<Vector3, 12> kVertices = {{
    {0.8506508f, 0.5257311f, 0.f},
    {0.000000101405476f, 0.8506507f, -0.525731f},
    {0.000000101405476f, 0.8506506f, 0.525731f},
    {0.5257309f, -0.00000006267203f, -0.85065067f},
    {0.52573115f, -0.00000006267203f, 0.85065067f},
    {0.8506508f, -0.5257311f, 0.f},
    {-0.52573115f, 0.00000006267203f, -0.85065067f},
    {-0.8506508f, 0.5257311f, 0.f},
    {-0.5257309f, 0.00000006267203f, 0.85065067f},
    {-0.000000101405476f, -0.8506506f, -0.525731f},
    {-0.000000101405476f, -0.8506507f, 0.525731f},
    {-0.8506508f, -0.5257311f, 0.f},
}};

constexpr std::array<uint32_t, 60> kFaceIndices = {
    0, 1,  2, 0, 3,  1, 0, 2,  4, 3,  0, 5,  0, 4, 5, 1,  3,  6,  1,  7,
    2, 7,  1, 6, 4,  2, 8, 7,  8, 2,  9, 3,  5, 6, 3, 9,  5,  4,  10, 4,
    8, 10, 9, 5, 10, 7, 6, 11, 7, 11, 8, 11, 6, 9, 8, 11, 10, 10, 11, 9};

// Spherical linear interpolation between two points already on the unit sphere.
Vector3 slerp(Vector3 p0, Vector3 p1, float t) {
  float theta = acosf(Vector3DotProduct(p0, p1));
  if (theta < 1e-6f) {
    return p0;
  }

  float sinTheta = sinf(theta);
  Vector3 result =
      Vector3Add(Vector3Scale(p0, sinf((1.f - t) * theta) / sinTheta),
                 Vector3Scale(p1, sinf(t * theta) / sinTheta));
  return Vector3Normalize(result);
}

std::vector<Triangle> baseTriangles() {
  std::vector<Triangle> triangles;
  triangles.reserve(20);

  for (std::size_t i = 0; i < kFaceIndices.size(); i += 3) {
    triangles.push_back({
        kVertices[kFaceIndices[i + 0]],
        kVertices[kFaceIndices[i + 1]],
        kVertices[kFaceIndices[i + 2]],
    });
  }

  return triangles;
}

std::vector<Triangle> subdivide(std::vector<Triangle> triangles,
                                uint8_t depth) {
  for (uint8_t d = 0; d < depth; d++) {
    std::vector<Triangle> subdivided;
    subdivided.reserve(triangles.size() * 4);

    for (const Triangle &tri : triangles) {
      Vector3 mAB = slerp(tri.a, tri.b, 0.5f);
      Vector3 mBC = slerp(tri.b, tri.c, 0.5f);
      Vector3 mCA = slerp(tri.c, tri.a, 0.5f);

      subdivided.push_back({tri.a, mAB, mCA});
      subdivided.push_back({mAB, tri.b, mBC});
      subdivided.push_back({mCA, mBC, tri.c});
      subdivided.push_back({mAB, mBC, mCA});
    }

    triangles = std::move(subdivided);
  }

  return triangles;
}

Mesh buildMesh(const std::vector<Triangle> &triangles) {
  Mesh mesh{};
  mesh.triangleCount = static_cast<int>(triangles.size());
  mesh.vertexCount = static_cast<int>(triangles.size() * 3);

  mesh.vertices =
      static_cast<float *>(MemAlloc(mesh.vertexCount * 3 * sizeof(float)));
  mesh.normals =
      static_cast<float *>(MemAlloc(mesh.vertexCount * 3 * sizeof(float)));

  for (std::size_t i = 0; i < triangles.size(); i++) {
    const Vector3 verts[3] = {triangles[i].a, triangles[i].b, triangles[i].c};

    for (std::size_t j = 0; j < 3; j++) {
      std::size_t v = i * 3 + j;

      mesh.vertices[v * 3 + 0] = verts[j].x;
      mesh.vertices[v * 3 + 1] = verts[j].y;
      mesh.vertices[v * 3 + 2] = verts[j].z;

      // On a unit sphere centered at the origin, the outward normal at a
      // point is just that point's position, already normalized.
      mesh.normals[v * 3 + 0] = verts[j].x;
      mesh.normals[v * 3 + 1] = verts[j].y;
      mesh.normals[v * 3 + 2] = verts[j].z;
    }
  }

  UploadMesh(&mesh, false);
  return mesh;
}

} // namespace

Model Icosphere::generate(uint8_t subdivisions) {
  std::vector<Triangle> triangles = subdivide(baseTriangles(), subdivisions);
  Mesh mesh = buildMesh(triangles);
  return LoadModelFromMesh(mesh);
}
