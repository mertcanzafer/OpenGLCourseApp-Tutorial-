#pragma once
#include "Light.h"
namespace LNS 
{
    class PointLight : public Light
    {
      public:
          PointLight();
          // overloaded constructor
          PointLight(GLfloat red, GLfloat green, GLfloat blue,
              GLfloat aIntensity, GLfloat dIntensity,
              GLfloat xPos, GLfloat yPos, GLfloat zPos,
              GLfloat Constant, GLfloat Linear, GLfloat Exponent);

          // Function overloading with Inheritance
          void UseLight(GLfloat ambientIntensityLoc, GLfloat ambientColorLoc,
              GLfloat diffuseIntensityLoc, GLfloat positionLoc,
              GLfloat constantLoc,GLfloat linearLoc,GLfloat exponentLoc);

        ~PointLight();
      private:
          glm::vec3 position; // position of our light source in 3d world space

          /*
                                                             1
              Attenuation Factor =  -------------------------------------------------------
                                     quadratic * distance^2 + linaer * distance + constant

              distance = Dinstance betweem light and fragment
              quadratic = User-defined value, usually the lowest of the three.
              linear =  User-defined value, lower than constant.
              constant = Usually 1.0 to ensure denominator is always greater than 1.
          */

          GLfloat constant;
          GLfloat linear;
          GLfloat exponent;
    };
}