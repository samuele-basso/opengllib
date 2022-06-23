#include "gfxpch.h"
#include "Mesh.h"

#include "Base/Log.h"
#include "Base/UUID.h"
#include "Transform.h"

namespace Jam
{

	Material::Material()
		: AlbedoValue(glm::vec4(0.25, 0.25, 0.25, 1.0)), MetalnessValue(0.25), RoughnessValue(0.25), BaseReflectivity(0.4)
	{
	}

	Mesh::Mesh()
		: IndicesCount(0), VerticesSize(0), HasTextureCoords(false)
	{
	}

	std::vector<std::tuple<Mesh*, Material*>> Model::Load(const char* f_Path)
	{
		Assimp::Importer importer;

		auto config =
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ImproveCacheLocality |
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_FindDegenerates |
			aiProcess_FindInvalidData |
			aiProcess_FindInstances |
			aiProcess_ValidateDataStructure;

		const aiScene* scene = importer.ReadFile(f_Path, config);

		std::vector<std::tuple<Mesh*, Material*>> meshes;

		std::unordered_map<unsigned int, Material*> materialCache;

		ProcessNode(scene->mRootNode, (aiScene*)scene, nullptr, f_Path, meshes, materialCache);
		return meshes;
	}

	std::string Model::GenMeshName(const aiMesh* f_Mesh)
	{
		const char* name = f_Mesh->mName.C_Str();
		if (strlen(name) > 0)
		{
			return f_Mesh->mName.C_Str();
		}
		return "noname";
	}

	void Model::ProcessNode(const aiNode* f_Node, aiScene* f_Scene, const aiMatrix4x4* f_Transform, const char* f_Path, std::vector<std::tuple<Mesh*, Material*>>& f_Meshes, std::unordered_map<unsigned int, Material*>& f_Cache)
	{
		aiMatrix4x4* currentTransform = const_cast<aiMatrix4x4*>(&f_Node->mTransformation);
		if (f_Transform) *currentTransform *= *f_Transform;


		for (int i = 0; i < f_Node->mNumMeshes; i++)
		{
			aiMesh* mesh = f_Scene->mMeshes[f_Node->mMeshes[i]];
			
			std::tuple<Mesh*, Material*> loadedmesh = ProcessMesh(mesh, currentTransform, f_Scene, f_Path, f_Cache);
			if(std::get<0>(loadedmesh)) f_Meshes.push_back(loadedmesh);
		}

		for (int i = 0; i < f_Node->mNumChildren; i++)
		{
			ProcessNode(f_Node->mChildren[i], f_Scene, currentTransform, f_Path, f_Meshes, f_Cache);
		}
	}

	std::tuple<Mesh*, Material*> Model::ProcessMesh(const aiMesh* f_Mesh, const aiMatrix4x4* f_Transform, aiScene* f_Scene, const char* f_Path, std::unordered_map<unsigned int, Material*>& f_Cache)
	{
		if (!f_Mesh->HasNormals())
		{
			JAM_CORE_CRITICAL("invalid mesh format, normals missing");
			return { nullptr, nullptr };
		}

		auto& mesh = *(new Mesh());

		int stride = 12 * sizeof(float);

		if (f_Mesh->HasTextureCoords(0))
		{
			mesh.HasTextureCoords = true;
			stride += 2 * sizeof(float);
		}

		mesh.VerticesSize = f_Mesh->mNumVertices * stride;
		mesh.Vertices.reserve(mesh.VerticesSize);

		if (mesh.HasTextureCoords)
		{
			for (int i = 0; i < f_Mesh->mNumVertices; i++)
			{
				float vertex[] = {
					f_Mesh->mVertices[i].x, f_Mesh->mVertices[i].y, f_Mesh->mVertices[i].z,
					f_Mesh->mNormals[i].x, f_Mesh->mNormals[i].y, f_Mesh->mNormals[i].z,
					f_Mesh->mTangents[i].x, f_Mesh->mTangents[i].y, f_Mesh->mTangents[i].z,
					f_Mesh->mBitangents[i].x, f_Mesh->mBitangents[i].y, f_Mesh->mBitangents[i].z,
					f_Mesh->mTextureCoords[0][i].x, f_Mesh->mTextureCoords[0][i].y
				};

				auto const ptr = reinterpret_cast<char*>(&vertex);
				std::vector<char> buffer(ptr, ptr + sizeof(vertex));
				mesh.Vertices.insert(mesh.Vertices.end(), buffer.begin(), buffer.end());
			}
		}
		else
		{
			for (int i = 0; i < f_Mesh->mNumVertices; i++)
			{
				float vertex[] = {
					f_Mesh->mVertices[i].x, f_Mesh->mVertices[i].y, f_Mesh->mVertices[i].z,
					f_Mesh->mNormals[i].x, f_Mesh->mNormals[i].y, f_Mesh->mNormals[i].z,
					f_Mesh->mTangents[i].x, f_Mesh->mTangents[i].y, f_Mesh->mTangents[i].z,
					f_Mesh->mBitangents[i].x, f_Mesh->mBitangents[i].y, f_Mesh->mBitangents[i].z
				};

				auto const ptr = reinterpret_cast<char*>(&vertex);
				std::vector<char> buffer(ptr, ptr + sizeof(vertex));
				mesh.Vertices.insert(mesh.Vertices.end(), buffer.begin(), buffer.end());
			}
		}

		for (int i = 0; i < f_Mesh->mNumFaces; i++)
		{
			mesh.IndicesCount += f_Mesh->mFaces[i].mNumIndices;
		}

		mesh.Indices.reserve(mesh.IndicesCount);

		for (int i = 0; i < f_Mesh->mNumFaces; i++)
		{
			for (int j = 0; j < f_Mesh->mFaces[i].mNumIndices; j++)
			{
				mesh.Indices.push_back(f_Mesh->mFaces[i].mIndices[j]);
			}
		}

		auto location = f_Cache.find(f_Mesh->mMaterialIndex);
		if (location != f_Cache.end())
			return { &mesh, location->second };

		aiMaterial* material = f_Scene->mMaterials[f_Mesh->mMaterialIndex];

		Material& newMat = *new Material();
		f_Cache.insert(std::make_pair(f_Mesh->mMaterialIndex, &newMat));

		aiTextureType textureTypes[] = {
			aiTextureType_BASE_COLOR,
			aiTextureType_NORMAL_CAMERA,
			aiTextureType_METALNESS,
			aiTextureType_DIFFUSE_ROUGHNESS
		};

		std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> textureConfig{
			{ TextureParameter::WrapS, TextureParameterValue::Repeat },
			{ TextureParameter::WrapT, TextureParameterValue::Repeat },
			{ TextureParameter::FilterMin, TextureParameterValue::Linear },
			{ TextureParameter::FilterMag, TextureParameterValue::Linear }
		};

		for (aiTextureType type : textureTypes)
		{
			int textureCount = material->GetTextureCount(type);
			for (int i = 0; i < textureCount; i++)
			{
				aiString path;
				material->GetTexture(type, i, &path, NULL, NULL, NULL, NULL, NULL);

				const aiTexture* texture = f_Scene->GetEmbeddedTexture(path.C_Str());
				std::shared_ptr<Texture2D> matTexture = nullptr;
				if (texture)
				{
					std::pair<const aiTexture*, int> ti = f_Scene->GetEmbeddedTextureAndIndex(path.C_Str());
					int width = ti.first->mWidth, height = ti.first->mHeight;
					matTexture = std::make_shared<Texture2D>(textureConfig, width, height, 4, ti.first->pcData);
				}
				else
				{
					matTexture = std::make_shared<Texture2D>(
						textureConfig,
						std::filesystem::path(f_Path).remove_filename().concat(path.C_Str()).string()
					);
				}

				switch (type)
				{
				case aiTextureType_DIFFUSE:
					newMat.Albedo.Texture = matTexture;
					newMat.Albedo.Active = true;
					break;
				case aiTextureType_NORMALS:
					newMat.Normal.Texture = matTexture;
					break;
				case aiTextureType_METALNESS:
					newMat.Metalness.Texture = matTexture;
					break;
				case aiTextureType_DIFFUSE_ROUGHNESS:
					newMat.Roughness.Texture = matTexture;
					break;
				default:
					break;
				}
			}
		}

		return { &mesh, &newMat };
	}

}