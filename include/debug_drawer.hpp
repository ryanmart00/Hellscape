#ifndef DEBUG_DRAWER_HPP
#define DEBUG_DRAWER_HPP

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "shader.hpp"

class DebugDrawer : public btIDebugDraw {
public:
    unsigned int VBO, VAO;
    DebugDrawer();

	void SetMatrices(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color); 

	virtual void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &);
	
    virtual void reportErrorWarning(const char * warn);
	virtual void draw3dText(const btVector3 &, const char *);
	virtual void setDebugMode(int p);
	int getDebugMode(void) const;
    void drawLinesWorld();
	int m;
    Shader shader;
    std::vector<float> points_;
    int numPoints_;
};
#endif
