/**
 *  @file     descriptor.h
 *  @brief    �f�B�X�N���v�^�[�Ɋւ���w�b�_�t�@�C��
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
     *  @brief  �f�B�X�N���v�^�[�pinterface
     */
    class IDescriptor
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IDescriptor() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IDescriptor() = default;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IDescriptor(const IDescriptor& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IDescriptor& operator = (const IDescriptor& other) = delete;

      /**
       *  @brief  ���\�[�X���J������
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  CPU�f�B�X�N���v�^�[�n���h�����擾����
       *  @return CPU�n���h��
       */
      virtual D3D12_CPU_DESCRIPTOR_HANDLE GetHandleForCPU() const = 0;

      /**
       *  @brief  GPU�f�B�X�N���v�^�[�n���h�����擾����
       *  @return GPU�n���h��
       */
      virtual D3D12_GPU_DESCRIPTOR_HANDLE GetHandleForGPU() const = 0;

      /**
       *  @brief  �f�B�X�N���v�^�[�𐶐�����
       *  @param  handle_for_cpu:CPU�n���h��
       *  @param  handle_for_gpu:GPU�n���h��
       *  @return �f�B�X�N���v�^�[�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IDescriptor> Create(const D3D12_CPU_DESCRIPTOR_HANDLE& handle_for_cpu, const D3D12_GPU_DESCRIPTOR_HANDLE& handle_for_gpu);
    };
  };
};