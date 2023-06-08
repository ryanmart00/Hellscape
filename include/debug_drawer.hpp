#ifndef DEBUG_DRAWER_HPP
#define DEBUG_DRAWER_HPP

#include <btBulletDynamicsCommon.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"

class DebugDrawer : public btIDebugDraw {
public:
    GLuint VBO, VAO;
    DebugDrawer()
        : m{0}, shader{0,0,"assets/gl/debug.vs","assets/gl/debug.fs"}
    {
    }

	void SetMatrices(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) 
	{
        shader.use();
        shader.setMat4("projection", projectionMatrix);
        shader.setMat4("view", viewMatrix);
	}

    void Draw()
    {

    }

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) 
	{
		// Vertex data
		GLfloat points[12];

		points[0] = from.x();
		points[1] = from.y();
		points[2] = from.z();
		points[3] = color.x();
		points[4] = color.y();
		points[5] = color.z();

		points[6] = to.x();
		points[7] = to.y();
		points[8] = to.z();
		points[9] = color.x();
		points[10] = color.y();
		points[11] = color.z();

        shader.use();


		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 
            (void*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);

	}
	virtual void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) {}
	virtual void reportErrorWarning(const char * warn) 
    {
        std::cout << warn << std::endl;
    }
	virtual void draw3dText(const btVector3 &, const char *) {}
	virtual void setDebugMode(int p) {
		m = p;
	}
	int getDebugMode(void) const { return m; }
	int m;
    Shader shader;
};
#endif
