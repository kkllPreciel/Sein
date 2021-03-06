/**
 *  @file     direct3d11_vertex_buffer.cc
 *  @brief    Direct3D11の頂点バッファに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/06/03
 *  @version  1.0
 */

 // include
#include "direct3d11_vertex_buffer.h"
#include <functional>
#include "direct3d11_device_context.h"
#include "direct3d11_buffer.h"

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  頂点バッファ用クラス
       */
      class VertexBuffer final : public IVertexBuffer
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        VertexBuffer() : buffer_(nullptr)
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~VertexBuffer() override
        {
          Destroy();
        }

        /**
         *  @brief  作成する
         *  @param  device:Direct3D11のデバイス
         *  @param  size_in_bytes:頂点バッファのサイズ(頂点サイズ * 頂点数)
         */
        void Create(ID3D11Device* const device, const std::uint32_t size_in_bytes)
        {
          D3D11_BUFFER_DESC buffer_desc = {};
          buffer_desc.ByteWidth = size_in_bytes;                ///< バッファのサイズ
          buffer_desc.Usage = D3D11_USAGE_DYNAMIC;              ///< 動的に更新可能
          buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;     ///< 頂点バッファ
          buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  ///< CPUからアクセス可能
          buffer_desc.MiscFlags = 0;                            ///< その他のフラグ

          // バッファの作成
          buffer_ = IBuffer::Create(device, buffer_desc);
        }

        /**
         *  @brief  マップする
         *  @param  context:コンテキスト
         *  @param  size_in_bytes:頂点配列のサイズ
         *  @param  vertices:頂点配列へのポインタ
         */
        void Map(Renderer::IDeviceContext* const context, const std::uint32_t& size_in_bytes, const void* const vertices) override
        {
          auto& device_context = const_cast<ID3D11DeviceContext&>(static_cast<IDeviceContext*>(context)->GetD3D11DeviceContext());

          D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};
          mapped_buffer.pData = nullptr;

          buffer_->Map(&device_context, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);

          std::memcpy(mapped_buffer.pData, vertices, size_in_bytes);

          buffer_->Unmap(&device_context, 0);
        }

        /**
         *  @brief  終了処理を行う
         */
        void Destroy() override
        {
          buffer_.reset();
        }

      private:
        std::shared_ptr<IBuffer> buffer_; ///< バッファ
      };
    };

    /**
     *  @brief  頂点バッファを作成する
     *  @param  device:Direct3D11のデバイス
     *  @param  size_in_bytes:頂点バッファのサイズ(頂点サイズ * 頂点数)
     *  @return 頂点バッファへのユニークポインタ
     */
    std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(ID3D11Device* const device, const std::uint32_t& size_in_bytes)
    {
      auto vertex_buffer = std::make_shared<VertexBuffer>();

      vertex_buffer->Create(device, size_in_bytes);

      return vertex_buffer;
    }
  };
};