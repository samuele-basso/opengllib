#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include "Primitives/Texture.h"

#include "Scene.h"

#include "Base/UUID.h"

namespace Jam
{

	class Material
	{
	public:
		Material();
		~Material() = default;
		Material(const Material&) = default;

	public:
		struct MaterialComponent
		{
			MaterialComponent() : Texture(nullptr), Active(false) { }

			std::shared_ptr<Texture2D> Texture;
			bool Active;

		} Albedo, Metalness, Roughness, Normal;

		glm::vec3 BaseReflectivity;

		glm::vec4 AlbedoValue;
		glm::vec3 MetalnessValue;
		float RoughnessValue;
	};

	class Mesh : public UUID
	{
	public:
		Mesh();
		~Mesh() = default;
		Mesh(const Mesh&) = default;

		std::vector<unsigned int> Indices;
		int IndicesCount;

		std::vector<char> Vertices;
		size_t VerticesSize;

		bool HasTextureCoords;
	};

	class Model
	{
	public:
		static std::vector<std::tuple<Mesh*, Material*>> Load(const char* f_Path);

	private:
		static std::string GenMeshName(const aiMesh* f_Mesh);

		static void ProcessNode(
			const aiNode* f_Node,
			aiScene* f_Scene,
			const aiMatrix4x4* f_Transform,
			const char* f_Path,
			std::vector<std::tuple<Mesh*, Material*>>& f_Meshes,
			std::unordered_map<unsigned int, Material*>& f_Cache
		);

		static std::tuple<Mesh*, Material*> ProcessMesh(
			const aiMesh* f_Mesh,
			const aiMatrix4x4* f_Transform,
			aiScene* f_Scene,
			const char* f_Path,
			std::unordered_map<unsigned int, Material*>& f_Cache
		);
	};

}