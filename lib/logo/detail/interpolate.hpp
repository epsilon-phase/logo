#pragma once
#include <SFML/Graphics/Color.hpp>
namespace logo{
  struct Point;
  namespace __detail{
    
    template<typename T> T lerp(float f, const T& a,const T& b) {
      return a+(b-a)*f;
    }
  }
  
}