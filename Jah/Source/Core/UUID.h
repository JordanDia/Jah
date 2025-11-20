#pragma once

#include <xhash>

namespace Jah {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID = 0;
	};

}


namespace std {

	template<>
	struct hash<Jah::UUID>
	{
		size_t operator()(const Jah::UUID& uuid) const noexcept
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}