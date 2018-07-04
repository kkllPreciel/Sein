/**
 *  @file     vertex_buffer.h
 *  @brief    �����_���[�̒��_�o�b�t�@�Ɋւ���w�b�_�t�@�C��
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
     *  @brief  ���_�o�b�t�@�p�C���^�[�t�F�[�X
     */
    class IVertexBuffer
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IVertexBuffer() = default;
      
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IVertexBuffer() = default;
      
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IVertexBuffer(const IVertexBuffer& other) = delete;
      
      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IVertexBuffer& operator = (const IVertexBuffer& other) = delete;

      /**
       *  @brief  �}�b�v����
       *  @param  context:�R���e�L�X�g
       *  @param  size_in_bytes:���_�z��̃T�C�Y
       *  @param  vertices:���_�z��ւ̃|�C���^
       */
      virtual void Map(IDeviceContext* const context, const std::uint32_t& size_in_bytes, const void* const vertices) = 0;
      
      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;
    };
  };
};