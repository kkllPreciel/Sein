/**
 *  @file     texture_view.cc
 *  @brief    テクスチャ用ビューに関する処理を行うプログラムソース
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
     *  @param  texture:テクスチャ
     *  @param  descriptor_heap:ディスクリプターヒープ
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
    }

    /**
     *  @brief  テクスチャを取得する
     *  @return テクスチャへの参照
     */
    const ITexture& TextureView::GetTexture() const
    {
      return *texture_;
    }
  };
};