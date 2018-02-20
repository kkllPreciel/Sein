/**
 *  @file     texture_view.cc
 *  @brief    �e�N�X�`���p�r���[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/31
 *  @version  1.0
 */

 // include
#include "texture_view.h"
#include "descriptor.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �R���X�g���N�^
     */
    TextureView::TextureView()
    {

    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    TextureView::~TextureView()
    {
      Release();
    }

    /**
     *  @brief  �e�N�X�`���p�r���[�𐶐�����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  texture:�e�N�X�`��
     *  @param  descriptor_heap:�f�B�X�N���v�^�[�q�[�v
     */
    void TextureView::Create(ID3D12Device* const device, ITexture* const texture, IDescriptorHeap* const descriptor_heap) noexcept(false)
    {
      Release();

      texture_.reset(texture);
      auto desc = texture->Get().GetDesc();

      // Describe and create a SRV for the texture.
      D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
      srvDesc.Format = desc.Format;
      srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
      srvDesc.Texture2D.MipLevels = desc.MipLevels;

      // �V�F�[�_�[���\�[�X�r���[�p�̃f�B�X�N���v�^�[�𐶐�
      // �f�B�X�N���v�^�[�q�[�v�̗̈�ɍ쐬�����
      device->CreateShaderResourceView(&(texture_->Get()), &srvDesc, descriptor_heap->CreateDescriptor().GetHandleForCPU());
    }
    
    /**
     *  @brief  �e�N�X�`���p�r���[���J������
     */
    void TextureView::Release() noexcept
    {
      texture_.reset();
    }

    /**
     *  @brief  �e�N�X�`�����擾����
     *  @return �e�N�X�`���ւ̎Q��
     */
    const ITexture& TextureView::GetTexture() const
    {
      return *texture_;
    }
  };
};