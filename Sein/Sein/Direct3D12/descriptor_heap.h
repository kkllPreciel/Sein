/**
 *  @file     descriptor_heap.h
 *  @brief    ディスクリプターヒープに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2017/08/08
 *  @version  1.0
 */

#pragma once

// include
#include <d3d12.h>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  ディスクリプターヒープ用interface
     */
    class IDescriptorHeap
    {
    public:
      /**
       *  @brief  デストラクタ
       */
      virtual ~IDescriptorHeap() {};

      /**
       *  @brief  ディスクリプターヒープを生成する
       *  @param  device:Direct3D12のデバイス
       *  @param  desc:ディスクリプターヒープの設定情報
       */
      virtual void Create(ID3D12Device* const device, const D3D12_DESCRIPTOR_HEAP_DESC& desc) = 0;

      /**
       *  @brief  リソースを開放する
       */
      virtual void Release() = 0;

      /**
       *  @brief  ディスクリプターヒープを取得する
       *  @return ディスクリプターヒープへのポインタ
       */
      virtual ID3D12DescriptorHeap* Get() const = 0;

      /**
       *  @brief  ディスクリプターを生成する
       *  @return ディスクリプターハンドル
       */
      virtual D3D12_CPU_DESCRIPTOR_HANDLE CreateDescriptor() = 0;

      /**
       *  @brief  生成したディスクリプター数を取得する
       *  @return 生成したディスクリプター数
       */
      virtual unsigned short GetCreatedCount() const = 0;

      /**
       *  @brief  生成可能なディスクリプター数を取得する
       *  @return 生成可能なディスクリプター数
       */
      virtual unsigned short GetAvailableCount() const = 0;
    };
  };
};