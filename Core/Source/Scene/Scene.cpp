#include "gfxpch.h"
#include "Scene.h"

#include "Base/Log.h"

namespace Jam
{

	entt::entity Scene::CreateEntity(std::string f_Name)
	{
		entt::entity e = p_Registry.create();
		AttachComponent<NameComponent>(e, f_Name);
		return e;
	}

	void Scene::DestroyEntity(entt::entity f_Entity)
	{
		p_Registry.destroy(f_Entity);
	}

}