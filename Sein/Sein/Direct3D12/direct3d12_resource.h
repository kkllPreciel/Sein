/**
 *  @file     resource.h
 *  @brief    ���\�[�X�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2017/08/20
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
     *  @brief  ���\�[�X�p�C���^�[�t�F�C�X
     */
    class IResource
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IResource() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IResource() = default;

      /**
       *  @brief  ���\�[�X���J������
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  ���\�[�X���擾����
       *  @return ���\�[�X�̎Q��
       */
      virtual ID3D12Resource& Get() const noexcept = 0;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IResource(const IResource& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IResource& operator = (const IResource& other) = delete;
    };
  };
};