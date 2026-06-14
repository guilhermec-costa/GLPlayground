#include "transform.h"
#include <iostream>

void vectors() {
  Vec3 v1{5, 5 ,5};
  Vec3 v2{2, 3 ,-2};
  Vec3 v3 = v1+v2;
  Vec3 n = v3.normalize();

  Vec3 vv1 = {0.6, -0.8, 0};
  Vec3 vv2 = {0, 1, 0};
  std::cout << "Dod product: " << vv1.dotProduct(vv2) << "\n";
  std::cout << "length of normalized: " << n.length() << "\n";
}