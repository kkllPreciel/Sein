/**
 *  @file     texture.h
 *  @brief    テクスチャに関するヘッダファイル
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
     *  @brief  テクスチャ用インターフェイス
     */
    class ITexture : public IResource
    {
    public:
      /**
       *  @brief  デストラクタ
       */
      virtual ~ITexture() override = default;

      /**
       *  @brief  ファイルからテクスチャを生成する
       *  @param  device:Direct3D12のデバイス
       *  @param  upload_batch:リソース更新用クラスのインスタンス
       *  @param  file_path:テクスチャファイルパス
       *  @return テクスチャインターフェイスへのユニークポインタ
       */
      static std::unique_ptr<ITexture> CreateFromFile(ID3D12Device* const device, DirectX::ResourceUploadBatch& upload_batch, const std::wstring& file_path);
    };
  };
};