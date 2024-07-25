#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>
struct Vertex {
  // homogenous column matrix form
  float x, y, z, h;
  Vertex(float x, float y, float z, float h) : x(x), y(y), z(z), h(h) {}
};

class Matrix4x4 {
  std::vector<std::vector<float>> matrix;

public:
  Matrix4x4(const std::vector<std::vector<float>> &rows) : matrix(rows) {}

  // overload * for matrix-vector multiplication
  Vertex operator*(const Vertex &v) const {
    float thresold = 0.5f; // minimum thresold for abosulte value (avoids
                           // verysmall negative numbers)
    std::vector<float> result(4, 0.0f);

    // matrix-vector multiplication
    for (int i = 0; i < 4; ++i) {
      result[i] = matrix[i][0] * v.x + matrix[i][1] * v.y + matrix[i][2] * v.z +
                  matrix[i][3] * v.h;

      if (fabs(result[i]) < thresold) {
        result[i] = 0.0f;
      }
    }

    return Vertex(result[0], result[1], result[2], result[3]);
  }
};

/*
 * A box in a 3d coordinate system is represented by 8 vertices
 */
class Box {
private:
  std::vector<Vertex> m_vertices;
  int m_Size;

public:
  Box() {}
  Box(std::vector<Vertex> vertices) : m_vertices(vertices) {
    m_Size = m_vertices.size();

    // homogeneous normalization
    for (Vertex &v : m_vertices) {
      v.x /= v.h;
      v.y /= v.h;
      v.z /= v.h;
      v.h /= v.h;
    }
  }

  std::vector<Vertex> GetVerticies() const { return m_vertices; };

  void SetVerticies(std::vector<Vertex> vertices) { m_vertices = vertices; }

  /*
   * Prints all vertices of box with each vertex as a Column Vector
   */
  void Print() {
    for (int i = 0; i < m_Size * 4 * 2; i++)
      std::cout << "-";
    std::cout << std::endl;
    std::cout << std::setprecision(2);
    for (int i = 0; i < m_Size; i++) {
      std::cout << m_vertices[i].x << "\t";
    }
    std::cout << std::endl;
    for (int i = 0; i < m_Size; i++) {
      std::cout << m_vertices[i].y << "\t";
    }
    std::cout << std::endl;
    for (int i = 0; i < m_Size; i++) {
      std::cout << m_vertices[i].z << "\t";
    }
    std::cout << std::endl;
    for (int i = 0; i < m_Size; i++) {
      std::cout << m_vertices[i].h << "\t";
    }
    std::cout << std::endl;
  }
};

/*
 * Implements 3D transformation using Matrix multiplication.
 * ALL ANGLES ARE ASSUMED TO BE IN DEGREES.
 * Availabe transformations:
 * - Rotation about X axis
 * - Rotation about Y axis
 * - Rotation about Z axis
 * - Scaling
 */
class Tranformation {

public:
  static std::shared_ptr<Box> rotateAboutX(std::shared_ptr<Box> box,
                                           int angle_x) {
    float radians = angle_x * M_PI / 180;
    std::vector<Vertex> resultVerticies;
    Matrix4x4 XRotationMatrix =
        Matrix4x4({{1, 0, 0, 0},
                   {0, std::cos(radians), -std::sin(radians), 0},
                   {0, std::sin(radians), std::cos(radians), 0},
                   {0, 0, 0, 1}});

    for (Vertex v : box->GetVerticies()) {
      resultVerticies.push_back(XRotationMatrix * v);
    }

    return std::make_shared<Box>(resultVerticies);
  }

  static std::shared_ptr<Box> rotateAboutY(std::shared_ptr<Box> box,
                                           int angle_x) {
    float radians = angle_x * M_PI / 180;
    std::vector<Vertex> resultVerticies;
    Matrix4x4 YRotationMatrix =
        Matrix4x4({{std::cos(radians), 0, std::sin(radians), 0},
                   {0, 1, 0, 0},
                   {-std::sin(radians), 0, std::cos(radians), 0},
                   {0, 0, 0, 1}});

    for (Vertex v : box->GetVerticies()) {
      resultVerticies.push_back(YRotationMatrix * v);
    }

    return std::make_shared<Box>(resultVerticies);
  }

  static std::shared_ptr<Box> rotateAboutZ(std::shared_ptr<Box> box,
                                           int angle_x) {
    float radians = angle_x * M_PI / 180;
    std::vector<Vertex> resultVerticies;
    Matrix4x4 YRotationMatrix =
        Matrix4x4({{std::cos(radians), -std::sin(radians), 0, 0},
                   {std::sin(radians), std::cos(radians), 0, 0},
                   {0, 0, 1, 0},
                   {0, 0, 0, 1}});

    for (Vertex v : box->GetVerticies()) {
      resultVerticies.push_back(YRotationMatrix * v);
    }

    return std::make_shared<Box>(resultVerticies);
  }

  static std::shared_ptr<Box> scale(std::shared_ptr<Box> box,
                                    float scale_factor) {
    std::vector<Vertex> resultVerticies;
    Matrix4x4 YRotationMatrix = Matrix4x4({{scale_factor, 0, 0, 0},
                                           {0, scale_factor, 0, 0},
                                           {0, 0, scale_factor, 0},
                                           {0, 0, 0, 1}});

    for (Vertex v : box->GetVerticies()) {
      resultVerticies.push_back(YRotationMatrix * v);
    }

    return std::make_shared<Box>(resultVerticies);
  }
};

int main() {
  // vertices for unit cube
  std::vector<Vertex> boxVertices = {Vertex(0, 0, 0, 1), Vertex(1, 0, 0, 1),
                                     Vertex(1, 1, 0, 1), Vertex(0, 1, 0, 1),
                                     Vertex(0, 0, 1, 1), Vertex(1, 0, 1, 1),
                                     Vertex(1, 1, 1, 1), Vertex(0, 1, 1, 1)};

  std::shared_ptr<Box> box = std::make_shared<Box>(boxVertices);

  int transformationAngle, scaleFactor;

  std::cout << "Enter transformation angle in degrees: ";
  std::cin >> transformationAngle;
  std::cout << "Enter scaling factor: ";
  std::cin >> scaleFactor;

  std::shared_ptr<Box> rotatedX =
      Tranformation::rotateAboutX(box, transformationAngle);
  std::shared_ptr<Box> rotatedY =
      Tranformation::rotateAboutY(box, transformationAngle);
  std::shared_ptr<Box> rotatedZ =
      Tranformation::rotateAboutZ(box, transformationAngle);

  std::shared_ptr<Box> scaled = Tranformation::scale(box, scaleFactor);
  std::cout << "Original Box: " << std::endl;
  box->Print();
  std::cout << "Rotated about X axis by " << transformationAngle
            << " degrees: " << std::endl;
  rotatedX->Print();
  std::cout << "Rotated about Y axis by " << transformationAngle
            << " degrees: " << std::endl;
  rotatedY->Print();
  std::cout << "Rotated about Z axis by " << transformationAngle
            << " degrees: " << std::endl;
  rotatedZ->Print();
  std::cout << "Scaled by factor of " << scaleFactor << ": " << std::endl;
  scaled->Print();
}
