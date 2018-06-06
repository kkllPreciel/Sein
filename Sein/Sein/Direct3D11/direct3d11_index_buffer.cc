/**
 *  @file     direct3d11_index_buffer.cc
 *  @brief    Direct3D11のインデックスバッファに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/06/06
 *  @version  1.0
 */

 // include
#include "direct3d11_index_buffer.h"
#include <functional>
#include "direct3d11_buffer.h"

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  インデックスバッファ用クラス
       */
      class IndexBuffer final : public IIndexBuffer
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        IndexBuffer() : buffer_(nullptr)
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~IndexBuffer() override
        {
          Destroy();
        }

        /**
         *  @brief  作成する
         *  @param  device:Direct3D11のデバイス
         *  @param  size_in_bytes:インデックス配列のサイズ
         *  @param  type:インデックスの型
         */
        void Create(ID3D11Device* const device, const std::uint32_t size_in_bytes, Type type)
        {
          D3D11_BUFFER_DESC buffer_desc = {};
          buffer_desc.ByteWidth = size_in_bytes;                ///< バッファのサイズ
          buffer_desc.Usage = D3D11_USAGE_DYNAMIC;              ///< 動的に更新可能
          buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;      ///< インデックスバッファ
          buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  ///< CPUからアクセス可能
          buffer_desc.MiscFlags = 0;                            ///< その他のフラグ

          // バッファの作成
          buffer_ = IBuffer::Create(device, buffer_desc);

          type_ = type;
        }

        /**
         *  @brief  マップする
         *  @param  context:コンテキスト
         *  @param  size_in_bytes:インデックス配列のサイズ
         *  @param  indices:インデックス配列へのポインタ
         */
        void Map(ID3D11DeviceContext* const context, const std::uint32_t size_in_bytes, const void* const indices) override
        {
          D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};
          mapped_buffer.pData = nullptr;

          buffer_->Map(context, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);

          std::memcpy(mapped_buffer.pData, indices, size_in_bytes);

          buffer_->Unmap(context, 0);
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
        Type type_;                       ///< インデックスの型
      };
    };

    /**
     *  @brief  インデックスバッファを作成する
     *  @param  device:Direct3D11のデバイス
     *  @param  size_in_bytes:インデックスバッファのサイズ(インデックスサイズ * インデックス数)
     *  @param  index_type:インデックスの型
     *  @return インデックスバッファへのシェアードポインタ
     */
    std::shared_ptr<IIndexBuffer> IIndexBuffer::Create(ID3D11Device* const device, const std::uint32_t size_in_bytes, IIndexBuffer::Type index_type)
    {
      auto index_buffer = std::make_shared<IndexBuffer>();

      index_buffer->Create(device, size_in_bytes, index_type);

      return index_buffer;
    }
  };
};