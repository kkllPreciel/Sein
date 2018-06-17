/**
 *  @file     direct3d11_depth_stencil_state.h
 *  @brief    Direct3D11の深度ステンシルステートに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/17
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
     *  @brief  Direct3D11の深度ステンシルステート用インターフェース
     */
    class IDepthStencilState
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IDepthStencilState() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IDepthStencilState() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IDepthStencilState(const IDepthStencilState& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IDepthStencilState& operator = (const IDepthStencilState& other) = delete;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  深度ステンシルステートを作成する
       *  @param  device:Direct3D11のデバイス
       *  @param  desc:深度ステンシルの設定
       *  @return 深度ステンシルへのシェアードポインタ
       */
      static std::shared_ptr<IDepthStencilState> Create(ID3D11Device* const device, const D3D11_DEPTH_STENCIL_DESC& desc);
    };
  };
};