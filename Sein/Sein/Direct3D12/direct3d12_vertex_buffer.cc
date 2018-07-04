/**
 *  @file     vertex_buffer.cc
 *  @brief    頂点バッファに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/02
 *  @version  1.0
 */

 // include
#include "direct3d12_vertex_buffer.h"
#include "direct3d12_buffer.h"

namespace Sein
{
  namespace Direct3D12
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
        VertexBuffer() : buffer_(nullptr), vertex_buffer_view_({})
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~VertexBuffer() override
        {
          Release();
        }

        /**
         *  @brief  バッファを設定する
         *  @param  buffer:バッファ
         */
        void SetBuffer(Buffer* const buffer)
        {
          buffer_.reset(buffer);
        }

        /**
         *  @brief  マップする
         *  @param  stride_in_bytes:1頂点のサイズ
         *  @param  vertices:頂点配列へのポインタ
         */
        void Map(const std::uint32_t stride_in_bytes, const void* const vertices) override
        {
          // 頂点バッファ(リソース)へのポインタ
          unsigned char* pData;
          if (FAILED(buffer_->Get().Map(
            0,                                  // サブリソースのインデックス番号
            nullptr,                            // CPUからアクセスするメモリの範囲(nullptrは全領域にアクセスする)
            reinterpret_cast<void**>(&pData)    // リソースデータへのポインタ
          )))
          {
            throw std::exception("頂点バッファ用リソースへのポインタの取得に失敗しました。");
          }

          // 頂点バッファ(リソース)へ頂点データをコピー
          std::memcpy(pData, vertices, buffer_->Get().GetDesc().Width);

          // 頂点バッファ(リソース)へのポインタを無効にする
          buffer_->Get().Unmap(
            0,      // サブリソースインデックス番号
            nullptr // マップ解除するメモリの範囲、CPUが変更した可能性のある領域(nullptrは全領域)
          );

          // 頂点バッファのビューを初期化する
          vertex_buffer_view_.BufferLocation = buffer_->Get().GetGPUVirtualAddress(); // バッファのアドレス
          vertex_buffer_view_.SizeInBytes = buffer_->Get().GetDesc().Width;           // バッファ(全頂点合計)のサイズ(バイト単位)
          vertex_buffer_view_.StrideInBytes = stride_in_bytes;                        // 1頂点のサイズ(バイト単位)
        }

        /**
         *  @brief  頂点バッファビューを取得する
         *  @return 頂点バッファビュー
         */
        const D3D12_VERTEX_BUFFER_VIEW& GetView() override
        {
          return vertex_buffer_view_;
        }
        
        /**
         *  @brief  頂点バッファを開放する
         */
        void Release() noexcept override
        {
          vertex_buffer_view_ = {};
          buffer_.reset();
        }

      private:
        std::unique_ptr<Buffer> buffer_;              ///< バッファ(リソース)
        D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_; ///< 頂点バッファビュー
      };
    };

    /**
     *  @brief  頂点バッファを作成する
     *  @param  device:Direct3D12のデバイス
     *  @param  size_in_bytes:頂点バッファのサイズ(頂点サイズ * 頂点数)
     *  @return 頂点バッファへのユニークポインタ
     */
    std::unique_ptr<IVertexBuffer> IVertexBuffer::Create(ID3D12Device* const device, const std::uint32_t size_in_bytes)
    {
      // ヒープの設定
      D3D12_HEAP_PROPERTIES properties;
      properties.Type = D3D12_HEAP_TYPE_UPLOAD;                     // ヒープの種類(今回はCPU、GPUからアクセス可能なヒープに設定)
      properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPUページプロパティ(不明に設定)
      properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // ヒープのメモリプール(不明に設定)
      properties.CreationNodeMask = 1;                              // 恐らくヒープが生成されるアダプター(GPU)の番号
      properties.VisibleNodeMask = 1;                               // 恐らくヒープが表示されるアダプター(GPU)の番号

      auto buffer = new Buffer();
      auto vertex_buffer = std::make_unique<VertexBuffer>();
      vertex_buffer->SetBuffer(buffer);

      try
      {
        buffer->Create(device, properties, size_in_bytes, D3D12_RESOURCE_FLAG_NONE);
      }
      catch (const char*)
      {
        throw std::exception("頂点バッファ用リソースの作成に失敗しました。");
      }

      return vertex_buffer;
    }
  };
};