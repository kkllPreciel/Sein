/**
 *  @file     index_buffer.h
 *  @brief    ���_�C���f�b�N�X�o�b�t�@�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/04/03
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
     *  @brief  ���_�C���f�b�N�X�o�b�t�@�p�C���^�[�t�F�C�X
     */
    class IIndexBuffer
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IIndexBuffer() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IIndexBuffer() = default;

      /**
       *  @brief  �}�b�v����
       *  @param  format:���_�C���f�b�N�X�̃t�H�[�}�b�g
       *  @param  indices:���_�C���f�b�N�X�z��ւ̃|�C���^
       */
      virtual void Map(const DXGI_FORMAT format, const void* const indices) = 0;

      /**
       *  @brief  ���_�C���f�b�N�X�o�b�t�@�r���[���擾����
       *  @return ���_�C���f�b�N�X�o�b�t�@�r���[
       */
      virtual const D3D12_INDEX_BUFFER_VIEW& GetView() = 0;

      /**
       *  @brief  ���_�C���f�b�N�X�o�b�t�@���J������
       */
      virtual void Release() noexcept = 0;

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
       *  @brief  ���_�C���f�b�N�X�o�b�t�@���쐬����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  size_in_bytes:���_�C���f�b�N�X�o�b�t�@�̃T�C�Y(���_�C���f�b�N�X�T�C�Y * ���_�C���f�b�N�X��)
       *  @return ���_�C���f�b�N�X�o�b�t�@�ւ̃��j�[�N�|�C���^
       */
      static std::unique_ptr<IIndexBuffer> Create(ID3D12Device* const device, const std::uint32_t size_in_bytes);
    };
  };
};