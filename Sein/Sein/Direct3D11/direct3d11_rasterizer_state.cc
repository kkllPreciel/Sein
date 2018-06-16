/**
 *  @file     direct3d11_rasterizer_state.cc
 *  @brief    Direct3D11のラスタライザステートに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/06/14
 *  @version  1.0
 */

 // include
#include "direct3d11_rasterizer_state.h"
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  ラスタライザステート用クラス
       */
      class RasterizerState final : public IRasterizerState
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        RasterizerState() : rasterizer_state_(nullptr, [](ID3D11RasterizerState* p) { p->Release(); }), desc_({})
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~RasterizerState() override
        {
          Destroy();
        }

        /**
         *  @brief  作成する
         *  @param  device:Direct3D11のデバイス
         *  @param  desc:ラスタライザの設定
         */
        void Create(ID3D11Device* const device, const Desc& desc)
        {
          ID3D11RasterizerState* rasterizer_state;
          auto direct3d11_rasterizer_desc = Convert(desc);
          if (FAILED(device->CreateRasterizerState(&direct3d11_rasterizer_desc, &rasterizer_state)))
          {
            throw std::exception("ラスタライザステートの作成に失敗しました。");
          }

          rasterizer_state_.reset(rasterizer_state);
          desc_ = desc;
        }
        
        /**
         *  @brief  終了処理を行う
         */
        void Destroy() override
        {
          rasterizer_state_.reset();
        }

      private:
        /**
         *  @brief  ラスタライザの設定をDirect3D11用ラスタライザの設定に変換する
         *  @param  desc:ラスタライザの設定
         *  @return Direct3D11用ラスタライザの設定
         */
        D3D11_RASTERIZER_DESC Convert(const Desc& desc)
        {
          D3D11_RASTERIZER_DESC direct3d11_rasterizer_desc = {};

          direct3d11_rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
          switch (desc.fill_mode)
          {
          case FillMode::kWireframe:
            direct3d11_rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
            break;
          }

          direct3d11_rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
          switch (desc.cull_mode)
          {
          case CullMode::kNone:
            direct3d11_rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
            break;
          case CullMode::kFront:
            direct3d11_rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
            break;
          }

          direct3d11_rasterizer_desc.FrontCounterClockwise = desc.front_counter_clockwise;
          direct3d11_rasterizer_desc.DepthBias = desc.depth_bias;
          direct3d11_rasterizer_desc.DepthBiasClamp = desc.depth_bias_clamp;
          direct3d11_rasterizer_desc.SlopeScaledDepthBias = desc.slope_scaled_depth_bias;
          direct3d11_rasterizer_desc.DepthClipEnable = desc.depth_clip_enable;
          direct3d11_rasterizer_desc.ScissorEnable = desc.scissor_enable;
          direct3d11_rasterizer_desc.MultisampleEnable = desc.multisample_enable;
          direct3d11_rasterizer_desc.AntialiasedLineEnable = desc.antialiased_line_enable;

          return std::move(direct3d11_rasterizer_desc);
        }

        std::unique_ptr<ID3D11RasterizerState, std::function<void(ID3D11RasterizerState*)>> rasterizer_state_;  ///< ラスタライザステート
        Desc desc_;                                                                                             ///< ラスタライザの設定
      };
    };

    /**
     *  @brief  ラスタライザステートを作成する
     *  @param  device:Direct3D11のデバイス
     *  @param  desc:ラスタライザの設定
     *  @return ラスタライザステートへのシェアードポインタ
     */
    std::shared_ptr<IRasterizerState> IRasterizerState::Create(ID3D11Device* const device, const Desc& desc)
    {
      auto rasterizer_state = std::make_shared<RasterizerState>();

      rasterizer_state->Create(device, desc);

      return rasterizer_state;
    }
  };
};