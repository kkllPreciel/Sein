/**
 *  @file     buffer.cc
 *  @brief    Direct3D11のバッファに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/06/03
 *  @version  1.0
 */

 // include
#include "direct3d11_buffer.h"
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  バッファ用クラス
       */
      class Buffer final : public IBuffer
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        Buffer() : buffer_(nullptr, [](ID3D11Buffer* p) { p->Release(); }), buffer_desc_({})
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~Buffer() override
        {
          Destroy();
        }

        /**
         *  @brief  作成する
         *  @param  device:Direct3D11のデバイス
         *  @param  buffer_desc:バッファの設定
         */
        void Create(ID3D11Device* const device, const D3D11_BUFFER_DESC& buffer_desc)
        {
          ID3D11Buffer* buffer;
          if (FAILED(device->CreateBuffer(&buffer_desc, nullptr, &buffer)))
          {
            throw std::exception("バッファの作成に失敗しました。");
          }

          buffer_.reset(buffer);
          buffer_desc_ = buffer_desc;
        }

        /**
         *  @brief  終了処理を行う
         */
        void Destroy() override
        {
          buffer_desc_ = {};
          buffer_.reset();
        }

      private:
        std::unique_ptr<ID3D11Buffer, std::function<void(ID3D11Buffer*)>> buffer_;  ///< バッファ
        D3D11_BUFFER_DESC buffer_desc_;                                             ///< バッファの設定
      };
    };

    /**
     *  @brief  バッファを作成する
     *  @param  device:Direct3D11のデバイス
     *  @param  buffer_desc:バッファの設定
     *  @return バッファへのシェアードポインタ
     */
    std::shared_ptr<IBuffer> IBuffer::Create(ID3D11Device* const device, const D3D11_BUFFER_DESC& buffer_desc)
    {
      auto buffer = std::make_shared<Buffer>();

      buffer->Create(device, buffer_desc);

      return buffer;
    }
  };
};