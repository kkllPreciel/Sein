/**
 *  @file     direct3d11_device_context.cc
 *  @brief    Direct3D11のデバイスコンテキストに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/06/07
 *  @version  1.0
 */

 // include
#include "direct3d11_device_context.h"
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      class DeviceContext final : public IDeviceContext
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        DeviceContext() : context_(nullptr, [](ID3D11DeviceContext* p) { p->Release(); })
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~DeviceContext() override
        {
          Destroy();
        }

        /**
         *  @brief  作成する
         *  @param  device:Direct3D11のデバイス
         *  @param  context_type:デバイスコンテキストの種別
         *  @param  type:インデックスの型
         */
        void Create(ID3D11Device* const device, const IDeviceContext::Type& context_type)
        {
          ID3D11DeviceContext* context;
          if (context_type == Type::kDeferred)
          {
            device->CreateDeferredContext(0, &context);
          }
          else
          {
            device->GetImmediateContext(&context);
          }

          context_.reset(context);
          type_ = context_type;
        }
        
        /**
         *  @brief  終了処理を行う
         */
        void Destroy() override
        {
          context_->Release();
        }

      private:
        std::unique_ptr<ID3D11DeviceContext, std::function<void(ID3D11DeviceContext*)>> context_; ///< デバイスコンテキスト
        Type type_;                                                                               ///< デバイスコンテキストの種別

      };
    };

    /**
     *  @brief  デバイスコンテキストを作成する
     *  @param  device:Direct3D11のデバイス
     *  @param  context_type:デバイスコンテキストの種別
     *  @return デバイスコンテキストへのシェアードポインタ
     */
    std::shared_ptr<IDeviceContext> IDeviceContext::Create(ID3D11Device* const device, const IDeviceContext::Type& context_type)
    {
      auto device_context = std::make_shared<DeviceContext>();

      device_context->Create(device, context_type);

      return device_context;
    }
  };
};