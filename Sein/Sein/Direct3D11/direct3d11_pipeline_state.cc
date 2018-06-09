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
        PipelineState() :
          blend_state_(nullptr, [](ID3D11BlendState* p) { p->Release(); }), rasterizer_state_(nullptr, [](ID3D11RasterizerState* p) { p->Release(); }),
          depth_stencil_state_(nullptr, [](ID3D11DepthStencilState* p) { p->Release(); }), input_layout_(nullptr, [](ID3D11InputLayout* p) { p->Release(); }),
          vertex_shader_(nullptr, [](ID3D11VertexShader* p) { p->Release(); }), pixel_shader_(nullptr, [](ID3D11PixelShader* p) { p->Release(); }),
          geometry_shader_(nullptr, [](ID3D11GeometryShader* p) {p->Release(); }), domain_shader_(nullptr, [](ID3D11DomainShader* p) {p->Release(); }),
          hull_shader_(nullptr, [](ID3D11HullShader* p) {p->Release(); }), compute_shader_(nullptr, [](ID3D11ComputeShader* p) {p->Release(); })
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
        std::unique_ptr<ID3D11BlendState, std::function<void(ID3D11BlendState*)>> blend_state_;                       ///< ブレンドステート
        std::unique_ptr<ID3D11RasterizerState, std::function<void(ID3D11RasterizerState*)>> rasterizer_state_;        ///< ラスタライザーステート
        std::unique_ptr<ID3D11DepthStencilState, std::function<void(ID3D11DepthStencilState*)>> depth_stencil_state_; ///< 深度ステンシルステート
        std::unique_ptr<ID3D11InputLayout, std::function<void(ID3D11InputLayout*)>> input_layout_;                    ///< インプットレイアウト
        std::unique_ptr<ID3D11VertexShader, std::function<void(ID3D11VertexShader*)>> vertex_shader_;                 ///< 頂点シェーダー
        std::unique_ptr<ID3D11PixelShader, std::function<void(ID3D11PixelShader*)>> pixel_shader_;                    ///< ピクセルシェーダー
        std::unique_ptr<ID3D11GeometryShader, std::function<void(ID3D11GeometryShader*)>> geometry_shader_;           ///< ジオメトリシェーダー
        std::unique_ptr<ID3D11DomainShader, std::function<void(ID3D11DomainShader*)>> domain_shader_;                 ///< ドメインシェーダー
        std::unique_ptr<ID3D11HullShader, std::function<void(ID3D11HullShader*)>> hull_shader_;                       ///< ハルシェーダー
        std::unique_ptr<ID3D11ComputeShader, std::function<void(ID3D11ComputeShader*)>> compute_shader_;              ///< コンピュートシェーダー
      };
    };

    /**
     *  @brief  パイプラインステートを作成する
     *  @param  device:Direct3D11のデバイス
     *  @return パイプラインステートへのシェアードポインタ
     */
    std::shared_ptr<IPipelineState> IPipelineState::Create(ID3D11Device* const device)
    {
      auto pipeline_state = std::make_shared<PipelineState>();

      // TODO:生成処理

      return pipeline_state;
    }
  };
};