/**
 *  @file     direct3d11_rasterizer_state.h
 *  @brief    Direct3D11のラスタライザステートに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/12
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <d3d11.h>

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11のラスタライザステート用インターフェース
     */
    class IRasterizerState
    {
    public:
      /**
       *  @brief  塗りつぶしの種別
       */
      enum class FillMode : std::uint32_t {
        kUndefined, ///< 未定義
        kWireframe, ///< ワイヤーフレーム
        kSolid,     ///< 塗りつぶし
      };

      /**
       *  @brief  カリングの種別
       */
      enum class CullMode : std::uint32_t {
        kUndefined, ///< 未定義
        kNone,      ///< カリング無し
        kFront,     ///< 表面をカリングする
        kBack,      ///< 裏面をカリングする
      };

      /**
       *  @brief  ラスタライザの設定
       */
      class Desc
      {
      public:
        FillMode fill_mode;             ///< 塗りつぶしの種別
        CullMode cull_mode;             ///< カリングの種別
        bool front_counter_clockwise;   ///< 反時計回り(左周り)の面を表とするフラグ
        std::int32_t depth_bias;        ///< 指定のピクセルに加算するバイアス値
        std::float_t depth_bias_clamp;
        std::float_t slope_scaled_depth_bias;
        bool depth_clip_enable;
        bool scissor_enable;
        bool multisample_enable;
        bool antialiased_line_enable;
      };

      /**
       *  @brief  コンストラクタ
       */
      IRasterizerState() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IRasterizerState() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IRasterizerState(const IRasterizerState& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IRasterizerState& operator = (const IRasterizerState& other) = delete;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  ラスタライザステートを作成する
       *  @param  device:Direct3D11のデバイス
       *  @param  desc:ラスタライザの設定
       *  @return ラスタライザステートへのシェアードポインタ
       */
      static std::shared_ptr<IRasterizerState> Create(ID3D11Device* const device, const Desc& desc);

    };
  };
};