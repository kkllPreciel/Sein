/**
 *  @file     direct3d11_pipeline_state.h
 *  @brief    Direct3D11のパイプラインステートに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/07
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <vector>
#include <d3d11.h>

#include "direct3d11_shader.h"
#include "direct3d11_rasterizer_state.h"
#include "direct3d11_blend_state.h"
#include "direct3d11_depth_stencil_state.h"

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11のパイプラインステート用インターフェース
     */
    class IPipelineState
    {
    public:
      /**
       *  @brief  パイプラインステートの設定
       */
      class Desc
      {
      public:
        IRasterizerState::Desc rasterizer_desc;                         ///< ラスタライザの設定
        IBlendState::Desc blend_desc;                                   ///< ブレンドの設定
        D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;                    ///< 深度ステンシルの設定
        std::vector<D3D11_INPUT_ELEMENT_DESC> input_element_desc_list;  ///< 入力レイアウトの設定
        std::shared_ptr<IShader>& vertex_shader;                        ///< 頂点シェーダー
        std::shared_ptr<IShader>& pixel_shader;                         ///< ピクセルシェーダー
        std::shared_ptr<IShader>& geometry_shader;                      ///< ジオメトリシェーダー
        std::shared_ptr<IShader>& domain_shader;                        ///< ドメインシェーダー
        std::shared_ptr<IShader>& hull_shader;                          ///< ハルシェーダー
        std::shared_ptr<IShader>& compute_shader;                       ///< コンピュートシェーダー
      };

      /**
       *  @brief  コンストラクタ
       */
      IPipelineState() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IPipelineState() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IPipelineState(const IPipelineState& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IPipelineState& operator = (const IPipelineState& other) = delete;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  パイプラインステートを作成する
       *  @param  device:Direct3D11のデバイス
       *  @param  desc:パイプラインの設定
       *  @return パイプラインステートへのシェアードポインタ
       */
      static std::shared_ptr<IPipelineState> Create(ID3D11Device* const device, const Desc& desc);
    };
  };
};