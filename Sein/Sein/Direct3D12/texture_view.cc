/**
 *  @file     texture_view.cc
 *  @brief    �e�N�X�`���p�r���[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/31
 *  @version  1.0
 */

 // include
#include "texture_view.h"
#include "texture.h"
#include "buffer.h"
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
     *  @param  descriptor_heap:�f�B�X�N���v�^�[�q�[�v
     *  @param  width:�e�N�X�`���̉���
     *  @param  height:�e�N�X�`���̏c��
     */
    void TextureView::Create(ID3D12Device* const device, IDescriptorHeap* const descriptor_heap, const UINT64 width, const UINT64 height) noexcept(false)
    {
      Release();

      // �e�N�X�`���̐���
      try
      {
        // �q�[�v�̐ݒ�
        D3D12_HEAP_PROPERTIES properties = {};
        properties.Type = D3D12_HEAP_TYPE_DEFAULT;                    // �q�[�v�̎��
        properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPU�y�[�W�v���p�e�B(�s���ɐݒ�)
        properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // �q�[�v�̃������v�[��(�s���ɐݒ�)
        properties.CreationNodeMask = 1;                              // ���炭�q�[�v�����������A�_�v�^�[(GPU)�̔ԍ�
        properties.VisibleNodeMask = 1;                               // ���炭�q�[�v���\�������A�_�v�^�[(GPU)�̔ԍ�

        texture_ = std::make_unique<Texture>();
        texture_->Create(device, properties, width, height);
      }
      catch (const char*)
      {
        throw "�e�N�X�`���p�r���[�̃��\�[�X�쐬�Ɏ��s���܂����B";
      }

      // ���ԃ��\�[�X�̐���
      try
      {
        D3D12_RESOURCE_DESC Desc = texture_->Get().GetDesc();
        UINT64 RequiredSize = 0;
        device->GetCopyableFootprints(&Desc, 0, 1, 0, nullptr, nullptr, nullptr, &RequiredSize);

        D3D12_HEAP_PROPERTIES properties;
        properties.Type = D3D12_HEAP_TYPE_UPLOAD;
        properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        properties.CreationNodeMask = 1;
        properties.VisibleNodeMask = 1;

        buffer_ = std::make_unique<Buffer>();
        buffer_->Create(device, properties, RequiredSize);
      }
      catch (const char*)
      {
        throw "�e�N�X�`���p�r���[�̒��ԃ��\�[�X�쐬�Ɏ��s���܂����B";
      }

      // Describe and create a SRV for the texture.
      D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
      srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
      srvDesc.Texture2D.MipLevels = 1;

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
      buffer_.reset();
    }

    /**
     *  @brief  �f�[�^�̃}�b�v���s��
     *  @param  data:�e�N�X�`���f�[�^
     *  @param  bytesPerPixel:1�s�N�Z���Ŏg�p����o�C�g��
     */
    void TextureView::Map(const void* const data, const unsigned int bytesPerPixel) noexcept(false)
    {
      D3D12_RESOURCE_DESC Desc = texture_->Get().GetDesc();
      D3D12_SUBRESOURCE_DATA textureData = {};
      textureData.pData = data;
      textureData.RowPitch = Desc.Width * bytesPerPixel;
      textureData.SlicePitch = textureData.RowPitch * Desc.Height;

      unsigned char* pData;
      if (FAILED(buffer_->Get().Map(0, nullptr, reinterpret_cast<void**>(&pData))))
      {
        throw "�e�N�X�`���p���ԃ��\�[�X�ւ̃|�C���^�̎擾�Ɏ��s���܂����B";
      }
      std::memcpy(pData, data, sizeof(uint8_t) * textureData.SlicePitch);
      buffer_->Get().Unmap(0, nullptr);
    }

    /**
     *  @brief  �e�N�X�`�����擾����
     *  @return �e�N�X�`���ւ̎Q��
     */
    const Texture& TextureView::GetTexture() const
    {
      return *texture_;
    }
    
    /**
     *  @brief  ���ԃo�b�t�@���擾����
     *  @return ���ԃo�b�t�@�ւ̎Q��
     */
    const Buffer& TextureView::GetTemporaryBuffer() const
    {
      return *buffer_;
    }
  };
};