/**
 *  @file     command_list.h
 *  @brief    コマンドリストに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2017/08/21
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>
#include <memory>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  コマンドリスト用interface
     */
    class ICommandList
    {
    public:
      /**
       *  @brief  デストラクタ
       */
      virtual ~ICommandList() = default;

      /**
       *  @brief  コマンドリストを生成する
       *  @param  device:Direct3D12のデバイス
       *  @param  type:コマンドリストのタイプ
       */
      virtual void Create(ID3D12Device* const device, const D3D12_COMMAND_LIST_TYPE& type) = 0;

      /**
       *  @brief  リソースを開放する
       */
      virtual void Release() = 0;

      /**
       *  @brief  記録を開始する
       */
      virtual void Begin() = 0;

      /**
       *  @brief  記録を終了する
       */
      virtual void End() = 0;

      /**
       *  @brief  コマンドリストを取得する
       *  @return コマンドリストの参照
       */
      virtual ID3D12GraphicsCommandList& Get() const = 0;
    };

    /**
     *  @brief  コマンドリスト用クラス
     */
    class CommandList final : public ICommandList
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      CommandList();

      /**
       *  @brief  デストラクタ
       */
      ~CommandList() override;

      /**
       *  @brief  コマンドリストを生成する
       *  @param  device:Direct3D12のデバイス
       *  @param  type:コマンドリストのタイプ
       */
      void Create(ID3D12Device* const device, const D3D12_COMMAND_LIST_TYPE& type) override;

      /**
       *  @brief  リソースを開放する
       */
      void Release() override;

      /**
       *  @brief  記録を開始する
       */
      void Begin() override;

      /**
       *  @brief  記録を終了する
       */
      void End() override;

      /**
       *  @brief  コマンドリストを取得する
       *  @return コマンドリストの参照
       */
      ID3D12GraphicsCommandList& Get() const override;

    private:
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      CommandList(const CommandList& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      CommandList& operator = (const CommandList& other) = delete;

    private:
      std::unique_ptr<ID3D12CommandAllocator, void(*)(IUnknown*)> commandAllocator; ///< コマンドアロケータ
      std::unique_ptr<ID3D12GraphicsCommandList, void(*)(IUnknown*)> commandList;   ///< コマンドリスト
    };
  };
};