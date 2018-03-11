/**
 *  @file     root_parameter.h
 *  @brief    ���[�g�p�����[�^�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/03/11
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <d3d12.h>
#include "descriptor_range.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  ���[�g�p�����[�^�p�C���^�[�t�F�C�X
     */
    class IRootParameter
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IRootParameter() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IRootParameter() = default;

      /**
       *  @brief  ���[�g�p�����[�^���擾����
       *  @return ���[�g�p�����[�^
       */
      virtual const D3D12_ROOT_PARAMETER& Get() const = 0;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IRootParameter(const IRootParameter& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IRootParameter& operator = (const IRootParameter& other) = delete;

      /**
       *  @brief  �^�C�v���f�B�X�N���v�^�[�e�[�u���̃��[�g�p�����[�^���쐬����
       *  @param  descriptor_range_type:�f�B�X�N���v�^�[�����W�̎��
       *  @param  descriptor_count:�f�B�X�N���v�^�[�̌�
       *  @return �f�B�X�N���v�^�[�����W�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IRootParameter> CreateForDescriptorTable(const IDescriptorRange& descriptor_range, const D3D12_SHADER_VISIBILITY& shader_visibility);
    };
  };
};