/**
 *  @file     descriptor_range.h
 *  @brief    �f�B�X�N���v�^�[�����W�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/03/10
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
     *  @brief  �f�B�X�N���v�^�[�����W�p�C���^�[�t�F�C�X
     */
    class IDescriptorRange
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IDescriptorRange() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IDescriptorRange() = default;

      /**
       *  @brief  �f�B�X�N���v�^�[�����W���擾����
       *  @return �f�B�X�N���v�^�[�����W
       */
      virtual const D3D12_DESCRIPTOR_RANGE& Get() const = 0;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IDescriptorRange(const IDescriptorRange& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IDescriptorRange& operator = (const IDescriptorRange& other) = delete;

      /**
       *  @brief  �f�B�X�N���v�^�[�����W���쐬����
       *  @param  descriptor_range_type:�f�B�X�N���v�^�[�����W�̎��
       *  @param  descriptor_count:�f�B�X�N���v�^�[�̌�
       *  @return �f�B�X�N���v�^�[�����W�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IDescriptorRange> Create(const D3D12_DESCRIPTOR_RANGE_TYPE descriptor_range_type, std::uint32_t descriptor_count);
    };
  };
};