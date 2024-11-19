#pragma once

namespace stmc
{
	template<typename OwnerType>
	class StateBase
	{
	public:

		virtual void Begin(OwnerType* a_pOwner) {}

		virtual void Update(OwnerType* a_pOwner) {}

		virtual void End(OwnerType* a_pOwner) {}

	private:

	};
}
