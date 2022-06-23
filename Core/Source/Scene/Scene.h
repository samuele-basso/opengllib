#pragma once

#include "Component.h"

#include <entt.hpp>

namespace Jam
{

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;
		Scene(const Scene&) = default;

		entt::entity CreateEntity(std::string f_Name);
		void DestroyEntity(entt::entity f_Entity);

		template <typename T, typename ... A>
		T& AttachComponent(entt::entity f_Entity, A ... f_Args);

		template <typename T>
		void DetachComponent(entt::entity f_Entity);

		template<typename ... S>
		bool HasComponent(entt::entity f_Entity);

		template <typename ... S>
		decltype(auto) GetComponents(entt::entity f_Entity);

		template<typename ... S>
		decltype(auto) View();

		entt::registry& GetRegistry() { return p_Registry; }

	private:
		entt::registry p_Registry;
	};

	template<typename T, typename ...A>
	inline T& Scene::AttachComponent(entt::entity f_Entity, A ...f_Args)
	{
		if (p_Registry.all_of<T>(f_Entity)) {
			return p_Registry.replace<T>(f_Entity, f_Args ...);
		}
		else {
			return p_Registry.emplace<T>(f_Entity, f_Args ...);
		}
	}

	template<typename T>
	inline void Scene::DetachComponent(entt::entity f_Entity)
	{
		p_Registry.remove<T>(f_Entity);
	}

	template<typename ... S>
	inline bool Scene::HasComponent(entt::entity f_Entity)
	{
		return p_Registry.all_of<S ...>(f_Entity);
	}

	template<typename ... S>
	inline decltype(auto) Scene::GetComponents(entt::entity f_Entity)
	{
		return p_Registry.try_get<S ...>(f_Entity);
	}

	template<typename ... S>
	inline decltype(auto) Scene::View()
	{
		return p_Registry.view<S ...>();
	}

}