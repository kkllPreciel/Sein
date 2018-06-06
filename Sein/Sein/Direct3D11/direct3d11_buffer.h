/**
 *  @file     direct3d11_buffer.h
 *  @brief    Direct3D11�̃o�b�t�@�Ɋւ���w�b�_�t�@�C��
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
     *  @brief  Direct3D11�̃o�b�t�@�p�C���^�[�t�F�C�X
     */
    class IBuffer
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IBuffer() = default;
      
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IBuffer() = default;
      
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IBuffer(const IBuffer& other) = delete;
      
      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IBuffer& operator = (const IBuffer& other) = delete;

      /**
       *  @brief  �}�b�v����
       *  @param  context:�R���e�L�X�g
       *  @param  subresource:�T�u���\�[�X�̃C���f�b�N�X�ԍ�
       *  @param  map_type:���\�[�X�ɑ΂���CPU�̃A�N�Z�X���ݒ�
       *  @param  map_flags:GPU�Ŏg�p���������ꍇ��CPU�̑Ή����@
       *  @param  mapped_resource:�}�b�v���ꂽ�T�u���\�[�X�ւ̃|�C���^
       */
      virtual void Map(ID3D11DeviceContext* const context, UINT subresource, D3D11_MAP map_type, UINT map_flags, D3D11_MAPPED_SUBRESOURCE* const mapped_resource) = 0;

      /**
       *  @brief  �A���}�b�v����
       *  @param  context:�R���e�L�X�g
       *  @param  subresource:�A���}�b�v����T�u���\�[�X�̃C���f�b�N�X�ԍ�
       */
      virtual void Unmap(ID3D11DeviceContext* const context, UINT subresource) = 0;
      
      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  �o�b�t�@���쐬����
       *  @param  device:Direct3D11�̃f�o�C�X
       *  @param  buffer_desc:�o�b�t�@�̐ݒ�
       *  @return �o�b�t�@�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IBuffer> Create(ID3D11Device* const device, const D3D11_BUFFER_DESC& buffer_desc);
    };
  };
};