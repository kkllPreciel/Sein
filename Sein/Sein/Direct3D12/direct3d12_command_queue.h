/**
 *  @file     command_queue.h
 *  @brief    コマンドキューに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/19
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <d3d12.h>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  コマンドキュー用インターフェイス
     */
    class ICommandQueue
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      ICommandQueue() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~ICommandQueue() = default;

      /**
       *  @brief  コマンドリストを実行する
       *  @param  command_list_count:コマンドリストの数
       *  @param  command_list:の配列
       */
      virtual void ExecuteCommandLists(std::uint32_t command_list_count, ID3D12CommandList* const* command_list) = 0;

      /**
       *  @brief  コマンドキューを取得する
       *  @return コマンドキュー
       */
      virtual const ID3D12CommandQueue& Get() const = 0;

      /**
       *  @brief  コマンドキューを開放する
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      ICommandQueue(const ICommandQueue& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      ICommandQueue& operator = (const ICommandQueue& other) = delete;

      /**
       *  @brief  コマンドキューを作成する
       *  @param  device:Direct3D12のデバイス
       *  @param  command_queue_desc:コマンドキューの設定
       *  @return コマンドキューへのシェアードポインタ
       */
      static std::shared_ptr<ICommandQueue> Create(ID3D12Device* const device, const D3D12_COMMAND_QUEUE_DESC& command_queue_desc);
    };
  };
};