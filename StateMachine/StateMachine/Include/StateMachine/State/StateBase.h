#pragma once

namespace stmc
{
	template<typename OwnerType>
	class StateMachine;

	template<typename OwnerType>
	class StateBase
	{
	public:

		virtual void Start(OwnerType* a_pOwner) {}

		virtual void Update(OwnerType* a_pOwner) {}

		virtual void End(OwnerType* a_pOwner) {}

	protected:

		StateMachine<OwnerType>* GetMachine()
		{
			return m_pMachine;
		}

	private:
		friend class StateMachine<OwnerType>;

		void SetMachine(StateMachine<OwnerType>* a_pMachine)
		{
			m_pMachine = a_pMachine;
		}

	private:

		StateMachine<OwnerType>* m_pMachine = nullptr;

	};
}
