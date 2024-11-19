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

		// 持ち主をセットする
		void SetOwner(OwnerType* a_pOwner)
		{
			m_pOwner = a_pOwner;
		}

		//------------------------------------------------------
		// 更新
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
		// 遷移
		//------------------------------------------------------

		// ステートを変更する
		template<typename StateType, typename...Args>
		void ChangeState(Args...a_args)
		{
			// ステートを全て終了する
			ClearStates();

			// ステートを作成して開始処理を呼ぶ
			std::shared_ptr<StateBase<OwnerType>> spState = std::make_shared<StateType>(a_args...);
			if (spState == nullptr)
			{
				return;
			}

			// ステートを追加し開始処理を呼ぶ
			CallNowStateStart(spState);
		}


		// ステートを追加し変更する
		template<typename StateType, typename...Args>
		void PushState(Args...a_args)
		{
			// 既にステートを追加していたら終了処理を呼ぶ
			CallNowStateEnd();

			// ステートを作成して開始処理を呼ぶ
			std::shared_ptr<StateBase<OwnerType>> spState = std::make_shared<StateType>(a_args...);
			if (spState == nullptr)
			{
				return;
			}

			// ステートを追加し開始処理を呼ぶ
			CallNowStateStart(spState);
		}

		// 今のステートを破棄してひとつ前のステートに戻る
		void PopState()
		{
			// 今のステートを終了しポップする
			CallNowStateEnd(true);

			// ステートが残ってたら開始処理を実行する
			CallNowStateStart();
		}

		// 今あるステートを全てポップする
		void ClearStates()
		{
			// 一番最後のステートを終了する
			CallNowStateEnd();

			// 全てのステートをクリア
			m_lStatus.clear();
		}

	private:

		// 今のステートの開始処理を呼ぶ
		// ステートを追加するなら引き数に入れる
		void CallNowStateStart(std::shared_ptr<StateBase<OwnerType>> a_spState = nullptr)
		{
			if (m_pOwner == nullptr || m_lStatus.empty())
			{
				return;
			}

			// 追加するステートが指定されてたら追加する
			if (a_spState != nullptr)
			{
				a_spState->SetMachine(this);
				m_lStatus.emplace_back(a_spState);
			}

			// 開始処理
			m_lStatus.back()->Start(m_pOwner);
		}

		// 今のステートの終了関数を呼ぶ
		// 今のステートをポップするなら引き数にtrueを入れる
		void CallNowStateEnd(bool a_bPop = false)
		{
			if (m_pOwner == nullptr || m_lStatus.empty())
			{
				return;
			}

			// 終了処理
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
