/**
 *  @file     constant_buffer.h
 *  @brief    �萔�o�b�t�@�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/04/08
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
     *  @brief  �萔�o�b�t�@�p�C���^�[�t�F�C�X
     */
    class IConstantBuffer
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IConstantBuffer() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IConstantBuffer() = default;

      /**
       *  @brief  �}�b�v����
       *  @param  size_in_bytes:�}�b�v����f�[�^�̃T�C�Y
       *  @param  data:�}�b�v����f�[�^�ւ̃|�C���^
       */
      virtual void Map(const std::uint32_t size_in_bytes, const void* const data) = 0;

      /**
       *  @brief  �萔�o�b�t�@���J������
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IConstantBuffer(const IConstantBuffer& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IConstantBuffer& operator = (const IConstantBuffer& other) = delete;

      /**
       *  @brief  �萔�o�b�t�@���쐬����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  descriptor_handle:�f�B�X�N���v�^�[�n���h��
       *  @param  size_in_bytes:�萔�o�b�t�@�̃T�C�Y
       *  @return �萔�o�b�t�@�ւ̃��j�[�N�|�C���^
       */
      static std::unique_ptr<IConstantBuffer> Create(ID3D12Device* const device, const D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle, const std::uint32_t size_in_bytes);
    };
  };
};