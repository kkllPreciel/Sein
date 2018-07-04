/**
 *  @file     rasterizer_desc.cc
 *  @brief    ラスタライザの設定に関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/03/23
 *  @version  1.0
 */

 // include
#include "direct3d12_rasterizer_desc.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  デフォルト設定のラスタライザの設定を作成する
     *  @param  fill_mode:フィルモード(デフォルトは塗りつぶし)
     *  @param  cull_mode:カリングのモード(デフォルトはカリングなし)
     *  @return ラスタライザの設定
     */
    D3D12_RASTERIZER_DESC CreateDefaultRasterizerDesc(D3D12_FILL_MODE fill_mode, D3D12_CULL_MODE cull_mode)
    {
      // ラスタライザーステートの設定
      D3D12_RASTERIZER_DESC desc;
      desc.FillMode = fill_mode;                                           // 三角形を描画するときに使用する塗りつぶしモード(今回は塗りつぶし)
      desc.CullMode = cull_mode;                                           // カリングのモード(裏向きのポリゴンを描画しない)
      desc.FrontCounterClockwise = FALSE;                                  // ポリゴンの表裏の判定方法(今回は時計回りなら表)
      desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;                           // 深度バイアス(Z-Fighting対策?)
      desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;                // 深度バイアスのクランプ値
      desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;   // 傾斜を考慮した深度バイアス(ピーターパン現象対策?)
      desc.DepthClipEnable = TRUE;                                         // Zクリッピングを行うか(今回は行う)
      desc.MultisampleEnable = FALSE;                                      // マルチサンプリングのアンチエイリアシングを行うか(今回はしない)
      desc.AntialiasedLineEnable = FALSE;                                  // 線のアンチエイリアシングを行うか(今回はしない)
      desc.ForcedSampleCount = 0;                                          // UAVレンダリングまたはラスタライズ中に強制されるサンプル数(今回は強制しない)
      desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; // 保守的なラスタライズフラグ(今回はオフ)
      return desc;
    }
  };
};