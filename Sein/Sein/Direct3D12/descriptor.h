/**
 *  @file     descriptor.h
 *  @brief    �f�B�X�N���v�^�[�Ɋւ���w�b�_�t�@�C��
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
     *  @brief  �f�B�X�N���v�^�[�pinterface
     */
    class IDescriptor
    {
    public:
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IDescriptor() = default;

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
    };

    /**
     *  @brief  �f�B�X�N���v�^�[�p�N���X
     */
    class Descriptor final : public IDescriptor
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Descriptor();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~Descriptor() override;

      /**
       *  @brief  �f�B�X�N���v�^�[�𐶐�����
       *  @param  handleForCPU:CPU�n���h��
       *  @param  handleForGPU:GPU�n���h��
       */
      void Create(const D3D12_CPU_DESCRIPTOR_HANDLE& handleForCPU, const D3D12_GPU_DESCRIPTOR_HANDLE& handleForGPU);

      /**
       *  @brief  CPU�f�B�X�N���v�^�[�n���h�����擾����
       *  @return CPU�n���h��
       */
      D3D12_CPU_DESCRIPTOR_HANDLE GetHandleForCPU() const override;

      /**
       *  @brief  GPU�f�B�X�N���v�^�[�n���h�����擾����
       *  @return GPU�n���h��
       */
      D3D12_GPU_DESCRIPTOR_HANDLE GetHandleForGPU() const override;

    private:
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      Descriptor(const Descriptor& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      Descriptor& operator = (const Descriptor& other) = delete;

    private:
      D3D12_CPU_DESCRIPTOR_HANDLE handleCPU;  ///< GPU�n���h��
      D3D12_GPU_DESCRIPTOR_HANDLE handleGPU;  ///< GPU�n���h��
    };
  };
};