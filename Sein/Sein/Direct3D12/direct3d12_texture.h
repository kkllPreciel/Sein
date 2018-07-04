/**
 *  @file     texture.h
 *  @brief    �e�N�X�`���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/31
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <string>
#include <d3d12.h>
#include "DirectXTK12/include/ResourceUploadBatch.h"
#include "direct3d12_resource.h"

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
       *  @brief  �t�@�C������e�N�X�`���𐶐�����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  upload_batch:���\�[�X�X�V�p�N���X�̃C���X�^���X
       *  @param  file_path:�e�N�X�`���t�@�C���p�X
       *  @return �e�N�X�`���C���^�[�t�F�C�X�ւ̃��j�[�N�|�C���^
       */
      static std::unique_ptr<ITexture> CreateFromFile(ID3D12Device* const device, DirectX::ResourceUploadBatch& upload_batch, const std::wstring& file_path);
    };
  };
};