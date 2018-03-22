/**
 *  @file     rasterizer_desc.h
 *  @brief    ラスタライザの設定に関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/23
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <d3d12.h>

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
    D3D12_RASTERIZER_DESC CreateDefaultRasterizerDesc(D3D12_FILL_MODE fill_mode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE cull_mode = D3D12_CULL_MODE_NONE);
  };
};