/**
 *  @file     constant_buffer.cc
 *  @brief    定数バッファに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2017/04/08
 *  @version  1.0
 */

 // include
#include "constant_buffer.h"
#include "buffer.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  定数バッファ用クラス
       */
      class ConstantBuffer final : public IConstantBuffer
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        ConstantBuffer() : buffer_(nullptr), buffer_pointer_(nullptr), descriptor_handle_({})
        {

        }
        
        /**
         *  @brief  デストラクタ
         */
        ~ConstantBuffer() override
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
         *  @brief  ディスクリプターハンドルを設定する
         *  @param  descriptor_handle:ディスクリプターハンドル
         */
        void SetDescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle)
        {
          descriptor_handle_ = descriptor_handle;
        }

        /**
         *  @brief  バッファへのポインタを設定する
         *  @param  buffer_pointer:バッファへのポインタ
         */
        void SetBufferPointer(void* const buffer_pointer)
        {
          buffer_pointer_ = buffer_pointer;
        }
        
        /**
         *  @brief  マップする
         *  @param  size_in_bytes:マップするデータのサイズ
         *  @param  data:マップするデータへのポインタ
         */
        void Map(const std::uint32_t size_in_bytes, const void* const data) override
        {
          if (nullptr == buffer_pointer_)
          {
            throw std::exception("定数バッファ用リソースへのポインタが存在しません。");
          }

          std::memcpy(buffer_pointer_, data, size_in_bytes);
        }
        
        /**
         *  @brief  定数バッファを開放する
         */
        void Release() noexcept override
        {
          if (buffer_ && buffer_pointer_)
          {
            buffer_->Get().Unmap(0, nullptr);
          }

          if (buffer_)
          {
            buffer_.reset();
          }

          buffer_pointer_ = nullptr;
          descriptor_handle_ = {};
        }

      private:
        std::unique_ptr<Buffer> buffer_;                ///< バッファ(リソース)
        void* buffer_pointer_;                          ///< バッファへのポインタ
        D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle_; ///< ディスクリプターハンドル
      };
    };

    /**
     *  @brief  定数バッファを作成する
     *  @param  device:Direct3D12のデバイス
     *  @param  descriptor_handle:ディスクリプターハンドル
     *  @param  size_in_bytes:定数バッファのサイズ
     *  @return 定数バッファへのユニークポインタ
     */
    std::unique_ptr<IConstantBuffer> IConstantBuffer::Create(ID3D12Device* const device, const D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle, const std::uint32_t size_in_bytes)
    {
      // ヒープの設定
      D3D12_HEAP_PROPERTIES properties;
      properties.Type = D3D12_HEAP_TYPE_UPLOAD;                     // ヒープの種類(今回はCPU、GPUからアクセス可能なヒープに設定)
      properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPUページプロパティ(不明に設定)
      properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // ヒープのメモリプール(不明に設定)
      properties.CreationNodeMask = 1;                              // 恐らくヒープが生成されるアダプター(GPU)の番号
      properties.VisibleNodeMask = 1;                               // 恐らくヒープが表示されるアダプター(GPU)の番号

      // 定数バッファのサイズは256バイトでアラインメントされていなければならない為
      // 引数「size」で生成したバッファサイズだと領域が足りない可能性がある。
      // なので、256の倍数に変更する = (size + 255) & ~255
      std::uint32_t constant_buffer_size = (size_in_bytes + 255) & ~255;

      auto buffer = new Buffer();
      auto constant_buffer = std::make_unique<ConstantBuffer>();
      constant_buffer->SetBuffer(buffer);

      try
      {
        buffer->Create(device, properties, constant_buffer_size, D3D12_RESOURCE_FLAG_NONE);
      }
      catch (const char*)
      {
        throw std::exception("定数バッファ用リソースの作成に失敗しました。");
      }

      // 定数バッファビューの設定
      D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc = {};
      constantBufferViewDesc.BufferLocation = buffer->Get().GetGPUVirtualAddress();   // バッファのアドレス
      constantBufferViewDesc.SizeInBytes = constant_buffer_size;                      // 定数バッファは256バイトでアラインメントされていなければならない

      // 定数バッファビュー用のディスクリプターを生成
      // ディスクリプターヒープの領域に作成される
      device->CreateConstantBufferView(&constantBufferViewDesc, descriptor_handle);
      constant_buffer->SetDescriptorHandle(descriptor_handle);

      // マップ。Releaseが呼ばれるまでアンマップしない
      void* buffer_pointer;
      if (FAILED(buffer->Get().Map(
        0,                                            // サブリソースのインデックス番号
        nullptr,                                      // CPUからアクセスするメモリの範囲(nullptrは全領域にアクセスする)
        reinterpret_cast<void**>(&buffer_pointer)     // リソースデータへのポインタ
      )))
      {
        throw std::exception("定数バッファ用リソースへのポインタの取得に失敗しました。");
      }

      // 初期化
      std::memset(buffer_pointer, 0, constant_buffer_size);
      constant_buffer->SetBufferPointer(buffer_pointer);

      return constant_buffer;
    }
  };
};