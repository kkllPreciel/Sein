/**
 *  @file     direct3d11_depth_stencil_state.cc
 *  @brief    Direct3D11の深度ステンシルステートに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/06/17
 *  @version  1.0
 */

 // include
#include "direct3d11_depth_stencil_state.h"
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  深度ステンシルステート用クラス
       */
      class DepthStencilState final : public IDepthStencilState
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        DepthStencilState() : depth_stencil_state_(nullptr, [](ID3D11DepthStencilState* p) { p->Release(); }), desc_({})
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~DepthStencilState() override
        {
          Destroy();
        }

        /**
         *  @brief  作成する
         *  @param  device:Direct3D11のデバイス
         *  @param  desc:深度ステンシルの設定
         */
        void Create(ID3D11Device* const device, const D3D11_DEPTH_STENCIL_DESC& desc)
        {
          ID3D11DepthStencilState* depth_stencil_state;
          if (FAILED(device->CreateDepthStencilState(&desc, &depth_stencil_state)))
          {
            throw std::exception("深度ステンシルステートの作成に失敗しました。");
          }

          depth_stencil_state_.reset(depth_stencil_state);
          desc_ = desc;
        }
        
        /**
         *  @brief  終了処理を行う
         */
        void Destroy() override
        {
          depth_stencil_state_.reset();
        }

      private:
        std::unique_ptr<ID3D11DepthStencilState, std::function<void(ID3D11DepthStencilState*)>> depth_stencil_state_; ///< 深度ステンシルステート
        D3D11_DEPTH_STENCIL_DESC desc_;                                                                               ///< 深度ステンシルの設定
      };
    };

    /**
     *  @brief  深度ステンシルステートを作成する
     *  @param  device:Direct3D11のデバイス
     *  @param  desc:深度ステンシルの設定
     *  @return 深度ステンシルへのシェアードポインタ
     */
    std::shared_ptr<IDepthStencilState> IDepthStencilState::Create(ID3D11Device* const device, const D3D11_DEPTH_STENCIL_DESC& desc)
    {
      auto depth_stencil_state = std::make_shared<DepthStencilState>();

      depth_stencil_state->Create(device, desc);

      return depth_stencil_state;
    }
  };
};