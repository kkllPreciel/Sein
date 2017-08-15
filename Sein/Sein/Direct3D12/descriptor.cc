/**
 *  @file     descriptor.cc
 *  @brief    �f�B�X�N���v�^�[�Ɋւ��鏈�����s���v���O�����\�[�X
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
     *  @brief  �R���X�g���N�^
     */
    Descriptor::Descriptor() : handleCPU({}), handleGPU({})
    {

    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    Descriptor::~Descriptor()
    {

    }

    /**
     *  @brief  �f�B�X�N���v�^�[�𐶐�����
     *  @param  handleForCPU:CPU�n���h��
     *  @param  handleForGPU:GPU�n���h��
     */
    void Descriptor::Create(const D3D12_CPU_DESCRIPTOR_HANDLE& handleForCPU, const D3D12_GPU_DESCRIPTOR_HANDLE& handleForGPU)
    {
      handleCPU = handleForCPU;
      handleGPU = handleForGPU;
    }
    
    /**
     *  @brief  CPU�f�B�X�N���v�^�[�n���h�����擾����
     *  @return CPU�n���h��
     */
    D3D12_CPU_DESCRIPTOR_HANDLE Descriptor::GetHandleForCPU() const
    {
      return handleCPU;
    }
    
    /**
     *  @brief  GPU�f�B�X�N���v�^�[�n���h�����擾����
     *  @return GPU�n���h��
     */
    D3D12_GPU_DESCRIPTOR_HANDLE Descriptor::GetHandleForGPU() const
    {
      return handleGPU;
    }
  };
};