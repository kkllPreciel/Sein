/**
 *  @file     descriptor.h
 *  @brief    ディスクリプターに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2017/08/13
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
     *  @brief  ディスクリプター用interface
     */
    class IDescriptor
    {
    public:
      /**
       *  @brief  デストラクタ
       */
      virtual ~IDescriptor() = default;

      /**
       *  @brief  CPUディスクリプターハンドルを取得する
       *  @return CPUハンドル
       */
      virtual D3D12_CPU_DESCRIPTOR_HANDLE GetHandleForCPU() const = 0;

      /**
       *  @brief  GPUディスクリプターハンドルを取得する
       *  @return GPUハンドル
       */
      virtual D3D12_GPU_DESCRIPTOR_HANDLE GetHandleForGPU() const = 0;
    };

    /**
     *  @brief  ディスクリプター用クラス
     */
    class Descriptor final : public IDescriptor
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Descriptor();

      /**
       *  @brief  デストラクタ
       */
      ~Descriptor() override;

      /**
       *  @brief  ディスクリプターを生成する
       *  @param  handleForCPU:CPUハンドル
       *  @param  handleForGPU:GPUハンドル
       */
      void Create(const D3D12_CPU_DESCRIPTOR_HANDLE& handleForCPU, const D3D12_GPU_DESCRIPTOR_HANDLE& handleForGPU);

      /**
       *  @brief  CPUディスクリプターハンドルを取得する
       *  @return CPUハンドル
       */
      D3D12_CPU_DESCRIPTOR_HANDLE GetHandleForCPU() const override;

      /**
       *  @brief  GPUディスクリプターハンドルを取得する
       *  @return GPUハンドル
       */
      D3D12_GPU_DESCRIPTOR_HANDLE GetHandleForGPU() const override;

    private:
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      Descriptor(const Descriptor& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      Descriptor& operator = (const Descriptor& other) = delete;

    private:
      D3D12_CPU_DESCRIPTOR_HANDLE handleCPU;  ///< GPUハンドル
      D3D12_GPU_DESCRIPTOR_HANDLE handleGPU;  ///< GPUハンドル
    };
  };
};