#include "gfxpch.h"
#include "UUID.h"

namespace Jam
{

	std::unordered_map<UUID, std::string> UUIDHolder::p_UUIDName;

	static std::uniform_int_distribution<uint64_t> s_UniformIntDistribution;

	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Mersenne(s_RandomDevice());

	UUID::UUID()
		: p_UUID(s_UniformIntDistribution(s_Mersenne))
	{

	}

	UUID::~UUID()
	{

	}

	const std::string& UUIDHolder::GetName(UUID f_UUID)
	{
		std::unordered_map<UUID, std::string>::const_iterator got = p_UUIDName.find(f_UUID);
		if (got != p_UUIDName.end())
		{
			return got->second;
		}
		return std::string();
	}

	void UUIDHolder::SetName(UUID f_UUID, std::string& f_Name)
	{
		std::unordered_map<UUID, std::string>::iterator got = p_UUIDName.find(f_UUID);
		if (got != p_UUIDName.end())
		{
			got->second = f_Name;
		}
		else
		{
			p_UUIDName.insert(std::pair<UUID, std::string>(f_UUID, f_Name));
		}
	}

}