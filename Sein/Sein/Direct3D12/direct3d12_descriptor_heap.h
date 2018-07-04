/**
 *  @file     descriptor_heap.h
 *  @brief    �f�B�X�N���v�^�[�q�[�v�Ɋւ���w�b�_�t�@�C��
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
     *  @brief  �f�B�X�N���v�^�[�pinterface
     */
    class IDescriptorHeap
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IDescriptorHeap() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IDescriptorHeap() = default;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IDescriptorHeap(const IDescriptorHeap& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IDescriptorHeap& operator = (const IDescriptorHeap& other) = delete;

      /**
       *  @brief  ���\�[�X���J������
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  �f�B�X�N���v�^�[�𐶐�����
       *  @return �f�B�X�N���v�^�[�n���h��
       */
      virtual const IDescriptor& CreateDescriptor() = 0;

      /**
       *  @brief  �f�B�X�N���v�^�[���擾����
       *  @param  index:�f�B�X�N���v�^�[�ԍ�
       *  @return �f�B�X�N���v�^�[�n���h��
       */
      virtual const IDescriptor& GetDescriptor(const std::uint32_t index) = 0;

      /**
       *  @brief  ���������f�B�X�N���v�^�[�����擾����
       *  @return ���������f�B�X�N���v�^�[��
       */
      virtual std::uint32_t GetCreatedCount() const = 0;

      /**
       *  @brief  �����\�ȃf�B�X�N���v�^�[�����擾����
       *  @return �����\�ȃf�B�X�N���v�^�[��
       */
      virtual std::uint32_t GetAvailableCount() const = 0;

      /**
       *  @brief  �R�}���h���X�g�Ƀf�B�X�N���v�^�[�q�[�v��ݒ肷��
       *  @param  command_list:�R�}���h���X�g
       */
      virtual void SetDescriptorHeaps(ICommandList* const command_list) const = 0;

      /**
       *  @brief  �f�B�X�N���v�^�[�q�[�v�𐶐�����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  desc:�f�B�X�N���v�^�[�q�[�v�̐ݒ���
       *  @return �f�B�X�N���v�^�[�q�[�v�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IDescriptorHeap> Create(ID3D12Device* const device, const D3D12_DESCRIPTOR_HEAP_DESC& desc);
    };
  };
};