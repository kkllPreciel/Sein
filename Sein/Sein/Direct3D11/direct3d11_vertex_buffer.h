/**
 *  @file     direct3d11_vertex_buffer.h
 *  @brief    Direct3D11の頂点バッファに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/03
 *  @version  1.0
 */

#pragma once

// include
#include "../Renderer/vertex_buffer.h"
#include <d3d11.h>

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11の頂点バッファ用インターフェース
     */
    class IVertexBuffer : public Renderer::IVertexBuffer
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IVertexBuffer() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IVertexBuffer() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IVertexBuffer(const IVertexBuffer& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IVertexBuffer& operator = (const IVertexBuffer& other) = delete;

      /**
       *  @brief  頂点バッファを作成する
       *  @param  device:Direct3D11のデバイス
       *  @param  size_in_bytes:頂点バッファのサイズ(頂点サイズ * 頂点数)
       *  @return 頂点バッファへのシェアードポインタ
       */
      static std::shared_ptr<IVertexBuffer> Create(ID3D11Device* const device, const std::uint32_t& size_in_bytes);
    };
  };
};