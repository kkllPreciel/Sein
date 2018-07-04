/**
 *  @file     index_buffer.cc
 *  @brief    頂点インデックスバッファに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/03
 *  @version  1.0
 */

 // include
#include "direct3d12_index_buffer.h"
#include "direct3d12_buffer.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  頂点インデックスバッファ用クラス
       */
      class IndexBuffer final : public IIndexBuffer
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        IndexBuffer() : buffer_(nullptr), index_buffer_view_({})
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~IndexBuffer() override
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
         *  @param  format:頂点インデックスのフォーマット
         *  @param  indices:頂点インデックス配列へのポインタ
         */
        void Map(const DXGI_FORMAT format, const void* const indices) override
        {
          // 頂点インデックスバッファ(リソース)へのポインタ
          unsigned char* pData;
          if (FAILED(buffer_->Get().Map(
            0,                                  // サブリソースのインデックス番号
            nullptr,                            // CPUからアクセスするメモリの範囲(nullptrは全領域にアクセスする)
            reinterpret_cast<void**>(&pData)    // リソースデータへのポインタ
          )))
          {
            throw std::exception("頂点インデックスバッファ用リソースへのポインタの取得に失敗しました。");
          }

          // 頂点インデックスバッファ(リソース)へ頂点インデックスデータをコピー
          std::memcpy(pData, indices, buffer_->Get().GetDesc().Width);

          // 頂点インデックスバッファ(リソース)へのポインタを無効にする
          buffer_->Get().Unmap(
            0,        // サブリソースインデックス番号
            nullptr   // マップ解除するメモリの範囲、CPUが変更した可能性のある領域(nullptrは全領域)
          );

          // 頂点インデックスバッファのビューを初期化する
          index_buffer_view_.BufferLocation = buffer_->Get().GetGPUVirtualAddress();  // バッファのアドレス
          index_buffer_view_.SizeInBytes = buffer_->Get().GetDesc().Width;            // バッファ(全頂点インデックス合計)のサイズ(バイト単位)
          index_buffer_view_.Format = format;                                         // バッファのフォーマット
        }
        
        /**
         *  @brief  頂点インデックスバッファビューを取得する
         *  @return 頂点インデックスバッファビュー
         */
        const D3D12_INDEX_BUFFER_VIEW& GetView() override
        {
          return index_buffer_view_;
        }
        
        /**
         *  @brief  頂点インデックスバッファを開放する
         */
        void Release() noexcept override
        {
          index_buffer_view_ = {};
          buffer_.reset();
        }

      private:
        std::unique_ptr<Buffer> buffer_;            ///< バッファ(リソース)
        D3D12_INDEX_BUFFER_VIEW index_buffer_view_; ///< 頂点インデックスバッファビュー
      };
    };

    /**
     *  @brief  頂点インデックスバッファを作成する
     *  @param  device:Direct3D12のデバイス
     *  @param  size_in_bytes:頂点インデックスバッファのサイズ(頂点インデックスサイズ * 頂点インデックス数)
     *  @return 頂点インデックスバッファへのユニークポインタ
     */
    std::unique_ptr<IIndexBuffer> IIndexBuffer::Create(ID3D12Device* const device, const std::uint32_t size_in_bytes)
    {
      // ヒープの設定
      D3D12_HEAP_PROPERTIES properties;
      properties.Type = D3D12_HEAP_TYPE_UPLOAD;                     // ヒープの種類(今回はCPU、GPUからアクセス可能なヒープに設定)
      properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPUページプロパティ(不明に設定)
      properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // ヒープのメモリプール(不明に設定)
      properties.CreationNodeMask = 1;                              // 恐らくヒープが生成されるアダプター(GPU)の番号
      properties.VisibleNodeMask = 1;                               // 恐らくヒープが表示されるアダプター(GPU)の番号

      auto buffer = new Buffer();
      auto index_buffer = std::make_unique<IndexBuffer>();
      index_buffer->SetBuffer(buffer);

      try
      {
        buffer->Create(device, properties, size_in_bytes, D3D12_RESOURCE_FLAG_NONE);
      }
      catch (const char*)
      {
        throw std::exception("頂点インデックスバッファ用リソースの作成に失敗しました。");
      }

      return index_buffer;
    }
  };
};