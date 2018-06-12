/**
 *  @file     direct3d11_index_buffer.h
 *  @brief    Direct3D11�̃C���f�b�N�X�o�b�t�@�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/06/05
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
     *  @brief  Direct3D11�̃C���f�b�N�X�o�b�t�@�p�C���^�[�t�F�C�X
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
      virtual void Map(ID3D11DeviceContext* const context, const std::uint32_t size_in_bytes, const void* const indices) = 0;
      
      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  �C���f�b�N�X�o�b�t�@���쐬����
       *  @param  device:Direct3D11�̃f�o�C�X
       *  @param  size_in_bytes:�C���f�b�N�X�o�b�t�@�̃T�C�Y(�C���f�b�N�X�T�C�Y * �C���f�b�N�X��)
       *  @param  index_type:�C���f�b�N�X�̌^
       *  @return �C���f�b�N�X�o�b�t�@�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IIndexBuffer> Create(ID3D11Device* const device, const std::uint32_t size_in_bytes, IIndexBuffer::Type index_type);
    };
  };
};