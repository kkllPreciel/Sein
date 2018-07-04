/**
 *  @file     device.h
 *  @brief    レンダラーのデバイスに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/07/02
 *  @version  1.0
 */

#pragma once

// include
#include <cstdint>
#include <memory>

#include "device_context.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"

namespace Sein
{
  namespace Renderer
  {
    /**
     *  @brief  デバイス用インターフェース
     */
    class IDevice
    {
    public:
      /**
       *  @brief  デバイス種別
       */
      enum class Type : std::uint32_t {
        Direct3D11, ///< Direct3D11
        Direct3D12, ///< Direct3D12
      };

      /**
       *  @brief  コンストラクタ
       */
      IDevice() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IDevice() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IDevice(const IDevice& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IDevice& operator = (const IDevice& other) = delete;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  デバイスコンテキストを作成する
       *  @param  context_type:デバイスコンテキストの種別
       *  @return デバイスコンテキストへのスマートポインタ
       */
      virtual std::shared_ptr<IDeviceContext> CreateDeviceContext(const IDeviceContext::Type& context_type) = 0;

      /**
       *  @brief  頂点バッファを作成する
       *  @param  size_in_bytes:頂点バッファのサイズ(頂点サイズ * 頂点数)
       *  @return 頂点バッファへのスマートポインタ
       */
      virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const std::uint32_t& size_in_bytes) = 0;

      /**
       *  @brief  インデックスバッファを作成する
       *  @param  size_in_bytes:インデックスバッファのサイズ(インデックスサイズ * インデックス数)
       *  @param  index_type:インデックスの型
       *  @return インデックスバッファへのスマートポインタ
       */
      virtual std::shared_ptr<IIndexBuffer> CreateIndexBuffer(const std::uint32_t& size_in_bytes, const IIndexBuffer::Type& index_type) = 0;

      /**
       *  @brief  シェーダーを作成する
       *  @param  shader_desc:シェーダーの設定
       *  @return シェーダーへのスマートポインタ
       */
      virtual std::shared_ptr<IShader> CreateShader(const IShader::Desc& shader_desc) = 0;
    };
  };
};