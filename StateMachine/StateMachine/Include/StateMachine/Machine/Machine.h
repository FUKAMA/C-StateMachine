#pragma once
#include <memory>
#include <list>
#include "../State/StateBase.h"

namespace stmc
{
	template<typename OwnerType>
	class StateMachine
	{
	public:

		// ��������Z�b�g����
		void SetOwner(OwnerType* a_pOwner)
		{
			m_pOwner = a_pOwner;
		}

		//------------------------------------------------------
		// �X�V
		//------------------------------------------------------

		void SetUp(OwnerType* a_pOwner)
		{
			SetOwner(a_pOwner);
		}

		void Update()
		{
			if (m_pOwner == nullptr || m_lStatus.empty())
			{
				return;
			}
			// 
			m_lStatus.back()->Update(m_pOwner);
		}

		void Release()
		{
			ClearStates();
		}

		//------------------------------------------------------
		// �J��
		//------------------------------------------------------

		// �X�e�[�g��ύX����
		template<typename StateType, typename...Args>
		void ChangeState(Args...a_args)
		{
			// �X�e�[�g��S�ďI������
			ClearStates();

			// �X�e�[�g���쐬���ĊJ�n�������Ă�
			std::shared_ptr<StateBase<OwnerType>> spState = std::make_shared<StateType>(a_args...);
			if (spState == nullptr)
			{
				return;
			}

			// �X�e�[�g��ǉ����J�n�������Ă�
			CallNowStateStart(spState);
		}


		// �X�e�[�g��ǉ����ύX����
		template<typename StateType, typename...Args>
		void PushState(Args...a_args)
		{
			// ���ɃX�e�[�g��ǉ����Ă�����I���������Ă�
			CallNowStateEnd();

			// �X�e�[�g���쐬���ĊJ�n�������Ă�
			std::shared_ptr<StateBase<OwnerType>> spState = std::make_shared<StateType>(a_args...);
			if (spState == nullptr)
			{
				return;
			}

			// �X�e�[�g��ǉ����J�n�������Ă�
			CallNowStateStart(spState);
		}

		// ���̃X�e�[�g��j�����ĂЂƂO�̃X�e�[�g�ɖ߂�
		void PopState()
		{
			// ���̃X�e�[�g���I�����|�b�v����
			CallNowStateEnd(true);

			// �X�e�[�g���c���Ă���J�n���������s����
			CallNowStateStart();
		}

		// ������X�e�[�g��S�ă|�b�v����
		void ClearStates()
		{
			// ��ԍŌ�̃X�e�[�g���I������
			CallNowStateEnd();

			// �S�ẴX�e�[�g���N���A
			m_lStatus.clear();
		}

	private:

		// ���̃X�e�[�g�̊J�n�������Ă�
		// �X�e�[�g��ǉ�����Ȃ�������ɓ����
		void CallNowStateStart(std::shared_ptr<StateBase<OwnerType>> a_spState = nullptr)
		{
			if (m_pOwner == nullptr || m_lStatus.empty())
			{
				return;
			}

			// �ǉ�����X�e�[�g���w�肳��Ă���ǉ�����
			if (a_spState != nullptr)
			{
				a_spState->SetMachine(this);
				m_lStatus.emplace_back(a_spState);
			}

			// �J�n����
			m_lStatus.back()->Start(m_pOwner);
		}

		// ���̃X�e�[�g�̏I���֐����Ă�
		// ���̃X�e�[�g���|�b�v����Ȃ��������true������
		void CallNowStateEnd(bool a_bPop = false)
		{
			if (m_pOwner == nullptr || m_lStatus.empty())
			{
				return;
			}

			// �I������
			m_lStatus.back()->End(m_pOwner);

			if (a_bPop)
			{
				m_lStatus.pop_back();
			}
		}



	private:

		std::list<std::shared_ptr<StateBase<OwnerType>>> m_lStatus;

		//std::list<std::shared_ptr<StateBase<OwnerType>>>::iterator 

		OwnerType* m_pOwner = nullptr;

	};
}
