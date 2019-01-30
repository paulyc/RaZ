#include "catch/catch.hpp"
#include "RaZ/Math/Matrix.hpp"
#include "RaZ/Math/Vector.hpp"
#include "RaZ/Utils/FloatUtils.hpp"

namespace {

// Declaring vectors to be tested
const Raz::Vec3f vec31({ 3.18f, 42.f, 0.874f });
const Raz::Vec3f vec32({ 541.41f, 47.25f, 6.321f });

const Raz::Vec4f vec41({ 84.47f, 2.f, 0.001f, 847.12f });
const Raz::Vec4f vec42({ 13.01f, 0.15f, 84.8f, 72.f });

} // namespace

TEST_CASE("Vector near-equality") {
  REQUIRE_FALSE(vec31 == vec32);

  const Raz::Vec3f baseVec(1.f);
  Raz::Vec3f compVec = baseVec;

  REQUIRE(baseVec[0] == compVec[0]); // Copied, strict equality
  REQUIRE(baseVec[1] == compVec[1]);
  REQUIRE(baseVec[2] == compVec[2]);

  compVec += 0.0000001f; // Adding a tiny offset

  REQUIRE_FALSE(baseVec[0] == compVec[0]); // Values not strictly equal
  REQUIRE_FALSE(baseVec[1] == compVec[1]);
  REQUIRE_FALSE(baseVec[2] == compVec[2]);

  REQUIRE(Raz::FloatUtils::checkNearEquality(baseVec[0], compVec[0])); // Near-equality components check
  REQUIRE(Raz::FloatUtils::checkNearEquality(baseVec[1], compVec[1]));
  REQUIRE(Raz::FloatUtils::checkNearEquality(baseVec[2], compVec[2]));

  REQUIRE(baseVec == compVec); // Vector::operator== does a near-equality check on floating point types
}

TEST_CASE("Vector/scalar operations") {
  REQUIRE((vec31 * 3.f) == Raz::Vec3f({ 9.54f, 126.f, 2.622f }));
  REQUIRE((vec31 * 4.152f) == Raz::Vec3f({ 13.20336f, 174.384f, 3.628848f }));

  REQUIRE((vec41 * 7.5f) == Raz::Vec4f({ 633.525f, 15.f, 0.0075f, 6353.4f }));
  REQUIRE((vec41 * 8.0002f) == Raz::Vec4f({ 675.776894f, 16.0004f, 0.0080002f, 6777.129424f }));
  REQUIRE((vec41 * 0.f) == Raz::Vec4f({ 0.f, 0.f, 0.f, 0.f }));
}

TEST_CASE("Vector/vector operations") {
  REQUIRE((vec31 - vec31) == Raz::Vec3f(0.f));
  REQUIRE((vec31 * vec32) == Raz::Vec3f({ 1721.6838f, 1984.5f, 5.524554f }));

  REQUIRE(vec31.dot(vec31) == vec31.computeSquaredLength());
  REQUIRE(Raz::FloatUtils::checkNearEquality(vec31.dot(vec31), 1774.876276f));
  REQUIRE(Raz::FloatUtils::checkNearEquality(vec31.dot(vec32), 3711.708354f));
  REQUIRE(vec31.dot(vec32) == vec32.dot(vec31)); // A · B == B · A

  REQUIRE(vec31.cross(vec32) == Raz::Vec3f({ 224.1855f, 453.09156f, -22588.965f }));
  REQUIRE(vec31.cross(vec32) == -vec32.cross(vec31)); // A x B == -(B x A)
}

TEST_CASE("Vector/matrix operations") {
  const Raz::Mat3f mat3({{ 4.12f,  25.1f, 30.7842f },
                         { 3.04f,    5.f,   -64.5f },
                         {  -1.f, -7.54f,    8.41f }});
  REQUIRE((vec31 * mat3) == Raz::Vec3f({ 139.9076f, 283.22804f, -2603.755904f }));
  REQUIRE((vec32 * mat3) == Raz::Vec3f({ 2367.9282f, 13'777.980'66f, 13'672.408'332f }));

  const Raz::Mat4f mat4({{  -3.2f, 53.032f,  832.451f,   74.2f },
                         { 10.01f,   3.15f,   -91.41f, 187.46f },
                         {   -6.f,  -7.78f,      90.f,    38.f },
                         {  123.f,  -74.8f, 147.0001f,  748.6f }});
  REQUIRE((vec41 * mat4) == Raz::Vec4f({ 103'945.47f, -58878.67074f, 194'661.130'682f, 640'796.664f }));
  REQUIRE((vec42 * mat4) == Raz::Vec4f({ 8307.0695f, -5354.92518f, 29032.48321f, 58115.061f }));

  REQUIRE((vec31 * Raz::Mat3f::identity()) == vec31);
  REQUIRE((vec41 * Raz::Mat4f::identity()) == vec41);
}

TEST_CASE("Vector manipulations") {
  REQUIRE(Raz::FloatUtils::checkNearEquality(vec31.normalize().computeLength(), 1.f));
  REQUIRE(Raz::FloatUtils::checkNearEquality(vec41.normalize().computeSquaredLength(), 1.f));
  REQUIRE(Raz::FloatUtils::checkNearEquality(Raz::Vec3f({ 0.f, 1.f, 0.f }).computeLength(), 1.f));

  // Testing Vector::reflect():
  //
  // IncVec  N  Reflection
  //     \   |   /
  //      \  |  /
  //       \ | /
  //________\|/___________
  //
  REQUIRE(Raz::Vec3f({ 1.f, -1.f, 0.f }).reflect(Raz::Vec3f({ 0.f, 1.f, 0.f })) == Raz::Vec3f({ 1.f, 1.f, 0.f }));
  REQUIRE(vec31.reflect(Raz::Vec3f({ 0.f, 1.f, 0.f })) == Raz::Vec3f({ 3.18f, -42.f, 0.874f }));
  REQUIRE(vec31.reflect(vec32) == Raz::Vec3f({ -4'019'108.859'878'28f, -350'714.439'453f, -46'922.543'011'268f }));
}
