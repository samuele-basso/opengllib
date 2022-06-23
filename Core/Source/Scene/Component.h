#pragma once

#include <string>

namespace Jam
{

	struct NameComponent
	{
		NameComponent(const std::string& f_Name)
			: Name(f_Name) { }
		NameComponent(const NameComponent&) = default;
		~NameComponent() = default;

		std::string Name;
	};

}