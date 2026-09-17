#include "Icosphere.h"
#include "raylib/raylib.h"
#include <cmath>
#include <cstdint>
#include <vector>

float dot(Vector3 a, Vector3 b) {
  return (float((a.x * b.x) + (a.y * b.y) + (a.z * b.z)));
}

Vector3 mul(Vector3 a, float multiplier) {
  return Vector3{a.x * multiplier, a.y * multiplier, a.z * multiplier};
}

Vector3 add(Vector3 a, Vector3 b) {
  return Vector3{a.x + b.x, a.y + b.y, a.z + b.z};
}

Icosphere::Icosphere(uint8_t fd) : fragDepth(fd) {
  m_fragment(fd);
  UploadMesh(&mesh, true);
  model = LoadModelFromMesh(mesh);
}

Icosphere::~Icosphere() { UnloadModel(model); }

Vector3 Icosphere::m_slerp(Vector3 p0, Vector3 p1, float t) {
  float th = acos(dot(p0, p1));
  Vector3 slerp = add(mul(p0, (sin((1 - t) * th) / sin(th))),
                      mul(p1, (sin(t * th) / sin(th))));
  return slerp;
}
void Icosphere::m_fragment(uint8_t fragDepth) {
  struct Triangle {
    Vector3 a, b, c;
  };

  std::vector<Triangle> triangles;
  triangles.reserve(20);

  for (std::size_t i = 0; i < triangleFacesIndices.size(); i += 3) {
    int i0 = triangleFacesIndices[i + 0];
    int i1 = triangleFacesIndices[i + 1];
    int i2 = triangleFacesIndices[i + 2];

    triangles.push_back({
        vertices[i0],
        vertices[i1],
        vertices[i2],
    });
  }

  for (std::size_t depth = 0; depth < fragDepth; depth++) {
    std::vector<Triangle> subdivided;
    subdivided.reserve(triangles.size() * 4);

    for (const Triangle &tri : triangles) {
      Vector3 mAB = m_slerp(tri.a, tri.b, 0.5f);
      Vector3 mBC = m_slerp(tri.b, tri.c, 0.5f);
      Vector3 mCA = m_slerp(tri.c, tri.a, 0.5f);

      subdivided.push_back({tri.a, mAB, mCA});
      subdivided.push_back({mAB, tri.b, mBC});
      subdivided.push_back({mCA, mBC, tri.c});
      subdivided.push_back({mAB, mBC, mCA});
    }

    triangles = std::move(subdivided);
  }

  Mesh newMesh = {};
  newMesh.triangleCount = static_cast<int>(triangles.size());
  newMesh.vertexCount = static_cast<int>(triangles.size() * 3);

  newMesh.vertices =
      static_cast<float *>(MemAlloc(newMesh.vertexCount * 3 * sizeof(float)));

  for (std::size_t i = 0; i < triangles.size(); i++) {
    const Vector3 verts[3] = {
        triangles[i].a,
        triangles[i].b,
        triangles[i].c,
    };

    for (std::size_t j = 0; j < 3; j++) {
      newMesh.vertices[(i * 3 + j) * 3 + 0] = verts[j].x;
      newMesh.vertices[(i * 3 + j) * 3 + 1] = verts[j].y;
      newMesh.vertices[(i * 3 + j) * 3 + 2] = verts[j].z;
    }
  }

  mesh = newMesh;
}

void Icosphere::setFragmentDepth(uint8_t fd) {
  if (fd == fragDepth)
    return;

  fragDepth = fd;

  UnloadModel(model);

  m_fragment(fd);

  UploadMesh(&mesh, true);

  model = LoadModelFromMesh(mesh);
}
