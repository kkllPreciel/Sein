/**
 *  @file     direct3d11_blend_state.cc
 *  @brief    Direct3D11のブレンドステートに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/06/16
 *  @version  1.0
 */

 // include
#include "direct3d11_blend_state.h"
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  ブレンドステート用クラス
       */
      class BlendState final : public IBlendState
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        BlendState() : blend_state_(nullptr, [](ID3D11BlendState* p) { p->Release(); }), desc_({})
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~BlendState() override
        {
          Destroy();
        }

        /**
         *  @brief  作成する
         *  @param  device:Direct3D11のデバイス
         *  @param  desc:ブレンドの設定
         */
        void Create(ID3D11Device* const device, const Desc& desc)
        {
          ID3D11BlendState* blend_state;
          auto direct3d11_blend_state = Convert(desc);
          if (FAILED(device->CreateBlendState(&direct3d11_blend_state, &blend_state)))
          {
            throw std::exception("ブレンドステートの作成に失敗しました。");
          }

          blend_state_.reset(blend_state);
          desc_ = desc;
        }
        
        /**
         *  @brief  終了処理を行う
         */
        void Destroy() override
        {
          blend_state_.reset();
        }

      private:
        /**
         *  @brief  ブレンドの設定をDirect3D11用ブレンドの設定に変換する
         *  @param  desc:ブレンドの設定
         *  @return Direct3D11用ブレンドの設定
         */
        D3D11_BLEND_DESC Convert(const Desc& desc)
        {
          D3D11_BLEND_DESC direct3d11_blend_desc = {};

          direct3d11_blend_desc.AlphaToCoverageEnable = desc.AlphaToCoverageEnable;
          direct3d11_blend_desc.IndependentBlendEnable = desc.IndependentBlendEnable;
          direct3d11_blend_desc.RenderTarget[0] = desc.RenderTarget[0];
          direct3d11_blend_desc.RenderTarget[1] = desc.RenderTarget[1];
          direct3d11_blend_desc.RenderTarget[2] = desc.RenderTarget[2];
          direct3d11_blend_desc.RenderTarget[3] = desc.RenderTarget[3];
          direct3d11_blend_desc.RenderTarget[4] = desc.RenderTarget[4];
          direct3d11_blend_desc.RenderTarget[5] = desc.RenderTarget[5];
          direct3d11_blend_desc.RenderTarget[6] = desc.RenderTarget[6];
          direct3d11_blend_desc.RenderTarget[7] = desc.RenderTarget[7];

          return std::move(direct3d11_blend_desc);
        }

        std::unique_ptr<ID3D11BlendState, std::function<void(ID3D11BlendState*)>> blend_state_; ///< ブレンドステート
        Desc desc_;                                                                             ///< ブレンドの設定
      };
    };

    /**
     *  @brief  ブレンドステート作成する
     *  @param  device:Direct3D11のデバイス
     *  @param  desc:ブレンドの設定
     *  @return ブレンドステートへのシェアードポインタ
     */
    std::shared_ptr<IBlendState> IBlendState::Create(ID3D11Device* const device, const Desc& desc)
    {
      auto blend_state = std::make_shared<BlendState>();

      blend_state->Create(device, desc);

      return blend_state;
    }
  };
};