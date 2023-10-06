#pragma once
#include "Light.h"
#include"OmniShadowMap.h"

namespace LNS 
{
    class PointLight : public Light
    {
      public:
          PointLight();
          // overloaded constructor
          PointLight(GLuint shadowWidth,GLuint shadowHeight,
              GLfloat near, GLfloat far,
              GLfloat red, GLfloat green, GLfloat blue,
              GLfloat aIntensity, GLfloat dIntensity,
              GLfloat xPos, GLfloat yPos, GLfloat zPos,
              GLfloat Constant, GLfloat Linear, GLfloat Exponent);

          // Function overloading with Inheritance
         void useLight(GLuint ambientIntensityLoc, GLuint ambientColorLoc,
              GLuint diffuseIntensityLoc, GLuint positionLoc,
              GLuint constantLoc,GLuint linearLoc,GLuint exponentLoc);

         virtual void UseLight(GLfloat ambientIntensityLoc, GLfloat ambientColorLoc,
              GLfloat diffuseIntensityLoc, GLfloat directionLoc){}

          virtual glm::mat4 CalcLightTransform()override { return LightProj; }
          
          // It will return more than one matrix so that we can not inherited one matrix. Create another method returns a vector for each face
          std::vector<glm::mat4> calcLightTransform();

          inline GLfloat GetFarPlane()const { return farPlane; };
          inline glm::vec3 GetPosition()const { return position; }

          virtual ~PointLight();
      protected:
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

          GLfloat farPlane;
    };
}
