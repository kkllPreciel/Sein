/**
 *  @file     direct3d11_blend_state.h
 *  @brief    Direct3D11のブレンドステートに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/16
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
     *  @brief  Direct3D11のブレンドステート用インターフェース
     */
    class IBlendState
    {
    public:
      /**
       *  @brief  ブレンドの設定
       */
      class Desc
      {
      public:
        BOOL AlphaToCoverageEnable;
        BOOL IndependentBlendEnable;
        D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];
      };

      /**
       *  @brief  コンストラクタ
       */
      IBlendState() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IBlendState() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IBlendState(const IBlendState& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IBlendState& operator = (const IBlendState& other) = delete;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  ブレンドステート作成する
       *  @param  device:Direct3D11のデバイス
       *  @param  desc:ブレンドの設定
       *  @return ブレンドステートへのシェアードポインタ
       */
      static std::shared_ptr<IBlendState> Create(ID3D11Device* const device, const Desc& desc);
    };
  };
};