/**
 *  @file     texture.cc
 *  @brief    テクスチャに関する処理を行うプログラムソース
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
       *  @brief  テクスチャ用クラス
       */
      class Texture final : public ITexture
      {
      public:
        /**
         *  @brief  コンストラクタ
         *  @param  resource:リソース
         */
        Texture(ID3D12Resource* resource) : ITexture(), buffer(resource, [](IUnknown* p) {p->Release(); })
        {
        
        }

        /**
         *  @brief  デストラクタ
         */
        ~Texture() override
        {
          Release();
        }

        /**
         *  @brief  リソースを開放する
         */
        void Release() noexcept override
        {
          buffer.reset();
        }

        /**
         *  @brief  リソースを取得する
         *  @return リソースの参照
         */
        ID3D12Resource& Get() const noexcept override
        {
          return *buffer;
        }

      private:
        std::unique_ptr<ID3D12Resource, void(*)(IUnknown*)> buffer; ///< バッファ
      };
    };

    /**
     *  @brief  ファイルからテクスチャを生成する
     *  @param  device:Direct3D12のデバイス
     *  @param  upload_batch:リソース更新用クラスのインスタンス
     *  @param  file_path:テクスチャファイルパス
     *  @return テクスチャインターフェイスへのユニークポインタ
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