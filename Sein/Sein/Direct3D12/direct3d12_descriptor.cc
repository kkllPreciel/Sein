/**
 *  @file     descriptor.cc
 *  @brief    ディスクリプターに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2017/08/13
 *  @version  1.0
 */

 // include
#include "direct3d12_descriptor.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  ディスクリプター用クラス
       */
      class Descriptor final : public IDescriptor
      {
      public:
        /**
         *  @brief  コンストラクタ
         *  @param  handle_for_cpu:CPUハンドル
         *  @param  handle_for_gpu:GPUハンドル
         */
        Descriptor(const D3D12_CPU_DESCRIPTOR_HANDLE& handle_for_cpu, const D3D12_GPU_DESCRIPTOR_HANDLE& handle_for_gpu)
          : handle_for_cpu_(handle_for_cpu), handle_for_gpu_(handle_for_gpu)
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~Descriptor() override
        {
          Release();
        }

        /**
         *  @brief  リソースを開放する
         */
        void Release() noexcept override
        {

        }
        
        /**
         *  @brief  CPUディスクリプターハンドルを取得する
         *  @return CPUハンドル
         */
        D3D12_CPU_DESCRIPTOR_HANDLE GetHandleForCPU() const override
        {
          return handle_for_cpu_;
        }
        
        /**
         *  @brief  GPUディスクリプターハンドルを取得する
         *  @return GPUハンドル
         */
        D3D12_GPU_DESCRIPTOR_HANDLE GetHandleForGPU() const override
        {
          return handle_for_gpu_;
        }

      private:
        D3D12_CPU_DESCRIPTOR_HANDLE handle_for_cpu_;  ///< GPUハンドル
        D3D12_GPU_DESCRIPTOR_HANDLE handle_for_gpu_;  ///< GPUハンドル
      };
    };

    /**
     *  @brief  ディスクリプターを生成する
     *  @param  handle_for_cpu:CPUハンドル
     *  @param  handle_for_gpu:GPUハンドル
     *  @return ディスクリプターへのシェアードポインタ
     */
    std::shared_ptr<IDescriptor> IDescriptor::Create(const D3D12_CPU_DESCRIPTOR_HANDLE& handle_for_cpu, const D3D12_GPU_DESCRIPTOR_HANDLE& handle_for_gpu)
    {
      auto descriptor = std::make_shared<Descriptor>(handle_for_cpu, handle_for_gpu);

      return descriptor;
    }
  };
};