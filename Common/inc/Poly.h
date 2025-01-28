#pragma once

// Base for polymorphic types distinguished by an enum type
// Recommended not to create pointers to this type
template<typename EnumType, EnumType defaultVal = 0>
class __declspec(novtable) PolyBase
{
	protected:
		// An instance of the enum class
		EnumType m_Type;

		// Stops standalone creation of base class
		PolyBase(EnumType type = defaultVal) : m_Type(type) {}

		// Creates compile time errors when something tries to delete it
		~PolyBase() = default;

	public:
		// Allows global read-only access to the type
		inline const EnumType Type() const { return m_Type; }

		// Checks equality with another of the same enum type
		inline bool operator== (EnumType other) const { return m_Type == other; }

		// Checks inequality with another of the same enum type
		inline bool operator!= (EnumType other) const { return m_Type != other; }
};
