#pragma once

#include <unordered_map>

namespace Jam
{

	class UUID
	{
	public:
		UUID();
		~UUID();
		UUID(const UUID&) = default;

		inline bool operator==(const UUID& f_2) { return p_UUID == f_2.p_UUID; }

		inline uint64_t Value() const { return p_UUID; }

	private:
		uint64_t p_UUID;
	};

	class UUIDHolder
	{
	public:
		UUIDHolder() = default;
		~UUIDHolder() = default;
		UUIDHolder(const UUIDHolder&) = default;

	public:
		UUID ID;

	public:
		static const std::string& GetName(UUID f_UUID);

		static void SetName(UUID f_UUID, std::string& f_Name);

	private:
		static std::unordered_map<UUID, std::string> p_UUIDName;
	};

}

namespace std
{
	inline bool operator==(const Jam::UUID& f_1, const Jam::UUID& f_2) { return f_1.Value() == f_2.Value(); }

	template<>
	struct hash<Jam::UUID>
	{
		size_t operator()(const Jam::UUID& f_Key) const
		{
			return hash<uint64_t>()(f_Key.Value());
		}
	};
}