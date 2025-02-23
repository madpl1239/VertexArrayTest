/*
 * vaModel.hpp
 *
 * VAModel class defining a 3D model loaded
 * using the assimp library.
 *
 * 23-07-2023 by madpl (LANA Systems)
 */
#pragma once

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include <vector>
#include "vaMesh.hpp"


using meshContainer = std::vector<VAMesh>;


class VAModel
{
public:
	VAModel(const std::string path)
	{
		loadModel(path);
		
		#if DEBUG
		std::cout << "ctor VAModel\n";
		#endif
	}
	
	VAModel(const VAModel& rhs)
	{
		this->meshes = rhs.meshes;
		this->directory = rhs.directory;
		
		#if DEBUG
		std::cout << "ctor VAModel\n";
		#endif
	}
	
	VAModel& operator=(const VAModel& rhs)
	{
		this->meshes = rhs.meshes;
		this->directory = rhs.directory;
		
		return *this;
	}
	
	~VAModel()
	{
		meshes.clear();
		directory.clear();
		
		#if DEBUG
		std::cout << "dtor VAModel\n";
		#endif
	}
	
	void Draw()
	{
		for(auto mesh : meshes)
			mesh.Draw();
	}

	meshContainer meshes;
	std::string directory;
	
private:
	void loadModel(const std::string path)
	{
		Assimp::Importer importer;
		
		const aiScene* sc = importer.ReadFile(path, aiProcess_Triangulate);
		// const aiScene* sc = importer.ReadFile(path, 0);
		
		if(!sc or sc->mFlags & AI_SCENE_FLAGS_INCOMPLETE or !sc->mRootNode)
			throw std::runtime_error(importer.GetErrorString());
		
		this->directory = path.substr(0, path.find_last_of('/'));
		
		processNode(sc->mRootNode, sc);
	}

	void processNode(aiNode* node, const aiScene* scene)
	{
		// process node meshes if they exist
		for(unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(processMesh(mesh, scene));
		}
		
		// do the same for each child of the node
		for(unsigned int i = 0; i < node->mNumChildren; i++)
			processNode(node->mChildren[i], scene);
	}
	
	VAMesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		vertexContainer vertices{};
		uintContainer indices{};
		
		for(unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 v;
			
			// processing positions
			v.x = mesh->mVertices[i].x;
			v.y = mesh->mVertices[i].y;
			v.z = mesh->mVertices[i].z;
			vertex.Position = v;

			// processing normals
			if(mesh->HasNormals())
			{
				v.x = mesh->mNormals[i].x;
				v.y = mesh->mNormals[i].y;
				v.z = mesh->mNormals[i].z;
				vertex.Normal = v;
			}
			else
				vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
			
			vertices.push_back(std::move(vertex));
		}
		
		// processing indexes
		for(unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			
			for(unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(std::move(face.mIndices[j]));
		}
		
		return VAMesh(vertices, indices);
	}
};
