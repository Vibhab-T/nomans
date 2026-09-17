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
  mesh.vertexCount = vertices.size();
  mesh.triangleCount = triangleFacesIndices.size() / 3;
  // allocate vertex positions
  // vertexCount * 3 becuase one vertex is x,y,z
  // the loop:
  // raylib expects flat array just like the triangleFacesIndices for vertex
  //[x0,y0,z0, x1,y1,z1, x2,y2,z2] - the format
  mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
  for (std::size_t i = 0; i < mesh.vertexCount; i++) {
    mesh.vertices[i * 3 + 0] = vertices[i].x;
    mesh.vertices[i * 3 + 1] = vertices[i].y;
    mesh.vertices[i * 3 + 2] = vertices[i].z;
  }
  // allocate indices
  mesh.indices = (unsigned short *)MemAlloc(triangleFacesIndices.size() *
                                            sizeof(unsigned short));
  for (std::size_t i = 0; i < triangleFacesIndices.size(); i++) {
    mesh.indices[i] = triangleFacesIndices[i];
  }

  m_fragment(fd);
  UploadMesh(&mesh, true);
  model = LoadModelFromMesh(mesh);
}

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

  for (std::size_t i = 0; i < mesh.triangleCount; i++) {
    int i0 = mesh.indices[i * 3 + 0];
    int i1 = mesh.indices[i * 3 + 1];
    int i2 = mesh.indices[i * 3 + 2];

    Triangle tri;

    tri.a = {
        mesh.vertices[i0 * 3 + 0],
        mesh.vertices[i0 * 3 + 1],
        mesh.vertices[i0 * 3 + 2],
    };

    tri.b = {
        mesh.vertices[i1 * 3 + 0],
        mesh.vertices[i1 * 3 + 1],
        mesh.vertices[i1 * 3 + 2],
    };

    tri.c = {
        mesh.vertices[i2 * 3 + 0],
        mesh.vertices[i2 * 3 + 1],
        mesh.vertices[i2 * 3 + 2],
    };

    triangles.push_back(tri);
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

  // unload the previous mesh
  // and then overwrite it
  UnloadMesh(mesh);

  mesh = {};
  mesh.triangleCount = static_cast<int>(triangles.size());
  mesh.vertexCount = static_cast<int>(triangles.size() * 3);

  mesh.vertices =
      static_cast<float *>(MemAlloc(mesh.vertexCount * 3 * sizeof(float)));

  for (std::size_t i = 0; i < triangles.size(); i++) {
    Vector3 verts[3] = {
        triangles[i].a,
        triangles[i].b,
        triangles[i].c,
    };

    for (std::size_t j = 0; j < 3; j++) {
      mesh.vertices[(i * 3 + j) * 3 + 0] = verts[j].x;
      mesh.vertices[(i * 3 + j) * 3 + 1] = verts[j].y;
      mesh.vertices[(i * 3 + j) * 3 + 2] = verts[j].z;
    }
  }
}
