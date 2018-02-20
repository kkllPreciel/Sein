/**
 *  @file     texture.cc
 *  @brief    �e�N�X�`���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/31
 *  @version  1.0
 */

 // include
#include "texture.h"
#include "DirectXTK12/include/WICTextureLoader.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  �e�N�X�`���p�N���X
       */
      class Texture final : public ITexture
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         *  @param  resource:���\�[�X
         */
        Texture(ID3D12Resource* resource) : ITexture(), buffer(resource, [](IUnknown* p) {p->Release(); })
        {
        
        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~Texture() override
        {
          Release();
        }

        /**
         *  @brief  ���\�[�X���J������
         */
        void Release() noexcept override
        {
          buffer.reset();
        }

        /**
         *  @brief  ���\�[�X���擾����
         *  @return ���\�[�X�̎Q��
         */
        ID3D12Resource& Get() const noexcept override
        {
          return *buffer;
        }

      private:
        std::unique_ptr<ID3D12Resource, void(*)(IUnknown*)> buffer; ///< �o�b�t�@
      };
    };

    /**
     *  @brief  �t�@�C������e�N�X�`���𐶐�����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  upload_batch:���\�[�X�X�V�p�N���X�̃C���X�^���X
     *  @param  file_path:�e�N�X�`���t�@�C���p�X
     *  @return �e�N�X�`���C���^�[�t�F�C�X�ւ̃��j�[�N�|�C���^
     */
    std::unique_ptr<ITexture> ITexture::CreateFromFile(ID3D12Device* const device, DirectX::ResourceUploadBatch& upload_batch, const std::wstring& file_path)
    {
      ID3D12Resource* resource;
      if (FAILED(DirectX::CreateWICTextureFromFileEx(device, upload_batch, file_path.c_str(), 0u, D3D12_RESOURCE_FLAG_NONE, DirectX::WIC_LOADER_FORCE_SRGB, &resource)))
      {
        throw std::exception("CreateWICTextureFromFileEx");
      }

      return std::make_unique<Texture>(resource);
    }
  };
};