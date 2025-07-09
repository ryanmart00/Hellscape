#include "debug_drawer.hpp"
#include <glad/gl.h>
#include <iostream>
DebugDrawer::DebugDrawer()
    : m{0}, shader{0,0,"assets/gl/debug.vs","assets/gl/debug.fs"}
{
}

void DebugDrawer::SetMatrices(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) 
{
    shader.use();
    shader.setMat4("projection", projectionMatrix);
    shader.setMat4("view", viewMatrix);
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    points_.push_back(from.x());
    points_.push_back(from.y());
    points_.push_back(from.z());
    points_.push_back(color.x());
    points_.push_back(color.y());
    points_.push_back(color.z());
    points_.push_back(to.x());
    points_.push_back(to.y());
    points_.push_back(to.z());
    points_.push_back(color.x());
    points_.push_back(color.y());
    points_.push_back(color.z());
    numPoints_ += 2;
}

void DebugDrawer::drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) {}

void DebugDrawer::reportErrorWarning(const char * warn) 
{
    std::cout << warn << std::endl;
}

void DebugDrawer::draw3dText(const btVector3 &, const char *) {}

void DebugDrawer::setDebugMode(int p) { m = p; }

int DebugDrawer::getDebugMode(void) const { return m; }

void DebugDrawer::drawLinesWorld() {
    shader.use();
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points_.size() * sizeof(GLfloat), &points_[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6*sizeof(GLfloat),
        (void*) 0
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6*sizeof(GLfloat),
        (void*) (3 * sizeof(GLfloat))
    );
    glBindVertexArray(0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, numPoints_);
    glBindVertexArray(0);
    points_.clear();
    numPoints_ = 0;
}
