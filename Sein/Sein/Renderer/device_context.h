/**
 *  @file     device_context.h
 *  @brief    �����_���[�̃f�o�C�X�R���e�L�X�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/07/03
 *  @version  1.0
 */

#pragma once

// include
#include <cstdint>
#include <memory>

namespace Sein
{
  namespace Renderer
  {
    /**
     *  @brief  �f�o�C�X�R���e�L�X�g�p�C���^�[�t�F�[�X
     */
    class IDeviceContext
    {
    public:
      /**
       *  @brief  �R���e�L�X�g�̎��
       */
      enum class Type : std::uint32_t {
        kImmediate, ///< ����
        kDeferred,  ///< �x��
      };

      /**
       *  @brief  �R���X�g���N�^
       */
      IDeviceContext() = default;
      
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IDeviceContext() = default;
      
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IDeviceContext(const IDeviceContext& other) = delete;
      
      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IDeviceContext& operator = (const IDeviceContext& other) = delete;
      
      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;
    };
  };
};