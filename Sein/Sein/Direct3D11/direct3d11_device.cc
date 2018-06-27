/**
 *  @file     direct3d11_device.cc
 *  @brief    Direct3D11のデバイスに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/05/22
 *  @version  1.0
 */

 // include
#include "direct3d11_device.h"
#include <cassert>
#include <array>
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  デバイス用クラス
       */
      class Device final : public IDevice
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        Device()
          : device_(nullptr, [](ID3D11Device* p) { p->Release(); }),
          immediate_context_(nullptr, [](ID3D11DeviceContext* p) { p->Release(); }),
          swap_chain_(nullptr, [](IDXGISwapChain* p) { p->Release(); })
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~Device() override
        {
          Destroy();
        }

        /**
         *  @brief  終了処理を行う
         */
        void Destroy() override
        {
          swap_chain_.reset();
          immediate_context_.reset();
          device_.reset();
        }

        /**
         *  @brief  作成する
         *  @param  handle:ウィンドウハンドル
         *  @param  width:ウィンドウの横幅
         *  @param  height:ウィンドウの縦幅
         */
        void Create(const HWND& handle, const std::uint32_t& width, const std::uint32_t& height)
        {
          assert(device_ == nullptr);

          UINT createDeviceFlags = 0;
#ifdef _DEBUG
          createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
          // ドライバー種別
          std::array<D3D_DRIVER_TYPE, 3> driver_types = {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE,
          };

          // 機能レベル
          std::array<D3D_FEATURE_LEVEL, 2> feature_levels = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0
          };

          // スワップチェインの設定
          DXGI_SWAP_CHAIN_DESC swap_chain_desc = {0};
          swap_chain_desc.BufferCount = 1;
          swap_chain_desc.BufferDesc.Width = width;
          swap_chain_desc.BufferDesc.Height = height;
          swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
          swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
          swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
          swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
          swap_chain_desc.OutputWindow = handle;
          swap_chain_desc.SampleDesc.Count = 1;
          swap_chain_desc.SampleDesc.Quality = 0;
          swap_chain_desc.Windowed = TRUE;

          for (auto driver_type : driver_types)
          {
            ID3D11Device* device;
            ID3D11DeviceContext* context;
            IDXGISwapChain* swap_chain;

            // デバイスとスワップチェインを作成する
            // デバイスのみで作成することも可能
            // その場合、スワップチェインは後で作成する
            if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(
              nullptr,
              driver_type,
              nullptr,
              createDeviceFlags,
              feature_levels.data(),
              static_cast<UINT>(feature_levels.size()),
              D3D11_SDK_VERSION,
              &swap_chain_desc,
              &swap_chain,
              &device,
              &feature_level_,
              &context)))
            {
              device_.reset(device);
              immediate_context_.reset(context);
              swap_chain_.reset(swap_chain);
              driver_type_ = driver_type;

              break;
            }
          }

          if (!device_ || !immediate_context_ || !swap_chain_)
          {
            throw std::exception("デバイスの生成に失敗しました。");
          }
        }

        /**
         *  @brief  デバイスコンテキストを作成する
         *  @param  context_type:デバイスコンテキストの種別
         *  @return デバイスコンテキスト用インターフェースへのシェアードポインタ
         */
        std::shared_ptr<IDeviceContext> CreateDeviceContext(const IDeviceContext::Type& context_type) override
        {
          return IDeviceContext::Create(device_.get(), context_type);
        }

        /**
         *  @brief  パイプラインステートを作成する
         *  @param  pipeline_desc:パイプラインの設定
         *  @return パイプラインステート用インターフェースへのシェアードポインタ
         */
        std::shared_ptr<IPipelineState> CreatePipelineState(const IPipelineState::Desc& pipeline_desc) override
        {
          return IPipelineState::Create(device_.get(), pipeline_desc);
        }

        /**
         *  @brief  シェーダーを作成する
         *  @param  shader_desc:シェーダーの設定
         *  @return シェーダー用インターフェースへのシェアードポインタ
         */
        std::shared_ptr<IShader> CreateShader(const IShader::Desc& shader_desc) override
        {
          return IShader::Create(device_.get(), shader_desc);
        }

        /**
         *  @brief  頂点バッファを作成する
         *  @param  size_in_bytes:頂点バッファのサイズ(頂点サイズ * 頂点数)
         *  @return 頂点バッファへのスマートポインタ
         */
        std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const std::uint32_t& size_in_bytes) override
        {
          return IVertexBuffer::Create(device_.get(), size_in_bytes);
        }

      private:
        std::unique_ptr<ID3D11Device, std::function<void(ID3D11Device*)>> device_;                          ///< デバイス
        std::unique_ptr<ID3D11DeviceContext, std::function<void(ID3D11DeviceContext*)>> immediate_context_; ///< コンテキスト
        std::unique_ptr<IDXGISwapChain, std::function<void(IDXGISwapChain*)>> swap_chain_;                  ///< スワップチェーン
        D3D_DRIVER_TYPE driver_type_;                                                                       ///< ドライバー種別
        D3D_FEATURE_LEVEL feature_level_;                                                                   ///< 機能レベル
      };
    };

    /**
     *  @brief  デバイスを作成する
     *  @param  handle:ウィンドウハンドル
     *  @param  width:ウィンドウの横幅
     *  @param  height:ウィンドウの縦幅
     */
    std::shared_ptr<IDevice> IDevice::Create(const HWND& handle, const std::uint32_t& width, const std::uint32_t& height)
    {
      auto device = std::make_shared<Device>();
    
      device->Create(handle, width, height);
    
      return device;
    }
  };
};