#pragma once
#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 Texcoords;
};

struct Texture {
	unsigned int id;
	string type;
	aiString path;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int cubemap;
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, unsigned int cubemap) :vertices(vertices), indices(indices), textures(textures), cubemap(cubemap) {
		setupMesh();
	}
	void Draw(Shader shader) {
		glActiveTexture(GL_TEXTURE0);
		shader.setInt("skybox", 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		//顶点位置
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		//顶点法线
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Normal));
		//顶点uv
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Texcoords));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

};

#endif
