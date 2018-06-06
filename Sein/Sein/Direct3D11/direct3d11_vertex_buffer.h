/**
 *  @file     direct3d11_vertex_buffer.h
 *  @brief    Direct3D11�̒��_�o�b�t�@�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/06/03
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <d3d11.h>

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11�̒��_�o�b�t�@�p�C���^�[�t�F�C�X
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
      virtual void Map(ID3D11DeviceContext* const context, const std::uint32_t size_in_bytes, const void* const vertices) = 0;
      
      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  ���_�o�b�t�@���쐬����
       *  @param  device:Direct3D11�̃f�o�C�X
       *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
       *  @return ���_�o�b�t�@�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IVertexBuffer> Create(ID3D11Device* const device, const std::uint32_t size_in_bytes);
    };
  };
};