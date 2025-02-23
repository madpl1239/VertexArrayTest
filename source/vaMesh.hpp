/*
 * vaMesh.hpp
 *
 * The VAMesh class defines a mesh for 3D models that
 * will be rendered using vertex arrays without
 * using shaders. 
 *
 * 23-07-2023 by madpl (LANA Systems)
 */
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using uint = unsigned int;

using Vertex = struct 
{
	glm::vec3 Position;
	glm::vec3 Normal;
};

using vertexContainer = std::vector<Vertex>;
using uintContainer = std::vector<uint>;


class VAMesh
{
public:	
	VAMesh(vertexContainer vert, uintContainer inds)
	{
		this->vertices.clear();
		this->indices.clear();
		
		this->vertices = std::move(vert);
		this->indices = std::move(inds);
	}
	
	VAMesh(const VAMesh& rhs)
	{
		this->vertices = rhs.vertices;
		this->indices = rhs.indices;
	}	
	
	VAMesh& operator=(const VAMesh& rhs)
	{
		this->vertices = rhs.vertices;
		this->indices = rhs.indices;
		
		return *this;
	}
	
	~VAMesh()
	{
		this->vertices.clear();
		this->indices.clear();
		
		glDeleteBuffers(1, &VBO);
	}
	
	void Draw()
	{
		glGenBuffers(1, &VBO);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Position));
		glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)indices.data());
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

private:
	vertexContainer vertices{};
	uintContainer indices{};
	
	GLuint VBO{0};
};
