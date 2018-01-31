/**
 *  @file     texture.cc
 *  @brief    �e�N�X�`���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/31
 *  @version  1.0
 */

 // include
#include "texture.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �R���X�g���N�^
     */
    Texture::Texture() : buffer(nullptr, [](IUnknown* p) {p->Release(); })
    {

    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    Texture::~Texture()
    {
      Release();
    }

    /**
     *  @brief  �e�N�X�`�����\�[�X�𐶐�����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  properties:���\�[�X�̃q�[�v�v���p�e�B
     *  @param  width:���\�[�X�̉���
     *  @param  height:���\�[�X�̏c��
     *  @param  flag:���\�[�X����I�v�V�����t���O
     */
    void Texture::Create(ID3D12Device* const device, const D3D12_HEAP_PROPERTIES& properties, const UINT64 width, const UINT64 height, const D3D12_RESOURCE_FLAGS flag) noexcept(false)
    {
      Release();

      // ���\�[�X�̐ݒ�
      D3D12_RESOURCE_DESC resource_desc = {};
      resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // ���\�[�X�̎��(����͕K���o�b�t�@)
      resource_desc.Alignment = 0;                                  // �A���C�������g
      resource_desc.Width = width;                                  // ���\�[�X�̕�
      resource_desc.Height = height;                                // ���\�[�X�̍���
      resource_desc.DepthOrArraySize = 1;                           // ���\�[�X�̐[��(�e�N�X�`�����Ɏg�p���镨�A����͕K��1)
      resource_desc.MipLevels = 1;                                  // �~�b�v�}�b�v�̃��x��(����͕K��1)
      resource_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;            // ���\�[�X�f�[�^�t�H�[�}�b�g(R8G8B8A8��)
      resource_desc.SampleDesc.Count = 1;                           // �s�N�Z���P�ʂ̃}���`�T���v�����O��
      resource_desc.SampleDesc.Quality = 0;                         // �}���`�T���v�����O�̕i�����x��
      resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;          // �e�N�X�`�����C�A�E�g�I�v�V����
      resource_desc.Flags = flag;                                   // ���\�[�X����I�v�V�����t���O

      // ���\�[�X(�o�b�t�@)�̐���
      ID3D12Resource* resource = nullptr;
      if (FAILED(device->CreateCommittedResource(
        &properties,                        // �q�[�v�̐ݒ�
        D3D12_HEAP_FLAG_NONE,               // �q�[�v�I�v�V����(TODO:�K�؂ȃI�v�V������ݒ肷��)
        &resource_desc,                     // ���\�[�X�̐ݒ�
        D3D12_RESOURCE_STATE_COPY_DEST,     // ���\�[�X�̏��(TODO:�K�؂ȏ�Ԃ�ݒ肷��)
        nullptr,                            // �N���A�J���[�̃f�t�H���g�l
        IID_PPV_ARGS(&resource))))
      {
        throw "�e�N�X�`���p���\�[�X�̍쐬�Ɏ��s���܂����B";
      }
      buffer.reset(resource);
    }
    
    /**
     *  @brief  ���\�[�X���J������
     */
    void Texture::Release() noexcept
    {
      buffer.reset(nullptr);
    }
    
    /**
     *  @brief  ���\�[�X���擾����
     *  @return ���\�[�X�̎Q��
     */
    ID3D12Resource& Texture::Get() const noexcept
    {
      return *buffer;
    }
  };
};