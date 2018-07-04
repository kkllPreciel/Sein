/**
 *  @file     vertex_buffer.h
 *  @brief    ���_�o�b�t�@�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/04/02
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>
#include <memory>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  ���_�o�b�t�@�p�C���^�[�t�F�C�X
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
       *  @brief  �}�b�v����
       *  @param  stride_in_bytes:1���_�̃T�C�Y
       *  @param  vertices:���_�z��ւ̃|�C���^
       */
      virtual void Map(const std::uint32_t stride_in_bytes, const void* const vertices) = 0;

      /**
       *  @brief  ���_�o�b�t�@�r���[���擾����
       *  @return ���_�o�b�t�@�r���[
       */
      virtual const D3D12_VERTEX_BUFFER_VIEW& GetView() = 0;

      /**
       *  @brief  ���_�o�b�t�@���J������
       */
      virtual void Release() noexcept = 0;

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
       *  @brief  ���_�o�b�t�@���쐬����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
       *  @return ���_�o�b�t�@�ւ̃��j�[�N�|�C���^
       */
      static std::unique_ptr<IVertexBuffer> Create(ID3D12Device* const device, const std::uint32_t size_in_bytes);
    };
  };
};