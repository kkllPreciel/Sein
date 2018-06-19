/**
 *  @file     direct3d11_pipeline_state.cc
 *  @brief    Direct3D11のパイプラオインステートに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/06/07
 *  @version  1.0
 */

 // include
#include "direct3d11_pipeline_state.h"
#include <functional>

#include "direct3d11_input_layout.h"

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  パイプラインステート用クラス
       */
      class PipelineState final : public IPipelineState
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        PipelineState()
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~PipelineState() override
        {
          Destroy();
        }

        /**
         *  @brief  作成する
         *  @param  device:Direct3D11のデバイス
         *  @param  desc:パイプラインの設定
         */
        void Create(ID3D11Device* const device, const Desc& desc)
        {

        }
        
        /**
         *  @brief  終了処理を行う
         */
        void Destroy() override
        {
          compute_shader_.reset();
          hull_shader_.reset();
          domain_shader_.reset();
          geometry_shader_.reset();
          pixel_shader_.reset();
          vertex_shader_.reset();
          input_layout_.reset();
          depth_stencil_state_.reset();
          rasterizer_state_.reset();
          blend_state_.reset();
        }

      private:
        std::shared_ptr<IBlendState> blend_state_ = nullptr;                ///< ブレンドステート
        std::shared_ptr<IRasterizerState> rasterizer_state_ = nullptr;      ///< ラスタライザステート
        std::shared_ptr<IDepthStencilState> depth_stencil_state_ = nullptr; ///< 深度ステンシルステート
        std::shared_ptr<IInputLayout> input_layout_ = nullptr;              ///< 入力レイアウト
        std::shared_ptr<IShader> vertex_shader_ = nullptr;                  ///< 頂点シェーダー
        std::shared_ptr<IShader> pixel_shader_ = nullptr;                   ///< ピクセルシェーダー
        std::shared_ptr<IShader> geometry_shader_ = nullptr;                ///< ジオメトリシェーダー
        std::shared_ptr<IShader> domain_shader_ = nullptr;                  ///< ドメインシェーダー
        std::shared_ptr<IShader> hull_shader_ = nullptr;                    ///< ハルシェーダー
        std::shared_ptr<IShader> compute_shader_ = nullptr;                 ///< コンピュートシェーダー
      };
    };

    /**
     *  @brief  パイプラインステートを作成する
     *  @param  device:Direct3D11のデバイス
     *  @param  desc:パイプラインの設定
     *  @return パイプラインステートへのシェアードポインタ
     */
    std::shared_ptr<IPipelineState> IPipelineState::Create(ID3D11Device* const device, const Desc& desc)
    {
      auto pipeline_state = std::make_shared<PipelineState>();

      pipeline_state->Create(device, desc);

      return pipeline_state;
    }
  };
};