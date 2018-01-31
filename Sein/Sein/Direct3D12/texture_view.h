/**
 *  @file     texture_view.h
 *  @brief    �e�N�X�`���p�r���[�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/31
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>
#include <memory>
#include "texture.h"
#include "buffer.h"
#include "descriptor_heap.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �e�N�X�`���p�r���[�N���X
     */
    class TextureView
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      TextureView();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~TextureView();

      /**
       *  @brief  �e�N�X�`���p�r���[�𐶐�����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  descriptor_heap:�f�B�X�N���v�^�[�q�[�v
       *  @param  width:�e�N�X�`���̉���
       *  @param  height:�e�N�X�`���̏c��
       */
      void Create(ID3D12Device* const device, IDescriptorHeap* const descriptor_heap, const UINT64 width, const UINT64 height) noexcept(false);

      /**
       *  @brief  �e�N�X�`���p�r���[���J������
       */
      void Release() noexcept;

      /**
       *  @brief  �f�[�^�̃}�b�v���s��
       *  @param  data:�e�N�X�`���f�[�^
       *  @param  bytesPerPixel:1�s�N�Z���Ŏg�p����o�C�g��
       */
      void Map(const void* const data, const unsigned int bytesPerPixel) noexcept(false);

      /**
       *  @brief  �e�N�X�`�����擾����
       *  @return �e�N�X�`���ւ̎Q��
       */
      const Texture& GetTexture() const;

      /**
       *  @brief  ���ԃo�b�t�@���擾����
       *  @return ���ԃo�b�t�@�ւ̎Q��
       */
      const Buffer& GetTemporaryBuffer() const;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      TextureView(const TextureView& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      TextureView& operator = (const TextureView& other) = delete;

    private:
      std::unique_ptr<Texture> texture_;  ///< �e�N�X�`��
      std::unique_ptr<Buffer> buffer_;    ///< ���ԃo�b�t�@
    };
  };
};