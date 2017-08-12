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
#include <memory>

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
       *  @brief  ディスクリプターを取得する
       *  @param  index:ディスクリプター番号
       *  @return ディスクリプターハンドル
       */
      virtual D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptor(const unsigned int index) = 0;

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

    /**
     *  @brief  ディスクリプターヒープ用クラス
     */
    class DescriptorHeap final : public IDescriptorHeap
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      DescriptorHeap();

      /**
       *  @brief  デストラクタ
       */
      ~DescriptorHeap() override;

      /**
       *  @brief  ディスクリプターヒープを生成する
       *  @param  device:Direct3D12のデバイス
       *  @param  desc:ディスクリプターヒープの設定情報
       */
      void Create(ID3D12Device* const device, const D3D12_DESCRIPTOR_HEAP_DESC& desc) override;

      /**
       *  @brief  リソースを開放する
       */
      void Release() override;

      /**
       *  @brief  ディスクリプターヒープを取得する
       *  @return ディスクリプターヒープへのポインタ
       */
      ID3D12DescriptorHeap* Get() const override;

      /**
       *  @brief  ディスクリプターを生成する
       *  @return ディスクリプターハンドル
       */
      D3D12_CPU_DESCRIPTOR_HANDLE CreateDescriptor() override;

      /**
       *  @brief  ディスクリプターを取得する
       *  @param  index:ディスクリプター番号
       *  @return ディスクリプターハンドル
       */
      D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptor(const unsigned int index) override;

      /**
       *  @brief  生成したディスクリプター数を取得する
       *  @return 生成したディスクリプター数
       */
      unsigned short GetCreatedCount() const override;

      /**
       *  @brief  生成可能なディスクリプター数を取得する
       *  @return 生成可能なディスクリプター数
       */
      unsigned short GetAvailableCount() const override;

    private:
      std::unique_ptr<ID3D12DescriptorHeap, void(*)(IUnknown*)> heap; ///< ディスクリプターヒープ
      unsigned int incrementSize;   ///< インクリメントサイズ
      unsigned int createdCount;    ///< 生成したディスクリプター数
      unsigned int availableCount;  ///< 生成可能なディスクリプター数
    };
  };
};