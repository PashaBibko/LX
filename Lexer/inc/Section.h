#pragma once

#include <Common/inc/Poly.h>

#include <string_view>

enum class SectionType
{
	LX_FUNCTION,

	UNDEFINED = 0
};

class SourceSection final : public PolyBase
	<SectionType, SectionType::UNDEFINED>
{
	private:
		std::string_view m_Declaration;
		std::string_view m_Definition;

	public:
		SourceSection(SectionType type)
		{}
};
