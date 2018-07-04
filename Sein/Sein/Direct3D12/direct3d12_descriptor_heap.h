/**
 *  @file     descriptor_heap.h
 *  @brief    ディスクリプターヒープに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2017/08/08
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <d3d12.h>
#include "direct3d12_descriptor.h"
#include "direct3d12_command_list.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  ディスクリプター用interface
     */
    class IDescriptorHeap
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IDescriptorHeap() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IDescriptorHeap() = default;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IDescriptorHeap(const IDescriptorHeap& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IDescriptorHeap& operator = (const IDescriptorHeap& other) = delete;

      /**
       *  @brief  リソースを開放する
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  ディスクリプターを生成する
       *  @return ディスクリプターハンドル
       */
      virtual const IDescriptor& CreateDescriptor() = 0;

      /**
       *  @brief  ディスクリプターを取得する
       *  @param  index:ディスクリプター番号
       *  @return ディスクリプターハンドル
       */
      virtual const IDescriptor& GetDescriptor(const std::uint32_t index) = 0;

      /**
       *  @brief  生成したディスクリプター数を取得する
       *  @return 生成したディスクリプター数
       */
      virtual std::uint32_t GetCreatedCount() const = 0;

      /**
       *  @brief  生成可能なディスクリプター数を取得する
       *  @return 生成可能なディスクリプター数
       */
      virtual std::uint32_t GetAvailableCount() const = 0;

      /**
       *  @brief  コマンドリストにディスクリプターヒープを設定する
       *  @param  command_list:コマンドリスト
       */
      virtual void SetDescriptorHeaps(ICommandList* const command_list) const = 0;

      /**
       *  @brief  ディスクリプターヒープを生成する
       *  @param  device:Direct3D12のデバイス
       *  @param  desc:ディスクリプターヒープの設定情報
       *  @return ディスクリプターヒープへのシェアードポインタ
       */
      static std::shared_ptr<IDescriptorHeap> Create(ID3D12Device* const device, const D3D12_DESCRIPTOR_HEAP_DESC& desc);
    };
  };
};