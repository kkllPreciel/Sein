/**
 *  @file     descriptor.cc
 *  @brief    ディスクリプターに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2017/08/13
 *  @version  1.0
 */

 // include
#include "descriptor.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  引数付きコンストラクタ
     *  @param  handleForCPU:CPUハンドル
     *  @param  handleForGPU:GPUハンドル
     */
    Descriptor::Descriptor(const D3D12_CPU_DESCRIPTOR_HANDLE& handleForCPU, const D3D12_GPU_DESCRIPTOR_HANDLE& handleForGPU) : handleCPU(handleForCPU), handleGPU(handleForGPU)
    {

    }
    
    /**
     *  @brief  デストラクタ
     */
    Descriptor::~Descriptor()
    {

    }
    
    /**
     *  @brief  CPUディスクリプターハンドルを取得する
     *  @return CPUハンドル
     */
    D3D12_CPU_DESCRIPTOR_HANDLE Descriptor::GetHandleForCPU() const
    {
      return handleCPU;
    }
    
    /**
     *  @brief  GPUディスクリプターハンドルを取得する
     *  @return GPUハンドル
     */
    D3D12_GPU_DESCRIPTOR_HANDLE Descriptor::GetHandleForGPU() const
    {
      return handleGPU;
    }
  };
};