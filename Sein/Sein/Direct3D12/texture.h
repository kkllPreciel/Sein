/**
 *  @file     texture.h
 *  @brief    �e�N�X�`���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/31
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>
#include <memory>
#include "resource.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �e�N�X�`���p�C���^�[�t�F�C�X
     */
    class ITexture : public IResource
    {
    public:
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~ITexture() override = default;

      /**
       *  @brief  �e�N�X�`�����\�[�X�𐶐�����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  properties:���\�[�X�̃q�[�v�v���p�e�B
       *  @param  width:���\�[�X�̉���
       *  @param  height:���\�[�X�̏c��
       *  @param  flag:���\�[�X����I�v�V�����t���O
       */
      virtual void Create(ID3D12Device* const device, const D3D12_HEAP_PROPERTIES& properties, const UINT64 width, const UINT64 height, const D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE) noexcept(false) = 0;
    };

    /**
     *  @brief  �e�N�X�`���p�N���X
     */
    class Texture final : public ITexture
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Texture();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~Texture() override;

      /**
       *  @brief  �e�N�X�`�����\�[�X�𐶐�����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  properties:���\�[�X�̃q�[�v�v���p�e�B
       *  @param  width:���\�[�X�̉���
       *  @param  height:���\�[�X�̏c��
       *  @param  flag:���\�[�X����I�v�V�����t���O
       */
      void Create(ID3D12Device* const device, const D3D12_HEAP_PROPERTIES& properties, const UINT64 width, const UINT64 height, const D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE) noexcept(false) override;

      /**
       *  @brief  ���\�[�X���J������
       */
      void Release() noexcept override;

      /**
       *  @brief  ���\�[�X���擾����
       *  @return ���\�[�X�̎Q��
       */
      ID3D12Resource& Get() const noexcept override;

    private:
      std::unique_ptr<ID3D12Resource, void(*)(IUnknown*)> buffer; ///< �o�b�t�@
    };
  };
};