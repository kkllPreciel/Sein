/**
 *  @file     texture_view.cc
 *  @brief    テクスチャ用ビューに関する処理を行うプログラムソース
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
     *  @brief  コンストラクタ
     */
    TextureView::TextureView()
    {

    }
    
    /**
     *  @brief  デストラクタ
     */
    TextureView::~TextureView()
    {
      Release();
    }
    
    /**
     *  @brief  テクスチャ用ビューを生成する
     *  @param  device:Direct3D12のデバイス
     *  @param  descriptor_heap:ディスクリプターヒープ
     *  @param  width:テクスチャの横幅
     *  @param  height:テクスチャの縦幅
     */
    void TextureView::Create(ID3D12Device* const device, IDescriptorHeap* const descriptor_heap, const UINT64 width, const UINT64 height) noexcept(false)
    {
      Release();

      // テクスチャの生成
      try
      {
        // ヒープの設定
        D3D12_HEAP_PROPERTIES properties = {};
        properties.Type = D3D12_HEAP_TYPE_DEFAULT;                    // ヒープの種類
        properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPUページプロパティ(不明に設定)
        properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // ヒープのメモリプール(不明に設定)
        properties.CreationNodeMask = 1;                              // 恐らくヒープが生成されるアダプター(GPU)の番号
        properties.VisibleNodeMask = 1;                               // 恐らくヒープが表示されるアダプター(GPU)の番号

        texture_ = std::make_unique<Texture>();
        texture_->Create(device, properties, width, height);
      }
      catch (const char*)
      {
        throw "テクスチャ用ビューのリソース作成に失敗しました。";
      }

      // 中間リソースの生成
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
        throw "テクスチャ用ビューの中間リソース作成に失敗しました。";
      }

      // Describe and create a SRV for the texture.
      D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
      srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
      srvDesc.Texture2D.MipLevels = 1;

      // シェーダーリソースビュー用のディスクリプターを生成
      // ディスクリプターヒープの領域に作成される
      device->CreateShaderResourceView(&(texture_->Get()), &srvDesc, descriptor_heap->CreateDescriptor().GetHandleForCPU());
    }
    
    /**
     *  @brief  テクスチャ用ビューを開放する
     */
    void TextureView::Release() noexcept
    {
      texture_.reset();
      buffer_.reset();
    }

    /**
     *  @brief  データのマップを行う
     *  @param  data:テクスチャデータ
     *  @param  bytesPerPixel:1ピクセルで使用するバイト数
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
        throw "テクスチャ用中間リソースへのポインタの取得に失敗しました。";
      }
      std::memcpy(pData, data, sizeof(uint8_t) * textureData.SlicePitch);
      buffer_->Get().Unmap(0, nullptr);
    }

    /**
     *  @brief  テクスチャを取得する
     *  @return テクスチャへの参照
     */
    const Texture& TextureView::GetTexture() const
    {
      return *texture_;
    }
    
    /**
     *  @brief  中間バッファを取得する
     *  @return 中間バッファへの参照
     */
    const Buffer& TextureView::GetTemporaryBuffer() const
    {
      return *buffer_;
    }
  };
};