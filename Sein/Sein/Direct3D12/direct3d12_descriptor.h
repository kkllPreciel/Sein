/**
 *  @file     descriptor.h
 *  @brief    ディスクリプターに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2017/08/13
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
     *  @brief  ディスクリプター用interface
     */
    class IDescriptor
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IDescriptor() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IDescriptor() = default;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IDescriptor(const IDescriptor& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IDescriptor& operator = (const IDescriptor& other) = delete;

      /**
       *  @brief  リソースを開放する
       */
      virtual void Release() noexcept = 0;

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

      /**
       *  @brief  ディスクリプターを生成する
       *  @param  handle_for_cpu:CPUハンドル
       *  @param  handle_for_gpu:GPUハンドル
       *  @return ディスクリプターへのシェアードポインタ
       */
      static std::shared_ptr<IDescriptor> Create(const D3D12_CPU_DESCRIPTOR_HANDLE& handle_for_cpu, const D3D12_GPU_DESCRIPTOR_HANDLE& handle_for_gpu);
    };
  };
};