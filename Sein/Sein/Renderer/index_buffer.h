/**
 *  @file     index_buffer.h
 *  @brief    �����_���[�̃C���f�b�N�X�o�b�t�@�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/07/04
 *  @version  1.0
 */

#pragma once

// include
#include <cstdint>
#include <memory>

#include "device_context.h"

namespace Sein
{
  namespace Renderer
  {
    /**
     *  @brief  �C���f�b�N�X�o�b�t�@�p�C���^�[�t�F�[�X
     */
    class IIndexBuffer
    {
    public:
      /**
       *  @brief  �C���f�b�N�X�̌^
       */
      enum class Type : std::uint32_t {
        k16Bit,
        k32Bit,
      };

      /**
       *  @brief  �R���X�g���N�^
       */
      IIndexBuffer() = default;
      
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IIndexBuffer() = default;
      
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IIndexBuffer(const IIndexBuffer& other) = delete;
      
      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IIndexBuffer& operator = (const IIndexBuffer& other) = delete;

      /**
       *  @brief  �}�b�v����
       *  @param  context:�R���e�L�X�g
       *  @param  size_in_bytes:�C���f�b�N�X�z��̃T�C�Y
       *  @param  indices:�C���f�b�N�X�z��ւ̃|�C���^
       */
      virtual void Map(IDeviceContext* const context, const std::uint32_t& size_in_bytes, const void* const indices) = 0;
      
      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;
    };
  };
};